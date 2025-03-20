using UnrealBuildTool;
 
public class Command : ModuleRules
{
	public Command(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"Command/Public"});
		PrivateIncludePaths.AddRange(new string[] {"Command/Private"});
	}
}