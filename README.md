# StringSlice
A length-based string library for C. This is a work-in-progress project and has not been rigorously tested.

# Usage example
```c
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
