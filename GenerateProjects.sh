echo 'Generating Visual Studio solutions for Elixir'
output=$( cmake -B build -S . -DEXR_QUALITY_LEVEL:STRING="0" -DCMAKE_CONFIGURATION_TYPES:STRING="Debug;Release;" )

if [[ $output == '0' ]]; then
    echo Projects generated successfully
else
    echo 'CMake exited with errors'
    echo 'Projects generation failed'
fi

$SHELL
