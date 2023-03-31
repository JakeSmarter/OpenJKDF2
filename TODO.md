# OpenJKDF2 Todo List

## OpenJKDF2
* [ ] Retrieve version information via system APIs rather than global variables
  * Exclude [`src/version.h`](src/version.h) and [`src/version.c`](src/version.c)
    on all PE and ELF systems
* [ ] Fix HUD scaling to match integer multiples of viewport&rsquo;s resolution
* [ ] Add a game asset directory or folder chooser on first launch
  * The path to the game asset directory should be stored in OpenJKDF2&rsquo;s
    user config
  * [ ] Add an `--asset-dir [<path to game assets>]` or `--game-asset-dir [<path to game assets>]` command line option
    * When invoked without a *new* path OpenJKDF2 should prompt for the
      game asset directory with a directory or folder chooser (like on a first
      launch), otherwise update the path to the game asset directory in the
      user&rsquo;s config.
* [ ] Do not write into the asset game directory (like the original `JK.EXE` implementation on Windows 9x/ME did)
  * Writing into the game asset directory is a &#x26D4;**NO GO**&#x26D4; on modern operating
    systems. Besides, the game asset directory and file tree may be
    read&#x2011;only. Instead, write into user&rsquo;s config and cache
    directories.
* [ ] Add palette texturing support (only on GPUs that support palette texture formats)
* [ ] Add texture compression support, especially on [Android](#Android)
  * Textures should be compressed at runtime because hardware supports different
    compression algos and formats and we must not distribute
    pre&#x2011;compressed textures
  * Compressed textures should be cached for faster launches

### CMake

## Windows
* [x] Add assembly manifest
* [ ] Add Windows version information
  * [ ] Stringify macros in [`openjdkf2.rc`](packaging/win32/openjkdf2.rc "openjdkf2.rc") where applicable
    * [ ] `OPENJKDF2_FILE_VERSION`
    * [ ] `OPENJKDF2_COMPANY_NAME`
    * [ ] `OPENJKDF2_INTERNAL_NAME`
    * [ ] `OPENJKDF2_ORIGINAL_FILENAME`
* [ ] Link with `msvcr120.dll` aka [Microsoft Visual C++ 2013 Redistributable Package](https://www.microsoft.com/download/40784)
  * [ ] Update [`README.md`](README.md) for Windows users with download links to
    the Microsoft Visual C++ 2013 Redistributable Package
  * x86 and AMD64 architectures should update to
    [Microsoft Visual C++ 2013 Update 5 Redistributable Package](https://support.microsoft.com/help/3138367 "Visual C++ 2013 Update 5 Redistributable Package")

## Android
* [ ] Add texture compression support
  * [ ] Definitely support ETC1 for all non&#x2011;alpha and
    non&#x2011;color&#x2011;key (1&#x2011;bit alpha) textures
  * [ ] Perhaps test palette texture format rendering for performance advantage over compressed texture formats?
    * Some GPUs may perform better rendering palette texture formats than
      compressed texture formats, possibly the Raspberry Pis 3 and 4 (aka BCM2711)
