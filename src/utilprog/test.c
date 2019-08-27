#ifndef __LIBGRX_H_INCLUDED__
#include "libgrx.h"
#endif

#ifndef __MEMPEEK_H_INCLUDED__
#include "mempeek.h"
#endif

#include "access24.h"

#define EGAR_BLACK          0x000000
#define EGAR_BLUE           0x0000AA
#define EGAR_GREEN          0x00AA00
#define EGAR_CYAN           0x00AAAA
#define EGAR_RED            0xAA0000
#define EGAR_MAGENTA        0xAA00AA
#define EGAR_BROWN          0xAA5500
#define EGAR_LIGHTGRAY      0xAAAAAA
#define EGAR_DARKGRAY       0x555555
#define EGAR_LIGHTBLUE      0x5555FF
#define EGAR_LIGHTGREEN     0x55FF55
#define EGAR_LIGHTCYAN      0x55FFFF
#define EGAR_LIGHTRED       0xFF5555
#define EGAR_LIGHTMAGENTA   0xFF55FF
#define EGAR_YELLOW         0xFFFF55
#define EGAR_WHITE          0xFFFFFF

int main(void)
{
    int t = EGAR_BLUE;
    int t0=0, t1=0, t2=0;
    int _bo = 0, bo = 0, gcc = 0;
    int cix64 = 0, ci386 = 0;
    
    #if defined __BYTE_ORDER__
    _bo = __BYTE_ORDER__;
    #endif
    
    #if defined BYTE_ORDER
    bo = BYTE_ORDER;
    #endif
    
    //le = (BYTE_ORDER==LITTLE_ENDIAN);

    #if defined __GNUC__
    gcc = 1;
    #endif
    
    #if defined __i386__
    ci386 = 1;
    #endif

    #if defined __x86_64__
    cix64 = 1;
    #endif

    printf("_bo %d  bo %d  gcc %d  i386 %d  x64 %d\n", _bo, bo, gcc, ci386, cix64);

    #if defined __LITTLE_ENDIAN__
    printf("defined __LITTLE_ENDIAN__\n");
    #endif

    #if defined __ORDER_LITTLE_ENDIAN__
    printf("defined __ORDER_LITTLE_ENDIAN__\n");
    #endif

    #if defined _LITTLE_ENDIAN
    printf("defined _LITTLE_ENDIAN\n");
    #endif

    #if defined LITTLE_ENDIAN
    printf("defined LITTLE_ENDIAN %d\n", LITTLE_ENDIAN);
    #endif

    t0 = RD24BYTE(t, 0);
    t1 = RD24BYTE(t, 1);
    t2 = RD24BYTE(t, 2);
    
    printf("t %d  t0 %d  t1 %d  t2 %d\n", t, t0, t1, t2);
    
    return 0;
}
