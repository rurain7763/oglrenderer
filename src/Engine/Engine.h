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
    bool _isRunning;

    SDL_Window* _window;
    SDL_GLContext _glContext;
};

#endif