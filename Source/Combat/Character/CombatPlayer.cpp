// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatPlayer.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Combat/CombatGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

/** 显示debug信息 */
bool GShowDebugMsg = false;
/** 显示debug绘制 */
bool GShowDebugDraw = false;

/** 计算角度 */
inline float GetAngle(const FVector V1, const FVector V2)
{
	return 180.f / PI * FMath::Acos(FVector::DotProduct(V1.GetSafeNormal(),V2.GetSafeNormal()));
}

/** 长按的输入类型 */
static const TArray<EInputBufferingType> HoldInputTypeArr = {
	EInputBufferingType::E_HoldFront, EInputBufferingType::E_HoldBack,
	EInputBufferingType::E_HoldLeft, EInputBufferingType::E_HoldRight
};

ACombatPlayer::ACombatPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 不受控制器旋转影响
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 摄像机的弹簧臂
	CameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	CameraSpringArm->SetupAttachment(RootComponent);
	CameraSpringArm->TargetArmLength = 700.f;
	CameraSpringArm->bEnableCameraRotationLag = true;
	CameraSpringArm->CameraRotationLagSpeed = 20.f;
	CameraSpringArm->bEnableCameraLag = true;
	CameraSpringArm->CameraLagSpeed = 5.f;
	CameraSpringArm->bUsePawnControlRotation = true;

	// 跟随玩家的摄像机
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraSpringArm);

	// 设置默认Controller
	AIControllerClass = APlayerController::StaticClass();

	// 开启角色朝向移动方向
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate.Yaw = 720.f;
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = .3f;
}

void ACombatPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 获取并转换游戏实例
	CombatGameInstance = Cast<UCombatGameInstance>(GetGameInstance());
	if(CombatGameInstance)
	{
		GShowDebugMsg = CombatGameInstance->bShowDebugMsg;
		GShowDebugDraw = CombatGameInstance->bShowDebugDraw;
	}

	if (Controller)
	{
		PlayerController = Cast<APlayerController>(Controller);

		// 调整摄像机初始角度
		FRotator ControllerRotation = GetControlRotation();
		ControllerRotation += DefaultControllerRotation;
		PlayerController->SetControlRotation(ControllerRotation);

		// 设置镜头旋转角度范围
		PlayerController->PlayerCameraManager->ViewPitchMin = DefaultControllerRotation.Pitch + ViewPitchRangeMin;
		PlayerController->PlayerCameraManager->ViewPitchMax = DefaultControllerRotation.Pitch + ViewPitchRangeMax;
	}

	// 读取MontageDataTable,分类存入数组
	ReadMontage();
	for (FMontageInfo* MontageInfo : CurrentMontageInfoArr)
	{
		UE_LOG(LogTemp, Log, TEXT("ReadMontage: %s"), *MontageInfo->Name);
		if (GShowDebugMsg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, MontageInfo->Name);
		}
	}
}


void ACombatPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CharacterState.bIsDead)
	{
		return;
	}

	// 增加蓄力攻击时间
	if (bIsStartChargeNormalAttack)
	{
		CurrentChargeNormalAttackTime += DeltaTime;
	}
	if (LockActor)
	{
		if (GShowDebugDraw)
		{
			DrawDebugLine(GetWorld(), GetActorLocation(), LockActor->GetActorLocation(), FColor::Purple, false, 0, 0, 3.f);	
		}
		
		// 人物转向锁定对象
		if (!bIsDodge && CharacterState.bCanRotate)
		{
			const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockActor->GetActorLocation());	// 获取人物与目标旋转角度
			const FRotator LookAtYaw(0.f, LookAtRotation.Yaw, 0.f); // 只抽取Z轴旋转
			const FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, 15);	// 插值
			SetActorRotation(InterpRotation);	// 设置转向
		}

		// 调整镜头，以保证能同时容下人物与目标
		if (PlayerController)
		{
			APlayerCameraManager* CamManager = PlayerController->PlayerCameraManager;

			if (CamManager)
			{
				const FVector CameraLocation = CamManager->GetCameraLocation(); // 镜头位置
				const FVector CameraForwardVec = CamManager->GetActorForwardVector(); // 镜头前方方向

				const float CameraFOVAngle = CamManager->GetFOVAngle(); // 获取镜头FOV
				const FVector CameraToLockActor = (LockActor->GetActorLocation() - CameraLocation).GetSafeNormal();	// 镜头到目标方向的向量

				const float Angle = GetAngle(CameraToLockActor, CameraForwardVec);	// 镜头前方与目标方向的角度
				float DifAngle = Angle - CameraFOVAngle / 2;	// FOV外的差值角度
				DifAngle += 5.f;	// 额外再空出一定的角度
				// 根据角度旋转镜头
				if (DifAngle > 0)
				{
					FRotator ControllerRotation = GetControlRotation();

					// 通过到目标方向向量到摄像机右方向量的投影，判断旋转方向
					const FVector CameraRightVec = CamManager->GetActorRightVector();
					const int Direction = (CameraToLockActor | CameraRightVec) > 0 ? 1 : -1;
					ControllerRotation.Yaw += DifAngle * Direction;

					ControllerRotation = FMath::RInterpTo(GetControlRotation(), ControllerRotation, DeltaTime, 10.f);
					Controller->SetControlRotation(ControllerRotation);
				}
			}
		}
		
	}

	// 输入缓存
	InputBuffering.push_front(CurrentInputBuffering);	// 记录当前帧输入
	if (InputBuffering.size() > MaxInputBufferSize)
	{
		// 清除过期输入
		InputBuffering.pop_back();
	}
	if (!CurrentInputBuffering.empty()
		&& (*CurrentInputBuffering.rbegin() == EInputBufferingType::E_NormalAttack || *CurrentInputBuffering.rbegin() == EInputBufferingType::E_Dodge)
	)
	{
		// 解析输入指令
		ParseInputBuffering();
	}
	CurrentInputBuffering.clear();	// 清空当前帧输入

	if (GShowDebugMsg)
	{
		// 锁定状态
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString(bIsLocking ? "Lock" : "ReleaseLock"));

		// 输入缓存
		FString InputString;
		for (std::deque<std::deque<EInputBufferingType>>::iterator Iter = InputBuffering.end(); Iter != InputBuffering.begin();)
		{
			--Iter;
			for (std::deque<EInputBufferingType>::iterator Iter2 = Iter->begin(); Iter2 != Iter->end(); ++Iter2)
			{
				if (HoldInputTypeArr.Contains(*Iter2))
					continue;
				UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInputBufferingType"), true);
				
				InputString += EnumPtr->GetDisplayNameTextByIndex(static_cast<int32>(*Iter2)).ToString() + " ";
			}
		}
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, "InputList: " + InputString);
	}
	
	if (GShowDebugDraw)
	{
		// 锁定范围
		DrawDebugSphere(GetWorld(), GetActorLocation(), LockDistance, 32,  FColor::Red, false, 0, 0, 10.f);
		
		// 输入方向
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (InputForwardDirection + InputRightDirection) * 100, FColor::Black, false, 0, 0, 15.f);
		
		// 可锁定对象画线
		const TArray<AActor*> LockArr = GetLockArr();
		FString LockArrStr;
		for (int i = 0; i < LockArr.Num(); ++i)
		{
			// const float Distance = this->GetDistanceTo(LockArr[i]);
			// LockArrStr += LockArr[i]->GetName() + "  " + FString::SanitizeFloat(Distance) + " ,";
			DrawDebugLine(GetWorld(), GetActorLocation(), LockArr[i]->GetActorLocation(), FColor::Silver, false, 0, 0, 2.f);
		}
	}

}

void ACombatPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/** 摄像机旋转 */
	PlayerInputComponent->BindAxis("CameraRotationX", this, &ACombatPlayer::CameraRotationX);
	PlayerInputComponent->BindAxis("CameraRotationY", this, &ACombatPlayer::CameraRotationY);
	
	/** 输入移动 */
	PlayerInputComponent->BindAxis("MoveForward", this, &ACombatPlayer::InputMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACombatPlayer::InputMoveRight);

	/** 跳跃 */
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACombatPlayer::PressedJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACombatPlayer::ReleasedJump);
	
	/** 输入攻击 */
	PlayerInputComponent->BindAction("NormalAttack", IE_Pressed, this, &ACombatPlayer::PressedNormalAttack);
	PlayerInputComponent->BindAction("NormalAttack", IE_Released, this, &ACombatPlayer::ReleasedNormalAttack);
	
	/** 锁定 */
	PlayerInputComponent->BindAction("Lock", IE_Pressed, this, &ACombatPlayer::Lock);
	PlayerInputComponent->BindAction("Lock", IE_Released, this, &ACombatPlayer::ReleaseLock);

	/** 闪避 */
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &ACombatPlayer::PressedDodge);

	/** debug信息*/
	PlayerInputComponent->BindAction("ShowDebugMsg", IE_Pressed, this, &ACombatPlayer::ShowDebugMsg);
	PlayerInputComponent->BindAction("ShowDebugDraw", IE_Pressed, this, &ACombatPlayer::ShowDebugDraw);
	PlayerInputComponent->BindAction("GetHit", IE_Pressed, this, &ACombatPlayer::PressedToGetHit);
	PlayerInputComponent->BindAction("RecoveryHealth", IE_Pressed, this, &ACombatPlayer::PressedToRecoveryHealth);

}

/** 按下普通攻击 */
void ACombatPlayer::PressedNormalAttack()
{
	UE_LOG(LogTemp, Log, TEXT("PressedNormalAttack"));
	// if (GShowDebugMsg)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("PressedNormalAttack"));
	// }
	bIsStartChargeNormalAttack = true;
	CurrentInputBuffering.push_front(EInputBufferingType::E_NormalAttack);
}

/** 释放普通攻击 */
void ACombatPlayer::ReleasedNormalAttack()
{
	UE_LOG(LogTemp, Log, TEXT("ReleasedNormalAttack"));
	// if (GShowDebugMsg)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("ReleasedNormalAttack"));
	// }
	if (CurrentChargeNormalAttackTime > ChargeAttackTime)
	{
		CurrentInputBuffering.push_front(EInputBufferingType::E_ChargeNormalAttack);
		if (GShowDebugMsg)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "ChargeAttack");
		}
	}
	CurrentChargeNormalAttackTime = 0;
	bIsStartChargeNormalAttack = false;
}


/** 镜头水平旋转 */
void ACombatPlayer::CameraRotationX(const float Rate)
{
	AddControllerYawInput(Rate * BaseCameraRotationRateX * GetWorld()->GetDeltaSeconds());
}

/** 镜头垂直旋转 */
void ACombatPlayer::CameraRotationY(const float Rate)
{
	AddControllerPitchInput(Rate * BaseCameraRotationRateY * GetWorld()->GetDeltaSeconds());
}

/** 输入前后移动 */
void ACombatPlayer::InputMoveForward(const float Rate)
{
	if (Controller == nullptr)
		return;

	// 获取旋转角度
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	// 计算屏幕前方方向
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	// 输入向量到角色前方向量的投影
	// const float Projection = Direction | GetForwardDirection();
	if (CanMove(Rate))
	{
		AddMovementInput(Direction, Rate);
	}

	// 输入方向向量
	InputForwardDirection = Direction * Rate;
	// 输入缓存
	MoveInputBuffering(PreForwardRate, Rate, InputForwardDirection);
	PreForwardRate = Rate;
}

/** 输入左右移动 */
void ACombatPlayer::InputMoveRight(const float Rate)
{
	if (Controller == nullptr)
		return;
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	// add movement in that direction
	if (CanMove(Rate))
	{
		AddMovementInput(Direction, Rate);
	}

	// 输入方向
	InputRightDirection = Direction * Rate;

	// 输入缓存
	MoveInputBuffering(PreRightRate, Rate, InputRightDirection);
	PreRightRate = Rate;
}

/** 方向输入缓存 */
void ACombatPlayer::MoveInputBuffering(const float PreRate, const float Rate, const FVector InputDirection)
{
	if (fabs(Rate) < 1.f)
	{
		return;
	}
		
	EInputBufferingType PressedType;
	EInputBufferingType HoldType;
	// 计算输入向量与角色前方向量的夹角
	const float Angle = GetAngle(InputDirection, GetForwardDirection());
	if (Angle <= 45)
	{
		// 存入向前指令
		PressedType = EInputBufferingType::E_Front;
		HoldType = EInputBufferingType::E_HoldFront;
	}
	else if(Angle < 135)
	{
		// 存入向左或向右指令
		const float Projection = InputDirection | GetRightDirection();
		PressedType = Projection > 0 ? EInputBufferingType::E_Right : EInputBufferingType::E_Left;
		HoldType = Projection > 0 ? EInputBufferingType::E_HoldRight : EInputBufferingType::E_HoldLeft;
	}
	else
	{
		// 存入向后指令
		PressedType = EInputBufferingType::E_Back;
		HoldType = EInputBufferingType::E_HoldBack;
	}
	if (PreRate != Rate)
	{
		// 按下指令（当前输入量为1，且上一帧输入量不为1时）
		CurrentInputBuffering.push_front(PressedType);
	}
	// 长按指令
	CurrentInputBuffering.push_front(HoldType);

}

/** 锁定 */
void ACombatPlayer::Lock()
{
	bIsLocking = true; // 修改锁定状态
	GetCharacterMovement()->bOrientRotationToMovement = false;	// 关闭角色朝向移动方向
	const TArray<AActor*> LockArr = GetLockArr(); // 获取可锁定对象列表

	if (LockArr.Num() > 0)
	{
		// 获取距离最近的对象
		float MinDistance = this->GetDistanceTo(LockArr[0]);
		AActor* MinDistanceActor = LockArr[0];
		for (int i = 1; i < LockArr.Num(); ++i)
		{
			const float Distance = this->GetDistanceTo(LockArr[i]);
			if (Distance < MinDistance)
			{
				MinDistance = Distance;
				MinDistanceActor = LockArr[i];
			}
		}

		// 设置对象
		LockActor = MinDistanceActor;
	}
}

/** 释放锁定 */
void ACombatPlayer::ReleaseLock()
{
	bIsLocking = false; //修改锁定状态
	GetCharacterMovement()->bOrientRotationToMovement = true; // 开启角色朝向移动方向
	LockActor = nullptr;	// 清除锁定对象
}

/** 闪避 */
void ACombatPlayer::Dodge(FMontageInfo* MatchCommand)
{
	if (bIsAttacking || bIsDodge || bIsGetHit)
	{
		return;
	}
	UAnimMontage* Montage = MatchCommand->Montage;
	UAnimInstance* AnimaInstance = GetMesh()->GetAnimInstance();
	if (AnimaInstance && !AnimaInstance->Montage_IsPlaying(Montage))
	{
		bIsDodge = true;
		CharacterState.bIsFallDown = false;
		const FVector InputDirection = InputForwardDirection + InputRightDirection;	// 输入方向
		AnimaInstance->Montage_Play(Montage);
		if (bIsLocking)
		{
			FName MontageSection;

			const float ForwardInput =  InputDirection | ForwardDirection;	// 人物前后输入量
			const float RightInput = InputDirection | RightDirection;		// 人物左右输入量
			// 计算输入向量与角色前方向量的夹角
			const float Angle = GetAngle(InputDirection, GetForwardDirection());
			if (Angle > 75 && Angle < 105 && RightInput != 0)
			// if (fabs(ForwardInput) <= 0.1 && fabs(RightInput) >= 0.1)
			{
				// 只输入了左右方向，侧闪
				MontageSection = RightInput > 0 ? "Right" : "Left";
			}
			else
			{
				// 输入了前后方向，翻滚，默认后翻滚
				MontageSection = ForwardInput > 0 ? "Front" : "Back";

				if (!InputDirection.IsZero())
					this->SetActorRotation(ForwardInput > 0 ? InputDirection.Rotation() : (-InputDirection).Rotation());
			}
			AnimaInstance->Montage_JumpToSection(MontageSection);
		}
		else
		{
			// GetCharacterMovement()->AddImpulse(FVector(RightDirection.X, RightDirection.Y, 0) * 10000.f, true);
			// AddMovementInput(RightDirection, 10.f);
			if (!InputForwardDirection.IsZero() || !InputRightDirection.IsZero())
			{
				this->SetActorRotation(InputDirection.Rotation());
			}
		}

		
	}
}

/** 是否可以移动 */
bool ACombatPlayer::CanMove(const float Rate) const
{
	if (!Super::CanMove(Rate))
		return false;

	// 超出有效输入范围时不可移动
	if (fabsf(Rate) < MinIgnoreMoveInput || fabsf(Rate) > MaxIgnoreMoveInput)
		return false;
	
	return true;
}

/** 获得锁定状态 */
bool ACombatPlayer::GetLockState() const
{
	return bIsLocking;
}

/** 获取范围内可以锁定项列表 */
TArray<AActor*> ACombatPlayer::GetLockArr()
{
	// 获取所有CombatCharacter对象
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACombatCharacter::StaticClass(), Actors);

	for (TArray<AActor*>::TIterator It = Actors.CreateIterator(); It; ++It)
	{
		if (*It == this)
		{
			// 移除自己
			It.RemoveCurrent();
		}
		else
		{
			// 移除大于锁定范围的对象
			const float Distance = this->GetDistanceTo(*It);
			if (Distance > LockDistance)
			{
				It.RemoveCurrent();
			}
		}
	}
	return Actors;
}

/** 执行指令 */
void ACombatPlayer::ExecutionCommand(FMontageInfo* MatchCommand)
{
	switch (MatchCommand->MontageInfoType)
	{
		case EMontageInfoType::E_Attack:
		case EMontageInfoType::E_Skill:
			Attack(MatchCommand);
			break;
		case EMontageInfoType::E_Dodge:
			Dodge(MatchCommand);
			break;
		default:
			break;
	}
	
}

/** 攻击 */
void ACombatPlayer::Attack(FMontageInfo* MatchCommand)
{
	UAnimMontage* AttackMontage = MatchCommand->Montage;

	if (!bCanChangeAttack || bIsDodge || bIsGetHit || CharacterState.bIsFallDown)
	{
		// 攻击中除了Combo外，不能直接切动画
		return;
	}
	if (MatchCommand->MontageInfoType == EMontageInfoType::E_Attack)
	{
		// 替换Combo
		if (ComboMontage != nullptr)
		{
			AttackMontage = ComboMontage;
		}
	}
	
	UAnimInstance* AnimaInstance =  GetMesh()->GetAnimInstance();
	if (AnimaInstance && !AnimaInstance->Montage_IsPlaying(AttackMontage))
	{
		bIsAttacking = true;
		SetCanChangeAttack(false);
		AnimaInstance->Montage_Play(AttackMontage);

		if (!bIsLocking && (!InputForwardDirection.IsZero() || !InputRightDirection.IsZero()))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Purple, "AttackDirection");
		
			this->SetActorRotation((InputForwardDirection + InputRightDirection).Rotation());
		}
	}
}


/** 设置攻击Montage */
void ACombatPlayer::SetComboMontage(UAnimMontage* NewMontage)
{
	ComboMontage = NewMontage; 
}

/** 恢复攻击Montage */
void ACombatPlayer::ResetAttackMontage()
{
	if (DefaultAttackMontage)
	{
		ComboMontage = DefaultAttackMontage;
	}
}

/** 摄像机震动 */
void ACombatPlayer::CameraShake(TSubclassOf<UCameraShakeBase> CamShake, const float Scale)
{
	if (PlayerController && CamShake)
	{ 
		PlayerController->PlayerCameraManager->StartCameraShake(CamShake, Scale);
	}
}

/** 受到攻击 */
void ACombatPlayer::GetHit(AActor* AttackActor, const FVector GetHitPoint, const FVector GetHitDirection, FAttackDamage AttackDamage)
{
	GetCharacterMovement()->GravityScale = 1.0f;
	Super::GetHit(AttackActor, GetHitPoint, GetHitDirection, AttackDamage);
	
	// 摄像机震动
	CameraShake(AttackDamage.CamShake, 1.0f);
}

/** 攻击命中 */
void ACombatPlayer::Hit(AActor* HitActor, FAttackDamage AttackDamage)
{
	Super::Hit(HitActor, AttackDamage);
	
	// 摄像机震动
	CameraShake(AttackDamage.CamShake, 0.5f);
}

/** 改变可以切换攻击的状态 */
void ACombatPlayer::SetCanChangeAttack(const bool bState)
{
	bCanChangeAttack = bState;
}

void ACombatPlayer::PressedJump()
{

	if (bIsAttacking || bIsGetHit || bIsDodge || JumpMaxCount == JumpCurrentCount || CharacterState.bIsFallDown)
	{
		return;
	}
	UAnimInstance* AnimaInstance =  GetMesh()->GetAnimInstance();
	if (AnimaInstance)
	{
		AnimaInstance->StopAllMontages(0.f);
	}
	
	GetCharacterMovement()->Velocity = (InputForwardDirection + InputRightDirection).GetSafeNormal() * GetCharacterMovement()->MaxWalkSpeed;
	bIsAttacking = false;
	ComboMontage = nullptr;
	bCanChangeAttack = true;

	Super::Jump();
}

void ACombatPlayer::ReleasedJump()
{
	if (bIsAttacking || bIsGetHit || bIsDodge || CharacterState.bIsFallDown || JumpMaxCount == JumpCurrentCount)
	{
		return;
	}
	Super::StopJumping();
}

/** 按下闪避 */
void ACombatPlayer::PressedDodge()
{
	// 存入闪避指令
	CurrentInputBuffering.push_front(EInputBufferingType::E_Dodge);
}

/** 解析输入指令 */
void ACombatPlayer::ParseInputBuffering()
{

	// 匹配输入指令
	FMontageInfo* MatchSkill = MatchCommandList();
	if (MatchSkill == nullptr)
	{
		return;
	}

	// 执行对应操作
	ExecutionCommand(MatchSkill);
	
	if (GShowDebugMsg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, "MatchSkill: " + MatchSkill->Name);
	}
}


/** 匹配输入指令 */
FMontageInfo* ACombatPlayer::MatchCommandList()
{

	TArray<FMontageInfo*> SkillArr;
	for (FMontageInfo* MontageInfo : CurrentSkillArr)
	{
		FMontageInfo *Temp = new FMontageInfo();
		*Temp = *MontageInfo;
		SkillArr.Add(Temp);
	}
	
	FMontageInfo* MatchSkill = nullptr;
	if (!InputBuffering.empty() && SkillArr.Num() > 0)
	{
		// 遍历指令输入
		auto InputIter = InputBuffering.begin();
		for (int i = 0; InputIter != InputBuffering.end(); ++i)
		{
			// 每帧下的指令输入
			std::deque<EInputBufferingType> TempInputList = *InputIter;
			++InputIter;
			if (TempInputList.empty())
			{
				// 跳过空白输入帧
				continue;
			}
			
			// 遍历每帧下的指令输入
			for(auto TempIter = TempInputList.rbegin(); TempIter != TempInputList.rend(); ++TempIter)
			{
				// 遍历当前技能列表
				for (auto SkillIter = SkillArr.CreateIterator(); SkillIter; ++SkillIter)
				{
					FMontageInfo* Skill = *SkillIter;
					// 对比技能表指令
					// TArray<FSkillCommand> CommandArr = Skill.CommandList;
					// 比对出招表最后一位是否与输入缓存最新一位相同

					// 当前为长按类型指令时，跳过非长按类型的技能输入指令的校验
					if (Skill->CommandList.Num() > 0 && HoldInputTypeArr.Contains(*TempIter) && !HoldInputTypeArr.Contains(Skill->CommandList.Last().InputBufferingType))
						continue;

					if (Skill->CommandList.Num() == 0 || !Skill->bValid // 排除无效的技能
						|| Skill->bInAir != bIsInAir || Skill->bOnGround != bIsOnGround // 空中或地面状态
						|| Skill->bIsLock != bIsLocking // 锁定状态
						|| *TempIter != Skill->CommandList.Last().InputBufferingType // 输入类型
						|| i > Skill->CommandList.Last().ValidFrameLength	// 有效帧长度
					)
					{
						// 跳过无效技能表
						if (SkillArr.Num() == 1)
						{
							return MatchSkill;
						}
						SkillIter.RemoveCurrent();
						continue;
					}
				
					// 移除一位
					Skill->CommandList.Pop();
				
					// 出招表完全匹配
					if (Skill->CommandList.Num() == 0)
					{
						// 比较优先级
						if (MatchSkill == nullptr || Skill->Priority > MatchSkill->Priority)
						{
							MatchSkill = Skill;
						}
					}
				}
			}
		}
	}

	return MatchSkill;
}

/** 读取MontageDataTable，存入对应数组 */
void ACombatPlayer::ReadMontage()
{
	if (MontageDT)
	{
		FString ContextString;
		// 遍历所有数据
		for (const FName RowName : MontageDT->GetRowNames())
		{
			FMontageInfo* MontageInfo = MontageDT->FindRow<FMontageInfo>(RowName, ContextString);
			if (MontageInfo)
			{
				TArray<EWeaponType> WeaponTypeList = MontageInfo->WeaponTypeList;
				// 遍历所属武器类型
				for(const EWeaponType WeaponType : WeaponTypeList)
				{
					// 根据武器类型存入对应数组
					if (!AllMontageInfoMap.Contains(WeaponType))
					{
						TArray<FMontageInfo*> MontageInfoArr;
						AllMontageInfoMap.Add(WeaponType, MontageInfoArr);
					}
					AllMontageInfoMap[WeaponType].Add(MontageInfo);
					if (WeaponType == CurrentWeaponType)
					{
						// 存入当前武器Montage数组
						CurrentMontageInfoArr.Add(MontageInfo);

						// 受击Montage
						if (MontageInfo->MontageInfoType == EMontageInfoType::E_GetHit)
						{
							GetHitMontage = MontageInfo->Montage;
						}
						// 默认普通攻击Montage
						if (MontageInfo->MontageInfoType == EMontageInfoType::E_Attack)
						{
							DefaultAttackMontage = MontageInfo->Montage;
						}
						
					}
					
					// 根据武器类型划分存入对应技能数组
					if (MontageInfo->MontageInfoType == EMontageInfoType::E_Skill
						|| MontageInfo->MontageInfoType == EMontageInfoType::E_Attack
						|| MontageInfo->MontageInfoType == EMontageInfoType::E_Dodge)
					{
						if (!AllSkillMap.Contains(WeaponType))
						{
							TArray<FMontageInfo*> MontageInfoArr;
							AllSkillMap.Add(WeaponType, MontageInfoArr);
						}
						AllSkillMap[WeaponType].Add(MontageInfo);
						if (WeaponType == CurrentWeaponType)
						{
							// 存入当前武器技能数组
							CurrentSkillArr.Add(MontageInfo);
						}
					}
				}
			}
		}
	}
}

/** debug */
void ACombatPlayer::ShowDebugMsg()
{
	CombatGameInstance->bShowDebugMsg = !CombatGameInstance->bShowDebugMsg;
	GShowDebugMsg = CombatGameInstance->bShowDebugMsg;
}
void ACombatPlayer::ShowDebugDraw()
{
	CombatGameInstance->bShowDebugDraw = !CombatGameInstance->bShowDebugDraw;
	GShowDebugDraw = CombatGameInstance->bShowDebugDraw;
}

void ACombatPlayer::PressedToGetHit()
{
	GetHit(this, FVector(0,0,0), FVector(0,0,0), FAttackDamage());
}

void ACombatPlayer::PressedToRecoveryHealth()
{
	UpdateHealth(100.f);
}
