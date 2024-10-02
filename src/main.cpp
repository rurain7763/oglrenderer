#include "Engine/Engine.h"

int main() {
    Engine engine;
    engine.Init();
    engine.Run();
    engine.Destroy();

    return 0;
}