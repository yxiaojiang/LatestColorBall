// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class ColorBall : ModuleRules
{
	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/")); }
	}
	
	public ColorBall(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"SQLiteCore", "SQLiteSupport", "Json", "JsonUtilities", "HTTP"
		});

		PublicIncludePaths.AddRange(new string[]
		{
			EngineDirectory + "/Plugins" + "/Runtime/Database/SQLiteCore/Source/SQLiteCore/Private/"
		});


		string VcPkgInstalled = "vcpkg-installed";
		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			//G:\UE4LauncherVersion\UE_4.27\Engine\Plugins\Runtime\GeoReferencing\Source\ThirdParty\vcpkg-installed\overlay-x64-windows\lib
			string LibPath = Path.Combine(EngineDirectory, "Plugins", "Runtime", "GeoReferencing", "Source",
				"ThirdParty", VcPkgInstalled, "overlay-x64-windows", "lib");
			PublicAdditionalLibraries.Add(Path.Combine(LibPath, "sqlite3.lib"));
		}

		
		LoadLua(Target);

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
	
	private bool LoadLua(ReadOnlyTargetRules TargetRules)
	{
		bool isLibSupported = false;

		// Check if we are on Windows
		if ((Target.Platform == UnrealTargetPlatform.Win64))
		{
			isLibSupported = true;

			string LibrariesPath = Path.Combine(ThirdPartyPath, "Lua", "Libraries");
			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "lua54.lib"));
			PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Lua", "Include"));
		}

		PublicDefinitions.Add(string.Format("WITH_LUA_BINDING={0}", isLibSupported ? 1 : 0));

		return isLibSupported;
	}
	
}
