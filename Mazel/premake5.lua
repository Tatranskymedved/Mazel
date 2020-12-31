workspace "Mazel"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Mazel/vendor/GLFW/include"
IncludeDir["Glad"] = "Mazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Mazel/vendor/imgui"
IncludeDir["glm"] = "Mazel/vendor/glm"

include "Mazel/vendor/GLFW"
include "Mazel/vendor/Glad"
include "Mazel/vendor/imgui"

project "Mazel"
    location "Mazel"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mzpch.h"
    pchsource "Mazel/src/mzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/glm/glm/**.h",
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
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "MZ_PLATFORM_WINDOWS",
            "MZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines { "MZ_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "MZ_RELEASE" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "MZ_DIST" }
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
        "Mazel/vendor/spdlog/include",
        "Mazel/src",
		"Mazel/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "Mazel"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "MZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "MZ_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "MZ_RELEASE" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "MZ_DIST" }
        runtime "Release"
        optimize "on"
