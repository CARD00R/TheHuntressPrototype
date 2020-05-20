// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BowAndArrow/ATHArrow.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AATHArrow::AATHArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Hit Coll
	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->SetupAttachment(RootComponent);
	HitCollision->SetSphereRadius(3.0f);
	RootComponent = HitCollision;
	// Arrow Mesh
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(HitCollision);
	MeshComp->SetRelativeScale3D(ArrowSize);
	MeshComp->SetRelativeLocation(ArrowStartLocation);
	MeshComp->SetRelativeRotation(ArrowStartRotation);
	MeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//Projectile Movement Comp
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
}

// Called when the game starts or when spawned
void AATHArrow::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ArrowRotationHandle, this, &AATHArrow::ArrowRotation, 0.01, true);
}


// Called every frame
void AATHArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AATHArrow::ArrowRotation()
{
	FRotator NewArrowRotation = FRotator(GetVelocity().ToOrientationRotator().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	SetActorRotation(NewArrowRotation);
	
}

void AATHArrow::SetPowerVelocity(float PowerVel)
{
	ProjectileMovementComp->InitialSpeed = PowerVel;
}
