//
// CameraComponent.cpp
//
// Clark Kromenaker
//
#include "CameraComponent.h"
#include "Services.h"
#include "SDLRenderer.h"

const float kCameraSpeed = 50.0f;
const float kRunCameraMultiplier = 2.0f;
const float kCameraRotationSpeed = 1.0f;

CameraComponent::CameraComponent(Actor* owner) : Component(owner)
{
    Services::GetRenderer()->SetCamera(this);
}

void CameraComponent::Update(float deltaTime)
{
    float camSpeed = kCameraSpeed;
    if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_LSHIFT))
    {
        camSpeed = kCameraSpeed * kRunCameraMultiplier;
    }
    
    if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_W))
    {
        mOwner->Translate(mOwner->GetForward() * (camSpeed * deltaTime));
    }
    else if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_S))
    {
        mOwner->Translate(mOwner->GetForward() * (-camSpeed * deltaTime));
    }
    
    if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_E))
    {
        mOwner->Translate(Vector3(0.0f, camSpeed * deltaTime, 0.0f));
    }
    else if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_Q))
    {
        mOwner->Translate(Vector3(0.0f, -camSpeed * deltaTime, 0.0f));
    }
    
    if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_UP))
    {
        mOwner->Translate(Vector3(camSpeed * deltaTime, 0.0f, 0.0f));
    }
    else if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_DOWN))
    {
        mOwner->Translate(Vector3(-camSpeed * deltaTime, 0.0f, 0.0f));
    }
    
    if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_RIGHT))
    {
        mOwner->Translate(Vector3(0.0f, 0.0f, camSpeed * deltaTime));
    }
    else if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_LEFT))
    {
        mOwner->Translate(Vector3(0.0f, 0.0f, -camSpeed * deltaTime));
    }
    
    if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_A))
    {
        mOwner->Rotate(Vector3::UnitY, kCameraRotationSpeed * deltaTime);
    }
    else if(Services::GetInput()->IsKeyPressed(SDL_SCANCODE_D))
    {
        mOwner->Rotate(Vector3::UnitY, -kCameraRotationSpeed * deltaTime);
    }
}

Matrix4 CameraComponent::GetLookAtMatrix()
{
    Vector3 eye = mOwner->GetPosition();
    Vector3 lookAt = mOwner->GetPosition() + mOwner->GetForward() * 5.0f;
    //Vector3 lookAt = mOwner->GetPosition() - Vector3::UnitZ * 5.0f;
    Vector3 up = Vector3::UnitY;
    Matrix4 lookAtMat = Matrix4::MakeLookAt(eye, lookAt, up);
    return lookAtMat;
}

Matrix4 CameraComponent::GetProjectionMatrix()
{
    Matrix4 projMat = Matrix4::MakePerspective(fovAngleRad, 1.333f,
                                               nearClippingPlane,
                                               farClippingPlane);
    return projMat;
}
