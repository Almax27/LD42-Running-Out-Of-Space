// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class RunningOutOfSpaceEditorTarget : TargetRules
{
	public RunningOutOfSpaceEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "RunningOutOfSpace" } );
	}
}
