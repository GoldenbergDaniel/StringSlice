# StringSlice
Simple length-based string library for C. Ditch null termination for good. Note that the library has not been extensively tested and is still a work in progress. Use with caution.

**Note:** StringSlice depends on my arena allocator library. Arenas offer a fast and hassle-free way to manage memory, especially useful for string operations which often require frequent allocations. The library is included as a git submodule. To learn more about arenas, see this awesome article by Ryan Fleury: www.rfleury.com/p/untangling-lifetimes-the-arena-allocator

If you do not want to use arenas, there is an option to use malloc-based allocations. Just make sure to select stringslice_malloc.h instead of stringslice_arena.h.

# Usage example
```c
#include <stdio.h>
#include "stringslice.h"

int main(void)
{
  Arena arena = create_arena(MiB(1));

  String string = str("Hello, ");
  string = str_concat(string, str("world"), &arena);
  string = str_to_upper(&string, &arena);
  long location = str_find(string, str("LLO"));
  printf("%li\n", location);

  StringArray string_array = str_create_array(3, &arena);
  string_array.e[0] = str("string");
  string_array.e[1] = str("slice"); 
  string_array.e[2] = str("library");

  String output = str_join(string_array, str(", "), &arena);
  output = str_nullify(output, &arena);
  printf("%s\n", output.str);

  return 0;
}
```

# Memory Management
There are two options for memory management: arena-based and malloc-based. The arena-based header is easy to work with. When a function needs to allocate memory, it takes an arena as an argument. See the usage sample for an example. Note that for the malloc-based header, (NOT IMPLEMENTED)

# Documentation
`str(s) ((String) {s, cstr_len(s)-1})`

- Creates a string given a cstring literal

`u64 cstr_len(i8 *s)`

- Returns the length of a null-terminated string

`String alloc_str(u64 len, Arena *arena)`

- Allocates `len` bytes of memory to `str` field of `String` and returns new string

`String str_from_cstring(char *cstr)`

- Returns a copy of the `cstr` as a `String`

`bool str_equals(String s1, String s2)`

- Returns true if the characters in `s1` match `s2`

`bool str_contains(String s, String substr)`

- Returns true if `s` contains `substr`

`i64 str_find(String s, String substr, u64 start)`

Returns the index of the first instance of `substr` in `s` starting at index `start`

`i64 str_find_char(String s, i8 c, u64 start)`

- Returns the index of the first instance of `c` in `s` starting at index `start`

`String str_copy(String s, Arena *arena)`

- Allocates and returns a copy of `s`

`String str_copy_into(String *dest, String src)`

- Copies `src` into `dest.` Expects `src.len <= dest.len`

`String str_insert_at(String s, String substr, u64 loc)`

- Inserts `substr` into `s` at index `loc.` Expects `loc + substr.len <= s.len`

`String str_concat(String s1, String s2, Arena *arena)`

- Returns a new string combining `s1` and `s2` such that the characters of `s2` follow `s1`

`String str_substr(String s, u64 start, u64 end, Arena *arena)`

- Returns a substring of `s` starting at index `start` and ending before index `end`

`String str_strip_front(String s, String substr, Arena *arena)`

- Returns a new string with the first `substr.len` characters of `s` removed if they equal `substr`

`String str_strip_back(String s, String substr, Arena *arena)`

- Returns a new string with the last `substr.len` characters of `s` removed if they equal `substr`

`String str_nullify(String s, Arena *arena)`

- Returns a new string with a null terminator appended to the end

`String str_to_lower(String s, Arena *arena)`

- Returns a new string with each uppercase character in `s` a lowercase character

`String str_to_upper(String s, Arena *arena)`

- Returns a new string with each lowercase character in `s` a uppercase character

`String str_join(StringArray arr, String delimiter, Arena *arena)`

- Returns a new string consisting of each string in `arr`. `delimiter` added between each string

`StringArray str_split(String s, String delimiter, Arena *arena)`

- Splits `s` into substrings separated by `delimiter` and returns a `StringArray` (NOT IMPLEMENTED)

`void print_str(String s)`

- Prints string and a new line

`StringArray create_str_array(size_t count, Arena *arena)`

- Intializes and returns a new `StringArray` of size `count`

`void clear_str_array(StringArray *arr, Arena *arena)`

- Clears `arr` and resets count to zero
