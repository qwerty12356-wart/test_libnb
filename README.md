# What is this?
A custom libnb that patch native bridges in memory to fix some game compatibility

# Supported native bridges:
houdini11_38765 (waydroid_script houdini version)
houdini12_38818 (in beta)

# How to use:
1. Build libnb from source (See Build)
2. Copy the two libnb version to their respective folder. (/system/lib and system/lib64)
3. Edit build.prop and add or edit ``ro.dalvik.vm.native.bridge`` to ``ro.dalvik.vm.native.bridge=libnb.so``

# Build
1. Download android NDK, extract it and either export ANDROID_NDK_HOME to point to the extracted NDK, or add ``-DANDROID_NDK_HOME=(YOUR NDK PATH HERE)`` to the cmake command.
2. run ``cmake . -B build`` (Add more commands after this if you wish).
3. ``cd build``, ``make``.

Available build options:\
COMPILE_ARCH: Set compile architecture (x86_64 (default), or x86).\
ANDROID_NDK_HOME: Set android NDK path.
NATIVEBRIDGE: Set native bridge to use (default: houdini).\
NB_VERSION: Set version of native bridge that is being used (default: 11_38765).\
LOG_DEBUG: Enable debug logging (default: OFF).\
SKIP_READABLE_CHECK: Skip the ``is_readable`` check when patching (default: ON).\
SKIP_NB_ENABLED_CHECK: Skip ``is_native_bridge_enabled`` check (default: OFF). \
ENABLE_EXPERIMENTAL_PATCHES: enable experimental patches (duh, default: OFF).



