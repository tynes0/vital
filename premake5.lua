project "vital"
    kind "StaticLib"
	location "vital"
    language "C++"
	cppdialect "C++latest"
	targetdir "bin/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"
	objdir "bin-int/%{cfg.buildcfg}-%{cfg.system}/%{prj.name}"
    
	defines { "_CRT_SECURE_NO_WARNINGS" }

    files { "vital/src/**.cpp", "vital/src/**.h" }
    includedirs { "vital/src" }

    filter "configurations:Debug"
		defines { "VITAL_DEBUG" }
        symbols "On"

    filter "configurations:Release"
		defines { "VITAL_RELEASE" }
        optimize "On"