// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorBallGameInstance.h"
#include "HttpModule.h"
#include "IPlatformFilePak.h"
#include "Misc/Paths.h"
#include "Version.h"
#include "Interfaces/IHttpResponse.h"


// 在某个函数中进行程序更新
void UColorBallGameInstance::CheckForUpdates()
{
    FString ServerURL = "http://your-server.com/version-check"; // 版本检查接口的 URL

    // 发送 HTTP 请求获取最新版本信息
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(ServerURL);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UColorBallGameInstance::OnVersionCheckResponseReceived);
    HttpRequest->ProcessRequest();
}

// 版本检查的回调函数
void UColorBallGameInstance::OnVersionCheckResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        FString LatestVersion = GetLatestVersionFromResponse(Response); // 从响应中获取最新版本号
        FString UpdateFileURL = GetUpdateFileURLFromResponse(Response); // 从响应中获取更新文件的下载链接

        if (IsUpdateNeeded(LatestVersion))
        {
            DownloadUpdateFile(UpdateFileURL);
        }
    }
    else
    {
        // 版本检查请求失败
    }
}

// 下载更新文件
void UColorBallGameInstance::DownloadUpdateFile(const FString& FileURL)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb("GET");
    HttpRequest->SetURL(FileURL);
    
    // 设置回调函数
    HttpRequest->OnRequestProgress().BindUObject(this, &UColorBallGameInstance::OnDownloadProgress);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &UColorBallGameInstance::OnUpdateFileDownloadComplete);
    HttpRequest->ProcessRequest();
}

void UColorBallGameInstance::OnDownloadProgress(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
    // 计算下载进度百分比
    float ProgressPercentage = static_cast<float>(BytesReceived) / static_cast<float>(Request->GetResponse()->GetContentLength());

    // 调用委托或事件，传递进度百分比
    //UpdateProgressDelegate.ExecuteIfBound(ProgressPercentage);
}

// 更新文件下载完成的回调函数
void UColorBallGameInstance::OnUpdateFileDownloadComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid() && Response->GetResponseCode() == EHttpResponseCodes::Ok)
    {
        FString LocalFilePath = FPaths::ProjectSavedDir() + "UpdateFile.pak"; // 更新文件保存的本地路径

        // 保存下载的文件到本地
        bool bSaved = FFileHelper::SaveArrayToFile(Response->GetContent(), *LocalFilePath);

        if (bSaved)
        {
            // 下载完成，进行文件的校验和替换操作
            if (ValidateAndUpdateFile(LocalFilePath))
            {
                // 文件校验和替换成功，重启程序或执行其他更新完成的操作
            }
            else
            {
                // 文件校验和替换失败
            }
        }
        else
        {
            // 文件保存失败
        }
    }
    else
    {
        // 更新文件下载请求失败
    }
}

// 校验和替换文件
bool UColorBallGameInstance::ValidateAndUpdateFile(const FString& FilePath)
{
    // 进行文件校验，比较文件的哈希值或大小等信息，确保文件完整无误
    // 如果校验通过，可以进行文件的替换操作

    FString TargetFilePath = FPaths::ProjectContentDir() + "UpdateFile.pak"; // 目标文件的路径

    // 删除原有的目标文件
    IFileManager::Get().Delete(*TargetFilePath, false, true);

    // 移动下载的更新文件到目标路径
    bool bMoved = IFileManager::Get().Move(*TargetFilePath, *FilePath);

    if (bMoved)
    {
        return true;
    }
    else
    {
        // 文件替换失败
        return false;
    }
}


// 从响应中获取最新版本号
FString UColorBallGameInstance::GetLatestVersionFromResponse(FHttpResponsePtr Response)
{
    FString LatestVersion;

    // 解析响应的内容，获取最新版本号
    if (Response.IsValid())
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
        {
            if (JsonObject->HasField("latest_version"))
            {
                LatestVersion = JsonObject->GetStringField("latest_version");
            }
        }
    }

    return LatestVersion;
}

// 从响应中获取更新文件的下载链接
FString UColorBallGameInstance::GetUpdateFileURLFromResponse(FHttpResponsePtr Response)
{
    FString UpdateFileURL;

    // 解析响应的内容，获取更新文件的下载链接
    if (Response.IsValid())
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

        if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
        {
            if (JsonObject->HasField("update_file_url"))
            {
                UpdateFileURL = JsonObject->GetStringField("update_file_url");
            }
        }
    }

    return UpdateFileURL;
}

// 检查是否需要进行更新
bool UColorBallGameInstance::IsUpdateNeeded(const FString& LatestVersion)
{
    FString CurrentVersion = GetGameVersion(); // 获取当前程序的版本号
    return LatestVersion > CurrentVersion;
}

FString UColorBallGameInstance::GetGameVersion()
{
    // 在项目中定义一个包含版本号信息的文件（例如 Version.h），
    // 然后在该文件中定义一个宏或常量来表示当前的版本号。
    
    // 使用项目中定义的版本号宏或常量来获取当前的版本号
    FString CurrentVersion = FString::FromInt(VERSION_MAJOR) + "." + FString::FromInt(VERSION_MINOR) + "." + FString::FromInt(VERSION_PATCH);

    return CurrentVersion;
}



