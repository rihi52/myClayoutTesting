#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_events.h>

#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "../SDL3/clay_renderer_SDL3.c"

#include <stdio.h>
#include "global.h"
#include "main_window.h"

#include "db_query.h"
#include "start_encounter.h"

// --- Global AppState ---
AppState *gAppState = NULL;

uint32_t PreviousFocusId = 0;

Uint64 lastTime = 0;
Uint64 frequency = 0;
unsigned int frame_counter = 0;
double frame_timer = 0;
char fps_text[12] = {'0', 0};
double deltaTime = 0.0;

float MouseX = 0;
float MouseY = 0;

const int MinimumWidth = 1360;
const int MinimumHeight = 800;
const Uint32 FONT_ID = 0;

// --- Deferred scroll input ---
static Clay_Vector2 ScrollDelta = {0,0};
static bool g_dragScrolling = false;

static inline Clay_Dimensions SDL_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    TTF_Font **fonts = userData;
    TTF_Font *font = fonts[config->fontId];
    int width, height;

    TTF_SetFontSize(font, config->fontSize);
    if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to measure text: %s", SDL_GetError());
    }

    return (Clay_Dimensions) { (float) width, (float) height };
}

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    if (!TTF_Init()) {
        return SDL_APP_FAILURE;
    }

    DatabaseOpen();
    LoadDatabaseMonsters();
    LoadDatabasePlayers();

    InitializeTextBoxes();
    MouseDown = false;

    gAppState = SDL_calloc(1, sizeof(AppState));
    if (!gAppState) return SDL_APP_FAILURE;

    gAppState->focusedId = CLAY_ID("NULL");
    gAppState->SliderId = CLAY_ID("NULL");

    gAppState->StringToModify.isStaticallyAllocated = true;
    gAppState->StringToModify.chars = TextBuffer;
    gAppState->StringToModify.length = 0;

    gAppState->CurrentStatBlock = (StatBlock){0};
    InitStatBlock(&gAppState->CurrentStatBlock);
    gAppState->ActiveScreen = MAIN_SCREEN;
    gAppState->IsTextInputFocused = false;
    gAppState->IsModalOpen = false;
    gAppState->ModalParentId = CLAY_ID("NULL");
    memset(gAppState->ModalMessage, 0, sizeof(gAppState->ModalMessage));
    memset(gAppState->EncounterSaved, 0, sizeof(gAppState->EncounterSaved));

    StructureFieldArrays();

    if (!SDL_CreateWindowAndRenderer("GUIDNBATTER", 1280, 720, SDL_WINDOW_RESIZABLE, &gAppState->window, &gAppState->rendererData.renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_ShowWindow(gAppState->window);
    SDL_SetWindowMinimumSize(gAppState->window, MinimumWidth, MinimumHeight);
    SDL_StartTextInput(gAppState->window);

    gAppState->rendererData.textEngine = TTF_CreateRendererTextEngine(gAppState->rendererData.renderer);
    if (!gAppState->rendererData.textEngine) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create text engine from renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    gAppState->rendererData.fonts = SDL_calloc(1, sizeof(TTF_Font *));
    if (!gAppState->rendererData.fonts) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to allocate memory for the font array: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont("resources/Roboto-Regular.ttf", 24);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    gAppState->rendererData.fonts[FONT_ID] = font;

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = (Clay_Arena) {
        .memory = SDL_malloc(totalMemorySize),
        .capacity = totalMemorySize
    };

    lastTime = SDL_GetPerformanceCounter();
    frequency = SDL_GetPerformanceFrequency();
    frame_timer = lastTime;

    int width, height;
    SDL_GetWindowSize(gAppState->window, &width, &height);
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float) width, (float) height }, (Clay_ErrorHandler) { HandleClayErrors });
    Clay_SetMeasureTextFunction(SDL_MeasureText, gAppState->rendererData.fonts);
    Clay_SetDebugModeEnabled(true);

    *appstate = gAppState;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    (void)appstate; // we use global gAppState now
    SDL_AppResult ret_val = SDL_APP_CONTINUE;

    if (!SDL_GetWindowSizeInPixels(gAppState->window, &WindowWidth, &WindowHeight)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to get window size: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const bool * KeyState = SDL_GetKeyboardState(NULL);

    switch (event->type) {
        case SDL_EVENT_QUIT:
            ret_val = SDL_APP_SUCCESS;
            break;
        case SDL_EVENT_KEY_UP:
            if (event->key.scancode == SDL_SCANCODE_SPACE) {}
            break;
        case SDL_EVENT_TEXT_INPUT:
            Clay_ResetMeasureTextCache();
            size_t len = SDL_strlen(TextBuffer);

            // ignore if full
            if (len >= MAX_TEXT - 1) {
                break;
            }

            SDL_strlcat(TextBuffer, event->text.text, MAX_TEXT);

            // force null termination (extra safety)
            TextBuffer[MAX_TEXT - 1] = '\0';
            break;
        case SDL_EVENT_KEY_DOWN:
            if (SDL_SCANCODE_BACKSPACE == event->key.scancode) ModifyTypedString();
            if ((START_NEW_ENCOUNTER_SCREEN == gAppState->ActiveScreen || START_NEW_WITH_STATS_SCREEN == gAppState->ActiveScreen) && !gAppState->IsTextInputFocused) {
                if (SDL_SCANCODE_SPACE == event->key.scancode) {
                    if (gAppState->SortedListCount - 1 > Turn) {
                        Turn++;
                    }
                    else {
                        Turn = 0;
                    }
                }
            }
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            Clay_SetLayoutDimensions((Clay_Dimensions){(float)event->window.data1, (float)event->window.data2});
            break;
        case SDL_EVENT_MOUSE_MOTION:
            Clay_SetPointerState((Clay_Vector2){event->motion.x, event->motion.y},
                                 event->motion.state & SDL_BUTTON_LMASK);
            // MouseX = event->motion.x;
            // MouseY = event->motion.y;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            Clay_SetPointerState((Clay_Vector2){event->button.x, event->button.y},
                                 event->button.button == SDL_BUTTON_LEFT);
            MouseDown = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            Clay_SetPointerState((Clay_Vector2){event->button.x, event->button.y},
                                 event->button.button == SDL_BUTTON_LEFT);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            // store delta for frame update
            ScrollDelta.x += event->wheel.x;
            ScrollDelta.y += event->wheel.y;
            break;
        default:
            break;
    };

    return ret_val;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    (void)appstate; // using gAppState

    Uint64 frameStart = SDL_GetPerformanceCounter();

    Clay_RenderCommandArray render_commands = MainWindow(gAppState);
    PreviousFocusId = gAppState->focusedId.id;

    SDL_SetRenderDrawColor(gAppState->rendererData.renderer, 0, 0, 0, 255);
    SDL_RenderClear(gAppState->rendererData.renderer);

    SDL_Clay_RenderClayCommands(&gAppState->rendererData, &render_commands);

    SDL_RenderPresent(gAppState->rendererData.renderer);

    // --- Update scroll containers with deferred input ---
    Clay_UpdateScrollContainers(g_dragScrolling, ScrollDelta, 0.1f);
    ScrollDelta = (Clay_Vector2){0,0};

    SDL_GetMouseState(&MouseX, &MouseY);

    // --- Frame timing ---
    Uint64 frameEnd = SDL_GetPerformanceCounter();
    double frameMS = (frameEnd - frameStart) / (double)frequency * 1000.0;
    const double TARGET_FRAME_MS = 1000.0 / 60.0;  // 60 FPS
    if (frameMS < TARGET_FRAME_MS) SDL_Delay((Uint32)(TARGET_FRAME_MS - frameMS));

    lastTime = SDL_GetPerformanceCounter();

    // --- FPS counter ---
    if (lastTime > frame_timer + frequency) {
        // SDL_Log("FPS: %d", frame_counter);
        frame_counter = 0;
        frame_timer = lastTime;
    }
    frame_counter++;

    static Uint64 lastCounter = 0;

    Uint64 now = SDL_GetPerformanceCounter();

    if (lastCounter != 0) {
        deltaTime = (double)(now - lastCounter) / (double)frequency;
    }
    lastCounter = now;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void)appstate;

    if (result != SDL_APP_SUCCESS) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Application failed to run");
    }

    DatabaseClose();
    for (int i = 1; i < MAX_DB_COUNT; i++) {
        if (!DBPageHeaders[i].CreatureName.isStaticallyAllocated && DBPageHeaders[i].CreatureName.chars != NULL) {
            free((void *)DBPageHeaders[i].CreatureName.chars);
        }
    }
    FreeStatBlock(&gAppState->CurrentStatBlock);

    SDL_StopTextInput(gAppState->window);

    if (gAppState) {
        if (gAppState->rendererData.renderer) SDL_DestroyRenderer(gAppState->rendererData.renderer);
        if (gAppState->window) SDL_DestroyWindow(gAppState->window);

        if (gAppState->rendererData.fonts) {
            for(size_t i = 0; i < sizeof(gAppState->rendererData.fonts)/sizeof(*gAppState->rendererData.fonts); i++)
                TTF_CloseFont(gAppState->rendererData.fonts[i]);
            SDL_free(gAppState->rendererData.fonts);
        }

        if (gAppState->rendererData.textEngine) TTF_DestroyRendererTextEngine(gAppState->rendererData.textEngine);
        SDL_free(gAppState);
        gAppState = NULL;
    }

    TTF_Quit();
    SDL_Quit();
}
