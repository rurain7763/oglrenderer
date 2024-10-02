#include "Engine.h"

Engine::Engine() 
    : _isRunning(false)
{

}

void Engine::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef __APPLE__
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
#endif

    int windowWidth = 1024;
    int windowHeight = 720;
    _window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL
    );

    _glContext = SDL_GL_CreateContext(_window);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        // TODO: glew init error
        return;
    }

    _isRunning = true;
}

void Engine::Run() {
    while(_isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Engine::Destroy() {
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Engine::ProcessInput() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT:
                _isRunning = false;
                break;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    _isRunning = false;
                }
        }
    }
}

void Engine::Update() {
    // TODO:
}

void Engine::Render() {
    SDL_GL_SwapWindow(_window);
}