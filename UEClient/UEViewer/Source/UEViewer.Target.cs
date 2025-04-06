// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UEViewerTarget : TargetRules
{
	public UEViewerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
<<<<<<< HEAD
		ExtraModuleNames.AddRange( new string[] { "HTTP_Connection" } );
=======
		ExtraModuleNames.AddRange( new string[] { "Profiling" } );
>>>>>>> UE_Core
		ExtraModuleNames.AddRange( new string[] { "Command" } );
		ExtraModuleNames.AddRange( new string[] { "AssetContent" } );
		ExtraModuleNames.AddRange( new string[] { "AssetActor" } );
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "UEViewer" } );
	}
}
