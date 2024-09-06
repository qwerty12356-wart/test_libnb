#include <patches.h>

#include <nbtools/patchHex.h>
#include <log.h>

#ifdef IS_32
unsigned int sizeofNB = 5* 1024*1024
#else
unsigned int sizeofNB = 6*1024*1024;
#endif

void Patch_Check_CPU_Vendor(void* nbbase){
    int res = 0;
    #ifdef IS_32
        
    #else
        res |= PatchHex_32(nbbase, 0xE6723, 0x0D41830F, 0x90909090);
        res |= PatchHex_16(nbbase, 0xE6727, 0x0, 0x9090);
        res |= PatchHex_16(nbbase, 0xC5ADD, 0x5B73, 0x9090);
        res |= PatchHex_16(nbbase, 0xC62F5, 0x5a73, 0x9090);
        res |= PatchHex_16(nbbase, 0xC6F5F, 0x5b73, 0x9090);

    #endif
    if (res){
        error_print("Patch_Check_CPU_Vendor failed.");
    }
}

void Patch_Linker_Namespace(void* nbbase){
     int res = 0;
    #ifdef IS_32
        
    #else
        res |= PatchHex_32(nbbase, 0x460b78, 0xFFBB93E8, 0x90909090);
        res |= PatchHex_8(nbbase, 0x460b7c, 0xff, 0x90);
    #endif
    if (res){
        error_print("Patch_Linker_Namespace failed.");
    }
}

void Patch_Permissive_Mmap(void* nbbase){
    int res = 0;
    #ifdef IS_32
        
    #else
        res |= PatchHex_8(nbbase, 0x2efd53, 0xfb, 0xff);
    #endif
    if (res){
        error_print("Patch_Permissive_Mmap failed.");
    }
}

void Patch_Permissive_Pkey_Mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32
        
    #else
        res |= PatchHex_32(nbbase, 0x2f3778, 0x02F1BA0F, 0x909090f8);
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
        res |= PatchHex_32(nbbase, 0x2f0028, 0xF6BA0F41, 0x90909090);
        res |= PatchHex_8(nbbase, 0x2f002c, 0x02, 0xf8);
    #endif
    if (res){
        error_print("Patch_Permissive_Pkey_Mprotect2 failed.");
    }
}

void Patch_Performance_Pkey_Mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32
        
    #else
        res |= PatchHex_8(nbbase, 0x2f3903, 0xe8, 0x90);
        res |= PatchHex_32(nbbase, 0x2f3904, 0xFFED8F58, 0x90909090);
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