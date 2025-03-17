using UnrealBuildTool;
 
public class AssetActor : ModuleRules
{
	public AssetActor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine"
			, "glTFRuntime"
        });
 
		PublicIncludePaths.AddRange(new string[] {"AssetActor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"AssetActor/Private"});
	}
}