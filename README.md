# 언리얼 제작 (진행중)

[![12412457](https://user-images.githubusercontent.com/48274630/167828062-771ef76c-b375-4fc2-90e4-f92da2500628.PNG)](https://pjs5741.tistory.com/25)

---

### 무기장착
<details>
<summary>MyCharacter.cpp</summary>
<div markdown="1">

```
void AMyCharacter::EquipWeapon()
{
	if (IsEquip ||IsAttacking || IsRolling) return;

	else if (nullptr != WeaponItemClass)
			{
				GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 0.0f);
				GetCharacterMovement()->MaxWalkSpeed = 0.0f;
				MyAnim->GetSetWeapon(true);
				IsEquip = true;
				MyAnim->PlayEquipWeaponMontage();
				if (CanSetWeapon())
				{
					auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
					SetWeapon(NewWeapon);
				}
				else if (CanSetWeapon() == false)
				{
					MyAnim->GetSetWeapon(false);
					CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
					CurrentWeapon->Destroy();
					CurrentWeapon = nullptr;
				}
			}
	
	
}
```



```
void AMyCharacter::OnEquipWeaponMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	IsEquip = false;
}
```

</div>
</details>


무기의 장착여부를 IsEquip에 저장하여서 맨손일떄의 걷기모션이나 공격모션등을 무기장착시와 다르게 만들수있다.

![12412457](https://user-images.githubusercontent.com/48274630/167831976-7d1bacc9-c60d-49da-b579-9e3f263b8821.PNG)


![22](https://user-images.githubusercontent.com/48274630/167832336-47b50a02-de9e-4251-a717-b274cd050a20.PNG)
![21](https://user-images.githubusercontent.com/48274630/167832342-17bd73f6-63b5-43a3-9d4b-6645d3b7b5c3.PNG)


### 회피구르기

<details>
<summary>MyCharacter.cpp</summary>
<div markdown="1">

```
void AMyCharacter::Rolling()
{

	if (IsRolling||IsAttacking|| IsEquip) return;

	

	MyAnim->PlayRollingMontage();
	IsRolling = true;
	SetCanBeDamaged(false);
}
```

</div>
</details>

구르는 중에 SetCanBeDamaged()를 false로 바꾸어서 무적이 되게 만든다.


### 보스BT

![23](https://user-images.githubusercontent.com/48274630/167833527-015b9288-a5e8-431c-a651-3a0346eb534c.PNG)

보스의 범위안에 들어가게되면 거리와 1,2의 랜덤함수에 따라 행동유형을 달리한다.

<details>
<summary>BTService_random.cpp</summary>
<div markdown="1">

```
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Random.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_Random::UBTService_Random()
{
	NodeName = TEXT("Result");
	Interval = 1.0f;

	bNotifyBecomeRelevant = true;
}

void UBTService_Random::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	int x = 1;
	x = FMath::RandRange(1, 10);



	if (x <= 7)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyAIController::Result, 1);
	}
	else if (x <= 10)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyAIController::Result, 2);
	}
	

}
```

</div>
</details>



<details>
<summary>BTDecorator_Distance.cpp</summary>
<div markdown="1">

```
// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Distance.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_Distance::UBTDecorator_Distance()
{
	NodeName = TEXT("Distance>=1000");
}

bool UBTDecorator_Distance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return false;
	bResult = (Target->GetDistanceTo(ControllingPawn) >= 1000.0f);
	return bResult;

}


```

</div>
</details>

### 돌진공격

보스의 공격범위에들어가있지 않고, 거리가 1000이하면 돌진공격을 한다. 돌진공격은 LaunchCharacter함수를 사용하였다.


<details>
<summary>MyCharacter.cpp</summary>
<div markdown="1">

```
void ABossCharacter::Attack2()
{
	if (IsAttacking) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	GetCharacterMovement()->BrakingFrictionFactor = 0.f;
	LaunchCharacter(Direction * 1100, true, true);
	
	MyAnim->PlayAttackMontage2();
	IsAttacking = true;
}
```

</div>
</details>