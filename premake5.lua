workspace "TheEngine"
 	architecture "x64"
 
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
 	kind "SharedLib"
 	language "C++"
    staticruntime "off"
 
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
 		cppdialect "C++17"
 		systemversion "latest"
 
        buildoptions { "/utf-8" }

 		defines
 		{
 			"TE_PLATFORM_WINDOWS",
 			"TE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
 		}
 
 		postbuildcommands
 		{
 			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
 		}
 
 	filter "configurations:Debug"
 		defines "TE_DEBUG"
        runtime "Debug"
 		symbols "On"
 
 	filter "configurations:Release"
 		defines "TE_RELEASE"
        runtime "Release"
 		optimize "On"
 
 	filter "configurations:Dist"
 		defines "TE_DIST"
        runtime "Release"
 		optimize "On"
 
 project "Sandbox"
 	location "Sandbox"
 	kind "ConsoleApp"
 	language "C++"
    staticruntime "off"
 
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
        "%{IncludeDir.glm}"
 	}
 
 	links
 	{
 		"TheEngine"
 	}
 
 	filter "system:windows"
 		cppdialect "C++17"
 		systemversion "latest"
 
        buildoptions { "/utf-8" }

 		defines
 		{
 			"TE_PLATFORM_WINDOWS"
 		}
 
 	filter "configurations:Debug"
 		defines "TE_DEBUG"
        runtime "Debug"
 		symbols "On"
 
 	filter "configurations:Release"
 		defines "TE_RELEASE"
        runtime "Release"
 		optimize "On"
 
 	filter "configurations:Dist"
 		defines "TE_DIST"
        runtime "Release"
 		optimize "On"