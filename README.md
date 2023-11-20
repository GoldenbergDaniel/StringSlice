# StringSlice
A length-based string library for C. This is a work-in-progress project and has not been rigorously tested.

# Usage example
```c
#include <stdio.h>
#include "string_lib.h"

int main(void)
{
  Arena arena = create_arena(MiB(1));

  String string = str("Hello, ");
  string = str_concat(string, str("world"), &arena);
  string = str_to_upper(&string);
  long location = str_find(string, str("LLO"));
  printf("%li\n", location);

  StringArray string_array = create_str_array(3, &arena);
  string_array.e[0] = str("string");
  string_array.e[1] = str("slice");
  string_array.e[2] = str("library");

  String output = str_join(string_array, str(", "), &arena);
  output = str_nullify(output, &arena);
  printf("%s\n", output.str);

  return 0;
}
```

# Documentation
### `str(s) ((String) {s, cstr_len(s)-1})`

Creates a string given a cstring literal

### `uint32_t cstr_len(char *s)`

Returns the length of a null-terminated string

### `String alloc_str(uint32_t len, Arena *arena)`

Allocates `len` bytes of memory to `str` field of `String` and returns new string

### `bool str_equals(String s1, String s2)`

Returns true if the characters in `s1` match `s2`

### `bool str_contains(String s, String substr)`

Returns true is `s` contains `substr`

### `int64_t str_find(String s, String substr)`

Returns the index of the first instance of `substr` in `s`

### `int64_t str_find_char(String s, char c)`

Returns the index of the first instance of `c` in `s`

### `String str_copy(String s, Arena *arena)`

Allocates and returns a copy of `s`

### `String str_copy_to(String src, String *dest)`

Copies `src` into `dest.` Expects `src.len <= dest.len`

### `String str_insert_at(String s, String substr, uint32_t loc)`

Inserts `substr` into `s` starting at index `loc.` Expects `loc + substr.len <= s.len`

### `String str_concat(String s1, String s2, Arena *arena)`

Returns a new string combining `s1` and `s2` such that the characters of `s2` follow `s1`

### `String str_substr(String s, uint32_t start, uint32_t end, Arena *arena)`

Returns a substring of `s` starting at and including index `start` to ending at and excluding index `end.`

### `String str_strip_front(String s, String substr, Arena *arena)`

Returns a new string with the first `substr.len` characters of `s` removed if they equal `substr`

### `String str_strip_back(String s, String substr, Arena *arena)``

Returns a new string with the last `substr.len` characters of `s` removed if they equal `substr`

### `String str_nullify(String s, Arena *arena)`

Returns a new string with a null terminator appended to the end

### `String str_join(StringArray arr, String delimiter, Arena *arena)`

Returns a new string consisting of each string in `arr`. `delimiter` added between each string

### `void print_str(String s)`

Prints string and a new line

### `StringArray create_str_array(size_t count, Arena *arena)`

Intializes and returns a new `StringArray` of size `count`

### `void clear_str_array(StringArray *arr, Arena *arena)`

Clears `arr` and resets count to zero
