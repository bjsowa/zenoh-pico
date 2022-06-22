/*
 * Copyright (c) 2017, 2021 ADLINK Technology Inc.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
 * which is available at https://www.apache.org/licenses/LICENSE-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
 *
 * Contributors:
 *     ADLINK zenoh team, <zenoh@adlink-labs.tech>
 */

#include <string.h>
#include "zenoh-pico/collections/string.h"

/*-------- string --------*/
_z_string_t z_string_make(const char *value)
{
    _z_string_t s;
    s.val = _z_str_clone(value);
    s.len = strlen(value);
    return s;
}

void _z_string_copy(_z_string_t *dst, const _z_string_t *src)
{
    if (src->val)
        dst->val = _z_str_clone(src->val);
    else
        dst->val = NULL;
    dst->len = src->len;
}

void _z_string_move(_z_string_t *dst, _z_string_t *src)
{
    dst->val = src->val;
    dst->len = src->len;

    src->val = NULL;
    src->len = 0;
}

void _z_string_move_str(_z_string_t *dst, char *src)
{
    dst->val = src;
    dst->len = strlen(src);
}

void _z_string_reset(_z_string_t *str)
{
    str->val = NULL;
    str->len = 0;
}

void _z_string_clear(_z_string_t *str)
{
    free(str->val);
    _z_string_reset(str);
}

void _z_string_free(_z_string_t **str)
{
    _z_string_t *ptr = (_z_string_t *)*str;
    _z_string_clear(ptr);

    free(ptr);
    *str = NULL;
}

_z_string_t _z_string_from_bytes(_z_bytes_t *bs)
{
    _z_string_t s;
    s.len = 2 * bs->len;
    char *s_val = (char *)malloc(s.len * sizeof(char) + 1);

    char c[] = "0123456789ABCDEF";
    for (size_t i = 0; i < bs->len; i++)
    {
        s_val[2 * i] = c[(bs->start[i] & 0xF0) >> 4];
        s_val[2 * i + 1] = c[bs->start[i] & 0x0F];
    }
    s_val[s.len] = '\0';
    s.val = s_val;

    return s;
}

/*-------- str --------*/
size_t _z_str_size(const char *src)
{
    return strlen(src) + 1;
}

void _z_str_clear(char *src)
{
    free(src);
    src = NULL;
}

void _z_str_free(char **src)
{
    char *ptr = *src;
    _z_str_clear(ptr);
    *src = NULL;
}

void _z_str_copy(char *dst, const char *src)
{
    strcpy(dst, src);
}

char *_z_str_clone(const char *src)
{
    char *dst = (char *)malloc(_z_str_size(src));
    _z_str_copy(dst, src);
    return dst;
}

int _z_str_eq(const char *left, const char *right)
{
    return strcmp(left, right) == 0;
}

/*-------- str_array --------*/
void _z_str_array_init(_z_str_array_t *sa, size_t len)
{
    char ***val = (char ***)&sa->_val;
    *val = (char **)malloc(len * sizeof(char *));
    sa->_len = len;
}

_z_str_array_t _z_str_array_make(size_t len)
{
    _z_str_array_t sa;
    _z_str_array_init(&sa, len);
    return sa;
}

char **_z_str_array_get(const _z_str_array_t *sa, size_t pos)
{
    return &sa->_val[pos];
}

size_t _z_str_array_len(const _z_str_array_t *sa)
{
    return sa->_len;
}

uint8_t _z_str_array_is_empty(const _z_str_array_t *sa)
{
    return sa->_len == 0;
}

void _z_str_array_clear(_z_str_array_t *sa)
{
    for (size_t i = 0; i < sa->_len; i++)
        free(sa->_val[i]);
    free(sa->_val);
}

void _z_str_array_free(_z_str_array_t **sa)
{
    _z_str_array_t *ptr = *sa;
    _z_str_array_clear(ptr);
    free(ptr);
    *sa = NULL;
}

void _z_str_array_copy(_z_str_array_t *dst, const _z_str_array_t *src)
{
    _z_str_array_init(dst, src->_len);
    for (size_t i = 0; i < src->_len; i++)
        dst->_val[i] = _z_str_clone(src->_val[i]);
    dst->_len = src->_len;
}

void _z_str_array_move(_z_str_array_t *dst, _z_str_array_t *src)
{
    dst->_val = src->_val;
    dst->_len = src->_len;

    src->_val = NULL;
    src->_len = 0;
}
