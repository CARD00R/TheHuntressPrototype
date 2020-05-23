// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BowAndArrow/ATHBow.h"
#include "Character/Ascila/ATHAscila.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Weapon/BowAndArrow/ATHArrow.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "AscilaTheHuntress/AscilaTheHuntress.h"
#include "Kismet/KismetMathLibrary.h"
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
	AATHAscila* AscilaOwner = Cast<AATHAscila>(MyOwner);
	
	if (AscilaOwner) {
		// Spawn Params
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		FHitResult Hit;
		
		// Gets the location and rotation of the pawn's eyes
		FVector EyeLocation;
		FRotator EyeRotation;
		GetActorEyesViewPoint(EyeLocation, EyeRotation);
		EyeRotation.Pitch = AscilaOwner->GetPitch();
		FVector ShotDirection = EyeRotation.Vector();
		
		// Bullet Spread
		float BulletSpread = 0.0f;
		float HalfRad = FMath::DegreesToRadians(BulletSpread);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);

		// Spawn Arrow Transform
		ArrowTransform.SetScale3D(BowSize);
		ArrowTransform.SetLocation(EyeLocation);	
		// To convert FRotator to quart we normalize it then quaternion it
		ArrowTransform.SetRotation(EyeRotation.GetNormalized().Quaternion());

		
		FVector CamLocation = AscilaOwner->GetCameraManager()->GetCameraLocation();
		FVector CamForward = AscilaOwner->GetCameraManager()->GetCameraRotation().Vector();
		FVector TraceEnd = CamLocation + (CamForward * 70000);
		
		if (GetWorld()->LineTraceSingleByChannel(Hit, CamLocation, TraceEnd, COLLISION_BOW, QueryParams))
		{
			//Hit.ImpactPoint
			//DrawDebugLine(GetWorld(), CamLocation, TraceEnd, FColor::Blue, true, -1, 0, 3);
			// To convert FRotator to quart we normalize it then quaternion it
			ArrowTransform.SetRotation((UKismetMathLibrary::FindLookAtRotation(EyeLocation, Hit.ImpactPoint).GetNormalized()).Quaternion());
			UE_LOG(LogTemp, Error, TEXT("HIT"));
		}
		else
		{
			ArrowTransform.SetRotation((UKismetMathLibrary::FindLookAtRotation(EyeLocation, TraceEnd).GetNormalized()).Quaternion());
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
		DeferredArrow->SetPowerVelocity(3000.0f);
		UGameplayStatics::FinishSpawningActor(DeferredArrow, ArrowTransform);
	}
	else
	{
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

