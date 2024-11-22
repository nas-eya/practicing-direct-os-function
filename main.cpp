#include <SDL2/SDL.h>
#include <cmath>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Eye properties
const int EYE_RADIUS = 50;
const int PUPIL_RADIUS = 20;
const int EYE_DISTANCE = 200; // Distance between the eyes

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void drawEye(SDL_Renderer* renderer, int centerX, int centerY, int mouseX, int mouseY) {
    // Draw the eye
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    drawCircle(renderer, centerX, centerY, EYE_RADIUS);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black border
    for (int i = 0; i < 3; i++) {
        drawCircle(renderer, centerX, centerY, EYE_RADIUS - i);
    }

    // Calculate pupil position
    int dx = mouseX - centerX;
    int dy = mouseY - centerY;
    float distance = std::sqrt(dx * dx + dy * dy);
    float maxDistance = EYE_RADIUS - PUPIL_RADIUS;

    if (distance > maxDistance) {
        dx = dx * maxDistance / distance;
        dy = dy * maxDistance / distance;
    }

    int pupilX = centerX + dx;
    int pupilY = centerY + dy;

    // Draw the pupil
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    drawCircle(renderer, pupilX, pupilY, PUPIL_RADIUS);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Eyes Following Mouse",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); // Sky blue background
        SDL_RenderClear(renderer);

        // Draw the eyes
        drawEye(renderer, SCREEN_WIDTH / 2 - EYE_DISTANCE / 2, SCREEN_HEIGHT / 2, mouseX, mouseY);
        drawEye(renderer, SCREEN_WIDTH / 2 + EYE_DISTANCE / 2, SCREEN_HEIGHT / 2, mouseX, mouseY);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
