#include <patches.h>

#include <nbtools/patchHex.h>
#include <log.h>

#ifdef IS_32
unsigned int sizeofNB = 5* 1024*1024
#else
unsigned int sizeofNB = 8*1024*1024;
#endif

void Patch_Check_Build_Vendor(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    res |= PatchHex_32(nbbase, 0xf74df, 0x0CEF830F, 0x90909090);
    res |= PatchHex_16(nbbase, 0xf74e3, 0x0, 0x9090);
    res |= PatchHex_16(nbbase, 0xd663d, 0x5b73, 0x9090);
    res |= PatchHex_16(nbbase, 0xd6e75, 0x5a73, 0x9090);
    res |= PatchHex_16(nbbase, 0xd7aff, 0x5b73, 0x9090);
    #endif
    if (res){
        error_print("Patch_Check_Build_Vendor failed.");
    }

}


void Patch_Check_CPU_Vendor(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    //TODO: Add all 24 remaining patches for houdini to make it work on AMD
    res |= PatchHex_32(nbbase, 0xf7386, 0x0D9A850F, 0x90909090);
    res |= PatchHex_16(nbbase, 0xf738a, 0x0, 0x9090);
    
    
    
    #endif
    if (res){
        error_print("Patch_Check_Build_Vendor failed.");
    }
}
//TODO: Complete these 2 patches
void Patch_Permissive_Mmap(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else

    #endif
    if (res){
        error_print("Patch_Permissive_Mmap failed.");
    }
}

void Patch_Permissive_Mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else

    #endif
    if (res){
        error_print("Patch_Permissive_Mprotect failed.");
    }
}


void Patch_NB(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa){
    Patch_Check_Build_Vendor(nbbase);

}