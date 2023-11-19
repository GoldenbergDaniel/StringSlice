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

#ifndef STRING_LIB_H
#define STRING_LIB_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "ArenaAllocator/arena_lib.h"

typedef struct String String;
typedef struct StringArray StringArray;

struct String
{
  char *str;
  uint32_t len;
};

struct StringArray
{
  String *e;
  size_t count;
};

#define STR_NIL ((String) {NULL, 0})

bool str_equals(String s1, String s2);

uint32_t cstr_len(char *s)
{
  uint32_t len = 0;
  for (; s[len]; len++);
  return len+1;
}

// @String =====================================================================================

String alloc_str(uint32_t len, Arena *arena)
{
  String result;
  result.str = arena_alloc(arena, len);
  result.len = len;

  return result;
}

inline
String str(char *s)
{
  return (String) {s, cstr_len(s)-1};
}

String str_copy(String s, Arena *arena)
{
  String dest = {0};
  dest.str = arena_alloc(arena, s.len);
  dest.len = s.len;

  for (uint32_t i = 0; i < s.len; i++)
  {
    dest.str[i] = s.str[i];
  }
  
  dest.len = s.len;

  return dest;
}


String str_copy_to(String src, String *dest)
{
  for (uint32_t i = 0; i < src.len; i++)
  {
    dest->str[i] = src.str[i];
  }
  
  dest->len = src.len;

  return *dest;
}

String str_insert_at(String s, String substr, uint32_t loc)
{
  assert(loc + substr.len <= s.len);

  uint32_t dest_idx = loc;
  for (uint32_t i = 0; i < substr.len; i++)
  {
    s.str[dest_idx] = substr.str[i];
    dest_idx++;
  }

  return s;
}

String str_concat(String s1, String s2, Arena *arena)
{
  String result = alloc_str(s1.len + s2.len, arena);

  for (uint32_t i = 0; i < s1.len; i++)
  {
    result.str[i] = s1.str[i];
  }

  for (uint32_t i = 0; i < s2.len; i++)
  {
    result.str[i+s1.len] = s2.str[i];
  }

  return result;
}

String str_substr(String s, uint32_t start, uint32_t end, Arena *arena)
{
  assert(start >= 0 && start < s.len && end > 0 && end <= s.len && start < end);

  String result = alloc_str(end-start, arena);

  uint32_t result_idx = 0;
  for (uint32_t i = start; i < end; i++)
  {
    result.str[result_idx] = s.str[i];
    result_idx++;
  }

  return result;
}

String str_strip_front(String s, String substr, Arena *arena)
{
  String result = s;
  Arena scratch = get_scratch_arena(arena);

  uint32_t front_len = substr.len;
  String front = str_substr(s, 0, front_len, &scratch);
  if (str_equals(front, substr))
  {
    result = str_substr(s, front_len, s.len, arena);
    result.len = s.len-front_len;
  }

  clear_arena(&scratch);
  
  return result;
}

String str_strip_back(String s, String substr, Arena *arena)
{
  String result = s;
  Arena scratch = get_scratch_arena(arena);

  uint32_t end_len = s.len - substr.len;
  String end = str_substr(s, end_len, s.len, &scratch);
  if (str_equals(end, substr))
  {
    result = str_substr(s, 0, end_len, arena);
    result.len = end_len;
  }

  clear_arena(&scratch);
  
  return result;
}

String str_nullify(String s, Arena *arena)
{
  String result = alloc_str(s.len, arena);

  for (uint32_t i = 0; i < result.len; i++)
  {
    result.str[i] = s.str[i];
  }

  result.str[result.len] = '\0';

  return result;
}

String str_join(StringArray arr, String delimiter, Arena *arena)
{
  String result = {0};

  uint32_t total_len = (arr.count-1) * delimiter.len;
  for (uint32_t i = 0; i < arr.count; i++)
  {
    total_len += arr.e[i].len;
  }

  result = alloc_str(total_len, arena);

  uint32_t start_offset = 0;
  for (uint32_t i = 0; i < arr.count; i++)
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

bool str_equals(String s1, String s2)
{
  if (s1.len != s2.len) return false;

  bool equals = true;

  for (uint32_t i = 0; i < s1.len; i++)
  {
    if (s1.str[i] != s2.str[i])
    {
      equals = false;
      break;
    }
  }

  return equals;
}

bool str_contains(String s, String substr)
{
  if (s.len < substr.len) return false;

  bool result = false;

  for (uint32_t i = 0; i < s.len-substr.len+1; i++)
  {
    if (s.str[i] == substr.str[0])
    {
      for (uint32_t j = 0; j < substr.len; j++)
      {
        if (s.str[i+j] != substr.str[j]) break;

        if (j == substr.len-1)
        {
          result = true;
          return result;
        }
      }
    }
  }

  return result;
}

int64_t str_find(String s, String substr)
{
  if (s.len < substr.len) return -1;

  int64_t result = -1;

  for (uint32_t i = 0; i < s.len-substr.len+1; i++)
  {
    if (s.str[i] == substr.str[0])
    {
      for (uint32_t j = 1; j < substr.len; j++)
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

int64_t str_find_char(String s, char c)
{
  int64_t loc = -1;

  for (uint32_t i = 0; i < s.len; i++)
  {
    if (s.str[i] == c)
    {
      loc = i;
      break;
    }
  }

  return loc;
}

void print_str(String s)
{
  for (uint32_t i = 0; i < s.len; i++)
  {
    printf("%c", s.str[i]);
  }

  printf("\n");
}

// @StringArray ================================================================================

StringArray create_str_array(size_t count, Arena *arena)
{
  StringArray arr = {0};
  arr.count = count;
  arr.e = arena_alloc(arena, sizeof (String) * count);

  return arr;
}

void clear_str_array(StringArray *arr, Arena *arena)
{
  for (size_t i = 0; i < arr->count; i++)
  {
    arr->e[i] = (String) {0};
  }

  arr->count = 0;
}

#endif
