/**
 ** recode.c ---- MGRX recode, tables borrowed from libiconv
 **
 ** Copyright (C) 2008 Mariano Alvarez Fernandez
 ** [e-mail: malfer at telefonica.net]
 **
 ** This file is part of the GRX graphics library.
 **
 ** The GRX graphics library is free software; you can redistribute it
 ** and/or modify it under some conditions; see the "copying.grx" file
 ** for details.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 **
 **/

#include <stdlib.h>
#include <string.h>
#include "libgrx.h"
#include "mgrxkeys.h"
#include "ninput.h"
 
int _GrRecode_CP437_UCS2(unsigned char src, long *des)
{
  static unsigned short cp437[128] = {
  /* 0x80 */
  0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
  0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
  /* 0x90 */
  0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
  0x00ff, 0x00d6, 0x00dc, 0x00a2, 0x00a3, 0x00a5, 0x20a7, 0x0192,
  /* 0xa0 */
  0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
  0x00bf, 0x2310, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
  /* 0xb0 */
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556,
  0x2555, 0x2563, 0x2551, 0x2557, 0x255d, 0x255c, 0x255b, 0x2510,
  /* 0xc0 */
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x255e, 0x255f,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x2567,
  /* 0xd0 */
  0x2568, 0x2564, 0x2565, 0x2559, 0x2558, 0x2552, 0x2553, 0x256b,
  0x256a, 0x2518, 0x250c, 0x2588, 0x2584, 0x258c, 0x2590, 0x2580,
  /* 0xe0 */
  0x03b1, 0x00df, 0x0393, 0x03c0, 0x03a3, 0x03c3, 0x00b5, 0x03c4,
  0x03a6, 0x0398, 0x03a9, 0x03b4, 0x221e, 0x03c6, 0x03b5, 0x2229,
  /* 0xf0 */
  0x2261, 0x00b1, 0x2265, 0x2264, 0x2320, 0x2321, 0x00f7, 0x2248,
  0x00b0, 0x2219, 0x00b7, 0x221a, 0x207f, 0x00b2, 0x25a0, 0x00a0
  };

  if (src < 0x80)
    *des = src;
  else
    *des = cp437[src-0x80];
  return 1;
}

int _GrRecode_CP850_UCS2(unsigned char src, long *des)
{
  static unsigned short cp850[128] = {
  /* 0x80 */
  0x00c7, 0x00fc, 0x00e9, 0x00e2, 0x00e4, 0x00e0, 0x00e5, 0x00e7,
  0x00ea, 0x00eb, 0x00e8, 0x00ef, 0x00ee, 0x00ec, 0x00c4, 0x00c5,
  /* 0x90 */
  0x00c9, 0x00e6, 0x00c6, 0x00f4, 0x00f6, 0x00f2, 0x00fb, 0x00f9,
  0x00ff, 0x00d6, 0x00dc, 0x00f8, 0x00a3, 0x00d8, 0x00d7, 0x0192,
  /* 0xa0 */
  0x00e1, 0x00ed, 0x00f3, 0x00fa, 0x00f1, 0x00d1, 0x00aa, 0x00ba,
  0x00bf, 0x00ae, 0x00ac, 0x00bd, 0x00bc, 0x00a1, 0x00ab, 0x00bb,
  /* 0xb0 */
  0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00c1, 0x00c2, 0x00c0,
  0x00a9, 0x2563, 0x2551, 0x2557, 0x255d, 0x00a2, 0x00a5, 0x2510,
  /* 0xc0 */
  0x2514, 0x2534, 0x252c, 0x251c, 0x2500, 0x253c, 0x00e3, 0x00c3,
  0x255a, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256c, 0x00a4,
  /* 0xd0 */
  0x00f0, 0x00d0, 0x00ca, 0x00cb, 0x00c8, 0x0131, 0x00cd, 0x00ce,
  0x00cf, 0x2518, 0x250c, 0x2588, 0x2584, 0x00a6, 0x00cc, 0x2580,
  /* 0xe0 */
  0x00d3, 0x00df, 0x00d4, 0x00d2, 0x00f5, 0x00d5, 0x00b5, 0x00fe,
  0x00de, 0x00da, 0x00db, 0x00d9, 0x00fd, 0x00dd, 0x00af, 0x00b4,
  /* 0xf0 */
  0x00ad, 0x00b1, 0x2017, 0x00be, 0x00b6, 0x00a7, 0x00f7, 0x00b8,
  0x00b0, 0x00a8, 0x00b7, 0x00b9, 0x00b3, 0x00b2, 0x25a0, 0x00a0
  };

  if (src < 0x80)
    *des = src;
  else
    *des = cp850[src-0x80];
  return 1;
}

int _GrRecode_CP1252_UCS2(unsigned char src, long *des)
{
  static unsigned short cp1252[32] = {
  /* 0x80 */
  0x20ac, 0x0081, 0x201a, 0x0192, 0x201e, 0x2026, 0x2020, 0x2021,
  0x02c6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008d, 0x017d, 0x008f,
  /* 0x90 */
  0x0090, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,
  0x02dc, 0x2122, 0x0161, 0x203a, 0x0153, 0x009d, 0x017e, 0x0178
  };

  if (src < 0x80 || src >= 0xa0)
    *des = src;
  else
    *des = cp1252[src-0x80];
  return 1;
}

int _GrRecode_ISO88591_UCS2(unsigned char src, long *des)
{
  *des = src;
  return 1;
}

int _GrRecode_UTF8_UCS2(unsigned char *src, long *des)
{
  if (src[0] < 0x80) {  /* one byte */
    *des = src[0];
    return 1;
  }
  if (((src[0] & 0xe0) == 0xc0) && ((src[1] & 0xc0) == 0x80)) { /* two bytes */
    *des = (((long)(src[0]) & 0x1f) << 6) | ((long)(src[1]) & 0x3f);
    return 2;
  }
  if (((src[0] & 0xf0) == 0xe0) && ((src[1] & 0xc0) == 0x80) &&
      ((src[2] & 0xc0) == 0x80)) { /* three bytes */
    *des = (((long)(src[0]) & 0x0f) << 12) | (((long)(src[1]) & 0x3f) << 6) |
           ((long)(src[2]) & 0x3f);
    return 3;
  }
  if (((src[0] & 0xf8) == 0xf0) && ((src[1] & 0xc0) == 0x80) &&
      ((src[2] & 0xc0) == 0x80) && ((src[3] & 0xc0) == 0x80)) { /* four bytes */
    *des = (((long)(src[0]) & 0x07) << 18) | (((long)(src[1]) & 0x3f) << 12) |
           (((long)(src[2]) & 0x3f) << 6) | ((long)(src[3]) & 0x3f);
    return 4;
  }
  return 0;
}

static int compara(const void *arg1, const void *arg2)
{
  if (*(unsigned short *)arg1 < *(unsigned short *)arg2) return -1;
  else if (*(unsigned short *)arg1 > *(unsigned short *)arg2) return 1;
  else return 0;
}

int _GrRecode_UCS2_CP437(long src, unsigned char *des)
{
  static unsigned short r_cp437[128][2] = {
  {0x00a0, 255}, {0x00a1, 173}, {0x00a2, 155}, {0x00a3, 156},
  {0x00a5, 157}, {0x00aa, 166}, {0x00ab, 174}, {0x00ac, 170},
  {0x00b0, 248}, {0x00b1, 241}, {0x00b2, 253}, {0x00b5, 230},
  {0x00b7, 250}, {0x00ba, 167}, {0x00bb, 175}, {0x00bc, 172},
  {0x00bd, 171}, {0x00bf, 168}, {0x00c4, 142}, {0x00c5, 143},
  {0x00c6, 146}, {0x00c7, 128}, {0x00c9, 144}, {0x00d1, 165},
  {0x00d6, 153}, {0x00dc, 154}, {0x00df, 225}, {0x00e0, 133},
  {0x00e1, 160}, {0x00e2, 131}, {0x00e4, 132}, {0x00e5, 134},
  {0x00e6, 145}, {0x00e7, 135}, {0x00e8, 138}, {0x00e9, 130},
  {0x00ea, 136}, {0x00eb, 137}, {0x00ec, 141}, {0x00ed, 161},
  {0x00ee, 140}, {0x00ef, 139}, {0x00f1, 164}, {0x00f2, 149},
  {0x00f3, 162}, {0x00f4, 147}, {0x00f6, 148}, {0x00f7, 246},
  {0x00f9, 151}, {0x00fa, 163}, {0x00fb, 150}, {0x00fc, 129},
  {0x00ff, 152}, {0x0192, 159}, {0x0393, 226}, {0x0398, 233},
  {0x03a3, 228}, {0x03a6, 232}, {0x03a9, 234}, {0x03b1, 224},
  {0x03b4, 235}, {0x03b5, 238}, {0x03c0, 227}, {0x03c3, 229},
  {0x03c4, 231}, {0x03c6, 237}, {0x207f, 252}, {0x20a7, 158},
  {0x2219, 249}, {0x221a, 251}, {0x221e, 236}, {0x2229, 239},
  {0x2248, 247}, {0x2261, 240}, {0x2264, 243}, {0x2265, 242},
  {0x2310, 169}, {0x2320, 244}, {0x2321, 245}, {0x2500, 196},
  {0x2502, 179}, {0x250c, 218}, {0x2510, 191}, {0x2514, 192},
  {0x2518, 217}, {0x251c, 195}, {0x2524, 180}, {0x252c, 194},
  {0x2534, 193}, {0x253c, 197}, {0x2550, 205}, {0x2551, 186},
  {0x2552, 213}, {0x2553, 214}, {0x2554, 201}, {0x2555, 184},
  {0x2556, 183}, {0x2557, 187}, {0x2558, 212}, {0x2559, 211},
  {0x255a, 200}, {0x255b, 190}, {0x255c, 189}, {0x255d, 188},
  {0x255e, 198}, {0x255f, 199}, {0x2560, 204}, {0x2561, 181},
  {0x2562, 182}, {0x2563, 185}, {0x2564, 209}, {0x2565, 210},
  {0x2566, 203}, {0x2567, 207}, {0x2568, 208}, {0x2569, 202},
  {0x256a, 216}, {0x256b, 215}, {0x256c, 206}, {0x2580, 223},
  {0x2584, 220}, {0x2588, 219}, {0x258c, 221}, {0x2590, 222},
  {0x2591, 176}, {0x2592, 177}, {0x2593, 178}, {0x25a0, 254}
  };
  unsigned short *ind;

  if (src < 0x80) {
    *des = src;
    return 1;
  }

  ind = bsearch((void *)&src, (void *)r_cp437, 128,
                2*sizeof(unsigned short), compara);
  if (ind != NULL) {
    *des = *(ind+1);
    return 1;
  }

  return 0;
}

int _GrRecode_UCS2_CP850(long src, unsigned char *des)
{
  static unsigned short r_cp850[128][2] = {
  {0x00a0, 255}, {0x00a1, 173}, {0x00a2, 189}, {0x00a3, 156},
  {0x00a4, 207}, {0x00a5, 190}, {0x00a6, 221}, {0x00a7, 245},
  {0x00a8, 249}, {0x00a9, 184}, {0x00aa, 166}, {0x00ab, 174},
  {0x00ac, 170}, {0x00ad, 240}, {0x00ae, 169}, {0x00af, 238},
  {0x00b0, 248}, {0x00b1, 241}, {0x00b2, 253}, {0x00b3, 252},
  {0x00b4, 239}, {0x00b5, 230}, {0x00b6, 244}, {0x00b7, 250},
  {0x00b8, 247}, {0x00b9, 251}, {0x00ba, 167}, {0x00bb, 175},
  {0x00bc, 172}, {0x00bd, 171}, {0x00be, 243}, {0x00bf, 168},
  {0x00c0, 183}, {0x00c1, 181}, {0x00c2, 182}, {0x00c3, 199},
  {0x00c4, 142}, {0x00c5, 143}, {0x00c6, 146}, {0x00c7, 128},
  {0x00c8, 212}, {0x00c9, 144}, {0x00ca, 210}, {0x00cb, 211},
  {0x00cc, 222}, {0x00cd, 214}, {0x00ce, 215}, {0x00cf, 216},
  {0x00d0, 209}, {0x00d1, 165}, {0x00d2, 227}, {0x00d3, 224},
  {0x00d4, 226}, {0x00d5, 229}, {0x00d6, 153}, {0x00d7, 158},
  {0x00d8, 157}, {0x00d9, 235}, {0x00da, 233}, {0x00db, 234},
  {0x00dc, 154}, {0x00dd, 237}, {0x00de, 232}, {0x00df, 225},
  {0x00e0, 133}, {0x00e1, 160}, {0x00e2, 131}, {0x00e3, 198},
  {0x00e4, 132}, {0x00e5, 134}, {0x00e6, 145}, {0x00e7, 135},
  {0x00e8, 138}, {0x00e9, 130}, {0x00ea, 136}, {0x00eb, 137},
  {0x00ec, 141}, {0x00ed, 161}, {0x00ee, 140}, {0x00ef, 139},
  {0x00f0, 208}, {0x00f1, 164}, {0x00f2, 149}, {0x00f3, 162},
  {0x00f4, 147}, {0x00f5, 228}, {0x00f6, 148}, {0x00f7, 246},
  {0x00f8, 155}, {0x00f9, 151}, {0x00fa, 163}, {0x00fb, 150},
  {0x00fc, 129}, {0x00fd, 236}, {0x00fe, 231}, {0x00ff, 152},
  {0x0131, 213}, {0x0192, 159}, {0x2017, 242}, {0x2500, 196},
  {0x2502, 179}, {0x250c, 218}, {0x2510, 191}, {0x2514, 192},
  {0x2518, 217}, {0x251c, 195}, {0x2524, 180}, {0x252c, 194},
  {0x2534, 193}, {0x253c, 197}, {0x2550, 205}, {0x2551, 186},
  {0x2554, 201}, {0x2557, 187}, {0x255a, 200}, {0x255d, 188},
  {0x2560, 204}, {0x2563, 185}, {0x2566, 203}, {0x2569, 202},
  {0x256c, 206}, {0x2580, 223}, {0x2584, 220}, {0x2588, 219},
  {0x2591, 176}, {0x2592, 177}, {0x2593, 178}, {0x25a0, 254}
  };
  unsigned short *ind;

  if (src < 0x80) {
    *des = src;
    return 1;
  }

  ind = bsearch((void *)&src, (void *)r_cp850, 128,
                2*sizeof(unsigned short), compara);
  if (ind != NULL) {
    *des = *(ind+1);
    return 1;
  }

  return 0;
}

int _GrRecode_UCS2_CP1252(long src, unsigned char *des)
{
  static unsigned short r_cp1252[32][2] = {
  {0x0081, 129}, {0x008d, 141}, {0x008f, 143}, {0x0090, 144},
  {0x009d, 157}, {0x0152, 140}, {0x0153, 156}, {0x0160, 138},
  {0x0161, 154}, {0x0178, 159}, {0x017d, 142}, {0x017e, 158},
  {0x0192, 131}, {0x02c6, 136}, {0x02dc, 152}, {0x2013, 150},
  {0x2014, 151}, {0x2018, 145}, {0x2019, 146}, {0x201a, 130},
  {0x201c, 147}, {0x201d, 148}, {0x201e, 132}, {0x2020, 134},
  {0x2021, 135}, {0x2022, 149}, {0x2026, 133}, {0x2030, 137},
  {0x2039, 139}, {0x203a, 155}, {0x20ac, 128}, {0x2122, 153}
  };
  unsigned short *ind;

  if (src < 0x80 || (src >=0xa0 && src <0x100)) {
    *des = src;
    return 1;
  }

  ind = bsearch((void *)&src, (void *)r_cp1252, 32,
                2*sizeof(unsigned short), compara);
  if (ind != NULL) {
    *des = *(ind+1);
    return 1;
  }

  return 0;
}

int _GrRecode_UCS2_ISO88591(long src, unsigned char *des)
{
  if (src < 0x100) {
    *des = (unsigned char)src;
    return 1;
  }
  return 0;
}

int _GrRecode_UCS2_UTF8(long src, unsigned char *des)
{
  des[0] = des[1] = des[2] = des[3] = 0;

  if (src < 0x80) {  /* one byte */
    des[0] = src;
    return 1;
  }
  if (src < 0x800) { /* two bytes */
    des[0] = 0xc0 | (src >> 6);
    des[1] = 0x80 | (src & 0x3f);
    return 2;
  }
  if (src < 0x10000) {/* three bytes */
    des[0] = 0xe0 | (src >> 12);
    des[1] = 0x80 | ((src >> 6) & 0x3f);
    des[2] = 0x80 | (src & 0x3f);
    return 3;
  }
  /* four bytes */
  des[0] = 0xf0 | ((src >> 18) & 0x07);
  des[1] = 0x80 | ((src >> 12) & 0x3f);
  des[2] = 0x80 | ((src >> 6) & 0x3f);
  des[3] = 0x80 | (src & 0x3f);
  return 4;
}

int _GrRecode_UCS2_mgrx512(long src, long *des)
{
  static unsigned short r_mgrx512[273][2] = {
  {0x0152,140}, {0x0153,156}, {0x0160,138}, {0x0161,154}, {0x0178,159}, {0x017D,142}, 
  {0x017E,158}, {0x0192,131}, {0x02C6,136}, {0x02DC,152}, {0x0384,436}, {0x0385,437}, 
  {0x0386,438}, {0x0388,440}, {0x0389,441}, {0x038A,442}, {0x038C,444}, {0x038E,446}, 
  {0x038F,447}, {0x0390,448}, {0x0391,449}, {0x0392,450}, {0x0393,451}, {0x0394,452}, 
  {0x0395,453}, {0x0396,454}, {0x0397,455}, {0x0398,456}, {0x0399,457}, {0x039A,458}, 
  {0x039B,459}, {0x039C,460}, {0x039D,461}, {0x039E,462}, {0x039F,463}, {0x03A0,464}, 
  {0x03A1,465}, {0x03A3,467}, {0x03A4,468}, {0x03A5,469}, {0x03A6,470}, {0x03A7,471}, 
  {0x03A8,472}, {0x03A9,473}, {0x03AA,474}, {0x03AB,475}, {0x03AC,476}, {0x03AD,477}, 
  {0x03AE,478}, {0x03AF,479}, {0x03B0,480}, {0x03B1,481}, {0x03B2,482}, {0x03B3,483}, 
  {0x03B4,484}, {0x03B5,485}, {0x03B6,486}, {0x03B7,487}, {0x03B8,488}, {0x03B9,489}, 
  {0x03BA,490}, {0x03BB,491}, {0x03BC,492}, {0x03BD,493}, {0x03BE,494}, {0x03BF,495}, 
  {0x03C0,496}, {0x03C1,497}, {0x03C2,498}, {0x03C3,499}, {0x03C4,500}, {0x03C5,501}, 
  {0x03C6,502}, {0x03C7,503}, {0x03C8,504}, {0x03C9,505}, {0x03CA,506}, {0x03CB,507}, 
  {0x03CC,508}, {0x03CD,509}, {0x03CE,510}, {0x0401,289}, {0x0402,290}, {0x0403,291}, 
  {0x0404,292}, {0x0405,293}, {0x0406,294}, {0x0407,295}, {0x0408,296}, {0x0409,297}, 
  {0x040A,298}, {0x040B,299}, {0x040C,300}, {0x040E,302}, {0x040F,303}, {0x0410,304}, 
  {0x0411,305}, {0x0412,306}, {0x0413,307}, {0x0414,308}, {0x0415,309}, {0x0416,310}, 
  {0x0417,311}, {0x0418,312}, {0x0419,313}, {0x041A,314}, {0x041B,315}, {0x041C,316}, 
  {0x041D,317}, {0x041E,318}, {0x041F,319}, {0x0420,320}, {0x0421,321}, {0x0422,322}, 
  {0x0423,323}, {0x0424,324}, {0x0425,325}, {0x0426,326}, {0x0427,327}, {0x0428,328}, 
  {0x0429,329}, {0x042A,330}, {0x042B,331}, {0x042C,332}, {0x042D,333}, {0x042E,334}, 
  {0x042F,335}, {0x0430,336}, {0x0431,337}, {0x0432,338}, {0x0433,339}, {0x0434,340}, 
  {0x0435,341}, {0x0436,342}, {0x0437,343}, {0x0438,344}, {0x0439,345}, {0x043A,346}, 
  {0x043B,347}, {0x043C,348}, {0x043D,349}, {0x043E,350}, {0x043F,351}, {0x0440,352}, 
  {0x0441,353}, {0x0442,354}, {0x0443,355}, {0x0444,356}, {0x0445,357}, {0x0446,358}, 
  {0x0447,359}, {0x0448,360}, {0x0449,361}, {0x044A,362}, {0x044B,363}, {0x044C,364}, 
  {0x044D,365}, {0x044E,366}, {0x044F,367}, {0x0451,369}, {0x0452,370}, {0x0453,371}, 
  {0x0454,372}, {0x0455,373}, {0x0456,374}, {0x0457,375}, {0x0458,376}, {0x0459,377}, 
  {0x045A,378}, {0x045B,379}, {0x045C,380}, {0x045E,382}, {0x045F,383}, {0x2013,150}, 
  {0x2014,151}, {0x2015,431}, {0x2018,145}, {0x2018,417}, {0x2019,146}, {0x2019,418}, 
  {0x201A,130}, {0x201C,147}, {0x201D,148}, {0x201E,132}, {0x2020,134}, {0x2021,135}, 
  {0x2022,149}, {0x2026,133}, {0x2030,137}, {0x2039,139}, {0x203A,155}, {0x20AC,128}, 
  {0x20AC,420}, {0x2116,368}, {0x2122,153}, {0x2190,256}, {0x2191,257}, {0x2192,258}, 
  {0x2193,259}, {0x2194,260}, {0x2195,261}, {0x21B5,262}, {0x21D0,263}, {0x21D1,264}, 
  {0x21D2,265}, {0x21D3,266}, {0x21D4,267}, {0x21D5,268}, {0x2200,270}, {0x2203,271}, 
  {0x2204,272}, {0x2205,273}, {0x2206,274}, {0x2208,275}, {0x2209,276}, {0x221A,277}, 
  {0x221E,278}, {0x2229,279}, {0x222A,280}, {0x2248,281}, {0x2260,282}, {0x2261,283}, 
  {0x2320,384}, {0x2321,386}, {0x239B,387}, {0x239C,388}, {0x239D,389}, {0x239E,390}, 
  {0x239F,391}, {0x23A0,392}, {0x23A1,393}, {0x23A2,394}, {0x23A3,395}, {0x23A4,396}, 
  {0x23A5,397}, {0x23A6,398}, {0x23A7,399}, {0x23A8,400}, {0x23A9,401}, {0x23AB,403}, 
  {0x23AC,404}, {0x23AD,405}, {0x23AE,385}, {0x2500,  1}, {0x2502,  2}, {0x2508,  3}, 
  {0x250A,  4}, {0x250C, 05}, {0x2510,  6}, {0x2514,  7}, {0x2518,  8}, {0x251C,  9}, 
  {0x2524, 10}, {0x252C, 11}, {0x2534, 12}, {0x253C, 13}, {0x2588, 17}, {0x2591, 14}, 
  {0x2592, 15}, {0x2593, 16}, {0x263A, 18}, {0x263B, 19}, {0x263C, 20}, {0x2640, 21}, 
  {0x2642, 22}, {0x2660, 23}, {0x2663, 24}, {0x2665, 25}, {0x2666, 26}, {0x266A, 27}, 
  {0x266B, 28}, {0x2714, 29}, {0x2718, 30}};
  
  unsigned short *ind;

  if ((src > 0x1F && src < 0x80) || (src > 0xA0 && src < 0x100)) {
    *des = src;
    return 1;
  }

  ind = bsearch((void *)&src, (void *)r_mgrx512, 273,
                2*sizeof(unsigned short), compara);
  if (ind != NULL) {
    *des = *(ind+1);
    return 1;
  }

  return 0;
}

int GrRecodeEvent(GrEvent *ev, int srcenc, int desenc)
{
  long aux;
  unsigned char caux[4];
  int i;

  if (ev->p2 == GRKEY_KEYCODE) return 0;

  switch (srcenc) {
  case GRENC_CP437:
    _GrRecode_CP437_UCS2((unsigned char)ev->p1, &aux);
    break;
  case GRENC_CP850:
    _GrRecode_CP850_UCS2((unsigned char)ev->p1, &aux);
    break;
  case GRENC_CP1252:
    _GrRecode_CP1252_UCS2((unsigned char)ev->p1, &aux);
    break;
  case GRENC_ISO_8859_1:
    _GrRecode_ISO88591_UCS2((unsigned char)ev->p1, &aux);
    break;
  case GRENC_UTF_8:
    _GrRecode_UTF8_UCS2((unsigned char *)ev->cp1, &aux);
    break;
  case GRENC_UCS_2:
    aux = ev->p1;
    break;
  }

  switch (desenc) {
  case GRENC_CP437:
    caux[0] = '?';
    _GrRecode_UCS2_CP437(aux, caux);
    ev->p1 = caux[0];
    ev->p2 = 1;
    return 1;
  case GRENC_CP850:
    caux[0] = '?';
    _GrRecode_UCS2_CP850(aux, caux);
    ev->p1 = caux[0];
    ev->p2 = 1;
    return 1;
  case GRENC_CP1252:
    caux[0] = '?';
    _GrRecode_UCS2_CP1252(aux, caux);
    ev->p1 = caux[0];
    ev->p2 = 1;
    return 1;
  case GRENC_ISO_8859_1:
    caux[0] = '?';
    _GrRecode_UCS2_ISO88591(aux, caux);
    ev->p1 = caux[0];
    ev->p2 = 1;
    return 1;
  case GRENC_UTF_8:
    ev->p2 = _GrRecode_UCS2_UTF8(aux, caux);
    ev->p1 = 0;
    for (i=0; i<ev->p2; i++)
      ev->cp1[i] = caux[i];
    return 1;
  case GRENC_UCS_2:
    ev->p1 = aux;
    ev->p2 = 2;
    return 1;
  }

  return 0;
}

/* UTF-8 utility functions */

int GrStrLen(const void *text, int chrtype)
{
  int length = 0;
  unsigned char *cs;
  unsigned short *ss;
    
  switch (chrtype) {
    case GR_BYTE_TEXT:
    case GR_CP437_TEXT:
    case GR_CP850_TEXT:
    case GR_CP1252_TEXT:
    case GR_ISO_8859_1_TEXT:
      cs = (unsigned char *)text;
      while(*cs) {
        length++;
        cs++;
      }
      return length;
    case GR_UTF8_TEXT:
      cs = (unsigned char *)text;
      while(*cs) {
        if ((*cs & 0xc0) != 0x80) length++;
        cs++;
      }
      return length;
    case GR_WORD_TEXT:
    case GR_UCS2_TEXT:
      ss = (unsigned short *)text;
      while(*ss) {
        length++;
        ss++;
      }
      return length;
    default:
      return 0;
  }
}

int GrUTF8StrLen(unsigned char *s)
{
  int l8 = 0;

  while (*s) {
    if ((*s & 0xc0) != 0x80) l8++;
    s++;
  }
      
  return l8;
}

/* returns a char[4] packed in a long */
long GrNextUTF8Char(unsigned char *s, int *nb)
{
  long res;
  unsigned char *caux;
  int i;

  caux = (unsigned char *)&res;

  *nb = 1; /* default one byte */

  if (((s[0] & 0xe0) == 0xc0) && ((s[1] & 0xc0) == 0x80)) { /* two bytes */
    *nb = 2;
  } else if (((s[0] & 0xf0) == 0xe0) && ((s[1] & 0xc0) == 0x80) &&
      ((s[2] & 0xc0) == 0x80)) { /* three bytes */
    *nb = 3;
  } else if (((s[0] & 0xf8) == 0xf0) && ((s[1] & 0xc0) == 0x80) &&
      ((s[2] & 0xc0) == 0x80) && ((s[3] & 0xc0) == 0x80)) { /* four bytes */
    *nb = 4;
  }

  for (i=0; i<*nb; i++)
    caux[i] = s[i];
    
  return res;
}

unsigned short GrUTF8ToUCS2(unsigned char *s)
{
  long des = (unsigned char)'?';

  _GrRecode_UTF8_UCS2(s, &des);
  return des;
}

/* returns a char[4] packed in a long */
long GrUCS2ToUTF8(unsigned short ch)
{
  long res = (unsigned char)'?';
  unsigned char *caux;

  caux = (unsigned char *)&res;
  _GrRecode_UCS2_UTF8(ch, caux);
  return res;
}

/*
unsigned short GrUTF8ToUCS2nb(unsigned char *s, int *nb)
{
  long des;

  *nb = _GrRecode_UTF8_UCS2(s, &des);
  if (*nb == 0) {
    des = (unsigned char)'?';
    *nb = 1;
  }
  return des;
}
*/

/* returns a char[4] packed in a long */
/*
long GrUCS2ToUTF8nb(unsigned short ch, int *nb)
{
  long res = (unsigned char)'?';
  unsigned char *caux;

  caux = (unsigned char *)&res;
  *nb = _GrRecode_UCS2_UTF8(ch, caux);
  return res;
}
*/

unsigned short *GrUTF8StrToUCS2Str(unsigned char *s, int *ulen, int maxlen)
{
  unsigned short *buf;
  long des;
  int i, nb;

  *ulen = GrUTF8StrLen(s);
  if (maxlen > 0 && *ulen > maxlen) *ulen = maxlen;
  buf = calloc(*ulen, sizeof(unsigned short));
  if (buf == NULL) return NULL;

  des = 0;
  for (i=0; i<*ulen; i++) {
    nb = _GrRecode_UTF8_UCS2(s, &des);
    if (nb == 0) {
      des = '?';
      nb = 1;
    }
    buf[i] = des;
    s += nb;
  }

  return buf;
}
