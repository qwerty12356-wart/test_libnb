#include "patches.h"

#include <nbtools/patchHex.h>

unsigned int sizeofNB = 6*1024*1024;

void Patch_Permissive_Mmap(void* nbbase){
    int res = 0;
    #ifdef IS_32
        res = PatchHex_32(nbbase, 0x26d3a7, 0x02F0BA0F, 0xF8909090);
    #else
        res = PatchHex_32(nbbase, 0x2f3ae1, 0xFFFBB848, 0xFFFFB848);
        
    #endif
    if (res){
        error_print("Patch_Permissive_Mmap failed.");
    }
}

void Patch_Permissive_Pkey_Mprotect(void* nbbase){
     int res = 0;
    #ifdef IS_32
        res = PatchHex_8(nbbase, 0x2719b3, 0xfb, 0xff);
    #else
       res = PatchHex_32(nbbase, 0x2f74e6, 0x02F1BA0F, 0xF8909090);
    #endif
    if (res){
        error_print("Patch_Permissive_Pkey_Mprotect failed.");
    }
}

void Patch_Linker_Namespace(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        res = PatchHex_32(nbbase, 0x335e1a, 0x0042D1E8, 0x90909090);
        res |= PatchHex_8(nbbase, 0x335e1e, 0x00, 0x90);
    #endif
    if (res){
        error_print("Patch_Linker_namespace failed.");
    }
}

void Patch_Performance_Pkey_Mprotect(void* nbbase){
    int res = 0;
    #ifdef IS32BIT
        res = PatchHex_32(nbbase, 0x271b56, 0xEC9CE5E8, 0x90909090);
        res |= PatchHex_8(nbbase, 0x271b5a, 0xff, 0x90);
    #else
        res = PatchHex_32(nbbase, 0x2f7660, 0xEE2ABBE8, 0x90909090);
        res |= PatchHex_8(nbbase, 0x2f7664, 0xff, 0x90);
    #endif
    if (res){
        error_print("Patch_Performance_Pkey_Mprotect failed.");
    }
}

void Patch_NB(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa){
    Patch_Permissive_Pkey_Mprotect(nbbase);
    Patch_Permissive_Mmap(nbbase);
    #ifdef ENABLE_EXPERIMENTAL_PATCHES
        Patch_Linker_Namespace(nbbase);
    #endif
    const char* dofound = strstr(app_code_cache_dir, "com.nexon.bluearchive");
    if (dofound){
        Patch_Performance_Pkey_Mprotect(nbbase);
    }

}