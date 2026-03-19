#include "Animator.h"

void Animator::AddAnimation(AnimState state, const Animation& anim)
{
    animations[state] = anim;
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
    const Animation& anim = animations[state];

    frameTime += deltaTime;

    if (frameTime >= anim.frameDuration)
    {
        frameTime = 0.0f;
        currentFrame = (currentFrame + 1) % anim.frameCount;
    }
}

void Animator::Render(SDL_Renderer* renderer, float x, float y, float scale, bool flipX)
{
    const Animation& anim = animations[state];

    SDL_FRect src;
    src.x = static_cast<float>(currentFrame * anim.frameWidth);
    src.y = 0.0f;
    src.w = static_cast<float>(anim.frameWidth);
    src.h = static_cast<float>(anim.frameHeight);

    SDL_FRect dst;
    dst.x = x;
    dst.y = y;
    dst.w = static_cast<float>(anim.frameWidth) * scale;
    dst.h = static_cast<float>(anim.frameHeight) * scale;

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