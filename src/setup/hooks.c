/*
 * hooks.c ---- functions to set up some hooks and control flags
 *
 * Copyright (c) 1995 Csaba Biegl, 820 Stirrup Dr, Nashville, TN 37221
 * [e-mail: csaba@vuse.vanderbilt.edu]
 *
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#include "libgrx.h"

void grx_set_restore_mode(int restoreFlag)
{
    DRVINFO->moderestore = restoreFlag;
}

void grx_set_error_handling(int exitIfError)
{
    DRVINFO->errsfatal = exitIfError;
}

/**
 * grx_set_screen_active_callback:
 * @func: Callback for handling change in screen active state.
 * @user_data: User data for @func.
 * @notify: Callback to destroy @user_data.
 *
 * Sets a callback that can be used to receive notification when the screen is
 * active.
 *
 * For example, when using a Linux framebuffer, a user program should not write
 * to the framebuffer when the screen is not active because it means that we
 * have switched to another console that is now also writing to the framebuffer.
 */
void grx_set_screen_active_callback(GrxScreenActiveFunc func,
                                    gpointer user_data,
                                    GDestroyNotify notify)
{
    if (DRVINFO->screen_active_func_notify) {
        DRVINFO->screen_active_func_notify (DRVINFO->screen_active_func_user_data);
    }
    DRVINFO->screen_active_func = func;
    DRVINFO->screen_active_func_user_data = user_data;
    DRVINFO->screen_active_func_notify = notify;
}
