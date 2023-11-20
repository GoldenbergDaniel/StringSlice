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
#include <stdlib.h>
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
struct String
{
  char *str;
  size_t len;
  ss_bool allocated;
};

typedef struct StringArray StringArray;
struct StringArray
{
  String *e;
  size_t count;
};

#define str(s) ((String) {s, cstr_len(s)-1, 0})

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

// Allocates memory for str member and returns new string
inline
String ss_alloc_str(size_t len)
{
  String result;
  result.str = malloc(len);
  result.len = len;
  result.allocated = TRUE;

  return result;
}

// If string was alloced, frees memory and returns true. Else, return false
inline
ss_bool ss_free_str(String *s)
{
  if (!s->allocated) return FALSE;
  
  free(s->str);
  s->len = 0;
  s->allocated = FALSE;

  return TRUE;  
}

// Returns true if the characters in `s1` match `s2`
ss_bool str_equals(String s1, String s2)
{
  if (s1.len != s2.len) return FALSE;

  ss_bool result = TRUE;

  for (size_t i = 0; i < s1.len; i++)
  {
    if (s1.str[i] != s2.str[i])
    {
      result = FALSE;
      break;
    }
  }

  return result;
}

// Returns true if `s` contains `substr`
ss_bool str_contains(String s, String substr)
{
  if (s.len < substr.len) return FALSE;

  ss_bool result = FALSE;

  for (size_t i = 0; i < s.len-substr.len+1; i++)
  {
    if (s.str[i] == substr.str[0])
    {
      for (size_t j = 0; j < substr.len; j++)
      {
        if (s.str[i+j] != substr.str[j]) break;

        if (j == substr.len-1)
        {
          result = TRUE;
          return result;
        }
      }
    }
  }

  return result;
}

// Returns the index of the first instance of `substr` in `s` starting at index `start`
int64_t str_find(String s, String substr, size_t start)
{
  if (s.len < substr.len || start >= s.len) return -1;

  int64_t result = -1;

  for (size_t i = start; i < s.len-substr.len+1; i++)
  {
    if (s.str[i] == substr.str[0])
    {
      for (size_t j = 0; j < substr.len; j++)
      {
        if (s.str[i+j] != substr.str[j]) break;

        if (j == substr.len-1)
        {
          result = i;
          return result;
        }
      }
    }
  }

  return result;
}

// Returns the index of the first instance of `c` in `s` starting at index `start`
int64_t str_find_char(String s, char c, size_t start)
{
  if (start >= s.len) return FALSE;

  int64_t result = -1;

  for (size_t i = start; i < s.len; i++)
  {
    if (s.str[i] == c)
    {
      result = i;
      break;
    }
  }

  return result;
}

// Allocates and returns a copy of `s`
String str_copy(String s)
{
  String result = ss_alloc_str(s.len);

  for (size_t i = 0; i < s.len; i++)
  {
    result.str[i] = s.str[i];
  }
  
  result.len = s.len;

  return result;
}

// Copies `src` into `dest.` Expects `src.len <= dest.len`
String str_copy_into(String *dest, String src)
{
  assert(src.len <= dest->len);

  for (size_t i = 0; i < src.len; i++)
  {
    dest->str[i] = src.str[i];
  }
  
  dest->len = src.len;

  return *dest;
}

// Inserts `substr` into `s` at index `loc.` Expects `loc + substr.len <= s.len`
String str_insert_at(String s, String substr, size_t loc)
{
  assert(loc + substr.len <= s.len);

  size_t dest_idx = loc;
  for (size_t i = 0; i < substr.len; i++)
  {
    s.str[dest_idx] = substr.str[i];
    dest_idx++;
  }

  return s;
}

// Returns a new string combining `s1` and `s2` such that the characters of `s2` follow `s1`
String str_concat(String s1, String s2)
{
  String result = ss_alloc_str(s1.len + s2.len);

  for (size_t i = 0; i < s1.len; i++)
  {
    result.str[i] = s1.str[i];
  }

  for (size_t i = 0; i < s2.len; i++)
  {
    result.str[i+s1.len] = s2.str[i];
  }

  return result;
}

// Returns a substring of `s` starting at index `start` and ending before index `end`
String str_substr(String s, size_t start, size_t end)
{
  assert(start >= 0 && start < s.len && end > 0 && end <= s.len && start < end);

  String result = ss_alloc_str(end - start);

  size_t result_idx = 0;
  for (size_t i = start; i < end; i++)
  {
    result.str[result_idx] = s.str[i];
    result_idx++;
  }

  return result;
}

// Returns a new string with the first `substr.len` characters of `s` removed if they equal `substr`
String str_strip_front(String s, String substr)
{
  assert(substr.len <= s.len);

  String result = s;

  size_t front_len = substr.len;
  String front = str_substr(s, 0, front_len);
  if (str_equals(front, substr))
  {
    result = str_substr(s, front_len, s.len);
    result.len = s.len-front_len;
  }

  ss_free_str(&front);
  
  return result;
}

// Returns a new string with the last `substr.len` characters of `s` removed if they equal `substr`
String str_strip_back(String s, String substr)
{
  assert(substr.len <= s.len);

  String result = s;

  size_t end_len = s.len - substr.len;
  String end = str_substr(s, end_len, s.len);
  if (str_equals(end, substr))
  {
    result = str_substr(s, 0, end_len);
    result.len = end_len;
  }

  ss_free_str(&end);
  
  return result;
}

// Returns a new string with a null terminator appended to the end
String str_nullify(String s)
{
  String result = ss_alloc_str(s.len);

  for (size_t i = 0; i < result.len; i++)
  {
    result.str[i] = s.str[i];
  }

  result.str[result.len] = '\0';

  return result;
}

// Replaces each uppercase character in `s` with a lowercase character
String str_to_lower(String *s)
{
  for (size_t i = 0; i < s->len; i++)
  {
    if (s->str[i] >= 'A' && s->str[i] <= 'Z')
    {
      s->str[i] += 32;
    }
  }

  return *s;
}

// Replaces each lowercase character in `s` with an uppercase character
String str_to_upper(String *s)
{
  for (size_t i = 0; i < s->len; i++)
  {
    if (s->str[i] >= 'a' && s->str[i] <= 'z')
    {
      s->str[i] -= 32;
    }
  }

  return *s;
}

// Returns a new string consisting of each string in `arr`. `delimiter` added between each string
String str_join(StringArray arr, String delimiter)
{
  String result = {0};

  size_t total_len = (arr.count-1) * delimiter.len;
  for (size_t i = 0; i < arr.count; i++)
  {
    total_len += arr.e[i].len;
  }

  result = ss_alloc_str(total_len);

  size_t start_offset = 0;
  for (size_t i = 0; i < arr.count; i++)
  {
    result = str_insert_at(result, arr.e[i], start_offset);
    start_offset += arr.e[i].len;

    if (i != arr.count-1)
    {
      result = str_insert_at(result, delimiter, start_offset);
      start_offset += delimiter.len;
    }
  }

  return result;
}

// Prints string and a new line
void print_str(String s)
{
  for (size_t i = 0; i < s.len; i++)
  {
    printf("%c", s.str[i]);
  }

  printf("\n");
}

// @StringArray ================================================================================

// Intializes and returns a new `StringArray` of size `count`
StringArray create_str_array(size_t count)
{
  StringArray arr = {0};
  arr.count = count;
  arr.e = malloc(sizeof (String) * count);

  return arr;
}

// Clears `arr` and resets count to zero
void clear_str_array(StringArray *arr)
{
  for (size_t i = 0; i < arr->count; i++)
  {
    arr->e[i] = str("");
  }

  arr->count = 0;
  free(arr->e);
}

#endif
