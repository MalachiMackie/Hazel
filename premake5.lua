workspace "Cheezy"
	architecture "x86_64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]		= "Cheezy/vendor/GLFW/include"
IncludeDir["Glad"]		= "Cheezy/vendor/Glad/include"
IncludeDir["lua"]		= "Cheezy/vendor/lua/include"
IncludeDir["LuaBridge"]	= "Cheezy/vendor/LuaBridge"
IncludeDir["ImGui"]		= "Cheezy/vendor/imgui"
IncludeDir["glm"]		= "Cheezy/vendor/glm"
IncludeDir["stb_image"] = "Cheezy/vendor/stb_image"


group "Dependencies"
	include "Cheezy/vendor/GLFW"
	include "Cheezy/vendor/Glad"
	include "Cheezy/vendor/imgui"

group ""

project "Cheezy"
	location "Cheezy"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "hzpch.h"
	pchsource "Cheezy/src/hzpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/lua/**.h",
		"%{prj.name}/vendor/lua/**.hpp",
		"%{prj.name}/vendor/LuaBridge/**.h"
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
		"%{IncludeDir.LuaBridge}",
		"%{IncludeDir.lua}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	libdirs
	{
		"%{prj.name}/vendor/lua"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"lua53"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CZ_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "CZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CZ_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location  "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/**.lua"
	}

	includedirs
	{
		"Cheezy/vendor/spdlog/include",
		"Cheezy/vendor/lua/include",
		"Cheezy/src",
		"Cheezy/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.LuaBridge}"
	}

	links
	{
		"Cheezy"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "CZ_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CZ_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CZ_DIST"
		runtime "Release"
		optimize "on"
		
	filter { "system:windows", "configurations:Dist or Release"}
		kind "WindowedApp"
		linkoptions {"/SUBSYSTEM:windows /ENTRY:mainCRTStartup"}