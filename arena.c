#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "arena.h"

#ifndef SCRATCH_SIZE
#define SCRATCH_SIZE MiB(16)
#endif

// @Arena ======================================================================================

Arena create_arena(u64 size)
{
  Arena arena = {0};
  arena.memory = malloc(size);
  arena.size = size;

  return arena;
}

void destroy_arena(Arena *arena)
{
  free(arena->memory);
  arena = (Arena *) {0};
}

void *arena_alloc(Arena *arena, u64 size)
{
  ASSERT(arena->size >= arena->used + size);

  byte *allocated = arena->memory + arena->used;
  arena->used += size;
  
  return allocated;
}

void arena_free(Arena *arena, u64 size)
{
  ASSERT(arena->used - size >= 0);

  arena->used -= size;
}

void clear_arena(Arena *arena)
{
  arena->used = 0;
}

Arena get_scratch_arena(Arena *conflict)
{
  static thread_local Arena scratch_1;
  static thread_local Arena scratch_2;
  static thread_local bool init = TRUE;

  if (init)
  {
    scratch_1 = create_arena(SCRATCH_SIZE);
    scratch_2 = create_arena(SCRATCH_SIZE);
    init = FALSE;
  }

  Arena scratch = scratch_1;
  
  if (conflict == &scratch_1)
  {
    scratch = scratch_2;
  }
  else if (conflict == &scratch_2)
  {
    scratch = scratch_1;
  }

  return scratch;
}
