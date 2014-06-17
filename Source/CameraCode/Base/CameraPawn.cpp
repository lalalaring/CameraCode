

#include "../CameraCode.h"
#include "CameraPawn.h"

#include <UnrealMath.h>

#include <assert.h>

ACameraPawn::ACameraPawn(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
    Sphere = PCIP.CreateDefaultSubobject<USphereComponent>(this, TEXT("Sphere"));
    Sphere->InitSphereRadius(5.0f);
    RootComponent = Sphere;
    
    CameraArm = PCIP.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArm"));
    CameraArm->AttachParent = RootComponent;

    MainCamera = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("Camera"));
    MainCamera->AttachParent = CameraArm;

    this->MaxDistance = 2000; // 20 meters
    this->MinDistance = 100;  // 1 meters
    this->MaxPitch = -66;
    this->MinPitch = -45;

    this->Yaw = 0;
    this->Roll = 0;

    this->NormDistance = 1.0;
    this->UpdateCameraDistance();

    this->bMouseEvents = true;

}

void ACameraPawn::UpdateCameraDistance()
{
    // Gets distance and pitch angle from max/min and the normalized value
    this->FocusDistance = FMath::Lerp(this->MinDistance, this->MaxDistance, this->NormDistance);
    this->Pitch = FMath::Lerp(this->MinPitch, this->MaxPitch, this->NormDistance);

    CameraArm->TargetArmLength = this->FocusDistance;

    FRotator rot(this->Pitch, this->Yaw, this->Roll);
    CameraArm->SetRelativeRotation(rot, true);
}

void ACameraPawn::SetYaw(float yaw)
{
    this->Yaw = yaw;

    FRotator rot(this->Pitch, this->Yaw, this->Roll);
    CameraArm->SetRelativeRotation(rot, true);
}

void ACameraPawn::SetRoll(float roll)
{
    this->Roll = roll;

    FRotator rot(this->Pitch, this->Yaw, this->Roll);
    CameraArm->SetRelativeRotation(rot, true);
}


void ACameraPawn::SetOrientation(float yaw, float roll)
{
    this->Yaw = yaw;
    this->Roll = roll;

    FRotator rot(this->Pitch, this->Yaw, this->Roll);
    CameraArm->SetRelativeRotation(rot, true);
}

void ACameraPawn::SetOrientationDistance(float distance, float yaw, float roll)
{
    this->Yaw = yaw;
    this->Roll = roll;
    this->NormDistance = distance;

    this->UpdateCameraDistance();
}

void ACameraPawn::MoveForward(float Val)
{
    FRotator rot(0, this->Yaw, 0);
    FVector relDir(Val, 0, 0); // X axis is in/out to the screen
    FVector dir = rot.RotateVector(relDir);

    this->AddMovementInput(relDir, 1);
}

void ACameraPawn::MoveRight(float Val)
{
    FRotator rot(0, this->Yaw, 0);
    FVector relDir(0, Val, 0); // Y axis is right/left to the screen
    FVector dir = rot.RotateVector(relDir);

    this->AddMovementInput(relDir, 1);
}

void ACameraPawn::Zoom(float Val)
{
    this->NormDistance += Val;
    this->UpdateCameraDistance();
}


/*
void ACameraPawn::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
    // Control by axis of the camera (keys and gamepad)
    InputComponent->BindAxis("MoveForward", this, &ACameraPawn::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &ACameraPawn::MoveRight);
    InputComponent->BindAxis("Zoom", this, &ACameraPawn::Zoom);
}
*/