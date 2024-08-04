cmake_minimum_required(VERSION 3.22)

include(FetchContent)

FetchContent_Declare(
    cmakescript
    GIT_REPOSITORY https://github.com/StableCoder/cmake-scripts.git
    GIT_TAG 45f72fdc2d715030c6cb61e3d5977811ea45abda
)

FetchContent_MakeAvailable(cmakescript)
include(${cmakescript_SOURCE_DIR}/c++-standards.cmake)

if (NOT DEFINED COMPILE_ARCH)
set(COMPILE_ARCH x86_64)
endif()

if(NOT DEFINED NATIVEBRIDGE)
set(NATIVEBRIDGE houdini)
endif()

if (NOT DEFINED NB_VERSION)
set(NB_VERSION 11_38765)
endif()


if (DEFINED ENV{ANDROID_NDK_HOME})
    set(ANDROID_NDK_HOME $ENV{ANDROID_NDK_HOME})
endif()

option(LOG_DEBUG "Enable debug logging" OFF)
option(SKIP_READABLE_CHECK "Skip the memory readable check" ON)
option(SKIP_NB_ENABLED_CHECK "Skip the native_bridge enabled check, native bridge will always run" ON)
option(ENABLE_EXPERIMENTAL_PATCHES "Enable experimental patches (Not recommended)" OFF)

if(ENABLE_EXPERIMENTAL_PATCHES)
add_compile_definitions(ENABLE_EXPERIMENTAL_PATCHES)
endif()

if(LOG_DEBUG)
add_compile_definitions(LOG_DEBUG)
endif()
if(SKIP_READABLE_CHECK)
add_compile_definitions(SKIP_READABLE_CHECK)
endif()
if(SKIP_NB_ENABLED_CHECK)
add_compile_definitions(SKIP_NB_ENABLED_CHECK)
endif()
add_compile_definitions(USE_NATIVEBRIDGE=\"${NATIVEBRIDGE}\")


set(CMAKE_ANDROID_NDK ${ANDROID_NDK_HOME})
set(ANDROID_PLATFORM 30) #Android 11 SDK ver



set(ANDROID_ABI ${COMPILE_ARCH})
set(ANDROID_STL c++_static)
set(ANDROID_CPP_FEATURES,"no-rtti no-exceptions")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_TOOLCHAIN_FILE ${ANDROID_NDK_HOME}/build/cmake/android.toolchain.cmake)

project(libnb_patch_int)
cxx_20()
set(CMAKE_SYSTEM_NAME Android)
set(CMAKE_SYSTEM_VERSION 30)
set(CMAKE_ANDROID_ARCH_ABI ${COMPILE_ARCH})
set(CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION clang)
set(CMAKE_ANDROID_STL_TYPE c++_static)

include_directories(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

