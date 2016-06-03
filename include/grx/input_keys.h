/*
 * input_keys.h
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

#ifndef __GRX_INPUT_KEYS_H__
#define __GRX_INPUT_KEYS_H__

/**
 * SECTION:input_keys
 * @short_description: TODO
 * @title: Keyboard Keys
 * @section_id: input_keys
 * @include: grx-3.0.h
 *
 * TODO
 */



/**
 * GrxKeyModifier:
 * @GRX_KEY_MODIFIER_LOCK: caps lock modifier
 * @GRX_KEY_MODIFIER_SHIFT: shift modifier
 * @GRX_KEY_MODIFIER_CTRL: control modifier
 * @GRX_KEY_MODIFIER_ALT: alt (option) modifier
 * @GRX_KEY_MODIFIER_SUPER: super (windows key) modifier
 *
 * Keyboard key modifier flags.
 */
typedef enum /*<flags>*/ {
    GRX_KEY_MODIFIER_LOCK   = 0x01,
    GRX_KEY_MODIFIER_SHIFT  = 0x02,
    GRX_KEY_MODIFIER_CTRL   = 0x04,
    GRX_KEY_MODIFIER_ALT    = 0x08,
    GRX_KEY_MODIFIER_SUPER  = 0x10,
} GrxKeyModifier;

#endif /* __GRX_INPUT_KEYS_H__ */
