

#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <log.h>

#ifndef SKIP_READABLE_CHECK
inline int is_readable(void* p){
    int dummypipe[2];
    if( pipe(dummypipe) == -1)
    {
        error_print("Pipe Error: %i", errno);
        return 0;
    }
    
    int writeable = write(dummypipe[1], p, 1);
    if (writeable == -1){
        return 0;
    }
    close(dummypipe[0]);
    close(dummypipe[1]);
    return 1;
}
#else
inline int is_readable(void* p){
    return 1;
}
#endif


inline int PatchHex_8(void* baseaddress, int offset, uint8_t original_hex, uint8_t new_hex){
    uint8_t* addrtocheck = (uint8_t*)baseaddress + offset;
    if (!is_readable(addrtocheck)){
        return 1;
    }
    uint8_t memtocheck = 0;
    memcpy(&memtocheck, addrtocheck, sizeof(uint8_t));
    if (memtocheck == original_hex){
        memcpy(addrtocheck, &new_hex, sizeof(uint8_t));
        return 0;
    }
    if (memtocheck == new_hex){
        return 0;
    }
    return 1;
}

inline int PatchHex_32(void *baseaddress, int offset, uint32_t original_hex, uint32_t new_hex){
    uint8_t* addrtocheck = (uint8_t*)baseaddress + offset;
    if (!is_readable(addrtocheck)){
        return 1;
    }
    uint32_t memtocheck = 0;
    memcpy(&memtocheck, addrtocheck, sizeof(uint32_t));
    if (memtocheck == original_hex){
        memcpy(addrtocheck, &new_hex, sizeof(uint32_t));
        return 0;
    }
    if (memtocheck == new_hex){
        return 0;
    }
    return 1;
}

inline int PatchHex_16(void* baseaddress, int offset, uint16_t original_hex, uint32_t new_hex){
    uint8_t* addrtocheck = (uint8_t*)baseaddress + offset;
    if (!is_readable(addrtocheck)){
        return 1;
    }
    uint16_t memtocheck = 0;
    memcpy(&memtocheck, addrtocheck, sizeof(uint16_t));
    if (memtocheck == original_hex){
        memcpy(addrtocheck, &new_hex, sizeof(uint16_t));
        return 0;
    }
    if (memtocheck == new_hex){
        return 0;
    }
    return 1;
}
