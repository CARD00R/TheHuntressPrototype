// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BowAndArrow/ATHBow.h"
//#include "Components/BoxComponent.h"
#include "AscilaTheHuntress/AscilaTheHuntress.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Weapon/BowAndArrow/ATHArrow.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"
// Sets default values
AATHBow::AATHBow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	//CollisionComp->SetupAttachment(RootComponent);
	//CollisionComp->SetBoxExtent()
	//RootComponent = CollisionComp;
	
	//Mesh
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	MeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AATHBow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AATHBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AATHBow::FireArrow()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner) {

		// Gets the location and rotation of the pawn's eyes
		FVector EyeLocation;
		FRotator EyeRotation;
		GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();
		// Bullet Spread
		float BulletSpread = 0.0f;
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TracerEndPoint = TraceEnd;
		ArrowTransform;
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_BOW, QueryParams))
		{
			// This should be on the arrow tip! 
			/*
			// Blocking hit! Process damage/effects
			AActor* HitActor = Hit.GetActor();
			EPhysicalSurface ObjectSurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			AATHAscila* MyPlayer = Cast<AATHAscila>(MyOwner);
			CheckArrowParameters();
			float ActualDamage = ArrowBaseDamage;
			if (ObjectSurfaceType == SURFACE_HEADARMOUR)
			{
				ActualDamage = 100.0f;
			}
			else if (ObjectSurfaceType == SURFACE_CHESTARMOUR)
			{
				ActualDamage = ActualDamage;
			}
			else if(ObjectSurfaceType == SURFACE_DEFAULTARMOUR)
			{
				ActualDamage = ActualDamage * 0.5f;
			}*/UE_LOG(LogTemp, Error, TEXT("Hit!"));

			ArrowTransform.SetScale3D(BowSize);
			ArrowTransform.SetLocation(GetActorLocation());
			// To convert FRotator to quart we normalize it then quaternion it
			ArrowTransform.SetRotation((UKismetMathLibrary::FindLookAtRotation(EyeLocation, Hit.ImpactPoint).GetNormalized()).Quaternion());
		}
		
		SpawnArrow();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Owner Found!"));
	}
}
void AATHBow::SpawnArrow()
{
	AATHArrow* DeferredArrow = Cast<AATHArrow>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ArrowClass, ArrowTransform,
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, this));
	if (DeferredArrow != nullptr)
	{
		DeferredArrow->SetPowerVelocity(1000.0f);
		UGameplayStatics::FinishSpawningActor(DeferredArrow, ArrowTransform);
		UE_LOG(LogTemp, Error, TEXT("Spawned Arrow"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Arrow"));
	}

}
void AATHBow::CheckArrowParameters()
{
	EquippedArrowType = Cast<AATHArrow>(ArrowClass);
	if(EquippedArrowType != nullptr)
	{
		ArrowBaseDamage = EquippedArrowType->GetDamage();
	}
}

