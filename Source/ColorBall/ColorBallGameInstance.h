// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/IHttpRequest.h"
#include "ColorBallGameInstance.generated.h"

/**
 *
*在 Unreal Engine 4 中，你可以使用网络下载和文件操作功能来实现程序的更新过程。

1、在程序初始化后，使用网络功能检查服务器上是否有新版本可用。可以使用 Unreal Engine 的 HTTP 请求功能，
发送一个请求到服务器上的版本检查接口，获取最新的版本信息。

2、如果有新版本可用，获取到最新的版本号和更新文件的下载链接。

3、使用 Unreal Engine 的文件操作功能，下载更新文件到本地。可以使用 FHttpModule 来发送下载请求，并将文件保存到指定的本地目录。

4、下载完成后，进行文件的校验和替换。可以比较本地文件和下载的更新文件的哈希值或者大小等信息，确保下载的文件完整无误。
如果校验通过，可以将下载的文件替换原有的程序文件。

5、更新完成后，继续正常运行程序。

 **/


/**
 * 
 */
UCLASS()
class COLORBALL_API UColorBallGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

     void CheckForUpdates();

     void OnVersionCheckResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

 void DownloadUpdateFile(const FString& FileURL);

 void OnDownloadProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);

 void OnUpdateFileDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

 bool ValidateAndUpdateFile(const FString& FilePath);



 FString GetLatestVersionFromResponse(FHttpResponsePtr Response);

 FString GetUpdateFileURLFromResponse(FHttpResponsePtr Response);

 bool IsUpdateNeeded(const FString& LatestVersion);

 FString GetGameVersion();
	
};
