#include <patches.h>

#include <nbtools/patchHex.h>
#include <log.h>

#ifdef IS_32
unsigned int sizeofNB = 5* 1024*1024;
#else
unsigned int sizeofNB = 6*1024*1024;
#endif

void Patch_Check_CPU_Vendor(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        res |= PatchHex_32(nbbase, 0xECF8F, 0x0CEF830F, 0x90909090);
        res |= PatchHex_16(nbbase, 0xECF93, 0x0, 0x9090);
        res |= PatchHex_16(nbbase, 0xCC0ED, 0x5B73, 0x9090);
        res |= PatchHex_16(nbbase, 0xCC925, 0x5A73, 0x9090);
        res |= PatchHex_16(nbbase, 0xCD5AF, 0x5B73, 0x9090);

    #endif
    if (res){
        error_print("Patch_Check_CPU_Vendor failed.");
    }
}

void Patch_Linker_Namespace(void* nbbase){
     int res = 0;
    #ifdef IS_32

    #else
        res |= PatchHex_32(nbbase, 0x4F5D68, 0xFFBE03E8, 0x90909090);
        res |= PatchHex_8(nbbase, 0x4F5D6C, 0xFF, 0x90);
    #endif
    if (res){
        error_print("Patch_Linker_Namespace failed.");
    }
}

void Patch_Permissive_Mmap(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        res |= PatchHex_8(nbbase, 0x314C33, 0xFB, 0xFF);
    #endif
    if (res){
        error_print("Patch_Permissive_Mmap failed.");
    }
}

void Patch_Permissive_Pkey_Mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        res |= PatchHex_32(nbbase, 0x3189B5, 0xF6BA0F41, 0x90909090);
        res |= PatchHex_8(nbbase, 0x3189B9, 0x02, 0xF8);
    #endif
    if (res){
        error_print("Patch_Permissive_Pkey_Mprotect failed.");
    }
}

//Found this random pkey_mprotect call in houdini binary, patching it just in case.
void Patch_Permissive_Pkey_Mprotect2(void* nbbase){
     int res = 0;
    #ifdef IS_32

    #else
        res |= PatchHex_32(nbbase, 0x314F1B, 0xF7BA0F41, 0x90909090);
        res |= PatchHex_8(nbbase, 0x314F1F, 0x02, 0xF8);
    #endif
    if (res){
        error_print("Patch_Permissive_Pkey_Mprotect2 failed.");
    }
}

void Patch_Performance_Pkey_Mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        res |= PatchHex_8(nbbase, 0x318B4A, 0xE8, 0x90);
        res |= PatchHex_32(nbbase, 0x318B4B, 0xFFEBD101, 0x90909090);
    #endif
    if (res){
        error_print("Patch_Performance_Pkey_Mprotect failed.");
    }
}

void Patch_NB(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa){
    Patch_Check_CPU_Vendor(nbbase);
    #ifdef ENABLE_EXPERIMENTAL_PATCHES
    Patch_Linker_Namespace(nbbase);
    Patch_Permissive_Pkey_Mprotect2(nbbase);
    #endif
    Patch_Permissive_Pkey_Mprotect(nbbase);
    Patch_Permissive_Mmap(nbbase);

    if (!app_code_cache_dir){
        return;
    }

    const char* dofound = strstr(app_code_cache_dir, "com.nexon.bluearchive");
    if (dofound){
       Patch_Performance_Pkey_Mprotect(nbbase);
    }

}
