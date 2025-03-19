using UnrealBuildTool;
 
public class Profiling : ModuleRules
{
	public Profiling(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG" });
 
		PublicIncludePaths.AddRange(new string[] {"Profiling/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Profiling/Private"});
	}
}