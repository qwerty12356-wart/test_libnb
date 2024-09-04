/*
 * Copyright (C) 2015-2017 The Android-x86 Open Source Project
 *
 * by Chih-Wei Huang <cwhuang@linux.org.tw>
 *
 * Licensed under the GNU General Public License Version 2 or later.
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/gpl.html
 *
 */




#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <nativebridge.h>
#include <dlfcn.h>
#include <log.h>
#include <sys/system_properties.h>
#include "patches.h"


#ifndef USE_NATIVEBRIDGE
#define USE_NATIVEBRIDGE "houdini"
#endif

namespace android {
    static bool isFakingISA = false;
    static void *native_handle = nullptr;
    static NativeBridgeCallbacks *callbacks = nullptr;
    static bool is_native_bridge_enabled()
    {
        #ifndef SKIP_NB_ENABLED_CHECK
        const prop_info* syspropinfo = __system_property_find("persist.sys.nativebridge");
        if (syspropinfo != nullptr){
            bool isenable = false;
            __system_property_read_callback(syspropinfo, 
            [](void* cookie, const char*, const char* value, unsigned int){
               *(bool*)cookie = atoi(value); // Looks safe to me
            }, &isenable);
            return isenable;
        }
        return 0;
        #else
        return 1;
        #endif
    }
    static NativeBridgeCallbacks *get_callbacks()
    {
      if (!callbacks) {
        const char *libnb = "/system/lib"
        #ifdef __LP64__
                "64"
        #endif
                "/lib" USE_NATIVEBRIDGE ".so";

        if (!native_handle) {
            native_handle = dlopen(libnb, RTLD_LAZY);
            if (!native_handle){
                error_print("Failed to open %s: %s", libnb, dlerror());
                return nullptr;
            }
        }
        
        callbacks = reinterpret_cast<NativeBridgeCallbacks *>(dlsym(native_handle, "NativeBridgeItf"));
        __android_log_print(ANDROID_LOG_INFO,"libnb_custom" , libnb, callbacks ? callbacks->version : 0);
      }
     return callbacks;
    }
    
    static bool native_bridge2_initialize(const NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa)
    {
        debug_print("Entering native_bridge2_initialize %s, %s", app_code_cache_dir, isa);
        if (is_native_bridge_enabled()){
            if (NativeBridgeCallbacks* nbcb = get_callbacks()){
                Dl_info dlinf{};
                dladdr(nbcb, &dlinf);
                if (dlinf.dli_fbase){
                    initPatches(dlinf.dli_fbase, art_cbs, app_code_cache_dir, isa);
                }
                const char* fake_isa = isa;
                return nbcb->initialize(art_cbs, app_code_cache_dir, fake_isa);
            }
        }
        else{
            error_print("native bridge is not enabled");
        }
        return false;
    }

    static void *native_bridge2_loadLibrary(const char *libpath, int flag)
    {
        debug_print("Entering native_bridge2_loadLibrary libpath: %s, flag: %i", libpath, flag);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->loadLibrary(libpath, flag) : nullptr;
    }

    static void *native_bridge2_getTrampoline(void *handle, const char *name, const char* shorty, uint32_t len){
        debug_print("Entering native_bridge2_getTrampoline name: %s, shorty: %s, len : %u", name, shorty, len);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->getTrampoline(handle, name, shorty, len) : nullptr;
    }
    static bool native_bridge2_isSupported(const char *libpath){
        debug_print("Entering native_bridge2_isSupported, libpath %s", libpath);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->isSupported(libpath) : false;
    }
    static const struct NativeBridgeRuntimeValues *native_bridge2_getAppEnv(const char *abi){
        debug_print("Entering native_bridge2_getAppEnv, abi: %s", abi);
        NativeBridgeCallbacks *cb = get_callbacks();
        const char* fake_isa = abi;
        return cb ? cb->getAppEnv(fake_isa) : nullptr;
    }
    static bool native_bridge2_isCompatibleWith(uint32_t version)
    {
        static uint32_t my_version = 0;
        debug_print("Entering native_bridge2_isCompatibleWith, version: %u", version);
        if (my_version == 0 && is_native_bridge_enabled()) {
            if (NativeBridgeCallbacks *cb = get_callbacks()) {
            my_version = cb->version;
            } 
        }
        return version <= (my_version ? my_version : 3);
    }
    
    static NativeBridgeSignalHandlerFn native_bridge2_getSignalHandler(int signal)
    {
        debug_print("Entering native_bridge2_getSignalHandler, signal: %i", signal);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->getSignalHandler(signal) : nullptr;
    }
    
    static int native_bridge3_unloadLibrary(void *handle)
    {
        debug_print("Entering native_bridge3_unloadLibrary, handle: %p", handle);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->unloadLibrary(handle) : -1;
    }
    static const char *native_bridge3_getError()
    {
        debug_print("Entering native_bridge3_getError");
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->getError() : "unknown";
    }
    static bool native_bridge3_isPathSupported(const char *path)
    {
        debug_print("Entering native_bridge3_isPathSupported, path: %s", path);
        NativeBridgeCallbacks *cb = get_callbacks();
        bool CheckingVariable = cb && cb->isPathSupported(path);
        if (!CheckingVariable){
            debug_print("Path is not supported.");
            if (isFakingISA){
                debug_print("Now faking supported Path...");
                return true;
            }
        }
        return CheckingVariable;
    }
    static bool native_bridge3_initAnonymousNamespace(const char *public_ns_sonames,const char *anon_ns_library_path)
    {
        debug_print("Entering native_bridge3_initAnonymousNamespace, public_ns_sonames: %s, anon_ns_library_path: %s", public_ns_sonames, anon_ns_library_path);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb && cb->initAnonymousNamespace(public_ns_sonames, anon_ns_library_path);
    }
    static native_bridge_namespace_t *
    native_bridge3_createNamespace(const char *name,const char *ld_library_path,const char *default_library_path,uint64_t type,const char *permitted_when_isolated_path,native_bridge_namespace_t *parent_ns)
    {
        debug_print("Entering native_bridge3_createNamespace, name: %s, ld_library_path: %s, default_library_path: %s, type: %lu, permitted_when_isolated_path: %s", name, ld_library_path, default_library_path, type,permitted_when_isolated_path);
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->createNamespace(name, ld_library_path, default_library_path, type, permitted_when_isolated_path, parent_ns) : nullptr;
    }
    static bool native_bridge3_linkNamespaces(native_bridge_namespace_t *from, native_bridge_namespace_t *to,const char *shared_libs_soname)
    {
        debug_print("Entering native_bridge3_linkNamespaces, shared_libs_soname: %s", shared_libs_soname);
         NativeBridgeCallbacks *cb = get_callbacks();
        return cb && cb->linkNamespaces(from, to, shared_libs_soname);
    }
    static void *native_bridge3_loadLibraryExt(const char *libpath,int flag,native_bridge_namespace_t *ns){
        debug_print("Entering native_bridge3_loadLibraryExt, libpath: %s, flag: %i", libpath, flag);
        NativeBridgeCallbacks *cb = get_callbacks();
        void *result = cb ? cb->loadLibraryExt(libpath, flag, ns) : nullptr;
        debug_print("native_bridge3_loadLibraryExt result: %p", result);
        return result;
    }
    static native_bridge_namespace_t *native_bridge4_getVendorNamespace(){
        debug_print("Entering native_bridge4_getVendorNamespace");
        NativeBridgeCallbacks *cb = get_callbacks();
        return cb ? cb->getVendorNamespace() : nullptr;
    }
    static native_bridge_namespace_t* native_bridge5_getExportedNamespace(const char* name){
        debug_print("Entering native_bridge5_getExportedNamespace, name: %s", name);
        NativeBridgeCallbacks* cb = get_callbacks();
        return cb ? cb->getExportedNamespace(name) : nullptr;
    }
    static void native_bridge6_preZygoteFork(){
        debug_print("Entering native_bridge6_preZygoteFork");
        NativeBridgeCallbacks* cb =  get_callbacks();
        cb->preZygoteFork();
    }
    static void* native_bridge6_getTrampolineWithJNICallType(void* handle,const char* name,const char* shorty,uint32_t len,enum JNICallType jni_call_type){
        debug_print("Entering native_bridge6_getTrampolineWithJNICallType, handle: %p, name: %s, shorty: %s, len: %u", handle, name, shorty, len);
        NativeBridgeCallbacks* cb = get_callbacks();
        return cb ? cb->getTrampolineWithJNICallType(handle, name, shorty, len, jni_call_type) : nullptr;
    }
    static void* native_bridge6_getTrampolineForFunctionPointer(const void* method,const char* shorty,uint32_t len,enum JNICallType jni_call_type){
        debug_print("Entering native_bridge6_getTrampolineForFunctionPointer, method: %p, shorty: %s, len: %u", method, shorty, len);
        NativeBridgeCallbacks* cb = get_callbacks();
        return cb ? cb->getTrampolineForFunctionPointer(method, shorty, len, jni_call_type) : nullptr;
    }
    static void __attribute__ ((destructor)) on_dlclose()
    {
        if (native_handle) {
            dlclose(native_handle);
            native_handle = nullptr;
        }
    }
extern "C" {
NativeBridgeCallbacks NativeBridgeItf = {
    // v1
    .version = 5,
    .initialize = native_bridge2_initialize,
    .loadLibrary = native_bridge2_loadLibrary,
    .getTrampoline = native_bridge2_getTrampoline,
    .isSupported = native_bridge2_isSupported,
    .getAppEnv = native_bridge2_getAppEnv,
    // v2
    .isCompatibleWith = native_bridge2_isCompatibleWith,
    .getSignalHandler = native_bridge2_getSignalHandler,
    // v3
    .unloadLibrary = native_bridge3_unloadLibrary,
    .getError = native_bridge3_getError,
    .isPathSupported = native_bridge3_isPathSupported,
    .initAnonymousNamespace = native_bridge3_initAnonymousNamespace,
    .createNamespace = native_bridge3_createNamespace,
    .linkNamespaces = native_bridge3_linkNamespaces,
    .loadLibraryExt = native_bridge3_loadLibraryExt,
    // v4
    .getVendorNamespace = native_bridge4_getVendorNamespace,
    // v5
    .getExportedNamespace = native_bridge5_getExportedNamespace,
    // v6?
    .preZygoteFork = native_bridge6_preZygoteFork,
    .getTrampolineWithJNICallType = native_bridge6_getTrampolineWithJNICallType,
    .getTrampolineForFunctionPointer = native_bridge6_getTrampolineForFunctionPointer,

    };

}
}
