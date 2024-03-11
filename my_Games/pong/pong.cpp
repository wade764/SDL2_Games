#include <SDL.h>
#include <iostream>

// Screen dimension constants
const int SCREEN_WIDTH = 970; // 640, 860, 970, 1080
const int SCREEN_HEIGHT = 810; // 480, 700, 810, 920

// Paddle dimensions and speed
const int PADDLE_WIDTH = 15;
const int PADDLE_HEIGHT = 60;
const int PADDLE_SPEED = 6;

// Ball dimensions and speed
const int BALL_SIZE = 15;
const int BALL_SPEED = 5;

// The window we'll be rendering to
SDL_Window* window = nullptr;

// The surface contained by the window
SDL_Renderer* renderer = nullptr;

// Game elements
SDL_Rect leftPaddle;
SDL_Rect rightPaddle;
SDL_Rect ball;
int ballVelX = BALL_SPEED, ballVelY = BALL_SPEED;

bool init(); // Starts up SDL and creates window
void close(); // Frees media and shuts down SDL
void update(); // Updates game objects
void render(); // Renders game objects to the screen

int main(int argc, char* args[]) {
    if (!init()) {
        std::cerr << "Failed to initialize!\n";
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    // Initialize game elements
    leftPaddle.x = 0;
    leftPaddle.y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
    leftPaddle.w = PADDLE_WIDTH;
    leftPaddle.h = PADDLE_HEIGHT;

    rightPaddle.x = SCREEN_WIDTH - PADDLE_WIDTH;
    rightPaddle.y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
    rightPaddle.w = PADDLE_WIDTH;
    rightPaddle.h = PADDLE_HEIGHT;

    ball.x = (SCREEN_WIDTH - BALL_SIZE) / 2;
    ball.y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
    ball.w = BALL_SIZE;
    ball.h = BALL_SIZE;

    // Game loop
    while (!quit) {
        Uint32 startTick = SDL_GetTicks();

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            rightPaddle.y -= PADDLE_SPEED;
            if (rightPaddle.y < 0) {
                rightPaddle.y = 0;
            }
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            rightPaddle.y += PADDLE_SPEED;
            if (rightPaddle.y > SCREEN_HEIGHT - PADDLE_HEIGHT) {
                rightPaddle.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
            }
        }
        if (currentKeyStates[SDL_SCANCODE_W]) {
            leftPaddle.y -= PADDLE_SPEED;
            if (leftPaddle.y < 0) {
                leftPaddle.y = 0;
            }
        }
        if (currentKeyStates[SDL_SCANCODE_S]) {
            leftPaddle.y += PADDLE_SPEED;
            if (leftPaddle.y > SCREEN_HEIGHT - PADDLE_HEIGHT) {
                leftPaddle.y = SCREEN_HEIGHT - PADDLE_HEIGHT;
            }
        }

        update();
        render();

        // Cap the frame rate
        int frameTicks = SDL_GetTicks() - startTick;
        if (frameTicks < 1000 / 60) {
            SDL_Delay((1000 / 60) - frameTicks);
        }
    }

    close();
    return 0;
}

bool init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create window
    window = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize renderer color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}

void close() {
    // Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}

void update() {
    // Move the ball
    ball.x += ballVelX;
    ball.y += ballVelY;

    // Check for collision with top or bottom
    if ((ball.y <= 0) || (ball.y + BALL_SIZE >= SCREEN_HEIGHT)) {
        ballVelY = -ballVelY;
    }

    // Check for collision with paddles
    if (SDL_HasIntersection(&ball, &leftPaddle) || SDL_HasIntersection(&ball, &rightPaddle)) {
        ballVelX = -ballVelX;
    }

    // Reset ball if it goes past paddle
    if (ball.x <= 0 || ball.x + BALL_SIZE >= SCREEN_WIDTH) {
        ball.x = (SCREEN_WIDTH - BALL_SIZE) / 2;
        ball.y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
        ballVelX = -ballVelX; // Optionally change direction
    }
}

void render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render paddles and ball
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &ball);

    // Update screen
    SDL_RenderPresent(renderer);
}
