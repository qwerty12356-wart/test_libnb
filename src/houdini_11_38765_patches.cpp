#include <cstdint>
#include <cstring>
#include <patches.h>
#include <nbtools/patchHex.h>
#include <log.h>

#ifdef IS_32
unsigned int sizeofNB = 5* 1024*1024;
#else
unsigned int sizeofNB = 6*1024*1024;
#endif

void Patch_Permissive_pkey_mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    res |= PatchHex_8(nbbase, 0x3099D8, 0xFB, 0xFF);
    #endif
    if (res){
        error_print("Patch_Permissive_pkey_Mprotect Failed.");
    }
}

void Patch_Permissive_mmap(void* nbbase){
    int res = 0;
    #ifdef IS_32
    
    #else
    res |= PatchHex_32(nbbase, 0x3062A5, 0xFFFBB848, 0xFFFFB848);
    #endif

    if (res){
        error_print("Patch_Permissive_mmap Failed.");
    }
}

void Patch_Performance_pkey_mprotect(void* nbbase){
     int res = 0;
    #ifdef IS_32
    
    #else
    res |= PatchHex_32(nbbase, 0x309B42, 0xEE2F89E8, 0x90909090);
    res |= PatchHex_8(nbbase, 0x309B46, 0xff, 0x90);
    #endif

    if (res){
        error_print("Patch_Performance_pkey_mprotect Failed.");
    }
}

//return 1 to skip execution, 0 if we want to continue
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

void Patch_android_dlopen_ext_internal(void* nbbase){
    int res = 0;
    #ifdef IS_32
    
    #else
    res |= PatchHex_8(nbbase, 0x3614a0, 0x41, 0xe8);
    res |= PatchHex_32(nbbase, 0x3614a1, 0x41554154, (char*)android_dlopen_ext_internal_hook_stub-((char*)nbbase+0x3614a5));
    res |= PatchHex_8(nbbase, 0x3614a5, 0x56, 0x90);
    #endif
    if (res)
    {
        error_print("Patch_android_dlopen_ext_internal failed.");
    }
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

void Patch_call_constructor(void* nbbase){
    int res = 0;
    #ifdef IS_32
    
    #else
       res |= PatchHex_8(nbbase, 0x363b60, 0x55, 0xe8); //CALL
       res |= PatchHex_32(nbbase, 0x363b61, 0x10EC8148,  (char*)call_constructor_func_hook_stub-((char*)nbbase+0x363b65)); //cursed pointer calculations
       res |= PatchHex_8(nbbase,0x363b65 , 0x04, 0x90);
       res |= PatchHex_8(nbbase,0x363b66 , 0x00, 0x90);
       res |= PatchHex_8(nbbase,0x363b67 , 0x0, 0x90);
    #endif
    if (res){
        error_print("Patch_call_constructor failed.");
    }
}

void Patch_exp_01(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
        PatchHex_16(nbbase, 0x2f7877, 0x940f, 0x01b1);
        PatchHex_8(nbbase, 0x2f7879, 0xc1, 0x90);
    #endif
    if (res){
        error_print("Patch_exp_01 failed.");
    }
}


//Backported from mumu, idk what RDI contains
extern "C"
char* internal_dladdr_hook_func(char* whatisthis){
    if (whatisthis){
        debug_print("internal_dladdr_hook RDI value: %s", whatisthis);
        char* result = strchr(whatisthis, '/');
        if (!result){
            result = whatisthis;
        }
        return result+1;
    }
    return NULL;
}



__attribute__((naked))
void internal_dladdr_hook_stub(){
      #ifdef IS_32

    #else
    __asm__ volatile(
        ".intel_syntax\n"
        "MOV RAX, qword ptr [RSI + 0x198]\n"
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
        //"MOV RDI, RAX\n"
        //"CALL internal_dladdr_hook_func\n"
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
        "RET \n"
    );
    #endif
}



void Patch_internal_dladdr(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    res |= PatchHex_8(nbbase, 0x3686f9, 0x48, 0xe8);
    res |= PatchHex_32(nbbase, 0x3686fa, 0x01A0868B, (uint32_t)((char*)internal_dladdr_hook_stub - ((char*)nbbase + 0x3686fe)));
    res |= PatchHex_8(nbbase, 0x3686fe, 0x0, 0x90);
    res |= PatchHex_8(nbbase, 0x3686ff, 0x0, 0x90);
    #endif
    if (res){
        error_print("Patch_internal_dladdr failed");
    }
}


void Patch_Permissive_pkey_Mprotect2(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    res |= PatchHex_8(nbbase, 0x306552, 0xfb, 0xff);
    #endif
    if (res){
        error_print("Patch_Permissive_pkey_Mprotect2 failed.");
    }
}

void Patch_Permissive_mprotect(void* nbbase){
    int res = 0;
    #ifdef IS_32

    #else
    res |= PatchHex_8(nbbase, 0x30647a, 0xfb, 0xff);
    #endif
    if (res){
        error_print("Patch_Permissive_mprotect failed.");
    }

}


void Patch_NB(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa){
    Patch_Permissive_pkey_mprotect(nbbase);
    Patch_Permissive_mmap(nbbase);
    #ifdef ENABLE_EXPERIMENTAL_PATCHES
    Patch_exp_01(nbbase);
    #endif

    if (!app_code_cache_dir){
        return;
    }
    const char* dofound = strstr(app_code_cache_dir, "com.nexon.bluearchive");
    if (dofound){
        Patch_Performance_pkey_mprotect(nbbase);
    }
    dofound = strstr(app_code_cache_dir, "com.roblox.client");
    if (dofound){
        Patch_android_dlopen_ext_internal(nbbase);
        Patch_call_constructor(nbbase);
    }
    //No command line for this. Must be enabled by modifying the source.
    //As you can see, this patch is for supercell games, but since it doesnt works, it is junk.
    
    dofound = strstr(app_code_cache_dir, ".supercell.");
    if (dofound){
        Patch_internal_dladdr(nbbase);
        Patch_Permissive_mprotect(nbbase);
        Patch_Permissive_pkey_Mprotect2(nbbase);
    }
}