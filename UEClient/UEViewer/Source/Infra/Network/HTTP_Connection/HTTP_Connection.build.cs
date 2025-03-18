using UnrealBuildTool;
 
public class HTTP_Connection : ModuleRules
{
	public HTTP_Connection(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Json", "HTTP", "Networking", "Sockets", "JsonUtilities" });
 
		PublicIncludePaths.AddRange(new string[] {"HTTP_Connection/Public"});
		PrivateIncludePaths.AddRange(new string[] {"HTTP_Connection/Private"});
	}
}