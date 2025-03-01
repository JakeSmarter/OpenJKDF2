## Building OpenJKDF2

Building is currently tested primarily on Arch Linux, Ubuntu 20.04, and MacOS Monterey. Windows releases are cross-compiled using MinGW, however MSVC/Visual Studio is supported.

### 1) Install dependencies

<details>
  <summary>Arch Linux Dependencies</summary>

```
# All
pacman -S git cmake base-devel make python python-pip bison imagemagick gtk3 openssl protobuf zsh libpng curl
pip3 install cogapp

# Win32/MinGW
pacman -S mingw-w64

# Linux 32-bit
pacman -S multilib-devel lib32-sdl2 lib32-sdl2_mixer lib32-glew lib32-openal

# Linux 64-bit
pacman -S clang sdl2 sdl2_mixer glew openal

# WebAssembly
pacaur -S emscripten
```
</details>

<details>
  <summary>Ubuntu Dependencies</summary>

```
# Make sure everything is up to date before installing new dependencies
sudo apt-get update
sudo apt-get upgrade

# All
sudo apt install git build-essential cmake make python3 python3-pip bison imagemagick libgtk-3-dev protobuf-compiler zsh libpng-dev libcurl-dev
pip3 install cogapp

# On some Ubuntu versions (22.04?)
sudo apt install libstdc++-12-dev

# Win32/MinGW
sudo apt install mingw-w64

# Linux 32-bit
# TODO find equivalents: multilib-devel lib32-sdl2 lib32-glew lib32-openal

# Linux 64-bit
sudo apt install clang libsdl2-dev libsdl2-mixer-dev libopenal-dev libglew-dev libssl-dev libprotobuf-dev

# WebAssembly
# TODO find equivalents: emscripten

# Flatpak
sudo apt install flatpak qemu-user-static
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.freedesktop.Platform/aarch64/22.08 org.freedesktop.Sdk/aarch64/22.08
flatpak install flathub org.freedesktop.Platform/x86_64/22.08 org.freedesktop.Sdk/x86_64/22.08
```

Add the following to the end of ~/.bashrc:
```
export PATH=$PATH:~/.local/bin
```
</details>

<details>
  <summary>MacOS Dependencies</summary>

Before starting, install Xcode. This is required for OpenGL headers, among other things. For packaging a Universal build on ARM64 Macs, all `brew` steps should be done both natively and with an x86_64 prefix (see [here](https://gist.github.com/progrium/b286cd8c82ce0825b2eb3b0b3a0720a0) for how to set that up).
```
# All
brew install git cmake make python3 imagemagick
pip3 install cogapp generate-iconset

# After installing cogapp, make sure the following is in your ~/.zshrc:
# export PATH=$PATH:$HOME/Library/Python/3.8/bin

# Win32/MinGW building
brew install mingw-w64

# MacOS 64-bit
brew install openal-soft sdl2 sdl2_mixer glew openssl@1.1 protobuf libpng libmodplug

# WebAssembly
brew install emscripten
```
</details>

<details>
  <summary>Windows Dependencies</summary>

Download and install [Python 3.8](https://www.python.org/downloads/release/python-380/) (be sure it is added to your PATH when installing).

Install [cog](https://nedbatchelder.com/code/cog/index.html#h_installation) using pip (`pip3 install cogapp`). You may need to specify `pip` instead of `pip3`.

Download and install the OpenAL 1.1 SDK from [here](https://www.openal.org/downloads/). Then, add `OPENALDIR` to your system environment variables as `C:\Program Files (x86)\OpenAL 1.1 SDK`.

Download and install the latest CMake from [here](https://cmake.org/download/), or use Visual Studio 2022 which includes CMake support.

</details>

### 2) Compile the desired target

<details>
  <summary>64-bit Linux/SDL2</summary>

64-bit Linux supports both x86_64 and ARM64 targets, and has been tested on Intel, NVIDIA and V3D (Raspberry Pi 4) graphics cards. **GCC <11.1 is currently not supported due to crashes, use clang instead.**

```
git clone https://github.com/shinyquagsire23/OpenJKDF2.git
cd OpenJKDF2

export CC=clang
export CXX=clang++

chmod +x build_linux64.sh
./build_linux64.sh
LD_LIBRARY_PATH=build_linux64 ./build_linux64/openjkdf2
```
</details>

<details>
  <summary>64-bit Windows/SDL2, using MinGW</summary>

64-bit Windows can be cross-compiled from Linux or MacOS, and has been tested on Intel and NVIDIA graphics cards.

```
git clone https://github.com/shinyquagsire23/OpenJKDF2.git
cd OpenJKDF2

chmod +x build_win64.sh
./build_win64.sh
```
</details>

<details>
  <summary>MacOS SDL2</summary>

A full, universal MacOS appbundle can be created on ARM64 Macs using
```
git clone https://github.com/shinyquagsire23/OpenJKDF2.git
cd OpenJKDF2

./distpkg_macos.sh
```

Otherwise, a plain binary and single-architecture appbundle can be compiled using:
```
chmod +x ./.github/build_macos.sh
./.github/build_macos.sh
```
</details>

<details>
  <summary>Emscripten/WebAssembly</summary>

WASM builds are semi-supported, but break often. The last tested tag for WASM is `v0.2.0`.

```
mkdir -p wasm_out
```

Copy your `episode/` and `resource/` directory to `wasm_out`, then

```
chmod +x ./build_run_openjkdf2_wasm.sh
./build_run_openjkdf2_wasm.sh
```
</details>

<details>
  <summary>x86 Linux/SDL2, mmap blobs</summary>

OpenJKDF2 supports an experimental hybrid compilation for Linux/SDL2 which uses `JK.EXE` for any unimplemented functions. Compile using: 

```
mkdir -p build_blobs
cd build_blobs

cmake .. -DOPENJKDF2_USE_BLOBS=true
make -j10
``` 

then copy `openjkdf2` to the same directory as JK.EXE and run it. *JK.EXE version 1.0.0 is required in order to use blobs!*

`mmap` is used to maintain all `.rodata`, `.data`, and `.bss` variables in the same addresses as `JK.EXE`, and if `openjkdf2` invokes an unimplemented function, it will jump to the mapped `JK.EXE` implementation.
</details>

<details>
  <summary>32-bit Linux/SDL2, blobless</summary>

```
mkdir -p build
cd build

cmake .. --toolchain ../cmake_modules/toolchain_linux_32.cmake
make -j10
```
</details>

<details>
  <summary>x86 Win32/MinGW hook DLL</summary>

`./build.sh`
</details>

<details>
  <summary>x86_64 Visual Studio 2022 Project</summary>

Clone the repository using git, then open VS 2022 and select `Open a local folder`. Right click CMakeLists.txt and select `Configure OpenJKDF2` until it succeeds (for some reason it errors a few times initially on SDL2_mixer, etc). Once it succeeds, right click CMakeLists.txt and select `Build`. 

After it builds you can set it as a startup item, from there it should work and debug as expected.
</details>

<details>
  <summary>x86_64 Windows MSVC/Visual Studio Project (Legacy, deprecated)</summary>

Run the CMake GUI and select the `OpenJKDF2/` directory for your sources. Create `OpenJKDF2/build` and set it as your build folder. Click `Configure` until it succeeds (for some reason it errors a few times initially), then click `Generate` and `Open Project`.

You'll probably want to set `openjkdf2-64` as the default project, from there it should work and debug as expected.
</details>
