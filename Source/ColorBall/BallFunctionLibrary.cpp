// Fill out your copyright notice in the Description page of Project Settings.


#include "BallFunctionLibrary.h"

void UBallFunctionLibrary::WriteBallResult(const TArray<int32>& BallResult, const FString& ResultPath)
{
	FString TempStr = FString();
	for (int32 Sresult : BallResult)
	{
		TempStr.Append(FString::FromInt(Sresult) + TEXT("  "));
	}

	FFileHelper::SaveStringToFile(TempStr + TEXT("\r\n"), *ResultPath, FFileHelper::EEncodingOptions::ForceUTF8,
		&IFileManager::Get(), EFileWrite::FILEWRITE_Append);
}

FString UBallFunctionLibrary::GetBallGuid()
{
	return FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphens);
}

FString UBallFunctionLibrary::GetBallNowTime()
{
	return FDateTime::Now().ToString();
}
