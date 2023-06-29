// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "sqlite/sqlite3.h"
#include "ColorBallSubsystem.generated.h"

//#pragma comment(lib, "sqlite3.lib")

/**
 * 
 */
UCLASS()
class COLORBALL_API UColorBallSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	
	sqlite3* CBDatabase = nullptr;

	TArray<int> EveryPeriod;
	TArray<FString> EveryPeriodNumber;

	//_________________________________-------------------------------------------------//

	UColorBallSubsystem();

	bool ConnectToDataBase(sqlite3*& InDataBase, const FString& InDataBasePath);

	void CloseDataBase(sqlite3* InDataBase);

	bool ExecuteSQLStatement(sqlite3* Database, const FString& Query);

	UFUNCTION(BlueprintCallable)
	void FillColorBallTableDatas();

	UFUNCTION(BlueprintCallable)
	void QueryDataFromTable();
	
	UFUNCTION(BlueprintCallable)
	void InsertDataIntoTable();
	
};
