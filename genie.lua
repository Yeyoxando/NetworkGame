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
			targetdir ("./bin")
			targetsuffix "_d"
			objdir ("./build/Debug")
			flags { "Symbols", "NoPCH" }

		configuration "Release"
			targetdir ("./bin")
			objdir ("./build/Release")
			flags { "Optimize", "NoPCH" }

	end


project "NetworkGame"

  language "C++"
	kind "ConsoleApp"
	

	includedirs{
	  "./include/",
	  "./include/game_engine/",
	  "./src/",
	  "./tests/",
	  "./deps/glm/glm/",
	  "./deps/SDL2_ttf-2.0.15/include",
	  "./deps/SDL2-2.0.14/include",
	  "./deps/SDL2_image-2.0.5/include",
	}

	--Common files
	files{
		"./include/**.h",
	  	"./include/game_engine/**.h",
		"./src/**.cpp",
		"./src/game_engine/**.cpp",
		"./tests/client_main.cpp",
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
		"./include/game_data.h",
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

