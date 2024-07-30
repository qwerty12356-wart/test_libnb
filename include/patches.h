#include <asm-generic/mman-common.h>
#include <sys/mman.h>
#include <nativebridge.h>


#include <cstdint>
#if INTPTR_MAX == INT32_MAX
    #define IS_32
#elif INTPTR_MAX == INT64_MAX
    #define IS_64
#else
    #error "How did we get here."
#endif
//Must define size of NB in bytes.
//NOTE: DO NOT set this number too high, because you may accidentally mark
//Houdini memory as read-only
extern unsigned int sizeofNB;

void Patch_NB(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa);

inline void initPatches(void* nbbase,const android::NativeBridgeRuntimeCallbacks *art_cbs,const char *app_code_cache_dir,const char *isa){
    mprotect(nbbase, sizeofNB, PROT_EXEC | PROT_READ | PROT_WRITE);
    Patch_NB(nbbase, art_cbs, app_code_cache_dir, isa);
    mprotect(nbbase, sizeofNB, PROT_EXEC | PROT_READ);
}

