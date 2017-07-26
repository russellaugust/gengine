//
//  GEngine.h
//  GEngine
//
//  Created by Clark Kromenaker on 7/22/17.
//
#pragma once

#include "SDLRenderer.h"

class GEngine
{
public:
    GEngine();
    
    //TODO: Return more meaningful status code.
    bool Initialize();
    void Shutdown();
    void Run();
    
    void Quit();
    
private:
    void ProcessInput();
    void Update();
    void GenerateOutput();
    
    SDLRenderer mRenderer;
    
    bool mRunning;
};
