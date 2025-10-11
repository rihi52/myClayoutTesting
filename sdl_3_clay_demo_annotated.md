```c
// Constant font ID for demo purposes
static const Uint32 FONT_ID = 0; // ID for font array

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
```

