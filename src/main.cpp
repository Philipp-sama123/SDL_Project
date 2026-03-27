#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <filesystem>
#include "Animator.h"

enum class ActionState
{
    None,
    Attack,
    Dodge
};

int main(int argc, char* argv[])
{
    std::cout << std::filesystem::current_path() << std::endl;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("SDL3 Animation", 800, 600, 0);
    if (!window)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    auto loadTexture = [&](const char* path) -> SDL_Texture*
        {
            SDL_Texture* tex = IMG_LoadTexture(renderer, path);
            if (!tex)
                std::cerr << "Failed to load texture: " << path << " | " << SDL_GetError() << "\n";
            return tex;
        };

    SDL_Texture* idleTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_idle_strip8.png");
    SDL_Texture* walkTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_walk_strip8.png");
    SDL_Texture* runTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_run_strip4.png");
    SDL_Texture* jumpTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_jump_strip4.png");
    SDL_Texture* fallTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_fall_strip3.png");
    SDL_Texture* dashTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_dash_strip9.png");
    SDL_Texture* attackTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_attack_strip7.png");
    SDL_Texture* dieTexture = loadTexture("assets/textures/catset_spritesheets/cat01_spritesheets/cat01_die_strip8.png");

    if (!idleTexture || !walkTexture || !runTexture || !jumpTexture || !fallTexture ||
        !dashTexture || !attackTexture || !dieTexture)
    {
        std::cerr << "One or more textures failed to load.\n";

        SDL_DestroyTexture(idleTexture);
        SDL_DestroyTexture(walkTexture);
        SDL_DestroyTexture(runTexture);
        SDL_DestroyTexture(jumpTexture);
        SDL_DestroyTexture(fallTexture);
        SDL_DestroyTexture(dashTexture);
        SDL_DestroyTexture(attackTexture);
        SDL_DestroyTexture(dieTexture);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Animator animator;
    animator.AddAnimation(AnimState::Idle, { idleTexture,   40, 40, 8, 0.10f, true });
    animator.AddAnimation(AnimState::Walk, { walkTexture,   40, 40, 8, 0.10f, true });
    animator.AddAnimation(AnimState::Run, { runTexture,    40, 40, 4, 0.10f, true });
    animator.AddAnimation(AnimState::Jump, { jumpTexture,   40, 40, 4, 0.10f, false });
    animator.AddAnimation(AnimState::Fall, { fallTexture,   40, 40, 3, 0.10f, false });
    animator.AddAnimation(AnimState::Attack, { attackTexture, 40, 40, 7, 0.10f, false });
    animator.AddAnimation(AnimState::Dodge, { dashTexture,   40, 40, 9, 0.10f, false });

    float x = 0.0f;
    float y = 0.0f;
    float velY = 0.0f;

    const float walkSpeed = 50.0f;
    const float runSpeed = 150.0f;
    const float jumpImpulse = -500.0f;
    const float gravity = 900.0f;
    const float lowJumpMultiplier = 1.5f;
    const float fallMultiplier = 2.0f;
    const float groundY = 400.0f;
    const float dodgeSpeed = 320.0f;

    bool grounded = true;
    bool flipX = false;
    bool running = true;

    Uint64 prev = SDL_GetTicks();

    ActionState actionState = ActionState::None;
    float actionTimer = 0.0f;

    const float attackDuration = 7 * 0.10f;
    const float dodgeDuration = 9 * 0.10f;

    bool wasFalling = false;

    while (running)
    {
        Uint64 now = SDL_GetTicks();
        float deltaTime = static_cast<float>(now - prev) / 1000.0f;
        prev = now;

        if (deltaTime > 0.05f)
            deltaTime = 0.05f;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            if (event.type == SDL_EVENT_KEY_DOWN && event.key.repeat == 0)
            {
                if (actionState == ActionState::None && grounded)
                {
                    if (event.key.scancode == SDL_SCANCODE_E)
                    {
                        actionState = ActionState::Attack;
                        actionTimer = attackDuration;
                        animator.SetState(AnimState::Attack);
                    }
                    else if (event.key.scancode == SDL_SCANCODE_Q)
                    {
                        actionState = ActionState::Dodge;
                        actionTimer = dodgeDuration;
                        animator.SetState(AnimState::Dodge);
                    }
                }
            }
        }

        const bool* keys = SDL_GetKeyboardState(nullptr);

        if (actionState == ActionState::None)
        {
            bool isMoving = false;
            bool isRunningInput = keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT];
            float currentSpeed = isRunningInput ? runSpeed : walkSpeed;

            if (keys[SDL_SCANCODE_A])
            {
                x -= currentSpeed * deltaTime;
                isMoving = true;
                flipX = true;
            }

            if (keys[SDL_SCANCODE_D])
            {
                x += currentSpeed * deltaTime;
                isMoving = true;
                flipX = false;
            }

            bool jumpHeld = keys[SDL_SCANCODE_SPACE];

            if (jumpHeld && grounded)
            {
                velY = jumpImpulse;
                grounded = false;
            }

            float currentGravity = gravity;

            if (velY < 0.0f)
            {
                if (!jumpHeld)
                    currentGravity *= lowJumpMultiplier;
            }
            else if (velY > 0.0f)
            {
                currentGravity *= fallMultiplier;
            }

            velY += currentGravity * deltaTime;
            y += velY * deltaTime;

            if (y >= groundY)
            {
                y = groundY;
                velY = 0.0f;
                grounded = true;
            }

            if (!grounded)
            {
                if (velY < 0.0f)
                {
                    animator.SetState(AnimState::Jump);
                    wasFalling = false;
                }
                else
                {
                    if (!wasFalling)
                    {
                        animator.SetState(AnimState::Fall);
                        wasFalling = true;
                    }
                }
            }
            else
            {
                wasFalling = false;

                if (isMoving)
                    animator.SetState(isRunningInput ? AnimState::Run : AnimState::Walk);
                else
                    animator.SetState(AnimState::Idle);
            }
        }
        else
        {
            actionTimer -= deltaTime;

            if (actionState == ActionState::Dodge)
                x += (flipX ? -1.0f : 1.0f) * dodgeSpeed * deltaTime;

            if (actionTimer <= 0.0f)
                actionState = ActionState::None;
        }

        animator.Update(deltaTime);

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        animator.Render(renderer, x, y, 2.0f, flipX);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(idleTexture);
    SDL_DestroyTexture(walkTexture);
    SDL_DestroyTexture(runTexture);
    SDL_DestroyTexture(jumpTexture);
    SDL_DestroyTexture(fallTexture);
    SDL_DestroyTexture(dashTexture);
    SDL_DestroyTexture(attackTexture);
    SDL_DestroyTexture(dieTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}