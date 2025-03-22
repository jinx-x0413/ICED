// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class UEViewerEditorTarget : TargetRules
{
	public UEViewerEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.AddRange(new string[] { "Timebar" });
        ExtraModuleNames.AddRange( new string[] { "UIComponent" } );
		ExtraModuleNames.AddRange( new string[] { "Command" } );
		ExtraModuleNames.AddRange( new string[] { "AssetContent" } );
		ExtraModuleNames.AddRange( new string[] { "AssetActor" } );
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "UEViewer" } );
	}
}
