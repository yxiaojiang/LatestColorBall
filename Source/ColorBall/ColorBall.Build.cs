// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ColorBall : ModuleRules
{
	public ColorBall(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
			"SQLiteCore", "SQLiteSupport"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		PublicIncludePaths.AddRange(new string[]
		{
			EngineDirectory + "/Plugins" + "/Runtime/Database/SQLiteCore/Source/SQLiteCore/Private/"
		});
		
		
		string VcPkgInstalled = "vcpkg-installed";
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			//G:\UE4LauncherVersion\UE_4.27\Engine\Plugins\Runtime\GeoReferencing\Source\ThirdParty\vcpkg-installed\overlay-x64-windows\lib
			string LibPath = Path.Combine(EngineDirectory, "Plugins","Runtime", "GeoReferencing", "Source", "ThirdParty", VcPkgInstalled, "overlay-x64-windows", "lib");
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "sqlite3.lib"));
		}

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
