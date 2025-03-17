using UnrealBuildTool;
 
public class AssetContent : ModuleRules
{
	public AssetContent(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"AssetContent/Public"});
		PrivateIncludePaths.AddRange(new string[] {"AssetContent/Private"});
	}
}