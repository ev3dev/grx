/*
 * error.h
 *
 * Copyright (c) 2016 David Lechner <david@lechnology.com>
 * This file is part of the GRX graphics library.
 *
 * The GRX graphics library is free software; you can redistribute it
 * and/or modify it under some conditions; see the "copying.grx" file
 * for details.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef __GRX_ERROR_H__
#define __GRX_ERROR_H__

/**
 * SECTION:error
 * @short_description: GRX error domain
 * @title: Errors
 * @section_id: error
 * @include: grx-3.0.h
 *
 * TODO
 */

/**
 * GRX_ERROR:
 *
 * Error domain for GRX. Errors in this domain will be from #GrxError.
 */
#define GRX_ERROR grx_error_quark()

GQuark grx_error_quark(void);

/**
 * GrxError:
 * @GRX_ERROR_FAILED: Generic error.
 * @GRX_ERROR_PLUGIN_FILE_NOT_FOUND: Plugin file was not found in search paths.
 * @GRX_ERROR_PLUGIN_SYMBOL_NOT_FOUND: A required symbol in a plugin was not found.
 *
 * Error codes returned by GRX functions.
 */
typedef enum {
    GRX_ERROR_FAILED,
    GRX_ERROR_PLUGIN_FILE_NOT_FOUND,
    GRX_ERROR_PLUGIN_SYMBOL_NOT_FOUND,
} GrxError;

#endif /* __GRX_ERROR_H__ */
