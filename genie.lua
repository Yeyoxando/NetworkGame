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
	  "./src/",
	  "./tests/",
	  "./deps/SDL2-2.0.14/include",
	}

	--Common files
	files{
		"./include/**.h",
		"./src/**.cpp",
		"./tests/client_main.cpp",
	    "./deps/SDL2-2.0.14/include/**.h",
	}

	defines { 	
		"WIN32",
		"_WIN32",
		"_WINDOWS",
	}	 
	
	links{
		"ws2_32",
		"./deps/SDL2-2.0.14/lib/x64/SDL2",
		"./deps/SDL2-2.0.14/lib/x64/SDL2main",
	}



project "NetworkServer"

  language "C++"
	kind "ConsoleApp"
	

	includedirs{
	  "./include/",
	  "./src/",
	  "./tests/",
	  "./deps/SDL2-2.0.14/include",
	}

	--Common files
	files{
		"./include/**.h",
		"./src/**.cpp",
		"./tests/server_main.cpp",
	    --"./deps/SDL2-2.0.14/include/**.h",
	}

	defines { 	
		"WIN32",
		"_WIN32",
		"_WINDOWS",
	}	 
	
	links{
		"ws2_32",
		--"./deps/SDL2-2.0.14/lib/x64/SDL2",
		--"./deps/SDL2-2.0.14/lib/x64/SDL2main",
	}

