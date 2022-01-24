workspace "TickerTimescaleSwap"
	architecture "x64"
	startproject "TickerTimescaleSwap"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}"

	IncludeDir = {}
    IncludeDir["csv_parser"] = "vendor/csv-parser"

	CppVersion = "C++17"

	project "TickerTimescaleSwap"
		location "src/"
		kind "ConsoleApp"
		language "C++"

		targetdir ("bin/" .. outputdir)
		objdir ("bin/int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"src/**.hpp",
			"src/**.h",
			"src/**.cpp",
			"src/**.asm"
		}

		includedirs
		{
            "%{IncludeDir.csv_parser}",
			"src/"
		}

		libdirs
		{
			"bin/lib"
		}

		links
		{
			"pthread"
		}

		flags { "NoImportLib", "Maps" }

		filter "configurations:Debug"
			flags { "LinkTimeOptimization", "MultiProcessorCompile" }
			editandcontinue "Off"
			defines { "DEBUG" }

		filter "configurations:Release"
			flags { "LinkTimeOptimization", "NoManifest", "MultiProcessorCompile" }
			defines { "RELEASE" }
			optimize "speed"
