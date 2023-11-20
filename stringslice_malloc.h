/*
MIT License

Copyright (c) 2023 Daniel Goldenberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#ifndef STRINGSLICE_H
#define STRINGSLICE_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef uint8_t ss_bool;

typedef struct String String;
typedef struct StringArray StringArray;

struct String
{
  char *str;
  size_t len;
};

struct StringArray
{
  String *e;
  size_t count;
};

#define str(s) ((String) {s, cstr_len(s)-1})

// @CString ====================================================================================

// Returns the length of a null-terminated string
inline
size_t cstr_len(char *s)
{
  size_t len = 0;
  for (; s[len]; len++);
  return len+1;
}

// @String =====================================================================================

#endif
