#ifndef Engine_H
#define Engine_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

class Engine {
public:
    Engine();
    ~Engine() = default;

    void Init();
    void Run();
    void Destroy();

private:
    void ProcessInput();
    void Update();
    void Render();

private:
    const int FPS = 60;
    const int MILLISECS_PER_FRAME = 1000 / FPS;

    bool _isRunning;
    int _prevFrameMilliSecs;

    SDL_Window* _window;
    SDL_GLContext _glContext;
};

#endif