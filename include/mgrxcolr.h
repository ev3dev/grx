/**
 ** mgrxcolr.h ---- some useful color definitions
 **
 ** Copyright (C) 2019 Mariano Alvarez Fernandez
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

#ifndef __GRCOLORS_H_INCLUDED__
#define __GRCOLORS_H_INCLUDED__

#ifndef __MGRX_H_INCLUDED__
#include <mgrx.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *name;
    long rgbcolor;
    GrColor color;
} GrColorTableDef;

//
// EGA colors
//

#define NUM_EGA_COLORS 16

// RGB color definitions, valid to init vars,
// but them you need GrAllocColor2(EGAR_xxx)

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

// a convenient table of WEB colors

extern GrColorTableDef GrEgaColorTable[];

int GrGenEgaColorTable(void);

// GrColors you can use after calling GrGenEgaColorTable,
// but they can not be used to init vars

#define EGAC_BLACK          GrEgaColorTable[0].color
#define EGAC_BLUE           GrEgaColorTable[1].color
#define EGAC_GREEN          GrEgaColorTable[2].color
#define EGAC_CYAN           GrEgaColorTable[3].color
#define EGAC_RED            GrEgaColorTable[4].color
#define EGAC_MAGENTA        GrEgaColorTable[5].color
#define EGAC_BROWN          GrEgaColorTable[6].color
#define EGAC_LIGHTGRAY      GrEgaColorTable[7].color
#define EGAC_DARKGRAY       GrEgaColorTable[8].color
#define EGAC_LIGHTBLUE      GrEgaColorTable[9].color
#define EGAC_LIGHTGREEN     GrEgaColorTable[10].color
#define EGAC_LIGHTCYAN      GrEgaColorTable[11].color
#define EGAC_LIGHTRED       GrEgaColorTable[12].color
#define EGAC_LIGHTMAGENTA   GrEgaColorTable[13].color
#define EGAC_YELLOW         GrEgaColorTable[14].color
#define EGAC_WHITE          GrEgaColorTable[15].color

// Indexes to Ega table, valid to init vars,
// but after calling GrGenEgaColorTable you must use
// GrEgaColorTable[EGAI_xxx].color or the macro EgaColor

#define EGAI_BLACK          0
#define EGAI_BLUE           1
#define EGAI_GREEN          2
#define EGAI_CYAN           3
#define EGAI_RED            4
#define EGAI_MAGENTA        5
#define EGAI_BROWN          6
#define EGAI_LIGHTGRAY      7
#define EGAI_DARKGRAY       8
#define EGAI_LIGHTBLUE      9
#define EGAI_LIGHTGREEN     10
#define EGAI_LIGHTCYAN      11
#define EGAI_LIGHTRED       12
#define EGAI_LIGHTMAGENTA   13
#define EGAI_YELLOW         14
#define EGAI_WHITE          15

#define EgaColor(i) (GrEgaColorTable[i].color)

//
// WEB colors, as defined by CSS and SVG specifications
//

#define NUM_WEB_COLORS 138

// RGB color definitions, valid to init vars,
// but them you need GrAllocColor2(WEBR_xxx)

#define WEBR_ALICEBLUE              0xF0F8FF 
#define WEBR_ANTIQUEWHITE           0xFAEBD7 
#define WEBR_AQUAMARINE             0x7FFFD4 
#define WEBR_AZURE                  0xF0FFFF 
#define WEBR_BEIGE                  0xF5F5DC 
#define WEBR_BISQUE                 0xFFE4C4 
#define WEBR_BLACK                  0x000000 
#define WEBR_BLANCHEDALMOND         0xFFEBCD 
#define WEBR_BLUE                   0x0000FF 
#define WEBR_BLUEVIOLET             0x8A2BE2 
#define WEBR_BROWN                  0xA52A2A 
#define WEBR_BURLYWOOD              0xDEB887 
#define WEBR_CADETBLUE              0x5F9EA0 
#define WEBR_CHARTREUSE             0x7FFF00 
#define WEBR_CHOCOLATE              0xD2691E 
#define WEBR_CORAL                  0xFF7F50 
#define WEBR_CORNFLOWERBLUE         0x6495ED 
#define WEBR_CORNSILK               0xFFF8DC 
#define WEBR_CRIMSON                0xDC143C 
#define WEBR_CYAN                   0x00FFFF 
#define WEBR_DARKBLUE               0x00008B 
#define WEBR_DARKCYAN               0x008B8B 
#define WEBR_DARKGOLDENROD          0xB8860B 
#define WEBR_DARKGRAY               0xA9A9A9 
#define WEBR_DARKGREEN              0x006400 
#define WEBR_DARKKHAKI              0xBDB76B 
#define WEBR_DARKMAGENTA            0x8B008B 
#define WEBR_DARKOLIVEGREEN         0x556B2F 
#define WEBR_DARKORANGE             0xFF8C00 
#define WEBR_DARKORCHID             0x9932CC 
#define WEBR_DARKRED                0x8B0000 
#define WEBR_DARKSALMON             0xE9967A 
#define WEBR_DARKSEAGREEN           0x8FBC8F 
#define WEBR_DARKSLATEBLUE          0x483D8B 
#define WEBR_DARKSLATEGRAY          0x2F4F4F 
#define WEBR_DARKTURQUOISE          0x00CED1 
#define WEBR_DARKVIOLET             0x9400D3 
#define WEBR_DEEPPINK               0xFF1493 
#define WEBR_DEEPSKYBLUE            0x00BFFF 
#define WEBR_DIMGRAY                0x696969 
#define WEBR_DODGERBLUE             0x1E90FF 
#define WEBR_FIREBRICK              0xB22222 
#define WEBR_FLORALWHITE            0xFFFAF0 
#define WEBR_FORESTGREEN            0x228B22 
#define WEBR_GAINSBORO              0xDCDCDC 
#define WEBR_GHOSTWHITE             0xF8F8FF 
#define WEBR_GOLD                   0xFFD700 
#define WEBR_GOLDENROD              0xDAA520 
#define WEBR_GRAY                   0x808080 
#define WEBR_GREEN                  0x008000 
#define WEBR_GREENYELLOW            0xADFF2F 
#define WEBR_HONEYDEW               0xF0FFF0 
#define WEBR_HOTPINK                0xFF69B4 
#define WEBR_INDIANRED              0xCD5C5C 
#define WEBR_INDIGO                 0x4B0082 
#define WEBR_IVORY                  0xFFFFF0 
#define WEBR_KHAKI                  0xF0E68C 
#define WEBR_LAVENDER               0xE6E6FA 
#define WEBR_LAVENDERBLUSH          0xFFF0F5 
#define WEBR_LAWNGREEN              0x7CFC00 
#define WEBR_LEMONCHIFFON           0xFFFACD 
#define WEBR_LIGHTBLUE              0xADD8E6 
#define WEBR_LIGHTCORAL             0xF08080 
#define WEBR_LIGHTCYAN              0xE0FFFF 
#define WEBR_LIGHTGOLDENRODYELLOW   0xFAFAD2 
#define WEBR_LIGHTGRAY              0xD3D3D3 
#define WEBR_LIGHTGREEN             0x90EE90 
#define WEBR_LIGHTPINK              0xFFB6C1 
#define WEBR_LIGHTSALMON            0xFFA07A 
#define WEBR_LIGHTSEAGREEN          0x20B2AA 
#define WEBR_LIGHTSKYBLUE           0x87CEFA 
#define WEBR_LIGHTSLATEGRAY         0x778899 
#define WEBR_LIGHTSTEELBLUE         0xB0C4DE 
#define WEBR_LIGHTYELLOW            0xFFFFE0 
#define WEBR_LIME                   0x00FF00 
#define WEBR_LIMEGREEN              0x32CD32 
#define WEBR_LINEN                  0xFAF0E6 
#define WEBR_MAGENTA                0xFF00FF 
#define WEBR_MAROON                 0x800000 
#define WEBR_MEDIUMAQUAMARINE       0x66CDAA 
#define WEBR_MEDIUMBLUE             0x0000CD 
#define WEBR_MEDIUMORCHID           0xBA55D3 
#define WEBR_MEDIUMPURPLE           0x9370DB 
#define WEBR_MEDIUMSEAGREEN         0x3CB371 
#define WEBR_MEDIUMSLATEBLUE        0x7B68EE 
#define WEBR_MEDIUMSPRINGGREEN      0x00FA9A 
#define WEBR_MEDIUMTURQUOISE        0x48D1CC 
#define WEBR_MEDIUMVIOLETRED        0xC71585 
#define WEBR_MIDNIGHTBLUE           0x191970 
#define WEBR_MINTCREAM              0xF5FFFA 
#define WEBR_MISTYROSE              0xFFE4E1 
#define WEBR_MOCCASIN               0xFFE4B5 
#define WEBR_NAVAJOWHITE            0xFFDEAD 
#define WEBR_NAVY                   0x000080 
#define WEBR_OLDLACE                0xFDF5E6 
#define WEBR_OLIVE                  0x808000 
#define WEBR_OLIVEDRAB              0x6B8E23 
#define WEBR_ORANGE                 0xFFA500 
#define WEBR_ORANGERED              0xFF4500 
#define WEBR_ORCHID                 0xDA70D6 
#define WEBR_PALEGOLDENROD          0xEEE8AA 
#define WEBR_PALEGREEN              0x98FB98 
#define WEBR_PALETURQUOISE          0xAFEEEE 
#define WEBR_PALEVIOLETRED          0xDB7093 
#define WEBR_PAPAYAWHIP             0xFFEFD5 
#define WEBR_PEACHPUFF              0xFFDAB9 
#define WEBR_PERU                   0xCD853F 
#define WEBR_PINK                   0xFFC0CB 
#define WEBR_PLUM                   0xDDA0DD 
#define WEBR_POWDERBLUE             0xB0E0E6 
#define WEBR_PURPLE                 0x800080 
#define WEBR_RED                    0xFF0000 
#define WEBR_ROSYBROWN              0xBC8F8F 
#define WEBR_ROYALBLUE              0x4169E1 
#define WEBR_SADDLEBROWN            0x8B4513 
#define WEBR_SALMON                 0xFA8072 
#define WEBR_SANDYBROWN             0xF4A460 
#define WEBR_SEAGREEN               0x2E8B57 
#define WEBR_SEASHELL               0xFFF5EE 
#define WEBR_SIENNA                 0xA0522D 
#define WEBR_SILVER                 0xC0C0C0 
#define WEBR_SKYBLUE                0x87CEEB 
#define WEBR_SLATEBLUE              0x6A5ACD 
#define WEBR_SLATEGRAY              0x708090 
#define WEBR_SNOW                   0xFFFAFA 
#define WEBR_SPRINGGREEN            0x00FF7F 
#define WEBR_STEELBLUE              0x4682B4 
#define WEBR_TAN                    0xD2B48C 
#define WEBR_TEAL                   0x008080 
#define WEBR_THISTLE                0xD8BFD8 
#define WEBR_TOMATO                 0xFF6347 
#define WEBR_TURQUOISE              0x40E0D0 
#define WEBR_VIOLET                 0xEE82EE 
#define WEBR_WHEAT                  0xF5DEB3 
#define WEBR_WHITE                  0xFFFFFF 
#define WEBR_WHITESMOKE             0xF5F5F5 
#define WEBR_YELLOW                 0xFFFF00 
#define WEBR_YELLOWGREEN            0x9ACD32 

//  some common synonyms

#define WEBR_AQUA                   0x00FFFF // CYAN
#define WEBR_DARKGREY               0xA9A9A9 // DARKGRAY
#define WEBR_DARKSLATEGREY          0x2F4F4F // DARKSLATEGRAY
#define WEBR_DIMGREY                0x696969 // DIMGRAY
#define WEBR_FUCHSIA                0xFF00FF // MAGENTA
#define WEBR_GREY                   0x808080 // GRAY
#define WEBR_LIGHTGREY              0xD3D3D3 // LIGHTGRAY
#define WEBR_LIGHTSLATEGREY         0x778899 // LIGHTSLATEGRAEY
#define WEBR_SLATEGREY              0x708090 // SLATEGRAY

// a convenient table of WEB colors

extern GrColorTableDef GrWebColorTable[];

int GrGenWebColorTable(void);

// GrColors you can use after calling GrGenWebColorTable,
// but they can not be used to init vars

#define WEBC_ALICEBLUE              GrWebColorTable[0].color 
#define WEBC_ANTIQUEWHITE           GrWebColorTable[1].color 
#define WEBC_AQUAMARINE             GrWebColorTable[2].color 
#define WEBC_AZURE                  GrWebColorTable[3].color 
#define WEBC_BEIGE                  GrWebColorTable[4].color 
#define WEBC_BISQUE                 GrWebColorTable[5].color 
#define WEBC_BLACK                  GrWebColorTable[6].color 
#define WEBC_BLANCHEDALMOND         GrWebColorTable[7].color 
#define WEBC_BLUE                   GrWebColorTable[8].color 
#define WEBC_BLUEVIOLET             GrWebColorTable[9].color 
#define WEBC_BROWN                  GrWebColorTable[10].color 
#define WEBC_BURLYWOOD              GrWebColorTable[11].color 
#define WEBC_CADETBLUE              GrWebColorTable[12].color 
#define WEBC_CHARTREUSE             GrWebColorTable[13].color 
#define WEBC_CHOCOLATE              GrWebColorTable[14].color 
#define WEBC_CORAL                  GrWebColorTable[15].color 
#define WEBC_CORNFLOWERBLUE         GrWebColorTable[16].color 
#define WEBC_CORNSILK               GrWebColorTable[17].color 
#define WEBC_CRIMSON                GrWebColorTable[18].color 
#define WEBC_CYAN                   GrWebColorTable[19].color 
#define WEBC_DARKBLUE               GrWebColorTable[20].color 
#define WEBC_DARKCYAN               GrWebColorTable[21].color 
#define WEBC_DARKGOLDENROD          GrWebColorTable[22].color 
#define WEBC_DARKGRAY               GrWebColorTable[23].color 
#define WEBC_DARKGREEN              GrWebColorTable[24].color 
#define WEBC_DARKKHAKI              GrWebColorTable[25].color 
#define WEBC_DARKMAGENTA            GrWebColorTable[26].color 
#define WEBC_DARKOLIVEGREEN         GrWebColorTable[27].color 
#define WEBC_DARKORANGE             GrWebColorTable[28].color 
#define WEBC_DARKORCHID             GrWebColorTable[29].color 
#define WEBC_DARKRED                GrWebColorTable[30].color 
#define WEBC_DARKSALMON             GrWebColorTable[31].color 
#define WEBC_DARKSEAGREEN           GrWebColorTable[32].color 
#define WEBC_DARKSLATEBLUE          GrWebColorTable[33].color 
#define WEBC_DARKSLATEGRAY          GrWebColorTable[34].color 
#define WEBC_DARKTURQUOISE          GrWebColorTable[35].color 
#define WEBC_DARKVIOLET             GrWebColorTable[36].color 
#define WEBC_DEEPPINK               GrWebColorTable[37].color 
#define WEBC_DEEPSKYBLUE            GrWebColorTable[38].color 
#define WEBC_DIMGRAY                GrWebColorTable[39].color 
#define WEBC_DODGERBLUE             GrWebColorTable[40].color 
#define WEBC_FIREBRICK              GrWebColorTable[41].color 
#define WEBC_FLORALWHITE            GrWebColorTable[42].color 
#define WEBC_FORESTGREEN            GrWebColorTable[43].color 
#define WEBC_GAINSBORO              GrWebColorTable[44].color 
#define WEBC_GHOSTWHITE             GrWebColorTable[45].color 
#define WEBC_GOLD                   GrWebColorTable[46].color 
#define WEBC_GOLDENROD              GrWebColorTable[47].color 
#define WEBC_GRAY                   GrWebColorTable[48].color 
#define WEBC_GREEN                  GrWebColorTable[49].color 
#define WEBC_GREENYELLOW            GrWebColorTable[50].color 
#define WEBC_HONEYDEW               GrWebColorTable[51].color 
#define WEBC_HOTPINK                GrWebColorTable[52].color 
#define WEBC_INDIANRED              GrWebColorTable[53].color 
#define WEBC_INDIGO                 GrWebColorTable[54].color 
#define WEBC_IVORY                  GrWebColorTable[55].color 
#define WEBC_KHAKI                  GrWebColorTable[56].color 
#define WEBC_LAVENDER               GrWebColorTable[57].color 
#define WEBC_LAVENDERBLUSH          GrWebColorTable[58].color 
#define WEBC_LAWNGREEN              GrWebColorTable[59].color 
#define WEBC_LEMONCHIFFON           GrWebColorTable[60].color 
#define WEBC_LIGHTBLUE              GrWebColorTable[61].color 
#define WEBC_LIGHTCORAL             GrWebColorTable[62].color 
#define WEBC_LIGHTCYAN              GrWebColorTable[63].color 
#define WEBC_LIGHTGOLDENRODYELLOW   GrWebColorTable[64].color 
#define WEBC_LIGHTGRAY              GrWebColorTable[65].color 
#define WEBC_LIGHTGREEN             GrWebColorTable[66].color 
#define WEBC_LIGHTPINK              GrWebColorTable[67].color 
#define WEBC_LIGHTSALMON            GrWebColorTable[68].color 
#define WEBC_LIGHTSEAGREEN          GrWebColorTable[69].color 
#define WEBC_LIGHTSKYBLUE           GrWebColorTable[70].color 
#define WEBC_LIGHTSLATEGRAY         GrWebColorTable[71].color 
#define WEBC_LIGHTSTEELBLUE         GrWebColorTable[72].color 
#define WEBC_LIGHTYELLOW            GrWebColorTable[73].color 
#define WEBC_LIME                   GrWebColorTable[74].color 
#define WEBC_LIMEGREEN              GrWebColorTable[75].color 
#define WEBC_LINEN                  GrWebColorTable[76].color 
#define WEBC_MAGENTA                GrWebColorTable[77].color 
#define WEBC_MAROON                 GrWebColorTable[78].color 
#define WEBC_MEDIUMAQUAMARINE       GrWebColorTable[79].color 
#define WEBC_MEDIUMBLUE             GrWebColorTable[80].color 
#define WEBC_MEDIUMORCHID           GrWebColorTable[81].color 
#define WEBC_MEDIUMPURPLE           GrWebColorTable[82].color 
#define WEBC_MEDIUMSEAGREEN         GrWebColorTable[83].color 
#define WEBC_MEDIUMSLATEBLUE        GrWebColorTable[84].color 
#define WEBC_MEDIUMSPRINGGREEN      GrWebColorTable[85].color 
#define WEBC_MEDIUMTURQUOISE        GrWebColorTable[86].color 
#define WEBC_MEDIUMVIOLETRED        GrWebColorTable[87].color 
#define WEBC_MIDNIGHTBLUE           GrWebColorTable[88].color 
#define WEBC_MINTCREAM              GrWebColorTable[89].color 
#define WEBC_MISTYROSE              GrWebColorTable[90].color 
#define WEBC_MOCCASIN               GrWebColorTable[91].color 
#define WEBC_NAVAJOWHITE            GrWebColorTable[92].color 
#define WEBC_NAVY                   GrWebColorTable[93].color 
#define WEBC_OLDLACE                GrWebColorTable[94].color 
#define WEBC_OLIVE                  GrWebColorTable[95].color 
#define WEBC_OLIVEDRAB              GrWebColorTable[96].color 
#define WEBC_ORANGE                 GrWebColorTable[97].color 
#define WEBC_ORANGERED              GrWebColorTable[98].color 
#define WEBC_ORCHID                 GrWebColorTable[99].color 
#define WEBC_PALEGOLDENROD          GrWebColorTable[100].color 
#define WEBC_PALEGREEN              GrWebColorTable[101].color 
#define WEBC_PALETURQUOISE          GrWebColorTable[102].color 
#define WEBC_PALEVIOLETRED          GrWebColorTable[103].color 
#define WEBC_PAPAYAWHIP             GrWebColorTable[104].color 
#define WEBC_PEACHPUFF              GrWebColorTable[105].color 
#define WEBC_PERU                   GrWebColorTable[106].color 
#define WEBC_PINK                   GrWebColorTable[107].color 
#define WEBC_PLUM                   GrWebColorTable[108].color 
#define WEBC_POWDERBLUE             GrWebColorTable[109].color 
#define WEBC_PURPLE                 GrWebColorTable[110].color 
#define WEBC_RED                    GrWebColorTable[111].color 
#define WEBC_ROSYBROWN              GrWebColorTable[112].color 
#define WEBC_ROYALBLUE              GrWebColorTable[113].color 
#define WEBC_SADDLEBROWN            GrWebColorTable[114].color 
#define WEBC_SALMON                 GrWebColorTable[115].color 
#define WEBC_SANDYBROWN             GrWebColorTable[116].color 
#define WEBC_SEAGREEN               GrWebColorTable[117].color 
#define WEBC_SEASHELL               GrWebColorTable[118].color 
#define WEBC_SIENNA                 GrWebColorTable[119].color 
#define WEBC_SILVER                 GrWebColorTable[120].color 
#define WEBC_SKYBLUE                GrWebColorTable[121].color 
#define WEBC_SLATEBLUE              GrWebColorTable[122].color 
#define WEBC_SLATEGRAY              GrWebColorTable[123].color 
#define WEBC_SNOW                   GrWebColorTable[124].color 
#define WEBC_SPRINGGREEN            GrWebColorTable[125].color 
#define WEBC_STEELBLUE              GrWebColorTable[126].color 
#define WEBC_TAN                    GrWebColorTable[127].color 
#define WEBC_TEAL                   GrWebColorTable[128].color 
#define WEBC_THISTLE                GrWebColorTable[129].color 
#define WEBC_TOMATO                 GrWebColorTable[130].color 
#define WEBC_TURQUOISE              GrWebColorTable[131].color 
#define WEBC_VIOLET                 GrWebColorTable[132].color 
#define WEBC_WHEAT                  GrWebColorTable[133].color 
#define WEBC_WHITE                  GrWebColorTable[134].color 
#define WEBC_WHITESMOKE             GrWebColorTable[135].color 
#define WEBC_YELLOW                 GrWebColorTable[136].color 
#define WEBC_YELLOWGREEN            GrWebColorTable[137].color 

//  some common synonyms

#define WEBC_AQUA                   WEBC_CYAN
#define WEBC_DARKGREY               WEBC_DARKGRAY
#define WEBC_DARKSLATEGREY          WEBC_DARKSLATEGRAY
#define WEBC_DIMGREY                WEBC_DIMGRAY
#define WEBC_FUCHSIA                WEBC_MAGENTA
#define WEBC_GREY                   WEBC_GRAY
#define WEBC_LIGHTGREY              WEBC_LIGHTGRAY
#define WEBC_LIGHTSLATEGREY         WEBC_LIGHTSLATEGRAEY
#define WEBC_SLATEGREY              WEBC_SLATEGRAY

// Indexes to Web table, valid to init vars,
// but after calling GrGenEgaColorTable you must use
// GrWebColorTable[WEBI_xxx].color or the macro WebColor

#define WEBI_ALICEBLUE              0 
#define WEBI_ANTIQUEWHITE           1 
#define WEBI_AQUAMARINE             2 
#define WEBI_AZURE                  3 
#define WEBI_BEIGE                  4 
#define WEBI_BISQUE                 5 
#define WEBI_BLACK                  6 
#define WEBI_BLANCHEDALMOND         7 
#define WEBI_BLUE                   8 
#define WEBI_BLUEVIOLET             9 
#define WEBI_BROWN                  10 
#define WEBI_BURLYWOOD              11 
#define WEBI_CADETBLUE              12 
#define WEBI_CHARTREUSE             13 
#define WEBI_CHOCOLATE              14 
#define WEBI_CORAL                  15 
#define WEBI_CORNFLOWERBLUE         16 
#define WEBI_CORNSILK               17 
#define WEBI_CRIMSON                18 
#define WEBI_CYAN                   19 
#define WEBI_DARKBLUE               20 
#define WEBI_DARKCYAN               21 
#define WEBI_DARKGOLDENROD          22 
#define WEBI_DARKGRAY               23 
#define WEBI_DARKGREEN              24 
#define WEBI_DARKKHAKI              25 
#define WEBI_DARKMAGENTA            26 
#define WEBI_DARKOLIVEGREEN         27 
#define WEBI_DARKORANGE             28 
#define WEBI_DARKORCHID             29 
#define WEBI_DARKRED                30 
#define WEBI_DARKSALMON             31 
#define WEBI_DARKSEAGREEN           32 
#define WEBI_DARKSLATEBLUE          33 
#define WEBI_DARKSLATEGRAY          34 
#define WEBI_DARKTURQUOISE          35 
#define WEBI_DARKVIOLET             36 
#define WEBI_DEEPPINK               37 
#define WEBI_DEEPSKYBLUE            38 
#define WEBI_DIMGRAY                39 
#define WEBI_DODGERBLUE             40 
#define WEBI_FIREBRICK              41 
#define WEBI_FLORALWHITE            42 
#define WEBI_FORESTGREEN            43 
#define WEBI_GAINSBORO              44 
#define WEBI_GHOSTWHITE             45 
#define WEBI_GOLD                   46 
#define WEBI_GOLDENROD              47 
#define WEBI_GRAY                   48 
#define WEBI_GREEN                  49 
#define WEBI_GREENYELLOW            50 
#define WEBI_HONEYDEW               51 
#define WEBI_HOTPINK                52 
#define WEBI_INDIANRED              53 
#define WEBI_INDIGO                 54 
#define WEBI_IVORY                  55 
#define WEBI_KHAKI                  56 
#define WEBI_LAVENDER               57 
#define WEBI_LAVENDERBLUSH          58 
#define WEBI_LAWNGREEN              59 
#define WEBI_LEMONCHIFFON           60 
#define WEBI_LIGHTBLUE              61 
#define WEBI_LIGHTCORAL             62 
#define WEBI_LIGHTCYAN              63 
#define WEBI_LIGHTGOLDENRODYELLOW   64 
#define WEBI_LIGHTGRAY              65 
#define WEBI_LIGHTGREEN             66 
#define WEBI_LIGHTPINK              67 
#define WEBI_LIGHTSALMON            68 
#define WEBI_LIGHTSEAGREEN          69 
#define WEBI_LIGHTSKYBLUE           70 
#define WEBI_LIGHTSLATEGRAY         71 
#define WEBI_LIGHTSTEELBLUE         72 
#define WEBI_LIGHTYELLOW            73 
#define WEBI_LIME                   74 
#define WEBI_LIMEGREEN              75 
#define WEBI_LINEN                  76 
#define WEBI_MAGENTA                77 
#define WEBI_MAROON                 78 
#define WEBI_MEDIUMAQUAMARINE       79 
#define WEBI_MEDIUMBLUE             80 
#define WEBI_MEDIUMORCHID           81 
#define WEBI_MEDIUMPURPLE           82 
#define WEBI_MEDIUMSEAGREEN         83 
#define WEBI_MEDIUMSLATEBLUE        84 
#define WEBI_MEDIUMSPRINGGREEN      85 
#define WEBI_MEDIUMTURQUOISE        86 
#define WEBI_MEDIUMVIOLETRED        87 
#define WEBI_MIDNIGHTBLUE           88 
#define WEBI_MINTCREAM              89 
#define WEBI_MISTYROSE              90 
#define WEBI_MOCCASIN               91 
#define WEBI_NAVAJOWHITE            92 
#define WEBI_NAVY                   93 
#define WEBI_OLDLACE                94 
#define WEBI_OLIVE                  95 
#define WEBI_OLIVEDRAB              96 
#define WEBI_ORANGE                 97 
#define WEBI_ORANGERED              98 
#define WEBI_ORCHID                 99 
#define WEBI_PALEGOLDENROD          100 
#define WEBI_PALEGREEN              101 
#define WEBI_PALETURQUOISE          102 
#define WEBI_PALEVIOLETRED          103 
#define WEBI_PAPAYAWHIP             104 
#define WEBI_PEACHPUFF              105 
#define WEBI_PERU                   106 
#define WEBI_PINK                   107 
#define WEBI_PLUM                   108 
#define WEBI_POWDERBLUE             109 
#define WEBI_PURPLE                 110 
#define WEBI_RED                    111 
#define WEBI_ROSYBROWN              112 
#define WEBI_ROYALBLUE              113 
#define WEBI_SADDLEBROWN            114 
#define WEBI_SALMON                 115 
#define WEBI_SANDYBROWN             116 
#define WEBI_SEAGREEN               117 
#define WEBI_SEASHELL               118 
#define WEBI_SIENNA                 119 
#define WEBI_SILVER                 120 
#define WEBI_SKYBLUE                121 
#define WEBI_SLATEBLUE              122 
#define WEBI_SLATEGRAY              123 
#define WEBI_SNOW                   124 
#define WEBI_SPRINGGREEN            125 
#define WEBI_STEELBLUE              126 
#define WEBI_TAN                    127 
#define WEBI_TEAL                   128 
#define WEBI_THISTLE                129 
#define WEBI_TOMATO                 130 
#define WEBI_TURQUOISE              131 
#define WEBI_VIOLET                 132 
#define WEBI_WHEAT                  133 
#define WEBI_WHITE                  134 
#define WEBI_WHITESMOKE             135 
#define WEBI_YELLOW                 136 
#define WEBI_YELLOWGREEN            137 

//  some common synonyms

#define WEBI_AQUA                   WEBI_CYAN
#define WEBI_DARKGREY               WEBI_DARKGRAY
#define WEBI_DARKSLATEGREY          WEBI_DARKSLATEGRAY
#define WEBI_DIMGREY                WEBI_DIMGRAY
#define WEBI_FUCHSIA                WEBI_MAGENTA
#define WEBI_GREY                   WEBI_GRAY
#define WEBI_LIGHTGREY              WEBI_LIGHTGRAY
#define WEBI_LIGHTSLATEGREY         WEBI_LIGHTSLATEGRAEY
#define WEBI_SLATEGREY              WEBI_SLATEGRAY

#define WebColor(i) (GrWebColorTable[i].color)

#ifdef __cplusplus
}
#endif

#endif /* whole file */
