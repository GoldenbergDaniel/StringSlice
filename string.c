#include <stdio.h>

#include "common.h"
#include "arena.h"
#include "string.h"

// @String =====================================================================================

String alloc_str(u32 len, Arena *arena)
{
  String result;
  result.str = arena_alloc(arena, len);
  result.len = len;

  return result;
}

inline
String str_lit(i8 *s)
{
  return (String) {s, cstr_len(s)-1};
}

String str_copy(String s, Arena *arena)
{
  String dest = {0};
  dest.str = arena_alloc(arena, s.len);
  dest.len = s.len;

  for (u32 i = 0; i < s.len; i++)
  {
    dest.str[i] = s.str[i];
  }
  
  dest.len = s.len;

  return dest;
}


String str_copy_to(String src, String *dest)
{
  for (u32 i = 0; i < src.len; i++)
  {
    dest->str[i] = src.str[i];
  }
  
  dest->len = src.len;

  return *dest;
}

String str_insert_at(String s, String substr, u32 loc)
{
  ASSERT(loc + substr.len <= s.len);

  u32 dest_idx = loc;
  for (u32 i = 0; i < substr.len; i++)
  {
    s.str[dest_idx] = substr.str[i];
    dest_idx++;
  }

  return s;
}

String str_concat(String s1, String s2, Arena *arena)
{
  String result = alloc_str(s1.len + s2.len, arena);

  for (u32 i = 0; i < s1.len; i++)
  {
    result.str[i] = s1.str[i];
  }

  for (u32 i = 0; i < s2.len; i++)
  {
    result.str[i+s1.len] = s2.str[i];
  }

  return result;
}

String str_substr(String s, u32 start, u32 end, Arena *arena)
{
  ASSERT(start >= 0 && start < s.len && end > 0 && end <= s.len && start < end);

  String result = alloc_str(end-start, arena);

  u32 result_idx = 0;
  for (u32 i = start; i < end; i++)
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

  u32 front_len = substr.len;
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

  u32 end_len = s.len - substr.len;
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

  for (u32 i = 0; i < result.len; i++)
  {
    result.str[i] = s.str[i];
  }

  result.str[result.len] = '\0';

  return result;
}

String str_join(StringArray arr, String delimiter, Arena *arena)
{
  String result = {0};

  u32 total_len = (arr.count-1) * delimiter.len;
  for (u32 i = 0; i < arr.count; i++)
  {
    total_len += arr.e[i].len;
  }

  result = alloc_str(total_len, arena);

  u32 start_offset = 0;
  for (u32 i = 0; i < arr.count; i++)
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

StringArray str_split(String s, String delimiter, Arena *arena)
{
  return (StringArray) {0};
}

bool str_equals(String s1, String s2)
{
  if (s1.len != s2.len) return FALSE;

  bool equals = TRUE;

  for (u32 i = 0; i < s1.len; i++)
  {
    if (s1.str[i] != s2.str[i])
    {
      equals = FALSE;
      break;
    }
  }

  return equals;
}

bool str_contains(String s, String substr)
{
  if (s.len < substr.len) return FALSE;

  bool result = FALSE;

  for (u32 i = 0; i < s.len-substr.len+1; i++)
  {
    if (s.str[i] == substr.str[0])
    {
      for (u32 j = 0; j < substr.len; j++)
      {
        if (s.str[i+j] != substr.str[j]) break;

        if (j == substr.len-1)
        {
          result = TRUE;
          goto exit;
        }
      }
    }
  }

  exit:
  return result;
}

i64 str_find(String s, String substr)
{
  if (s.len < substr.len) return -1;

  i64 loc = -1;

  for (u32 i = 0; i < s.len-substr.len+1; i++)
  {
    if (s.str[i] == substr.str[0])
    {
      for (u32 j = 1; j < substr.len; j++)
      {
        if (s.str[i+j] != substr.str[j]) break;

        if (j == substr.len-1)
        {
          loc = i;
          goto end;
        }
      }
    }
  }

  end:
  return loc;
}

i64 str_find_char(String s, i8 c)
{
  i64 loc = -1;

  for (u32 i = 0; i < s.len; i++)
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
  for (u32 i = 0; i < s.len; i++)
  {
    printf("%c", s.str[i]);
  }

  printf("\n");
}

// @StringArray ================================================================================

StringArray create_str_array(u64 count, Arena *arena)
{
  StringArray arr = {0};
  arr.count = count;
  arr.e = arena_alloc(arena, sizeof (String) * count);

  return arr;
}

void clear_str_array(StringArray *arr, Arena *arena)
{
  for (u64 i = 0; i < arr->count; i++)
  {
    arr->e[i] = (String) {0};
  }

  arr->count = 0;
}

// @CString ====================================================================================

u32 cstr_len(i8 *s)
{
  u32 len = 0;
  for (; s[len]; len++);
  return len+1;
}

void cstr_copy(String *dest, i8 *src)
{
  u32 len = cstr_len(src)-1;
  for (u32 i = 0; i < len; i++)
  {
    dest->str[i] = src[i];
  }
  
  dest->len = len;
}
