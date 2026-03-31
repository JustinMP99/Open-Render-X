#include <iostream>
#include "Header/Engine.h"

int main(int, char**)
{
    Engine engine;   
    engine.Initialize();
    engine.Loop();
    engine.Shutdown();
   return 0;
}
