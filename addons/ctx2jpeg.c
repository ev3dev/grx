/**
 ** CTX2JPEG.C ---- saves a context in a JPEG file
 **
 ** Copyright (c) 1998 Vincenzo Morello
 **
 ** requires jpeg-6a by  IJG (Independent JPEG Group)
 **        available at  ftp.uu.net as graphics/jpeg/jpegsrc.v6a.tar.gz
 **
 ** should work with every compiler supporting both,
 ** jpeg-6a and GRX libraries
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include <grx20.h>

#include "jpeglib.h"

#ifdef __MSDOS__
#define WMODE   "wb"
#else
#define WMODE   "w"
#endif

struct my_error_mgr {
    struct jpeg_error_mgr pub;  /* "public" fields */
    jmp_buf setjmp_buffer;      /* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)
JPEGErrorHandler (j_common_ptr cinfo)
{
    char buffer[JMSG_LENGTH_MAX];
    my_error_ptr myerr = (my_error_ptr) cinfo->err;

    (*cinfo->err->format_message) (cinfo, buffer);
/*    printf("JPEG ERROR: %s\n", buffer); */

    longjmp(myerr->setjmp_buffer, 1);
}

/* save a pixel in a temp line buffer */
#define puttobuf(buf,x,depth,col) do {          \
    if ((depth) == 24) {                        \
       buf[3*(x)+0] = GrRGBcolorRed(col);       \
       buf[3*(x)+1] = GrRGBcolorGreen(col);     \
       buf[3*(x)+2] = GrRGBcolorBlue(col);      \
    }                                           \
    else {                                      \
       memcpy(&buf[3*(x)], &rgb[3*(col)],3);    \
    }                                           \
} while(0)

/*
** SaveContextToJpeg - Dump a context in a JPEG file
**
** Arguments:
**   cxt:      Context to be saved (NULL -> use current context)
**   jpegn:    Name of the jpeg file
**   accuracy: Accuracy percentage (100 for no loss of quality)
**
**  Returns  0 on success
**          -1 on error
*/

int
SaveContextToJpeg(GrContext *volatile cxt, char *jpegn, int accuracy)
{
    struct jpeg_compress_struct cinfo;
    struct my_error_mgr jerr;
    FILE    *volatile outfile = NULL;
    JSAMPROW row_pointer[1];
    char    *buf;
    int      volatile depth, res, i, row;
    long     width, height, colors;
    unsigned char rgb[256*3];

    if (!cxt) cxt = (GrContext *)GrCurrentContext();
    if (!cxt) return -1;
    width  = cxt->gc_xmax+1;
    height = cxt->gc_ymax+1;
    colors = GrNumColors();
    switch (colors) {
       case   2:    depth = 1;  break;
       case  16:    depth = 4;  break;
       case 256:    depth = 8;  break;
       default:     depth = 24; break;
    }

    buf = malloc(3*width+3);
    if (!buf) return -1;

    if (setjmp(jerr.setjmp_buffer)) {
        if (buf) free(buf);
        jpeg_destroy_compress(&cinfo);
        fclose(outfile);
        return -1;
    }

    if ((outfile=fopen(jpegn, WMODE)) == NULL)
        return -1;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = JPEGErrorHandler;

    jpeg_create_compress(&cinfo);
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = width;
    cinfo.image_height = height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);

    jpeg_set_quality(&cinfo, accuracy, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    if (depth != 24)
        for (i = 0; i < colors; i++) {
            int r, g, b;
            GrQueryColor(i, &r, &g, &b);
            rgb[i*3+0] = r;
            rgb[i*3+1] = g;
            rgb[i*3+2] = b;
        }
    row_pointer[0] = buf;

    res = 0;
    for (row = 0; row < height; row++) {
      int x;
      GrColor c;
#if GRX_VERSION_API-0 >= 0x229
      const GrColor *rcb = GrGetScanlineC(cxt,0,width-1,row);
      if (rcb) {
        for (x=0; x < width; ++x) {
          c = rcb[x];
          puttobuf(buf,x,depth,c);
        }
      } else
#endif
      for (x=0; x < width; ++x) {
        c = GrPixelC(cxt,x,row);
        puttobuf(buf,x,depth,c);
      }
      if (jpeg_write_scanlines(&cinfo, row_pointer, 1) < 0) {
          res = -1;
          break;
      }
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);

    fclose(outfile);
    if (buf) free(buf);
    return res;
}
