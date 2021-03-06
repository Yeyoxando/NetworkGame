solution ("Project3")
	configurations { "Debug", "Release" }
	platforms { "x64" }
	location ("build")	
	
	projects = { "NetworkGame", "NetworkServer" }

	for i, prj in ipairs(projects) do 
		project (prj)
		targetname (prj)
		
		prj_path = "./build/" .. prj

		location (prj_path .. "/" .. _ACTION)
	
        flags { "ExtraWarnings", "Cpp17" }

		defines { 	
			"_CRT_SECURE_NO_WARNINGS",
			}
		configuration "vs2019"
			windowstargetplatformversion "10.0.19041.0"

		configuration "Debug"
			defines { "DEBUG", "ASSERT" }
			targetdir ("./bin/Debug/x64")
			targetsuffix "_d"
			objdir ("./build/Debug")
			flags { "Symbols", "NoPCH" }

		configuration "Release"
			targetdir ("./bin/Release/x64")
			objdir ("./build/Release")
			flags { "Optimize", "NoPCH" }

	end


project "NetworkGame"

  language "C++"
	kind "ConsoleApp"
	

	includedirs{
	  "./include/",
	  "./include/game_engine/",
	  "./include/game_script/",
	  "./src/",
	  "./src/game_engine/",
	  "./src/game_script/",
	  "./tests/",

	  "./deps/glm/glm/",

	  "./deps/imgui/",
	  "./deps/imgui_sdl/",
	  "./deps/imgui/backends/",

	  "./deps/SDL2_ttf-2.0.15/include",
	  "./deps/SDL2-2.0.14/include",
	  "./deps/SDL2_image-2.0.5/include",
	}

	--Common files
	files{
		"./include/**.h",
	  	"./include/game_engine/**.h",
	  	"./include/game_script/**.h",
		"./src/**.cpp",
		"./src/game_engine/**.cpp",
		"./src/game_script/**.cpp",
		"./tests/client_main.cpp",

	 	"./deps/imgui/*.h",
	  	"./deps/imgui/*.cpp",
	 	"./deps/imgui_sdl/*.h",
	  	"./deps/imgui_sdl/*.cpp",
	  	"./deps/imgui/backends/imgui_impl_sdl.h",
	  	"./deps/imgui/backends/imgui_impl_sdl.cpp",

	    	"./deps/SDL2-2.0.14/include/**.h",
	    	"./deps/SDL2_image-2.0.5/include/**.h",
	  	"./deps/SDL2_ttf-2.0.15/include/**.h",

		--GLM
		"./deps/glm/glm/**.h",
		"./deps/glm/glm/*.hpp",
	}

	defines { 	
		"WIN32",
		"_WIN32",
		"_WINDOWS",
		"GLM_FORCE_RADIANS",
	}	 
	
	links{
		"ws2_32",
		"./deps/SDL2-2.0.14/lib/x64/SDL2",
		"./deps/SDL2-2.0.14/lib/x64/SDL2main",
		"./deps/SDL2_image-2.0.5/lib/x64/SDL2_image",
		"./deps/SDL2_ttf-2.0.15/lib/x64/SDL2_ttf",
	}



project "NetworkServer"

  language "C++"
	kind "ConsoleApp"
	

	includedirs{
	  "./include/",
	  "./deps/glm/glm/",
	  "./tests/",
	}

	--Common files
	files{
		"./include/network_data.h",
		"./include/network_helpers.h",
		"./tests/server_main.cpp",

		--GLM
		"./deps/glm/glm/**.h",
		"./deps/glm/glm/*.hpp",
	}

	defines { 	
		"WIN32",
		"_WIN32",
		"_WINDOWS",
		"GLM_FORCE_RADIANS",
	}	 
	
	links{
		"ws2_32",
	}

