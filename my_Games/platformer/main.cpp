#include <SDL.h>
#include <iostream>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Player dimensions and speed
const int PLAYER_WIDTH = 15;
const int PLAYER_HEIGHT = 60;
const int PLAYER_SPEED = 6;

// Ball dimensions and speed
//const int BALL_SIZE = 15;
//const int BALL_SPEED = 5;

// Game elements
SDL_Rect player1;
//SDL_Rect rightPaddle;
//SDL_Rect ball;
//int ballVelX = BALL_SPEED, ballVelY = BALL_SPEED;



// The surface contained by the window
SDL_Renderer* renderer = nullptr;


void render();


int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL2 Scrolling Platformer",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer for window
    //SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;

    // Initialize game elements
    player1.x = (SCREEN_WIDTH - PLAYER_WIDTH) / 2;
    player1.y = (SCREEN_HEIGHT - PLAYER_HEIGHT) / 2;
    player1.w = PLAYER_WIDTH;
    player1.h = PLAYER_HEIGHT;

    // While application is running
    while (!quit) {
        Uint32 startTick = SDL_GetTicks();

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // TODO: Add game rendering logic here
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        // Update screen
        SDL_RenderPresent(renderer);

        render();

        // Cap the frame rate
        int frameTicks = SDL_GetTicks() - startTick;
        if (frameTicks < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameTicks);
        }
    }

    // Destroy window    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}



void render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render paddles and ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderFillRect(renderer, &player1);
    //SDL_RenderFillRect(renderer, &rightPaddle);
    //SDL_RenderFillRect(renderer, &ball);

    // Update screen
    SDL_RenderPresent(renderer);
}