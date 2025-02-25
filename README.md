# sys2-f1-wheel-sim
A project to simulate an F1 wheel on a raspberry pi pico + LCD

### Setting up clangd to recognise PicoSDK

1. Navigate to the `pico/build` directory

2. Prepare cmake with the command `cmake .. -DPICO_SDK_PATH=../../../pico-sdk -DCMAKE_EXPORT_COMPILE_COMMANDS=ON`

3. We need to symlinkt to the project root, so navigate to the root project directory and symlink with `ln -s pico/build/compile_commands.json compile_commands.json`
