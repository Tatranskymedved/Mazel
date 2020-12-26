workspace "Mazel"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder (solution dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Mazel/vendor/GLFW/include"

include "Mazel/vendor/GLFW"

project "Mazel"
    location "Mazel"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "mzpch.h"
    pchsource "Mazel/src/mzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "MZ_PLATFORM_WINDOWS",
            "MZ_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines { "MZ_DEBUG" }
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines { "MZ_RELEASE" }
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines { "MZ_DIST" }
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

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
        "Mazel/src"
    }

    links
    {
        "Mazel"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "MZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "MZ_DEBUG" }
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines { "MZ_RELEASE" }
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines { "MZ_DIST" }
        buildoptions "/MD"
        optimize "On"
