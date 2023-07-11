 workspace "GaussianBlur"
    configurations { "Debug", "Release" }
    startproject "GaussianBlur"

    flags { "MultiProcessorCompile" }

    filter "configurations:Debug"
        defines { "DEBUG", "DEBUG_SHADER" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "Speed"
        flags { "LinkTimeOptimization" }

project "GaussianBlur"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
	architecture "x86_64"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}"

    includedirs { "src", "src/Renderer/", "libs/glad/include/", "libs/glfw/include/", "libs/glm/", "libs/imgui/", "libs/imgui/examples", "libs/stb_image/"}
    
    files { "src/*.cpp", "src/Renderer/*.cpp", "libs/stb_image/*.cpp" }

    links { "GLFW", "GLM", "GLAD", "ImGui" }

    filter "system:linux"
        links { "dl", "pthread" }

        defines { "_X11" }

    filter "system:windows"
        defines { "_WINDOWS" }

include "libs/glfw.lua"
include "libs/glad.lua"
include "libs/glm.lua"
include "libs/imgui.lua"