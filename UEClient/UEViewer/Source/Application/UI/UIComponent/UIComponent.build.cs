using UnrealBuildTool;

public class UIComponent : ModuleRules
{
    public UIComponent(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core"
            , "CoreUObject"
            , "Engine"
            , "UMG"
            , "InputCore"
            , "Json"
            , "JsonUtilities"
            , "AssetActor"
            , "AssetContent"
        });

        PublicIncludePaths.AddRange(new string[] { "UIComponent/Public" });
        PrivateIncludePaths.AddRange(new string[] { "UIComponent/Private" });
    }
}