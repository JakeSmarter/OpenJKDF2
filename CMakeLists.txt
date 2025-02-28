cmake_minimum_required(VERSION 3.20)

if(PLAT_MACOS_X86_64)
    set(CMAKE_OSX_ARCHITECTURES "x86_64")

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    set(SDL2_DIR "/usr/local/opt/sdl2")
    set(SDL2_MIXER_PATH "/usr/local/opt/sdl2_mixer")
    set(GLEW_PATH "/usr/local/opt/glew")
    set(CMAKE_IGNORE_PATH "/opt/homebrew")
    set(CMAKE_IGNORE_PATH "/opt/homebrew/include")
    set(CMAKE_IGNORE_PATH "/opt/homebrew/lib")

    list(APPEND CMAKE_PREFIX_PATH /usr/local)
endif()

project(OpenJKDF2)

# All of our supported platforms
set(PLAT_LINUX_32 FALSE CACHE BOOL "Linux 32-bit target")
set(PLAT_LINUX_64 FALSE CACHE BOOL "Linux 64-bit target")
set(PLAT_MACOS FALSE CACHE BOOL "MacOS target")
set(PLAT_MACOS_X86_64 FALSE CACHE BOOL "MacOS x86_64")
set(PLAT_MINGW_X86_64 FALSE CACHE BOOL "MinGW Win64 target")
set(PLAT_WASM FALSE CACHE BOOL "Emscripten WASM target")
set(PLAT_HOOKS FALSE CACHE BOOL "Hook DLL target")
set(PLAT_MSVC FALSE CACHE BOOL "Windows 64-bit MSVC")
set(PLAT_ANDROID_ARM64 FALSE CACHE BOOL "Android AArch64")
set(PLAT_AUTO FALSE CACHE BOOL "Target the host OS")

# Set PLAT_AUTO if a platform isn't specifically selected
if(NOT PLAT_LINUX_32 AND NOT PLAT_LINUX_64 AND NOT PLAT_MACOS AND NOT PLAT_MACOS_X86_64 AND NOT PLAT_MINGW_X86_64 AND NOT PLAT_WASM AND NOT PLAT_HOOKS)
    set(PLAT_AUTO TRUE)
endif()

# Internal targets: Stuff like OpenGL vs D3D, OpenAL vs null sound, etc
set(TARGET_USE_PHYSFS FALSE)
set(TARGET_USE_BASICSOCKETS FALSE)
set(TARGET_USE_GAMENETWORKINGSOCKETS FALSE)
set(TARGET_USE_LIBSMACKER FALSE)
set(TARGET_USE_LIBSMUSHER FALSE)
set(TARGET_USE_SDL2 FALSE)
set(TARGET_USE_OPENGL FALSE)
set(TARGET_USE_OPENAL FALSE)
set(TARGET_USE_D3D FALSE)
set(TARGET_POSIX FALSE)
set(TARGET_LINUX FALSE)
set(TARGET_MACOS FALSE)
set(TARGET_WIN32 FALSE)
set(TARGET_WASM FALSE)
set(TARGET_ANDROID FALSE)
set(TARGET_ANDROID_ARM64 FALSE)
set(TARGET_HOOKS FALSE)
set(TARGET_NO_BLOBS FALSE)
set(OPENJKDF2_USE_BLOBS FALSE CACHE BOOL "Use blobs")

set(DEBUG_QOL_CHEATS $ENV{DEBUG_QOL_CHEATS})

# Options for compiling
if(DEFINED ENV{OPENJKDF2_NO_ASAN})
    set(OPENJKDF2_NO_ASAN $ENV{OPENJKDF2_NO_ASAN})
else()
    set(OPENJKDF2_NO_ASAN TRUE)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" AND PLAT_AUTO)
    set(PLAT_LINUX_64 TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND PLAT_AUTO)
    set(PLAT_MACOS TRUE)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows" AND PLAT_AUTO)
    set(PLAT_MSVC TRUE)
endif()

# General defs
add_definitions(-DOPENJKDF2_RELEASE_COMMIT="$ENV{OPENJKDF2_RELEASE_COMMIT}")
add_definitions(-DOPENJKDF2_RELEASE_COMMIT_SHORT="$ENV{OPENJKDF2_RELEASE_COMMIT_SHORT}")
add_definitions(-DOPENJKDF2_RELEASE_COMMIT_W=L"$ENV{OPENJKDF2_RELEASE_COMMIT}")
add_definitions(-DOPENJKDF2_RELEASE_COMMIT_SHORT_W=L"$ENV{OPENJKDF2_RELEASE_COMMIT_SHORT}")
add_definitions(-DQOL_IMPROVEMENTS)

set(CMAKE_CXX_STANDARD 17)

if(PLAT_LINUX_32)
    message( STATUS "Targeting Linux 32-bit" )
    set(BIN_NAME "openjkdf2")
    
    add_definitions(-D_XOPEN_SOURCE=500)
    add_definitions(-D_DEFAULT_SOURCE)

    set(TARGET_USE_PHYSFS TRUE)
    set(TARGET_USE_GAMENETWORKINGSOCKETS TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_LINUX TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_CAN_JKGM TRUE)
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_USE_CURL TRUE)

    if(OPENJKDF2_USE_BLOBS)
        set(TARGET_NO_BLOBS FALSE)
    endif()
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c11 -fshort-wchar -Wno-implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fshort-wchar -Wno-implicit-function-declaration")
    add_link_options(-fshort-wchar)

elseif(PLAT_LINUX_64)
    message( STATUS "Targeting Linux 64-bit" )
    set(BIN_NAME "openjkdf2")

    add_definitions(-DARCH_64BIT)
    add_definitions(-D_XOPEN_SOURCE=500)
    add_definitions(-D_DEFAULT_SOURCE)

    set(TARGET_USE_PHYSFS TRUE)
    set(TARGET_USE_GAMENETWORKINGSOCKETS TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_LINUX TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_CAN_JKGM TRUE)
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_USE_CURL TRUE)
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c11 -fshort-wchar -Wno-implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fshort-wchar -Wno-implicit-function-declaration")
    add_link_options(-fshort-wchar)

elseif(PLAT_MACOS)
    message( STATUS "Targeting MacOS" )
    set(BIN_NAME "openjkdf2-64")

    add_definitions(-DARCH_64BIT)

    # macOS specific options
    set(MACOSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version" FORCE)

    set(TARGET_USE_PHYSFS TRUE)
    #set(TARGET_USE_BASICSOCKETS TRUE)
    set(TARGET_USE_GAMENETWORKINGSOCKETS TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_MACOS TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_CAN_JKGM TRUE)
    set(TARGET_USE_CURL TRUE)

    set(BUNDLE "${PROJECT_SOURCE_DIR}/OpenJKDF2.app")
    set(HOMEBREW_PREFIX $ENV{HOMEBREW_PREFIX})

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c11 -O2 -Wuninitialized -fshort-wchar -Wall -Wno-unused-variable -Wno-parentheses -Wno-missing-braces -Wno-implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fshort-wchar")
    
    add_link_options(-fshort-wchar -Wl,-map,openjkdf2.map)

elseif(PLAT_MACOS_X86_64)
    message( STATUS "Targeting MacOS x86_64 ${CMAKE_OSX_ARCHITECTURES}" )
    set(BIN_NAME "openjkdf2-64")

    add_definitions(-DARCH_64BIT)

    # macOS specific options
    set(MACOSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "Minimum OS X deployment version" FORCE)

    set(TARGET_USE_PHYSFS TRUE)
    set(TARGET_USE_GAMENETWORKINGSOCKETS TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_MACOS TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_CAN_JKGM TRUE)
    set(TARGET_USE_CURL TRUE)

    set(BUNDLE "${PROJECT_SOURCE_DIR}/OpenJKDF2_x86_64.app")
    set(HOMEBREW_PREFIX "/usr/local")

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c11 -O2 -Wuninitialized -fshort-wchar -Wall -Wno-unused-variable -Wno-parentheses -Wno-missing-braces -Wno-implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fshort-wchar")
    add_link_options(-fshort-wchar)

    #set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${HOMEBREW_PREFIX}/opt/protobuf/lib/pkgconfig")

elseif(PLAT_MINGW_X86_64)
    message( STATUS "Targeting Win64 MinGW" )

    set(BIN_NAME "openjkdf2-64")

    add_definitions(-DWIN64)
    add_definitions(-DWIN64_STANDALONE)
    add_definitions(-DARCH_64BIT)
    add_definitions(-DWIN32)
    add_definitions(-DWIN64_MINGW)

    if (NOT DEFINED GITHUB_RUNNER_COMPILE)
        set(TARGET_USE_PHYSFS TRUE)
        set(TARGET_USE_GAMENETWORKINGSOCKETS TRUE)
    endif()
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_WIN32 TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_CAN_JKGM TRUE)
    set(TARGET_USE_CURL TRUE)

    # TODO: Bump to O2 eventually. MinGW likes to replace memset with calls to itself...
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -ggdb -fno-omit-frame-pointer -Wno-implicit-function-declaration")
    
    add_link_options(-Wl,-Map=output-win64.map -fno-trapping-math -fshort-wchar -D_MBCS)
    add_compile_options(-Wuninitialized -fno-trapping-math -fshort-wchar -Dmain=SDL_main -D_MBCS -static -pthread)

    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/freeglut/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/glew/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer/x86_64-w64-mingw32/include/SDL2)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/SDL2/x86_64-w64-mingw32/include/SDL2)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/libpng-1.2.37-lib/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/zlib-1.2.3-lib/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/GameNetworkingSockets/include)

elseif(PLAT_WASM)
    message( STATUS "Targeting Emscripten WASM" )

    set(BIN_NAME "openjkdf2")

    add_definitions(-DARCH_WASM)

    set(TARGET_USE_PHYSFS FALSE)
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_WASM TRUE)
    set(TARGET_CAN_JKGM FALSE)
    set(TARGET_USE_CURL FALSE)

    add_link_options(-fno-exceptions)
    add_compile_options(-fno-exceptions)
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_MIXER=2 -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES2=1 -s FULL_ES3=1 -s ASYNCIFY -s INITIAL_MEMORY=200mb -s STACK_SIZE=100mb")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} --profiling --preload-file ${PROJECT_SOURCE_DIR}/wasm_out@/ ")
    set(CMAKE_EXECUTABLE_SUFFIX .js)

    add_compile_options(-O2 -Wuninitialized -fshort-wchar -Wall -Wno-unused-variable -Wno-parentheses -Wno-missing-braces)

elseif(PLAT_HOOKS)
    message( STATUS "Targeting MinGW Hooks DLL" )

    set(BIN_NAME "df2_reimpl")

    add_definitions(-DWIN32)
    add_definitions(-DWIN32_BLOBS)
    add_definitions(-DARCH_X86)
    add_definitions(-DTARGET_HAS_DPLAY)

    set(TARGET_HOOKS TRUE)
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_WIN32 TRUE)
    set(TARGET_USE_D3D TRUE)

    add_compile_options(-g -Wuninitialized -fno-trapping-math)
    add_link_options(-g -Wl,--subsystem,windows -Wl,-Map=output.map -fno-trapping-math)
elseif(PLAT_ANDROID_ARM64)
    message( STATUS "Targeting Android ARM64" )

    set(BIN_NAME "openjkdf2-armv8a")

    add_definitions(-DTARGET_ANDROID)
    add_definitions(-DLINUX)
    add_definitions(-DSTDSOUND_NULL)

    set(TARGET_USE_PHYSFS FALSE)
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL FALSE)
    set(TARGET_POSIX TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_ANDROID TRUE)
    set(TARGET_ANDROID_ARM64 TRUE)
    set(TARGET_CAN_JKGM FALSE)
    set(TARGET_USE_CURL FALSE)

    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rdparty/glew")
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/freeglut/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/glew/include)

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -std=c11 -fshort-wchar -Wno-implicit-function-declaration")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fshort-wchar")
    add_link_options(-fshort-wchar)

elseif(PLAT_MSVC)
    message( STATUS "Targeting Win64 MSVC" )

    set(BIN_NAME "openjkdf2-64")

    add_definitions(-DWIN64)
    add_definitions(-DWIN64_STANDALONE)
    add_definitions(-DARCH_64BIT)
    add_definitions(-DWIN32)

    set(TARGET_USE_PHYSFS FALSE)
    set(TARGET_USE_GAMENETWORKINGSOCKETS TRUE)
    set(OPENJKDF2_NO_ASAN TRUE)
    set(TARGET_USE_LIBSMACKER TRUE)
    set(TARGET_USE_LIBSMUSHER TRUE)
    set(TARGET_USE_SDL2 TRUE)
    set(TARGET_USE_OPENGL TRUE)
    set(TARGET_USE_OPENAL TRUE)
    set(TARGET_POSIX TRUE)
    set(TARGET_WIN32 TRUE)
    set(TARGET_NO_BLOBS TRUE)
    set(TARGET_CAN_JKGM TRUE)
    set(TARGET_USE_CURL FALSE)
	
	set(SDL2_PATH ${CMAKE_SOURCE_DIR}/3rdparty/SDL2-2.24.0)
	set(SDL2_MIXER_PATH ${CMAKE_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2)
	#set(GLEW_DIR ${CMAKE_SOURCE_DIR}/3rdparty/glew)
	list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/3rdparty/glew")
	
	

    # TODO: Bump to O2 eventually. MinGW likes to replace memset with calls to itself...
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -ggdb -fno-omit-frame-pointer")
    
    #add_link_options(-Wl,-Map=output-win64.map -fno-trapping-math -fshort-wchar -D_MBCS)
    #add_compile_options(-Wuninitialized -fno-trapping-math -fshort-wchar -Dmain=SDL_main -D_MBCS)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /std:c11")
	include_directories(${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/zlib-1.2.3-lib/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/libpng-1.2.37-lib/include)
    include_directories(${PROJECT_SOURCE_DIR}/3rdparty/GameNetworkingSockets/include)
	
	add_link_options("${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/lib/libexchndl.a")
	add_link_options("${PROJECT_SOURCE_DIR}/3rdparty/freeglut/lib/x64/libfreeglut.a")
    add_link_options("${PROJECT_SOURCE_DIR}/3rdparty/zlib-1.2.3-lib/lib/libz.a")
    add_link_options("${PROJECT_SOURCE_DIR}/3rdparty/libpng-1.2.37-lib/lib/libpng.dll.a")
	add_link_options(OpenGL32.lib)
endif()

set(CMAKE_BUILD_TYPE Debug)

# CMake module paths
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")
if(TARGET_LINUX OR TARGET_MACOS OR PLAT_MSVC OR TARGET_ANDROID)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules_linux")
endif()

if(TARGET_ANDROID)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    #find_package(OpenAL REQUIRED)
    #find_package(GLEW 2.0 REQUIRED)

    set(OPENAL_INCLUDE_DIR "")
    set(GLEW_INCLUDE_DIRS "")
    set(JSON_Install OFF CACHE INTERNAL "")
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/json)
elseif(NOT PLAT_MINGW_X86_64 AND NOT TARGET_HOOKS AND NOT TARGET_WASM)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    if(TARGET_USE_OPENAL)
        find_package(OpenAL REQUIRED)
    endif()
    find_package(GLEW 2.0 REQUIRED)

    set(JSON_Install OFF CACHE INTERNAL "")
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/json)
else()
    # idk
    set(JSON_Install OFF CACHE INTERNAL "")
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/json)
endif()

# All of the includes
include_directories(${PROJECT_SOURCE_DIR}/src  ${PROJECT_SOURCE_DIR}/src/external/libsmacker ${PROJECT_SOURCE_DIR}/src/external/libsmusher/src ${PROJECT_SOURCE_DIR}/src/external/nativefiledialog-extended ${GLEW_INCLUDE_DIRS})

if(TARGET_USE_SDL2)
    include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR})
endif()

if(TARGET_USE_OPENAL)
    include_directories(${OPENAL_INCLUDE_DIR})
endif()

# All of the base sources
file(GLOB SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.c ${PROJECT_SOURCE_DIR}/src/*/*.c ${PROJECT_SOURCE_DIR}/src/General/*.cpp ${PROJECT_SOURCE_DIR}/src/Platform/Common/*.c ${PROJECT_SOURCE_DIR}/src/Platform/Common/*.cpp)

# Fcaseopen
file(GLOB FCASEOPEN_SRCS ${PROJECT_SOURCE_DIR}/src/external/fcaseopen/*.c)
list(APPEND SOURCE_FILES ${FCASEOPEN_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/src/external/fcaseopen)

# The rest of the conditional/TARGET_* dependencies
include(cmake_modules/config_platform_deps.cmake)

# globals.c/globals.c/precompiled resources preprocessing.
# This includes selection for most of the files in src/platform/*
include(cmake_modules/precompile_globals.cmake)

#set( CMAKE_VERBOSE_MAKEFILE on )

if(TARGET_ANDROID)
    add_library(${BIN_NAME} SHARED ${SOURCE_FILES})
elseif(NOT TARGET_HOOKS)
    add_executable(${BIN_NAME} ${SOURCE_FILES})
else()
    add_library(${BIN_NAME} SHARED ${SOURCE_FILES})
    add_library("${BIN_NAME}_kvm" SHARED ${SOURCE_FILES})

    SET_TARGET_PROPERTIES(${BIN_NAME} PROPERTIES PREFIX "")
    SET_TARGET_PROPERTIES("${BIN_NAME}_kvm" PROPERTIES PREFIX "")

    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${BIN_NAME}_kvm PRIVATE nlohmann_json::nlohmann_json)
endif()

# TODO byacc and flex

if(TARGET_MACOS)
    set_target_properties(${BIN_NAME} PROPERTIES
      LINK_SEARCH_START_STATIC ON
      LINK_SEARCH_END_STATIC ON
    )
    target_link_libraries(${BIN_NAME} PRIVATE "-framework AppKit")
    target_link_libraries(${BIN_NAME} PRIVATE "-framework Carbon")
    target_link_libraries(${BIN_NAME} PRIVATE "-framework SystemConfiguration")

    find_package(PkgConfig REQUIRED)
    
    pkg_check_modules(LIBPNG REQUIRED libpng)
    include_directories(${LIBPNG_STATIC_INCLUDE_DIRS})
    link_directories(${LIBPNG_STATIC_LIBRARY_DIRS})
    add_definitions(${LIBPNG_STATIC_CFLAGS_OTHER})

    pkg_check_modules(ZLIB REQUIRED zlib)
    include_directories(${LIBPNG_STATIC_INCLUDE_DIRS})
    link_directories(${LIBPNG_STATIC_LIBRARY_DIRS})
    add_definitions(${LIBPNG_STATIC_CFLAGS_OTHER})

    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/GameNetworkingSockets/include)
        target_link_libraries(${BIN_NAME} PUBLIC "-L${CMAKE_BINARY_DIR}/build_gns/bin" GameNetworkingSockets)
        target_link_libraries(${BIN_NAME} PUBLIC "-L${CMAKE_BINARY_DIR}/build_protobuf/" protobuf)
        find_package(OpenSSL REQUIRED)
        target_link_libraries(${BIN_NAME} PUBLIC OpenSSL::Crypto)
    endif()
endif()

if(TARGET_LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    include_directories(${GTK3_INCLUDE_DIRS})
    link_directories(${GTK3_LIBRARY_DIRS})
    add_definitions(${GTK3_CFLAGS_OTHER})
    
    pkg_check_modules(LIBPNG REQUIRED libpng)
    include_directories(${LIBPNG_STATIC_INCLUDE_DIRS})
    link_directories(${LIBPNG_STATIC_LIBRARY_DIRS})
    add_definitions(${LIBPNG_STATIC_CFLAGS_OTHER})

    if(TARGET_USE_CURL)
        pkg_check_modules(LIBCURL REQUIRED libcurl)
        include_directories(${LIBCURL_STATIC_INCLUDE_DIRS})
        link_directories(${LIBCURL_STATIC_LIBRARY_DIRS})
        add_definitions(${LIBCURL_STATIC_CFLAGS_OTHER})
    endif()

    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/GameNetworkingSockets/include)
    endif()

    if(TARGET_USE_PHYSFS)
        include_directories(${PROJECT_SOURCE_DIR}/3rdparty/physfs/src)
    endif()
endif()

if(NOT OPENJKDF2_NO_ASAN)
    #add_compile_options(-fsanitize=address -fsanitize=float-divide-by-zero)
    #add_link_options(-fsanitize=address -fsanitize=float-divide-by-zero)
    
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=float-divide-by-zero")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=float-divide-by-zero")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=float-divide-by-zero")
    set(SANITIZE_ADDRESS TRUE)
    target_link_libraries(${BIN_NAME} PRIVATE "-static-libsan")

    #add_compile_options(-fsanitize=leak)
    #add_link_options(-fsanitize=leak)
endif()

include(cmake_modules/misc_stdcpp_fs_fix.cmake)

if(PLAT_MINGW_X86_64)
    set_target_properties(${BIN_NAME} PROPERTIES
      LINK_SEARCH_START_STATIC ON
      LINK_SEARCH_END_STATIC ON
    )
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
    target_link_libraries(${BIN_NAME} PRIVATE -static)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    
    target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/freeglut/lib/x64/" freeglut)
    target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/glew/lib/Release/x64" opengl32 glew32)
    target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/SDL2/x86_64-w64-mingw32/lib" mingw32 SDL2main SDL2)
    target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer/x86_64-w64-mingw32/lib" SDL2_mixer)
    #target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/lib/" exchndl)
    target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/libpng-1.2.37-lib/lib/" png)
    target_link_libraries(${BIN_NAME} PRIVATE "-L${PROJECT_SOURCE_DIR}/3rdparty/zlib-1.2.3-lib/lib/" z)
    target_link_libraries(${BIN_NAME} PRIVATE m dinput8 dxguid dxerr8 user32 gdi32 winmm imm32 ole32 oleaut32 shell32 setupapi -lversion uuid ws2_32)

    target_link_libraries(${BIN_NAME} PRIVATE -mwindows -Wl,--subsystem,windows ${PROJECT_SOURCE_DIR}/3rdparty/openal-soft/libs/Win64/OpenAL32.lib)
    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)

    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        #target_link_libraries(${BIN_NAME} PRIVATE "-Lbuild_gns/src/" GameNetworkingSockets.dll)
    endif()
    target_link_libraries(${BIN_NAME} PRIVATE Threads::Threads)

    add_custom_command(TARGET ${BIN_NAME}
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/glew/bin/Release/x64/glew32.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/optional/libogg-0.dll ${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/optional/libopus-0.dll ${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/optional/libopusfile-0.dll ${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/openal-soft/bin/Win64/soft_oal.dll ${CMAKE_CURRENT_BINARY_DIR}/OpenAL32.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/exchndl.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/symsrv.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/mgwhelp.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/symsrv.yes ${CMAKE_CURRENT_BINARY_DIR}
    )

    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        add_custom_command(TARGET ${BIN_NAME}
        POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/build_gns/bin/libGameNetworkingSockets.dll ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/build_protobuf/libprotobuf.dll ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()

    if(TARGET_USE_PHYSFS)
        target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/build_physfs/libphysfs.a)
    endif()
elseif(TARGET_HOOKS)
    target_link_libraries(${BIN_NAME} PRIVATE -static-libgcc)
    target_link_libraries("${BIN_NAME}_kvm" PRIVATE -Wl,-e_hook_init -nostartfiles -static -static-libgcc -static-libstdc++)
elseif(TARGET_WASM)
    target_link_libraries(${BIN_NAME} PRIVATE -lm -lSDL2 -lSDL2_mixer -lGL -lGLEW -lopenal)
    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)
elseif(TARGET_LINUX)
    target_link_libraries(${BIN_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} GL ${OPENAL_LIBRARY} GLEW::GLEW ${GTK3_LIBRARIES} ${LIBPNG_STATIC_LDFLAGS})
     
    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        #target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libGameNetworkingSockets.so)
        #target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libprotobuf.so.3.21.4.0)
    endif()

    if(TARGET_USE_PHYSFS)
        target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/build_physfs/libphysfs.a)
    endif()

    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${BIN_NAME} PRIVATE dl) # dlopen, dlsym

    if(TARGET_USE_CURL)
        target_link_libraries(${BIN_NAME} PRIVATE curl)
    endif()
elseif(PLAT_MSVC)
    set_target_properties(${BIN_NAME} PROPERTIES
      LINK_SEARCH_START_STATIC ON
      LINK_SEARCH_END_STATIC ON
    )
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    find_package(Threads REQUIRED)
    target_link_libraries(${BIN_NAME} PRIVATE -static)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    target_link_libraries(${BIN_NAME} PRIVATE Threads::Threads)

	target_link_libraries(${BIN_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${OPENGL_LIBRARIES} ${OPENAL_LIBRARY} ${GLEW_LIBRARIES} ${LIBPNG_STATIC_LDFLAGS})
    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)

    target_link_libraries(${BIN_NAME} PRIVATE dinput8 dxguid user32 gdi32 winmm imm32 ole32 oleaut32 shell32 setupapi -lversion uuid ws2_32)

	add_custom_command(TARGET ${BIN_NAME} 
	POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/glew/bin/Release/x64/glew32.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/optional/libogg-0.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/optional/libopus-0.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/optional/libopusfile-0.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/openal-soft/bin/Win64/soft_oal.dll ${PROJECT_BINARY_DIR}/OpenAL32.dll
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/dbgcore.dll ${PROJECT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/dbghelp.dll ${PROJECT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/exchndl.dll ${PROJECT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/mgwhelp.dll ${PROJECT_BINARY_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/symsrv.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/drmingw-0.9.3-win64/bin/symsrv.yes ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2-2.24.0/lib/x64/SDL2.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/SDL2_mixer.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/SDL2_mixer-2.6.2/lib/x64/SDL2_mixer.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/libpng-1.2.37-lib/bin/libpng16-16.dll ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/libpng-1.2.37-lib/bin/zlib1.dll ${PROJECT_BINARY_DIR}
    )

    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        add_custom_command(TARGET ${BIN_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/GNS_compiled/GameNetworkingSockets.dll ${CMAKE_CURRENT_BINARY_DIR}/libGameNetworkingSockets.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/GNS_compiled/libprotobufd.dll ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/3rdparty/GNS_compiled/libcrypto-3-x64.dll ${CMAKE_CURRENT_BINARY_DIR}
    	)
    endif()
    #cp /opt/homebrew/Cellar/mingw-w64/9.0.0_2/toolchain-x86_64/x86_64-w64-mingw32/lib/libgcc_s_seh-1.dll .
    #cp /opt/homebrew/Cellar/mingw-w64/9.0.0_2/toolchain-x86_64/x86_64-w64-mingw32/bin/libwinpthread-1.dll .
    #cp /opt/homebrew/Cellar/mingw-w64/9.0.0_2/toolchain-x86_64/x86_64-w64-mingw32/lib/libstdc++-6.dll . 
elseif(TARGET_ANDROID)
    target_link_libraries(${BIN_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} GLESv1_CM GLESv2 GLESv3 log EGL ${GTK3_LIBRARIES} ${LIBPNG_STATIC_LDFLAGS}) #${OPENAL_LIBRARY} GLEW::GLEW
     
    if(TARGET_USE_GAMENETWORKINGSOCKETS)
        #target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libGameNetworkingSockets.so)
        #target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/libprotobuf.so.3.21.4.0)
    endif()

    if(TARGET_USE_PHYSFS)
        target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/build_physfs/libphysfs.a)
    endif()

    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)
    target_link_libraries(${BIN_NAME} PRIVATE dl) # dlopen, dlsym

    if(TARGET_USE_CURL)
        target_link_libraries(${BIN_NAME} PRIVATE curl)
    endif()
elseif(TARGET_MACOS)
    set(LIBPNG_LIST ${LIBPNG_LIBRARY_DIRS})
    separate_arguments(LIBPNG_LIST)
    message(STATUS "list = ${ZLIB_STATIC_LDFLAGS}")
    list(FILTER LIBPNG_LIST INCLUDE REGEX ${HOMEBREW_PREFIX}.*)
    message(STATUS "list = ${LIBPNG_LIST}")

    target_link_libraries(${BIN_NAME} PRIVATE ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${OPENGL_LIBRARY} ${OPENAL_LIBRARY} GLEW::GLEW ${LIBPNG_LIST}/libpng.a ${ZLIB_STATIC_LDFLAGS})
    target_link_libraries(${BIN_NAME} PRIVATE nlohmann_json::nlohmann_json)
    #target_link_libraries(${BIN_NAME} PRIVATE 3rdparty/protobuf/libprotobufd.a)

    if(TARGET_USE_PHYSFS)
        target_link_libraries(${BIN_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/build_physfs/libphysfs.a)
    endif()
else()
    message(FATAL_ERROR "Unknown target! Cannot continue.")
endif()

#
# macOS post-build packaging
#
include(cmake_modules/postcompile_macos.cmake)
