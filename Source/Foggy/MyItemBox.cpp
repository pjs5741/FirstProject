// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItemBox.h"
#include "MyWeapon.h"
#include "MyCharacter.h"

// Sets default values
AMyItemBox::AMyItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));
	WeaponItemClass = AMyWeapon::StaticClass();
}

// Called when the game starts or when spawned
void AMyItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyItemBox::OnCharacterOverlap);
}

void AMyItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto MyCharacter = Cast<AMyCharacter>(OtherActor);

	if (nullptr != MyCharacter && nullptr != WeaponItemClass)
	{
		if (MyCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			MyCharacter->SetWeapon(NewWeapon);
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect -> OnSystemFinished.AddDynamic(this, &AMyItemBox::OnEffectFinished);
		}
	
	}
}

void AMyItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}