#include <SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow(
        "Mac App Demo",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN
    );

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    // Clean up
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
