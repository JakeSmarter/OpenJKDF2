# OpenJKDF2 Todo List

## OpenJKDF2
* [ ] Retrieve version information via system APIs rather than global variables
  * Exclude [`src/version.h`](src/version.h) and [`src/version.c`](src/version.c)
    on all PE and ELF systems
* [ ] Fix HUD scaling to match integer multiples of viewport&rsquo;s resolution
* [ ] Add a game asset directory or folder chooser on first launch
  * The path to the game asset directory should be stored in OpenJKDF2&rsquo;s
    user config
  * [ ] Add an `--asset-dir [<path to game assets>]` or `--game-asset-dir [<path to game assets>]`
    command line option
    * When invoked without a *new* path OpenJKDF2 should prompt for the
      game asset directory with a directory or folder chooser (like on a first
      launch), otherwise update the path to the game asset directory in the
      user&rsquo;s config.
* [ ] Do not write into the asset game directory (like the original `JK.EXE` implementation on Windows 9x/ME did)
  * Writing into the game asset directory is a &#x26D4;**NO GO**&#x26D4; on
    modern operating systems. Besides, the game asset directory and file tree
    may be read&#x2011;only. Instead, write into user&rsquo;s config and cache
    directories.
* [ ] Add palette texturing support (only on GPUs that support palette texture formats)
* [ ] Add texture compression support, especially on [Android](#Android)
  * Textures should be compressed at runtime because hardware supports different
    compression algos and formats and we must not distribute
    pre&#x2011;compressed textures
  * Compressed textures should be cached for faster launches
* [ ] Add proper message and error logging support
  * [ ] Add colored &#x1F308; console messages
* [ ] Add l11n support
  * [ ] Investigate the best way to build OS specific translation resources from
    one message source base. In other words, do not naively assume that gettext
    is or can be the magic bullet.

### CMake
* [ ] Move building all dependencies into CMake scripts

## Windows
* [x] Add assembly manifest
* [x] Add Windows version information
  * [ ] Stringify macros in [`openjdkf2.rc`](packaging/win32/openjkdf2.rc "openjdkf2.rc")
    where applicable
    * [ ] `OPENJKDF2_FILE_VERSION`
    * [ ] `OPENJKDF2_COMPANY_NAME`
    * [ ] `OPENJKDF2_INTERNAL_NAME`
    * [ ] `OPENJKDF2_ORIGINAL_FILENAME`
* [ ] Link with `MSVCR120.DLL` aka [Microsoft Visual C++ 2013 Redistributable Package](https://www.microsoft.com/download/40784)
  * [ ] Update [`README.md`](README.md) for Windows users with download links to
    the Microsoft Visual C++ 2013 Redistributable Package
  * x86 and AMD64 architectures should update to
    [Microsoft Visual C++ 2013 Update 5 Redistributable Package](https://support.microsoft.com/help/3138367 "Visual C++ 2013 Update 5 Redistributable Package")
* [ ] Add [MSIX](https://learn.microsoft.com/en-us/windows/msix "MSIX") release &#x1F4E6;
  * MSIX is genuinely supported on all modern Windows releases since Windows 10
    and with [MSIXCore](https://github.com/microsoft/msix-packaging "MSIXCore")
    supported since Windows 7 SP1 and later
  * Can be cross&#x2011;built since its inception &#x1F44D; because conceptually
    it is actually a cross&#x2011;platform package manager
* [ ] [UWP](https://learn.microsoft.com/en-us/windows/uwp "Universal Windows Platform") (.appx) release &#x1F4E6;
  * Although [.appx](https://learn.microsoft.com/en-us/windows/uwp/packaging) is a
    subset of MSIX, do not confuse these two
  * A UWP release should also *automatically* enable Xbox One and Xbox Series X|S
    support &#x1F91E;
  * .appx packages can be cross&#x2011;built using the same tools as MSIX &#x1F44D;
  * In simple terms, UWP requires linking to `MSVCR120_APP.DLL`
* [ ] Add a [MSI](https://learn.microsoft.com/en-us/windows/win32/msi/windows-installer-portal "Microsoft Windows Installer") release &#x1F4E6;
  * Creating an MSI package, including cross&#x2011;building, is certainly
    doable. However, due to MSI&rsquo;s nature it is a quite complex and time
    consuming process and it does not support auto&#x2011;updates.
* [ ] Add full UNICODE support
  * OpenJKDF2 should use UNICODE internally and only *fall back* to ANSI when
    dealing with game assets

## Android
* [ ] Add texture compression support
  * [ ] Definitely support ETC1 for all non&#x2011;alpha and
    non&#x2011;color&#x2011;key (1&#x2011;bit alpha) textures
  * [ ] Perhaps test palette texture format rendering for performance advantage
    over compressed texture formats?
    * Some GPUs may perform better rendering palette texture formats than
      compressed texture formats, possibly the Raspberry Pis 3 and 4
      (aka BCM2711)
