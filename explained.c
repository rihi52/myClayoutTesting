#define SDL_MAIN_USE_CALLBACKS   // Use callback form of main for SDL, prevents SDL from redefining main

#include <SDL3/SDL.h>            // Include main SDL3 functions and types
#include <SDL3/SDL_main.h>       // Include SDL3 main entry macros
#include <SDL3_ttf/SDL_ttf.h>    // Include SDL_ttf for font rendering
#include <SDL3_image/SDL_image.h> // Include SDL_image for loading images

#define CLAY_IMPLEMENTATION      // Define implementation macro to include Clay source
#include "../clay.h"             // Include Clay GUI library header
#include "../SDL3/clay_renderer_SDL3.c" // Include SDL3-specific Clay renderer implementation

#include <stdio.h>               // Include standard I/O for printing to console

const int FONT_ID_BODY_16 = 0;    // Define a constant font ID for body text at size 16px

Clay_Color COLOR_WHITE = { 255, 255, 255, 255 }; // RGBA color white for text

// Function to render a header button with text
void RenderHeaderButton(Clay_String text) {
    CLAY_AUTO_ID({                                // Begin a Clay element with automatically assigned ID
        .layout = { .padding = { 16, 16, 8, 8 }}, // Set element padding: left, top, right, bottom
        .backgroundColor = { 140, 140, 140, 255 }, // Set gray background color
        .cornerRadius = CLAY_CORNER_RADIUS(5)    // Rounded corner radius of 5 pixels
    }) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({       // Render text inside the element
            .fontId = FONT_ID_BODY_16,           // Use the previously defined font ID
            .fontSize = 16,                      // Font size is 16 pixels
            .textColor = { 255, 255, 255, 255 } // Text color is white
        }));
    }
}

// Function to render a dropdown menu item with text
void RenderDropdownMenuItem(Clay_String text) {
    CLAY_AUTO_ID({                                // Begin a Clay element with auto-assigned ID
        .layout = { .padding = CLAY_PADDING_ALL(16) } // Uniform padding of 16 pixels
    }) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({       // Render text inside this element
            .fontId = FONT_ID_BODY_16,           // Font ID
            .fontSize = 16,                      // Font size 16
            .textColor = { 255, 255, 255, 255 } // White text color
        }));
    }
}

// Struct representing a document
typedef struct {
    Clay_String title;                           // Title of the document
    Clay_String contents;                        // Contents/text of the document
} Document;

// Struct representing an array of documents
typedef struct {
    Document *documents;                         // Pointer to the array of documents
    uint32_t length;                             // Number of documents
} DocumentArray;

// Allocate storage for 5 documents
Document documentsRaw[5];

// Initialize a DocumentArray using the raw documents
DocumentArray documents = {
    .length = 5,                                // Number of documents
    .documents = documentsRaw                    // Storage array
};

// Memory arena for temporary frame allocations in the demo
typedef struct {
    intptr_t offset;                             // Current offset in the arena
    intptr_t memory;                             // Base pointer to arena memory
} ClayVideoDemo_Arena;

// Struct containing data for the demo application
typedef struct {
    int32_t selectedDocumentIndex;              // Currently selected document index
    float yOffset;                               // Vertical scroll offset
    ClayVideoDemo_Arena frameArena;             // Temporary memory arena for layout/frame
} ClayVideoDemo_Data;

// Data struct for handling sidebar button clicks
typedef struct {
    int32_t requestedDocumentIndex;             // Index of document clicked
    int32_t* selectedDocumentIndex;             // Pointer to currently selected document index
} SidebarClickData;

// Callback function for handling sidebar interactions
void HandleSidebarInteraction(
    Clay_ElementId elementId,                    // ID of the element clicked
    Clay_PointerData pointerData,                // Information about pointer (mouse) state
    intptr_t userData                            // Arbitrary user data (cast to SidebarClickData*)
) {
    SidebarClickData *clickData = (SidebarClickData*)userData; // Cast user data to click struct
    // If this element was clicked this frame
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        if (clickData->requestedDocumentIndex >= 0 && clickData->requestedDocumentIndex < documents.length) {
            // Update the selected document index
            *clickData->selectedDocumentIndex = clickData->requestedDocumentIndex;
        }
    }
}

// Initialize demo data and populate documents
ClayVideoDemo_Data ClayVideoDemo_Initialize() {
    // Populate first document with squirrel article
    documents.documents[0] = (Document){
        .title = CLAY_STRING("Squirrels"),
        .contents = CLAY_STRING("The Secret Life of Squirrels: Nature's Clever Acrobats\n""...") // Text truncated for brevity
    };

    // Populate other documents with sample text
    documents.documents[1] = (Document){
        .title = CLAY_STRING("Lorem Ipsum"),
        .contents = CLAY_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit...") // Lorem ipsum text
    };

    documents.documents[2] = (Document){
        .title = CLAY_STRING("Vacuum Instructions"),
        .contents = CLAY_STRING("Chapter 3: Getting Started - Unpacking and Setup\n...") // Instructions text
    };

    documents.documents[3] = (Document){
        .title = CLAY_STRING("Article 4"),
        .contents = CLAY_STRING("Article 4")
    };

    documents.documents[4] = (Document){
        .title = CLAY_STRING("Article 5"),
        .contents = CLAY_STRING("Article 5")
    };

    // Allocate memory arena for frames
    ClayVideoDemo_Data data = {
        .frameArena = { .memory = (intptr_t)malloc(1024) } // Allocate 1KB for arena
    };

    return data; // Return initialized demo data
}

// Create the main UI layout for the video demo
Clay_RenderCommandArray ClayVideoDemo_CreateLayout(ClayVideoDemo_Data *data) {
    data->frameArena.offset = 0;                   // Reset memory arena offset

    Clay_BeginLayout();                             // Begin new Clay layout frame

    // Define layout that expands to fill parent
    Clay_Sizing layoutExpand = {
        .width = CLAY_SIZING_GROW(0),              // Grow to fill width
        .height = CLAY_SIZING_GROW(0)              // Grow to fill height
    };

    // Define background color for content areas
    Clay_Color contentBackgroundColor = { 90, 90, 90, 255 }; // Dark gray

    // Outer container of entire UI
    CLAY(CLAY_ID("OuterContainer"), {
        .backgroundColor = {43, 41, 51, 255 },     // Darker background
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM, // Stack children vertically
            .sizing = layoutExpand,               // Expand to fill
            .padding = CLAY_PADDING_ALL(16),     // Uniform padding 16
            .childGap = 16                        // Gap between children
        }
    }) {
        // Header bar element
        CLAY(CLAY_ID("HeaderBar"), {
            .layout = {
                .sizing = {
                    .height = CLAY_SIZING_FIXED(60), // Fixed height 60px
                    .width = CLAY_SIZING_GROW(0)    // Expand width
                },
                .padding = { 16, 16, 0, 0 },         // Padding top and left
                .childGap = 16,                      // Gap between buttons
                .childAlignment = {
                    .y = CLAY_ALIGN_Y_CENTER         // Center vertically
                }
            },
            .backgroundColor = contentBackgroundColor, // Dark gray background
            .cornerRadius = CLAY_CORNER_RADIUS(8)     // Rounded corners
        }) {
            // File button
            CLAY(CLAY_ID("FileButton"), {
                .layout = { .padding = { 16, 16, 8, 8 }}, // Padding inside button
                .backgroundColor = {140, 140, 140, 255 }, // Gray color
                .cornerRadius = CLAY_CORNER_RADIUS(5)    // Rounded corners
            }) {
                CLAY_TEXT(CLAY_STRING("File"), CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,           // Font ID
                    .fontSize = 16,                      // Font size
                    .textColor = { 255, 255, 255, 255 } // White text
                }));

                // Show file menu if mouse is over button or menu
                bool fileMenuVisible =
                    Clay_PointerOver(Clay_GetElementId(CLAY_STRING("FileButton")))
                    ||
                    Clay_PointerOver(Clay_GetElementId(CLAY_STRING("FileMenu")));

                if (fileMenuVisible) { // Render dropdown menu if visible
                    CLAY(CLAY_ID("FileMenu"), {
                        .floating = {
                            .attachTo = CLAY_ATTACH_TO_PARENT,
                            .attachPoints = { .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM }
                        },
                        .layout = { .padding = {0, 0, 8, 8 } }
                    }) {
                        CLAY_AUTO_ID({
                            .layout = {
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .sizing = { .width = CLAY_SIZING_FIXED(200) }
                            },
                            .backgroundColor = {40, 40, 40, 255 },
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }) {
                            RenderDropdownMenuItem(CLAY_STRING("New"));
                            RenderDropdownMenuItem(CLAY_STRING("Open"));
                            RenderDropdownMenuItem(CLAY_STRING("Close"));
                        }
                    }
                }
            }

            // Render remaining header buttons
            RenderHeaderButton(CLAY_STRING("Edit"));
            CLAY_AUTO_ID({ .layout = { .sizing = { CLAY_SIZING_GROW(0) }}}) {} // Spacer
            RenderHeaderButton(CLAY_STRING("Upload"));
            RenderHeaderButton(CLAY_STRING("Media"));
            RenderHeaderButton(CLAY_STRING("Support"));
        }

        // Lower content area (sidebar + main content)
        CLAY(CLAY_ID("LowerContent"), {
            .layout = { .sizing = layoutExpand, .childGap = 16 }
        }) {
            // Sidebar container
            CLAY(CLAY_ID("Sidebar"), {
                .backgroundColor = contentBackgroundColor,
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = CLAY_PADDING_ALL(16),
                    .childGap = 8,
                    .sizing = {
                        .width = CLAY_SIZING_FIXED(250),
                        .height = CLAY_SIZING_GROW(0)
                    }
                }
            }) {
                // Loop over documents to render sidebar buttons
                for (int i = 0; i < documents.length; i++) {
                    Document document = documents.documents[i];  // Current document
                    Clay_LayoutConfig sidebarButtonLayout = {
                        .sizing = { .width = CLAY_SIZING_GROW(0) },
                        .padding = CLAY_PADDING_ALL(16)
                    };

                    if (i == data->selectedDocumentIndex) {
                        CLAY_AUTO_ID({
                            .layout = sidebarButtonLayout,
                            .backgroundColor = {120, 120, 120, 255 },
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }) {
                            CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
                                .fontId = FONT_ID_BODY_16,
                                .fontSize = 20,
                                .textColor = { 255, 255, 255, 255 }
                            }));
                        }
                    } else {
                        SidebarClickData *clickData = (SidebarClickData *)(data->frameArena.memory + data->frameArena.offset);
                        *clickData = (SidebarClickData) { .requestedDocumentIndex = i, .selectedDocumentIndex = &data->selectedDocumentIndex };
                        data->frameArena.offset += sizeof(SidebarClickData);
                        CLAY_AUTO_ID({
                            .layout = sidebarButtonLayout,
                            .backgroundColor = (Clay_Color) { 120, 120, 120, Clay_Hovered() ? 120 : 0 },
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }) {
                            Clay_OnHover(HandleSidebarInteraction, (intptr_t)clickData);
                            CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
                                .fontId = FONT_ID_BODY_16,
                                .fontSize = 20,
                                .textColor = { 255, 255, 255, 255 }
                            }));
                        }
                    }
                }
            }

            // Main content area
            CLAY(CLAY_ID("MainContent"), {
                .backgroundColor = contentBackgroundColor,
                .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 16,
                    .padding = CLAY_PADDING_ALL(16),
                    .sizing = layoutExpand
                }
            }) {
                Document selectedDocument = documents.documents[data->selectedDocumentIndex]; // Currently selected
                CLAY_TEXT(selectedDocument.title, CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 24,
                    .textColor = COLOR_WHITE
                }));
                CLAY_TEXT(selectedDocument.contents, CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 24,
                    .textColor = COLOR_WHITE
                }));
            }
        }
    }

    Clay_RenderCommandArray renderCommands = Clay_EndLayout(); // Finish layout and get render commands

    // Apply vertical offset for scrolling
    for (int32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommandArray_Get(&renderCommands, i)->boundingBox.y += data->yOffset;
    }

    return renderCommands; // Return commands to render
}

// Constant font ID for demo purposes
static const Uint32 FONT_ID = 0;

// Define some additional colors
static const Clay_Color COLOR_ORANGE    = (Clay_Color) {225, 138, 50, 255}; // RGBA orange
static const Clay_Color COLOR_BLUE      = (Clay_Color) {111, 173, 162, 255}; // RGBA blue
static const Clay_Color COLOR_LIGHT     = (Clay_Color) {224, 215, 210, 255}; // RGBA light gray

// Struct containing global application state
typedef struct app_state {
    SDL_Window *window;                     // Pointer to SDL window
    Clay_SDL3RendererData rendererData;     // Renderer data specific to Clay + SDL3
    ClayVideoDemo_Data demoData;            // Demo data including documents, selected index, etc.
} AppState;

SDL_Texture *sample_image;                  // Global texture for sample image
bool show_demo = true;                       // Flag to toggle demo UI vs image demo

// Function to measure text using SDL_ttf, used by Clay for layout
static inline Clay_Dimensions SDL_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    TTF_Font **fonts = userData;            // User data is an array of fonts
    TTF_Font *font = fonts[config->fontId]; // Select font by ID
    int width, height;                       // Output width and height

    TTF_SetFontSize(font, config->fontSize); // Set font size
    if (!TTF_GetStringSize(font, text.chars, text.length, &width, &height)) { // Measure string
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to measure text: %s", SDL_GetError());
    }

    return (Clay_Dimensions) { (float) width, (float) height }; // Return dimensions
}

// Handle errors from Clay library
void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars); // Print error to console
}

// Create a layout for an image sample
Clay_RenderCommandArray ClayImageSample_CreateLayout() {
    Clay_BeginLayout();                      // Begin Clay layout frame

    Clay_Sizing layoutExpand = {
        .width = CLAY_SIZING_GROW(0),       // Expand width
        .height = CLAY_SIZING_GROW(0)       // Expand height
    };

    CLAY(CLAY_ID("OuterContainer"), {       // Outer container
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM, // Stack vertically
            .sizing = layoutExpand,                // Expand
            .padding = CLAY_PADDING_ALL(16),      // Padding 16
            .childGap = 16                         // Gap between children
        }
    }) {
        CLAY(CLAY_ID("SampleImage"), {      // Image element
            .layout = { .sizing = layoutExpand }, // Expand to fill
            .aspectRatio = { 23.0 / 42.0 },       // Maintain aspect ratio
            .image = { .imageData = sample_image } // Texture to render
        });
    }

    return Clay_EndLayout();                 // End layout and return commands
}

// Initialize SDL3 application
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    (void) argc; (void) argv;               // Unused parameters

    if (!TTF_Init()) {                       // Initialize SDL_ttf
        return SDL_APP_FAILURE;              // Fail if initialization fails
    }

    AppState *state = SDL_calloc(1, sizeof(AppState)); // Allocate AppState
    if (!state) {
        return SDL_APP_FAILURE;              // Fail if allocation fails
    }
    *appstate = state;                       // Set pointer to AppState

    // Create SDL window and renderer
    if (!SDL_CreateWindowAndRenderer("Clay Demo", 640, 480, 0, &state->window, &state->rendererData.renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetWindowResizable(state->window, true); // Allow resizing

    // Create text engine for rendering fonts in Clay
    state->rendererData.textEngine = TTF_CreateRendererTextEngine(state->rendererData.renderer);
    if (!state->rendererData.textEngine) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create text engine from renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Allocate font array
    state->rendererData.fonts = SDL_calloc(1, sizeof(TTF_Font *));
    if (!state->rendererData.fonts) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to allocate memory for the font array: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Load a font file
    TTF_Font *font = TTF_OpenFont("resources/Roboto-Regular.ttf", 24);
    if (!font) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    state->rendererData.fonts[FONT_ID] = font; // Store font in array

    // Load sample image
    sample_image = IMG_LoadTexture(state->rendererData.renderer, "resources/sample.png");
    if (!sample_image) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to load image: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    /* Initialize Clay library */
    uint64_t totalMemorySize = Clay_MinMemorySize(); // Minimum memory required by Clay
    Clay_Arena clayMemory = (Clay_Arena) {
        .memory = SDL_malloc(totalMemorySize), // Allocate Clay memory
        .capacity = totalMemorySize            // Set capacity
    };

    int width, height;
    SDL_GetWindowSize(state->window, &width, &height); // Get window size
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float) width, (float) height }, (Clay_ErrorHandler) { HandleClayErrors }); // Init Clay
    Clay_SetMeasureTextFunction(SDL_MeasureText, state->rendererData.fonts); // Set text measurement callback

    state->demoData = ClayVideoDemo_Initialize(); // Initialize demo data

    *appstate = state;                        // Store state
    return SDL_APP_CONTINUE;                  // Continue main loop
}

// Handle SDL events
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    SDL_AppResult ret_val = SDL_APP_CONTINUE; // Default to continue

    switch (event->type) {
        case SDL_EVENT_QUIT:                   // Quit event
            ret_val = SDL_APP_SUCCESS;         // End loop
            break;
        case SDL_EVENT_KEY_UP:                  // Key released
            if (event->key.scancode == SDL_SCANCODE_SPACE) {
                show_demo = !show_demo;        // Toggle demo vs image
            }
            break;
        case SDL_EVENT_WINDOW_RESIZED:          // Window resize
            Clay_SetLayoutDimensions((Clay_Dimensions) { (float) event->window.data1, (float) event->window.data2 }); // Update Clay layout
            break;
        case SDL_EVENT_MOUSE_MOTION:            // Mouse move
            Clay_SetPointerState((Clay_Vector2) { event->motion.x, event->motion.y }, event->motion.state & SDL_BUTTON_LMASK); // Update pointer
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:       // Mouse button pressed
            Clay_SetPointerState((Clay_Vector2) { event->button.x, event->button.y }, event->button.button == SDL_BUTTON_LEFT); // Update pointer
            break;
        case SDL_EVENT_MOUSE_WHEEL:             // Scroll
            Clay_UpdateScrollContainers(true, (Clay_Vector2) { event->wheel.x, event->wheel.y }, 0.01f); // Update scroll
            break;
        default:
            break;
    };

    return ret_val;                            // Return result
}

// Main iteration/rendering loop
SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *state = appstate;                // Cast app state

    // Choose layout based on demo toggle
    Clay_RenderCommandArray render_commands = (show_demo
        ? ClayVideoDemo_CreateLayout(&state->demoData)
        : ClayImageSample_CreateLayout()
    );

    SDL_SetRenderDrawColor(state->rendererData.renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(state->rendererData.renderer);                       // Clear screen

    SDL_Clay_RenderClayCommands(&state->rendererData, &render_commands); // Render Clay layout

    SDL_RenderPresent(state->rendererData.renderer);                     // Present final frame

    return SDL_APP_CONTINUE;                                              // Continue loop
}

// Cleanup and quit application
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    (void) result;                        // Unused result for now

    if (result != SDL_APP_SUCCESS) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Application failed to run");
    }

    AppState *state = appstate;

    if (sample_image) {
        SDL_DestroyTexture(sample_image); // Destroy loaded image
    }

    if (state) {
        if (state->rendererData.renderer)
            SDL_DestroyRenderer(state->rendererData.renderer); // Destroy renderer

        if (state->window)
            SDL_DestroyWindow(state->window);                  // Destroy window

        if (state->rendererData.fonts) {
            for(size_t i = 0; i < sizeof(state->rendererData.fonts) / sizeof(*state->rendererData.fonts); i++) {
                TTF_CloseFont(state->rendererData.fonts[i]);   // Close each font
            }
            SDL_free(state->rendererData.fonts);               // Free font array
        }

        if (state->rendererData.textEngine)
            TTF_DestroyRendererTextEngine(state->rendererData.textEngine); // Destroy text engine

        SDL_free(state);                                         // Free AppState
    }

    TTF_Quit();                                                  // Quit SDL_ttf
}
