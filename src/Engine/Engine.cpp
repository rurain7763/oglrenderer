#include "Engine.h"
#include "../Logger/Logger.h"
#include "../Shader/Shader.h"
#include "../VertexArray/VertexArray.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../VertexBuffer/VertexBufferLayoutGroup.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../Matrix/Matrix.h"
#include "../Vertex/Vertex.h"

Engine::Engine() 
    : _isRunning(false), _prevFrameMilliSecs(0)
{

}

void Engine::Init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif

    _windowWidth = 1024;
    _windowHeight = 720;
    _window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        _windowWidth,
        _windowHeight,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_OPENGL
    );

    _glContext = SDL_GL_CreateContext(_window);

    GLenum res = glewInit();
    if (res != GLEW_OK) {
        LOG_ERR("glew init error");
        return;
    }

    glFrontFace(GL_CW);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    _isRunning = true;
}

void Engine::Run() {
    Setup();
    while(_isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Engine::Destroy() {
    delete _shader;
    delete _iboLine;
    delete _ibo;
    delete _vboLine;
    delete _vbo;
    delete _vaoLine;
    delete _vao;

    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Engine::Setup() {
    const float aspect = _windowHeight / static_cast<float>(_windowWidth);

    _perspective = Perspective(45.f, aspect, 0.1f, 100.f);

    _vao = new VertexArray();
    _vaoLine = new VertexArray();
    _vbo = new VertexBuffer();
    _vboLine = new VertexBuffer();
    _ibo = new IndexBuffer();
    _iboLine = new IndexBuffer();
    _shader = new Shader();

    _vao->Bind();

    Vertex vertices[] = {
        { glm::vec3(-0.3f, -0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(-0.3f, 0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, 0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, -0.3f, -0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(-0.3f, -0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(-0.3f, 0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) },
        { glm::vec3(0.3f, -0.3f, 0.3f), glm::vec3(1.0, 1.0, 1.0) }
    };
    _vbo->Init(vertices, sizeof(vertices));

    GLuint indices[] = { 
        0, 1, 2, 
        0, 2, 3,
        7, 3, 2,
        7, 2, 6,
        4, 5, 6,
        4, 6, 7,
        0, 4, 7,
        0, 7, 3,
        1, 5, 6,
        1, 6, 2,
        0, 1, 5,
        0, 5, 4
    };
    _ibo->Init(indices, sizeof(indices) / sizeof(GLuint));

    VertexBufferLayoutGroup layoutGroup;
    layoutGroup.Push<float>(3);
    layoutGroup.Push<float>(3);

    _vao->Init(*_vbo, layoutGroup);

    _vaoLine->Bind();

    Vertex lineVertices[] = {
        { glm::vec3(-0.3f, -0.3f, -0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(-0.3f, 0.3f, -0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(0.3f, 0.3f, -0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(0.3f, -0.3f, -0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(-0.3f, -0.3f, 0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(-0.3f, 0.3f, 0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0, 0.0, 0.0) },
        { glm::vec3(0.3f, -0.3f, 0.3f), glm::vec3(1.0, 0.0, 0.0) }
    };
    _vboLine->Init(lineVertices, sizeof(lineVertices));

    GLuint indicesLine[] = {
        0, 1, 1, 2, 2, 3, 3, 0,
        4, 5, 5, 6, 6, 7, 7, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };
    _iboLine->Init(indicesLine, sizeof(indicesLine) / sizeof(GLuint));

    _vaoLine->Init(*_vboLine, layoutGroup);

    _shader->Init("./assets/shader/shader.vs", "./assets/shader/shader.fs");
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
    int timeToWait = (_prevFrameMilliSecs + MILLISECS_PER_FRAME) - SDL_GetTicks();
    if(timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - _prevFrameMilliSecs) / 1000.f;
    _prevFrameMilliSecs = SDL_GetTicks();
}

void Engine::Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 translate = Translation(0, 0, 1.5);
    glm::mat4 rotateX = RotationX(SDL_GetTicks() / 1000.f);
    glm::mat4 rotateY = RotationY(SDL_GetTicks() / 1000.f);
    glm::mat4 rotateZ = RotationZ(0);
    glm::mat4 scale = Scaling(1, 1, 1);
    glm::mat4 worldMat = WorldMatrix(translate, rotateX * rotateY * rotateZ, scale);

    glm::mat4 mvp = worldMat * _perspective;

    _vao->Bind();
    _shader->Bind();
    _shader->SetUniform<glm::mat4>("mvp", mvp);
    _ibo->Bind();
    glDrawElements(GL_TRIANGLES, _ibo->GetCount(), GL_UNSIGNED_INT, 0);

    _vaoLine->Bind();
    _shader->Bind();
    _shader->SetUniform<glm::mat4>("mvp", mvp);
    _iboLine->Bind();
    glDrawElements(GL_LINES, _iboLine->GetCount(), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(_window);
}