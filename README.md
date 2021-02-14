Raycasting implementation for Programming course - 3rd year.

# IDE
# VSCode 
Basic setup for VS Code ide.

## Plugins
Install following plugins:
- C/C++ Intellisense
- CMake Tools
- CMake

## Configurations

Configure under `.vscode/` the folliwing files:
- `settings.json`
```
{
    "cmake.configureSettings": { 
        "CMAKE_MODULE_PATH": "${workspaceFolder}/cmake/",
        "SDL2_PATH": "${workspaceFolder}/cmake/sdl2/",
        "SDL2_IMAGE_PATH": "${workspaceFolder}/cmake/sdl2_image/"
    },
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools"
}
```
- `launch.json`
```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(Windows) Launch",
            "type": "cppvsdbg",
            "request": "launch",
            // Resolved by CMake Tools:
            "program": "${command:cmake.launchTargetPath}",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [
                {
                    "name": "PATH",
                    "value": "$PATH:${command:cmake.launchTargetDirectory}"
                }
            ],
            "externalConsole": false
        }
    ]
}
```