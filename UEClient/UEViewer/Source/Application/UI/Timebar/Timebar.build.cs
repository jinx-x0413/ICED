using UnrealBuildTool;
 
public class Timebar : ModuleRules
{
	public Timebar(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core"
			, "CoreUObject"
			, "InputCore"
			, "Engine"
			, "UMG"});
 
		PublicIncludePaths.AddRange(new string[] {"Timebar/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Timebar/Private"});
	}
}