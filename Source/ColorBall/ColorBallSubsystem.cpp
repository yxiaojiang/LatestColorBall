// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorBallSubsystem.h"

static const FString InsertColorBallStatement =
	TEXT("INSERT INTO EvryDrawNumber (Period, Number) VALUES (2023073, '15,25,28,30,35,8,10'), (2023072, '5,6,9,11,14,2,4');");

static const FString QueryDataFromColorBallTable =
	TEXT("SELECT * FROM EvryDrawNumber");

UColorBallSubsystem::UColorBallSubsystem()
{
}

bool UColorBallSubsystem::ConnectToDataBase(sqlite3*& InDataBase, const FString& InDataBasePath)
{
	FString DatabaseFullPath = FPaths::ConvertRelativePathToFull(InDataBasePath);
	int Result = sqlite3_open(TCHAR_TO_UTF8(*DatabaseFullPath), &InDataBase);
    
	if (Result != SQLITE_OK)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to connect to database: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(InDataBase)));
		return false;
	}
    
	return true;
}

void UColorBallSubsystem::CloseDataBase(sqlite3* InDataBase)
{
	if (InDataBase)
	{
		sqlite3_close(InDataBase);	
	}
}

bool UColorBallSubsystem::ExecuteSQLStatement(sqlite3* Database, const FString& Query)
{
	char* ErrorMessage = nullptr;
	int Result = sqlite3_exec(Database, TCHAR_TO_UTF8(*Query), nullptr, nullptr, &ErrorMessage);
    
	if (Result != SQLITE_OK)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to execute SQL query: %s"), UTF8_TO_TCHAR(ErrorMessage));
		sqlite3_free(ErrorMessage);
		return false;
	}
    
	return true;
}

void UColorBallSubsystem::FillColorBallTableDatas()
{
	if (ConnectToDataBase(CBDatabase, FPaths::ProjectDir() + "ColorBall.db"))
	{
		// 执行SQL查询
		if (ExecuteSQLStatement(CBDatabase, InsertColorBallStatement))
		{
			UE_LOG(LogTemp, Display, TEXT("Table Excute successfully."));
        
			// 在这里可以执行更多的查询或操作
        
			// 关闭数据库连接
			CloseDataBase(CBDatabase);
		}
		else
		{
			// 处理查询失败的情况
			UE_LOG(LogTemp, Display, TEXT("Table Excute Failed."));
		}
	}
	else
	{
		// 处理连接失败的情况
		UE_LOG(LogTemp, Display, TEXT("Table Connected Failed."));
	}
}


// 在某个函数中执行查询操作
void UColorBallSubsystem::QueryDataFromTable()
{
	EveryPeriod.Empty();
	EveryPeriodNumber.Empty();
	
	char* ErrorMessage = nullptr;
	if (ConnectToDataBase(CBDatabase, FPaths::ProjectDir() + "ColorBall.db"))
	{
		// 执行查询
		if (sqlite3_exec(CBDatabase, TCHAR_TO_UTF8(*QueryDataFromColorBallTable), [](void* NotUsed, int ColumnCount, char** ColumnValues, char** ColumnNames) -> int
			{
				// 遍历每行数据
				for (int i = 0; i < ColumnCount; ++i)
				{
					FString ColumnName(ColumnNames[i]);
					FString ColumnValue(ColumnValues[i]);

					
				}

				return 0;
			}, nullptr, &ErrorMessage) == SQLITE_OK)
		{
			// 查询成功
			UE_LOG(LogTemp, Warning, TEXT("Query executed successfully."));
		}
		else
		{
			// 查询失败，处理错误
			UE_LOG(LogTemp, Error, TEXT("Failed to execute query: %s"), UTF8_TO_TCHAR(ErrorMessage));
			sqlite3_free(ErrorMessage);
		}
	}
	else
	{
		// 处理连接失败的情况
		UE_LOG(LogTemp, Display, TEXT("Table Connected Failed."));
	}
}


// 在某个函数中执行插入操作
void UColorBallSubsystem::InsertDataIntoTable()
{
	char* ErrorMessage = nullptr;
	if (ConnectToDataBase(CBDatabase, FPaths::ProjectDir() + "ColorBall.db"))
	{
		const char* InsertQuery = "INSERT INTO YourTable (Column1, Column2) VALUES ('Value1', 42)";
		// 执行插入
		if (sqlite3_exec(CBDatabase, InsertQuery, nullptr, nullptr, &ErrorMessage) == SQLITE_OK)
		{
			// 插入成功
			UE_LOG(LogTemp, Warning, TEXT("Data inserted successfully."));
		}
		else
		{
			// 插入失败，处理错误
			UE_LOG(LogTemp, Error, TEXT("Failed to insert data: %s"), UTF8_TO_TCHAR(ErrorMessage));
			sqlite3_free(ErrorMessage);
		}
	}
	else
	{
		// 处理连接失败的情况
		UE_LOG(LogTemp, Display, TEXT("Table Connected Failed."));
	}
	
}



