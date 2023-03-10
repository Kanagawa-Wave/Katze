-- Dependencies

-- VULKAN_SDK = os.getenv("VULKAN_SDK")

rule "CompileShaders"
    location "Rules"
    display "GLSL Shader Compiler"
    fileextension
    {
        ".frag",
        ".vert",
        ".comp"
    }
    
    buildmessage "Compiling %(Filename) with GLSL-SPV"
    buildcommands "glslangValidator -G -o $(OutDir)/%(Identity).spv %(Identity)"
    buildoutputs "$(OutDir)/%(Identity).spv"

workspace "Katze"
    architecture "x64"
    startproject "Editor"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist",
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
    
    debugdir ("Binaries/" .. outputdir .. "/Editor")

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Core"] = "Engine/Source/Core"
IncludeDir["Source"] = "Engine/Source"
-- IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["GLAD"] = "Engine/Vendor/glad/include"
IncludeDir["GLFW"] = "Engine/Vendor/glfw/include"
IncludeDir["GLM"] = "Engine/Vendor/glm"
IncludeDir["SPDLOG"] = "Engine/Vendor/spdlog/include"
IncludeDir["IMGUI"] = "Engine/Vendor/imgui"
IncludeDir["STB_IMAGE"] = "Engine/Vendor/stb"
IncludeDir["TINYOBJLOADER"] = "Engine/Vendor/tinyobjloader"
IncludeDir["ENTT"] = "Engine/Vendor/entt"

LibraryDir = {}
-- LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
-- LibraryDir["GLFW"] = "Engine/Vendor/glfw/lib-vc2022"

Libraries = {}
Libraries["OpenGL"] = "opengl32.lib"

project "Editor"
    kind "ConsoleApp"
    location "Editor"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Editor/Source/**.h",
        "Editor/Source/**.cpp"
    }
    
    includedirs
    {
        "%{wks.location}/Engine/Vendor/spdlog/include",
        "%{wks.location}/Engine/Source",
        "%{wks.location}/Engine/Vendor",
        "%{IncludeDir.ENTT}",
        "%{IncludeDir.GLM}",
    }
    
    links
    {
        "Shcore.lib",
        "Engine",
    }

    ignoredefaultlibraries
    {
        "LIBCMTD",
    }
    
    filter "system:windows"
        systemversion "latest"
        postbuildcommands
        {
            "{COPYDIR} %{wks.location}Engine/Assets %{wks.location}Binaries/" .. outputdir .. "/%{prj.name}/Assets",
            "{COPYDIR} %{wks.location}Binaries/" .. outputdir .. "/Engine/Shaders %{wks.location}Binaries/" .. outputdir .. "/%{prj.name}/Shaders"
        }
    
    filter "configurations:Debug"
        defines "_DEBUG"
        runtime "Debug"
        symbols "on"
    
    filter "configurations:Release"
        defines "_RELEASE"
        runtime "Release"
        optimize "on"
    
    filter "configurations:Dist"
        defines "_DIST"
        runtime "Release"
        optimize "on"

group "Dependencies"
    project "Glad"
        kind "StaticLib"
        location "Engine/Vendor/glad"
        language "C"
        staticruntime "On"
    
        targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
        objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")
    
        files
        {
            "Engine/Vendor/glad/include/glad/glad.h",
            "Engine/Vendor/glad/include/KHR/khrplatform.h",
            "Engine/Vendor/glad/src/glad.c"
        }
        
        includedirs
        {
            "Engine/Vendor/glad/include"
        }
        
        filter "system:windows"
            systemversion "latest"
        
        filter "configurations:Debug"
            runtime "Debug"
            symbols "on"
        
        filter "configurations:Release"
            runtime "Release"
            optimize "on"
    
    include "Engine/Vendor/imgui"
    include "Engine/Vendor/glfw"
group ""

--project "GLFW"
--    location "Projects"
--    kind "StaticLib"
--    language "C"
--    staticruntime "off"
--    
--    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
--    objdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
--    
--    files
--    {
--        "Engine/Vendor/glfw/include/GLFW/glfw3.h",
--        "Engine/Vendor/glfw/include/GLFW/glfw3native.h",
--        
--        "Engine/Vendor/glfw/src/glfw_config.h",
--        "Engine/Vendor/glfw/src/context.c",
--        "Engine/Vendor/glfw/src/init.c",
--        "Engine/Vendor/glfw/src/input.c",
--        "Engine/Vendor/glfw/src/monitor.c",
--
--        "Engine/Vendor/glfw/src/null_init.c",
--        "Engine/Vendor/glfw/src/null_joystick.c",
--        "Engine/Vendor/glfw/src/null_monitor.c",
--        "Engine/Vendor/glfw/src/null_window.c",
--        
--        "Engine/Vendor/glfw/src/platform.c",
--        "Engine/Vendor/glfw/src/vulkan.c",
--        "Engine/Vendor/glfw/src/window.c"
--    }
--
--    filter "system:linux"
--        pic "On"
--        systemversion "latest"
--    
--        files
--        {
--            "Engine/Vendor/glfw/src/x11_init.c",
--            "Engine/Vendor/glfw/src/x11_monitor.c",
--            "Engine/Vendor/glfw/src/x11_window.c",
--            "Engine/Vendor/glfw/src/xkb_unicode.c",
--            "Engine/Vendor/glfw/src/posix_time.c",
--            "Engine/Vendor/glfw/src/posix_thread.c",
--            "Engine/Vendor/glfw/src/glx_context.c",
--            "Engine/Vendor/glfw/src/egl_context.c",
--            "Engine/Vendor/glfw/src/osmesa_context.c",
--            "Engine/Vendor/glfw/src/linux_joystick.c"
--        }
--    
--        defines
--        {
--            "_GLFW_X11"
--        }
--    
--    filter "system:windows"
--        systemversion "latest"
--    
--        files
--        {
--            "Engine/Vendor/glfw/src/win32_init.c",
--            "Engine/Vendor/glfw/src/win32_joystick.c",
--            "Engine/Vendor/glfw/src/win32_monitor.c",
--            "Engine/Vendor/glfw/src/win32_time.c",
--            "Engine/Vendor/glfw/src/win32_thread.c",
--            "Engine/Vendor/glfw/src/win32_window.c",
--            "Engine/Vendor/glfw/src/wgl_context.c",
--            "Engine/Vendor/glfw/src/egl_context.c",
--            "Engine/Vendor/glfw/src/osmesa_context.c"
--        }
--        
--        defines
--        {
--            "_GLFW_WIN32",
--            "_CRT_SECURE_NO_WARNINGS"
--        }
--
--        links
--        {
--            "Dwmapi.lib"
--        }
--        
--    filter "configurations:Debug"
--        runtime "Debug"
--        symbols "on"
--        
--    filter "configurations:Release"
--        runtime "Release"
--        optimize "on"
--        
--    filter "configurations:Dist"
--        runtime "Release"
--        optimize "on"
--        symbols "off"
--
--project "ImGui"
--    location "Projects"
--    kind "StaticLib"
--    language "C++"
--    cppdialect "C++17"
--    
--    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
--    objdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
--    
--    files
--    {
--        "Engine/Vendor/imgui/imconfig.h",
--        "Engine/Vendor/imgui/imgui.h",
--        "Engine/Vendor/imgui/imgui.cpp",
--        "Engine/Vendor/imgui/imgui_draw.cpp",
--        "Engine/Vendor/imgui/imgui_internal.h",
--        "Engine/Vendor/imgui/imgui_widgets.cpp",
--        "Engine/Vendor/imgui/imstb_rectpack.h",
--        "Engine/Vendor/imgui/imstb_textedit.h",
--        "Engine/Vendor/imgui/imstb_truetype.h",
--        "Engine/Vendor/imgui/imgui_demo.cpp"
--    }
--    
--    filter "system:windows"
--        systemversion "latest"
--    
--    filter "system:linux"
--        pic "On"
--        systemversion "latest"
--    
--    filter "configurations:Debug"
--        runtime "Debug"
--        symbols "on"
--    
--    filter "configurations:Release"
--        runtime "Release"
--        optimize "on"

project "Engine"
    kind "StaticLib"
    location "Engine"
    staticruntime "off"
    language "C++"
    cppdialect "C++17"
    
    targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/Intermediates/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Engine/Source/pch.cpp"

    files
    {
        "%{prj.name}/Shaders/**.vert",
        "%{prj.name}/Shaders/**.frag",
        "%{prj.name}/Shaders/**.comp",
        
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.hpp",
        "%{prj.name}/Source/**.cpp",
        "%{prj.name}/Vendor/glm/glm/**.hpp",
        "%{prj.name}/Vendor/glm/glm/**.inl",
    }
    
    includedirs
    {
        "%{IncludeDir.Core}",
        "%{IncludeDir.Source}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.SPDLOG}",
        "%{IncludeDir.IMGUI}",
        "%{IncludeDir.STB_IMAGE}",
        "%{IncludeDir.TINYOBJLOADER}",
        "%{IncludeDir.ENTT}"
        -- "%{IncludeDir.VulkanSDK}",
    }
    
    libdirs
    {
        -- "%{LibraryDir.VulkanSDK}",
        -- "%{LibraryDir.GLFW}",
    }
    
    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }
    
    links
    {
        --"Shcore.lib",
        "%{Libraries.OpenGL}",
        "GLFW",
        "Glad",
        "ImGui",
    }
    
    ignoredefaultlibraries
    {
        "LIBCMTD",
    }
    
    filter "system:windows"
        rules { "CompileShaders" }
        systemversion "latest"
        
        defines
        {
            "_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "_RELEASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "_DISTRIBUTION"
        runtime "Release"
        optimize "On"