//
//  Renderer.h
//  GEngine
//
//  Created by Clark Kromenaker on 7/22/17.
//
#pragma once
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "Matrix4.h"
#include "Vector2.h"

class Shader;
class Camera;
class MeshRenderer;
class Model;
class BSP;
class Skybox;

class Renderer
{
public:
    bool Initialize();
    void Shutdown();
    
    void Render();
    
    void SetCamera(Camera* camera) { mCamera = camera; }
    Camera* GetCamera() { return mCamera; }
	
    void AddMeshRenderer(MeshRenderer* mc);
    void RemoveMeshRenderer(MeshRenderer* mc);
    
    void SetBSP(BSP* bsp) { mBSP = bsp; }
    
    void SetSkybox(Skybox* skybox) { mSkybox = skybox; }
    
    int GetWindowWidth() { return mScreenWidth; }
	int GetWindowHeight() { return mScreenHeight; }
	
	Vector2 GetWindowSize() { return Vector2(mScreenWidth, mScreenHeight); }
    
private:
    // Screen's width and height, in pixels.
    int mScreenWidth = 1024;
    int mScreenHeight = 768;
    
    // Handle for the window object (contains the game).
    SDL_Window* mWindow = nullptr;
    
    // Context handle for rendering in OpenGL.
    SDL_GLContext mContext;
    
    // Default shader.
    Shader* mDefaultShader = nullptr;
    
    // Our camera in the scene - we currently only support one.
    Camera* mCamera = nullptr;
	
	// All loaded shaders.
	std::vector<Shader*> mShaders;
    
    // List of mesh components to render.
    std::vector<MeshRenderer*> mMeshRenderers;
	
    // A BSP to render.
    BSP* mBSP = nullptr;
    
    // A skybox to render.
    Skybox* mSkybox = nullptr;
    
    // Skybox shader.
    Shader* mSkyboxShader = nullptr;
	
	Shader* LoadShader(std::string name);
};
