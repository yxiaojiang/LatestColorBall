// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BallFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class COLORBALL_API UBallFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable)
		static void WriteBallResult(const TArray<int32>& BallResult, const FString& ResultPath);

	UFUNCTION(BlueprintCallable)
		static FString GetBallGuid();

	UFUNCTION(BlueprintCallable)
		static FString GetBallNowTime();

};
