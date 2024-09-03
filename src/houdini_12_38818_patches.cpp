#include "patches.h"

#include <cstring>
#include <nbtools/patchHex.h>

unsigned int sizeofNB = 6*1024*1024;
//Copied from android 11 patches
extern "C"
int android_dlopen_ext_internal_hook_func(char* andr_namespace,char* dlname){
    debug_print("Entering android_dlopen_ext_internal_hook_func, namespace: %s, dlname: %s", andr_namespace, dlname);
    if (!dlname){
        return 0;
    }
    char* found = nullptr;
    found = strstr(dlname, "libneuralnetworks.so");
    if (found){
        debug_print("Skipped %s", dlname);
        return 1;
    }
    found = strstr(dlname, "librvcapture_camera.so");
    if (found){
        debug_print("Skipped %s", dlname);
        return 1;
    }

    return 0;
}

__attribute__((naked))
void android_dlopen_ext_internal_hook_stub(){
    #ifdef IS_32

    #else
    __asm__ volatile(
        ".intel_syntax\n"
        "PUSH rax\n"
        "PUSH rbx\n"
        "PUSH rcx\n"
        "PUSH rdx\n"
        "PUSH rsi\n"
        "PUSH rdi\n"
        "PUSH R8\n"
        "PUSH R9\n"
        "PUSH R10\n"
        "PUSH R11\n"
        "PUSH R12\n"
        "PUSH R13\n"
        "PUSH R14\n"
        "PUSH R15\n"
        "CALL android_dlopen_ext_internal_hook_func\n"
        "TEST rax,rax\n"
        "POP R15\n"
        "POP R14\n"
        "POP R13\n"
        "POP R12\n"
        "POP R11\n"
        "POP R10\n"
        "POP R9\n"
        "POP R8\n"
        "POP rdi\n"
        "POP rsi\n"
        "POP rdx\n"
        "POP rcx\n"
        "POP rbx\n"
        "POP rax\n"
        "JZ contexec1\n"
        "POP rax\n"
        "XOR rax,rax\n"
        "RET\n"
        "contexec1:\n"
        "POP rax\n"
        "PUSH R12\n"
        "PUSH R13\n"
        "PUSH r14\n"
        "PUSH rax\n"
        "RET\n"
    );
    #endif
}

extern "C"
int call_constructor_func_hook(void* custom_dlhandle){
     if (custom_dlhandle){
        uint8_t* workaroundvar = (uint8_t*)custom_dlhandle + 408; //This is here to workaround the compiler from multiplying the offset by 8
        char* dlname = *(char**)workaroundvar;//408 offset contains the dl name..... I think
        if (dlname){
            int isequal = strcmp(dlname, "libimg_utils.so");
            if (isequal == 0){
                debug_print("Skipped %s, custom_dlhandle is %lld", dlname, (unsigned long long)custom_dlhandle);
                return 1; //we skip it... IDK WHY
            }
        }
    }
    return 0;
}

__attribute__((naked))
void call_constructor_func_hook_stub(){
    #ifdef IS_32

    #else
    __asm__ volatile(
        ".intel_syntax\n"
        "PUSH rax\n"
        "PUSH rbx\n"
        "PUSH rcx\n"
        "PUSH rdx\n"
        "PUSH rsi\n"
        "PUSH rdi\n"
        "PUSH R8\n"
        "PUSH R9\n"
        "PUSH R10\n"
        "PUSH R11\n"
        "PUSH R12\n"
        "PUSH R13\n"
        "PUSH R14\n"
        "PUSH R15\n"
        "CALL call_constructor_func_hook\n"
        "TEST rax,rax\n"
        "POP R15\n"
        "POP R14\n"
        "POP R13\n"
        "POP R12\n"
        "POP R11\n"
        "POP R10\n"
        "POP R9\n"
        "POP R8\n"
        "POP rdi\n"
        "POP rsi\n"
        "POP rdx\n"
        "POP rcx\n"
        "POP rbx\n"
        "POP rax\n"
        "JZ contexec\n"
        "POP rax\n"
        "RET\n"
        "contexec:\n"
        "POP rax\n"
        "PUSH rbp\n"
        "SUB rsp,0x410\n"
        "PUSH rax\n"
        "RET\n"
    );
    #endif
}
//end android 11 patches copies.

void Patch_android_dlopen_ext_internal(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    res |= PatchHex_8(nbbase, 0x32f880, 0x41, 0xe8);
    res |= PatchHex_32(nbbase, 0x32f881, 0x41554154, (char*)android_dlopen_ext_internal_hook_stub-((char*)nbbase+0x32f881));
    res |= PatchHex_8(nbbase, 0x32f885, 0x56, 0x90);
    #endif
    if (res){
        error_print("Patch_android_dlopen_ext_internal failed.");
    }
}

void Patch_call_constructor(void* nbbase){
    int res = 0;
    #ifdef IS_32
    
    #else
    res |= PatchHex_8(nbbase, 0x331f60, 0x55, 0xe8);
    res |= PatchHex_32(nbbase, 0x331f61, 0x10EC8148, (char*)call_constructor_func_hook_stub-((char*)nbbase+0x331f61));
    res |= PatchHex_8(nbbase, 0x331f65, 0x04, 0x90);
    res |= PatchHex_16(nbbase, 0x331f66, 0x0, 0x9090);


    #endif
    if (res){
        error_print("Patch_call_constructor failed.");
    }
}









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
    #ifdef IS_32
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

void Patch_exp_01(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        PatchHex_16(nbbase, 0x2e5e77, 0x940f, 0x01b1);
        PatchHex_8(nbbase, 0x2e5e79, 0xc1, 0x90);
    #endif
    if (res){
        error_print("Patch_exp_01 failed.");
    }
}



void Patch_NB(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa){
    Patch_Permissive_Pkey_Mprotect(nbbase);
    Patch_Permissive_Mmap(nbbase);
    #ifdef ENABLE_EXPERIMENTAL_PATCHES
        Patch_Linker_Namespace(nbbase);
        Patch_exp_01(nbbase);
    #endif

    if (!app_code_cache_dir){
        return;
    }
    
    const char* dofound = strstr(app_code_cache_dir, "com.nexon.bluearchive");
    if (dofound){
        Patch_Performance_Pkey_Mprotect(nbbase);
    }
    dofound = strstr(app_code_cache_dir, "com.roblox.client");
    if (dofound){
        Patch_android_dlopen_ext_internal(nbbase);
        Patch_call_constructor(nbbase);
    }

}


