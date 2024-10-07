#ifndef Engine_H
#define Engine_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>

class Engine {
public:
    Engine();
    ~Engine() = default;

    void Init();
    void Run();
    void Destroy();

private:
    void Setup();
    void ProcessInput();
    void Update();
    void Render();

private:
    const int FPS = 60;
    const int MILLISECS_PER_FRAME = 1000 / FPS;

    bool _isRunning;
    int _prevFrameMilliSecs;
    float _deltaTime;

    SDL_Window* _window;
    int _windowWidth, _windowHeight;
    SDL_GLContext _glContext;

    class VertexArray* _vao;
    class VertexBuffer* _vbo;
    class IndexBuffer* _ibo;
    class Shader* _shader;

    glm::mat4 _perspective;
    class Camera* _camera;
};

#endif