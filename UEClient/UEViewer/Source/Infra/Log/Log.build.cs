using UnrealBuildTool;
 
public class Log : ModuleRules
{
	public Log(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"Log/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Log/Private"});
	}
}