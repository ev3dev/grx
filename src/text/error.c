
#include <ft2build.h>
#include <glib.h>
#include FT_FREETYPE_H

typedef struct {
    gint code;
    const gchar *msg;
} FtError;

#undef __FTERRORS_H__
#define FT_ERRORDEF(e, v, s) { e, s },
#define FT_ERROR_END_LIST \
    {                     \
        0, NULL           \
    }

static const FtError ft_errors[] = {

#include FT_ERRORS_H

};

const gchar *get_ft_error_msg(gint code)
{
    const FtError *err;

    for (err = ft_errors; err->msg; err++) {
        if (err->code == code) {
            return err->msg;
        }
    }

    g_return_val_if_reached(NULL);
}
