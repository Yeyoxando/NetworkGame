
del ".\build\Debug\" /s /q 
del ".\build\NetworkGame\" /s /q 
del ".\build\NetworkServer\" /s /q 
del ".\build\Project3.sln"

.\tools\genie.exe vs2019

.\build\Project3.sln

exit