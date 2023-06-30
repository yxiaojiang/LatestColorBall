// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorBallSubsystem.h"

static const FString InsertColorBallStatement =
	TEXT("INSERT INTO EvryDrawNumber (Period, Number) VALUES (2023073, '15,25,28,30,35,8,10'), (2023072, '5,6,9,11,14,2,4');");

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
	 /*这个写法是第一种写法
	使用了 sqlite3_open 函数打开数据库连接，并使用 sqlite3_prepare_v2 函数准备查询语句。
	然后，使用 sqlite3_step 函数来逐行获取查询结果，并使用 sqlite3_column_text 函数获取每列的数据。
	最后，将每列的数据保存到 EveryPeriod 和 EveryPeriodNumber 数组中，并输出保存的数据。*/
	
	EveryPeriod.Empty();
	EveryPeriodNumber.Empty();
	
	char* ErrorMessage = nullptr;
	const FString DatabasePath = FPaths::ProjectDir() + "ColorBall.db";

	// 打开数据库连接
	int resultCode = sqlite3_open(TCHAR_TO_UTF8(*DatabasePath), &CBDatabase);
	if (resultCode == SQLITE_OK)
	{
		const char* query = "SELECT * FROM EvryDrawNumber;"; // 查询语句
		sqlite3_stmt* Statement;

		// 准备查询语句
		resultCode = sqlite3_prepare_v2(CBDatabase, query, -1, &Statement, nullptr);
		if (resultCode == SQLITE_OK)
		{
			// 执行查询语句
			while (sqlite3_step(Statement) == SQLITE_ROW)
			{
				// 获取第一列和第二列的数据
				const char* columnValue1 = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 0));
				const char* columnValue2 = reinterpret_cast<const char*>(sqlite3_column_text(Statement, 1));

				// 将数据保存到数组中
				EveryPeriod.Add(columnValue1);
				EveryPeriodNumber.Add(columnValue2);
			}

			// 释放查询资源
			sqlite3_finalize(Statement);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to prepare query: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(CBDatabase)));
		}

		// 关闭数据库连接
		sqlite3_close(CBDatabase);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open database: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(CBDatabase)));
	}

	// 输出保存的数据
	for (int index = 0; index < EveryPeriod.Num(); ++index)
	{
		UE_LOG(LogTemp, Warning, TEXT("EveryPeriod %s, EveryPeriodNum %s"), *EveryPeriod[index], *EveryPeriodNumber[index]);
	}


	// 第二种写法,定义了一个回调函数 callback，它会在 sqlite3_exec 执行查询时被调用。
	// 在回调函数中，我们将第一列和第二列的数据保存到 QueryResult 结构体中的 ColumnValues 数组中。
	
	/*EveryPeriod.Empty();
	EveryPeriodNumber.Empty();

	sqlite3* DB;
	const FString DatabasePath = FPaths::ProjectDir() + "ColorBall.db";

	// 打开数据库连接
	int resultCode = sqlite3_open(TCHAR_TO_UTF8(*DatabasePath), &DB);
	if (resultCode == SQLITE_OK)
	{
		const char* query = "SELECT * FROM EvryDrawNumber;"; // 查询语句

		// 定义回调函数，用于处理查询结果
		auto callback = [](void* userData, int columnCount, char** columnValues, char** columnNames) -> int {
			auto result = static_cast<QueryResult*>(userData);

			// 保存第一列和第二列的数据
			result->ColumnValues.Add(columnValues[0]);
			result->ColumnValues.Add(columnValues[1]);

			return 0;
		};

		QueryResult result;

		// 执行查询
		resultCode = sqlite3_exec(DB, query, callback, &result, nullptr);
		if (resultCode == SQLITE_OK)
		{
			// 输出保存的数据
			for (int index = 0; index < result.ColumnValues.Num(); index += 2)
			{
				UE_LOG(LogTemp, Log, TEXT("EveryPeriod %s, EveryPeriodNum %s"),
					*result.ColumnValues[index],
					*result.ColumnValues[index + 1]);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute query: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(DB)));
		}

		// 关闭数据库连接
		sqlite3_close(DB);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to open database: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(DB)));
	}*/


	/*
	* 在查询数据库时，使用 sqlite3_exec 或 sqlite3_prepare_v2 都可以实现相同的功能。选择哪种方式更好，取决于具体需求和个人偏好。

使用 sqlite3_exec 函数的优点是它是一个高级的、简单的接口，能够一次性执行完整的 SQL 查询语句，并且使用回调函数处理查询结果。
这种方式适用于简单的查询操作，特别是当你不需要对每一行数据进行复杂的处理时。

使用 sqlite3_prepare_v2 函数的优点是它提供了更细粒度的控制，你可以分步执行查询操作。你可以使用 sqlite3_step 函数逐行获取查询结果，
然后使用相应的函数获取每一列的数据。这种方式适用于需要更灵活的查询操作，以及对查询结果进行更复杂处理的情况。

综上所述，如果只需要简单地获取查询结果并将数据保存到数组中，使用 sqlite3_exec 函数更为方便。如果需要更灵活地处理查询结果，
例如逐行处理、数据转换等，那么使用 sqlite3_prepare_v2 和相关函数会更合适。

选择哪种方式取决于具体需求和项目的情况。如果对代码的简洁性和方便性更为重视，而且查询操作相对简单，那么使用 sqlite3_exec 可能更适合。
如果需要更多的灵活性和控制，那么使用 sqlite3_prepare_v2 可能更适合。
	*/
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

// 在某个函数中执行查询和修改操作
void UColorBallSubsystem::QueryAndModifyData()
{
    sqlite3* DB;
    const FString DatabasePath = FPaths::ProjectDir() + "ColorBall.db";

    // 打开数据库连接
    int resultCode = sqlite3_open(TCHAR_TO_UTF8(*DatabasePath), &DB);
    if (resultCode == SQLITE_OK)
    {
        const char* query = "SELECT * FROM EvryDrawNumber;"; // 查询语句

        // 定义回调函数，用于处理查询结果和修改数据
        auto callback = [](void* userData, int columnCount, char** columnValues, char** columnNames) -> int {
            auto database = static_cast<sqlite3*>(userData);

            // 获取第一行数据
            const char* oldValue = columnValues[0];

            // 修改第一行数据（示例中将第一行数据替换为 "New Value"）
            const char* newValue = "New Value";
            const char* updateQuery = "UPDATE EvryDrawNumber SET ColumnName = ? WHERE RowID = 1;";
            sqlite3_stmt* updateStatement;
            int resultCode = sqlite3_prepare_v2(database, updateQuery, -1, &updateStatement, nullptr);
            if (resultCode == SQLITE_OK)
            {
                sqlite3_bind_text(updateStatement, 1, newValue, -1, SQLITE_STATIC);
                resultCode = sqlite3_step(updateStatement);
                if (resultCode != SQLITE_DONE)
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to update data: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(database)));
                }
                sqlite3_finalize(updateStatement);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to prepare update statement: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(database)));
            }

            return 0;
        };

        // 执行查询
        resultCode = sqlite3_exec(DB, query, callback, DB, nullptr);
        if (resultCode == SQLITE_OK)
        {
            UE_LOG(LogTemp, Log, TEXT("Data query and modification completed successfully."));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to execute query: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(DB)));
        }

        // 关闭数据库连接
        sqlite3_close(DB);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to open database: %s"), UTF8_TO_TCHAR(sqlite3_errmsg(DB)));
    }
}





