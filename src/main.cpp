#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>
#include "Animator.h"

int main(int argc, char* argv[])
{
    std::cout << std::filesystem::current_path() << std::endl;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL3 Animation", 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!window || !renderer)
    {
        std::cerr << SDL_GetError() << "\n";
        return 1;
    }

    // ---- Load textures ----
    SDL_Texture* idleTexture = IMG_LoadTexture(renderer,
        "assets/textures/catset_spritesheets/cat01_spritesheets/cat01_idle_strip8.png");

    SDL_Texture* walkTexture = IMG_LoadTexture(renderer,
        "assets/textures/catset_spritesheets/cat01_spritesheets/cat01_walk_strip8.png");

    if (!idleTexture || !walkTexture)
    {
        std::cerr << SDL_GetError() << "\n";
        return 1;
    }

    // ---- Animator ----
    Animator animator;

    animator.AddAnimation(AnimState::Idle, { idleTexture, 40, 40, 8, 0.1f });
    animator.AddAnimation(AnimState::Walk, { walkTexture, 40, 40, 8, 0.1f });

    float x = 300.0f;
    float y = 200.0f;
    float speed = 200.0f;

    bool flipX = false;

    bool running = true;
    Uint64 prev = SDL_GetTicks();

    while (running)
    {
        Uint64 now = SDL_GetTicks();
        float deltaTime = (now - prev) / 1000.0f;
        prev = now;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        const bool* keys = SDL_GetKeyboardState(nullptr);

        bool isMoving = false;

        if (keys[SDL_SCANCODE_W]) { y -= speed * deltaTime; isMoving = true; }
        if (keys[SDL_SCANCODE_S]) { y += speed * deltaTime; isMoving = true; }

        if (keys[SDL_SCANCODE_A])
        {
            x -= speed * deltaTime;
            isMoving = true;
            flipX = true;
        }

        if (keys[SDL_SCANCODE_D])
        {
            x += speed * deltaTime;
            isMoving = true;
            flipX = false;
        }

        animator.SetState(isMoving ? AnimState::Walk : AnimState::Idle);
        animator.Update(deltaTime);

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        animator.Render(renderer, x, y, 2.0f, flipX);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(walkTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}