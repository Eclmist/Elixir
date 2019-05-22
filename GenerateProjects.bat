@echo off

echo Generating Visual Studio solutions for Elixir
cmake -B build -S . -DEXR_QUALITY_LEVEL:STRING="0" -DCMAKE_CONFIGURATION_TYPES:STRING="Debug;Release;" 

if %errorlevel% == 0 (
    color 2F
    echo Projects generated successfully
) else (
    echo CMake exited with error code %errorlevel%
    echo Projects generation failed
    color CF
)

PAUSE