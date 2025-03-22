workspace "TheEngine"
 	architecture "x64"
    startproject "Sandbox"
 
 	configurations
 	{
 		"Debug",
 		"Release",
 		"Dist"
 	}
 
 outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

 -- Include directories relative to root folder (solution directory)
 IncludeDir = {}
 IncludeDir["GLFW"] = "TheEngine/vendor/GLFW/include"
 IncludeDir["Glad"] = "TheEngine/vendor/Glad/include"
 IncludeDir["ImGui"] = "TheEngine/vendor/imgui"
 IncludeDir["glm"] = "TheEngine/vendor/glm"

 include "TheEngine/vendor/GLFW"
 include "TheEngine/vendor/Glad"
 include "TheEngine/vendor/imgui"
 
 project "TheEngine"
 	location "TheEngine"
 	kind "StaticLib"
 	language "C++"
	cppdialect "C++17"
    staticruntime "on"
 
 	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
 	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "tepch.h"
    pchsource "TheEngine/src/tepch.cpp"
 
 	files
 	{
 		"%{prj.name}/src/**.h",
 		"%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
 	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

 	includedirs
 	{
        "%{prj.name}/src",
 		"%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
 	}

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dwmapi.lib"
    }
 
 	filter "system:windows"
 		systemversion "latest"
		linkoptions { "/IGNORE:4006" }
        buildoptions { "/utf-8" }

 		defines
 		{
 			"TE_PLATFORM_WINDOWS",
 			"TE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
 		}
 
 	filter "configurations:Debug"
 		defines "TE_DEBUG"
        runtime "Debug"
 		symbols "on"
 
 	filter "configurations:Release"
 		defines "TE_RELEASE"
        runtime "Release"
 		optimize "on"
 
 	filter "configurations:Dist"
 		defines "TE_DIST"
        runtime "Release"
 		optimize "on"
 
 project "Sandbox"
 	location "Sandbox"
 	kind "ConsoleApp"
 	language "C++"
	cppdialect "C++17"
    staticruntime "on"
 
 	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
 	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
 
 	files
 	{
 		"%{prj.name}/src/**.h",
 		"%{prj.name}/src/**.cpp"
 	}
 
 	includedirs
 	{
 		"TheEngine/vendor/spdlog/include",
 		"TheEngine/src",
        "TheEngine/vendor",
        "%{IncludeDir.glm}"
 	}
 
 	links
 	{
 		"TheEngine"
 	}
 
 	filter "system:windows"
 		systemversion "latest"
		linkoptions { "/IGNORE:4006" }
        buildoptions { "/utf-8" }

 		defines
 		{
 			"TE_PLATFORM_WINDOWS"
 		}
 
 	filter "configurations:Debug"
 		defines "TE_DEBUG"
        runtime "Debug"
 		symbols "on"
 
 	filter "configurations:Release"
 		defines "TE_RELEASE"
        runtime "Release"
 		optimize "on"
 
 	filter "configurations:Dist"
 		defines "TE_DIST"
        runtime "Release"
 		optimize "on"