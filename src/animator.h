#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>

enum class AnimState
{
    Idle,
    Walk,
    Run,
    Jump,
    Fall,
    Attack,
    Dodge
};

struct Animation
{
    SDL_Texture* texture = nullptr;
    int frameWidth = 0;
    int frameHeight = 0;
    int frameCount = 0;
    float frameDuration = 0.1f;
    bool loop = true;
};

class Animator
{
public:
    void AddAnimation(AnimState animState, const Animation& anim);
    void SetState(AnimState newState);
    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer, float x, float y, float scale, bool flipX);

private:
    std::unordered_map<AnimState, Animation> animations;

    AnimState state = AnimState::Idle;
    int currentFrame = 0;
    float frameTime = 0.0f;
};