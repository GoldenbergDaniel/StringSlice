#pragma once

#include "common.h"
#include "arena.h"

typedef struct String String;
typedef struct StringArray StringArray;

// @String =====================================================================================

struct String
{
  i8 *str;
  u32 len;
};

#define STR_NIL ((String) {NULL, 0})

String alloc_str(u32 len, Arena *arena);
String str_lit(i8 *s);
String str_copy(String src, Arena *arena);
String str_copy_into(String src, String *dest);
String str_insert_at(String s, String substr, u32 loc);
String str_concat(String s1, String s2, Arena *arena);
String str_substr(String s, u32 start, u32 end, Arena *arena);
String str_strip_front(String s, String substr, Arena *arena);
String str_strip_back(String s, String substr, Arena *arena);
String str_nullify(String s, Arena *arena);
String str_join(StringArray arr, String delimiter, Arena *arena);
StringArray str_split(String s, String delimiter, Arena *arena);
bool str_equals(String s1, String s2);
bool str_contains(String s, String substr);
i64 str_find(String s, String substr);
i64 str_find_char(String s, i8 c);
void print_str(String s);

// @StringArray ================================================================================

struct StringArray
{
  String *e;
  u64 count;
};

StringArray create_str_array(u64 count, Arena *arena);
void clear_str_array(StringArray *arr, Arena *arena);

// @CStr =======================================================================================

u32 cstr_len(i8 *cstr);
void cstr_copy(String *dest, i8 *src);
