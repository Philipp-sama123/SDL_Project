#include "Animator.h"

void Animator::AddAnimation(AnimState animState, const Animation& anim)
{
    animations[animState] = anim;
}

void Animator::SetState(AnimState newState)
{
    if (state != newState)
    {
        state = newState;
        currentFrame = 0;
        frameTime = 0.0f;
    }
}

void Animator::Update(float deltaTime)
{
    auto it = animations.find(state);
    if (it == animations.end())
        return;

    const Animation& anim = it->second;
    if (anim.frameCount <= 0 || anim.frameDuration <= 0.0f)
        return;

    frameTime += deltaTime;

    while (frameTime >= anim.frameDuration)
    {
        frameTime -= anim.frameDuration;

        if (anim.loop)
        {
            currentFrame = (currentFrame + 1) % anim.frameCount;
        }
        else
        {
            if (currentFrame < anim.frameCount - 1)
                ++currentFrame;
        }
    }
}

void Animator::Render(SDL_Renderer* renderer, float x, float y, float scale, bool flipX)
{
    auto it = animations.find(state);
    if (it == animations.end())
        return;

    const Animation& anim = it->second;
    if (!anim.texture || anim.frameWidth <= 0 || anim.frameHeight <= 0)
        return;

    SDL_FRect src{
        static_cast<float>(currentFrame * anim.frameWidth),
        0.0f,
        static_cast<float>(anim.frameWidth),
        static_cast<float>(anim.frameHeight)
    };

    SDL_FRect dst{
        x,
        y,
        static_cast<float>(anim.frameWidth) * scale,
        static_cast<float>(anim.frameHeight) * scale
    };

    SDL_FlipMode flip = flipX ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    SDL_RenderTextureRotated(
        renderer,
        anim.texture,
        &src,
        &dst,
        0.0,
        nullptr,
        flip
    );
}