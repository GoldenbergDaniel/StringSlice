#pragma once

#include "common.h"

#define KiB(bytes) ((u64) bytes << 10)
#define MiB(bytes) ((u64) bytes << 20)
#define GiB(bytes) ((u64) bytes << 30)

typedef struct Arena Arena;

// @Arena ======================================================================================

struct Arena
{
  byte *memory;
  u64 size;
  u64 used;
};

Arena create_arena(u64 size);
void destroy_arena(Arena *arena);
void *arena_alloc(Arena *arena, u64 size);
void arena_free(Arena *arena, u64 size);
void clear_arena(Arena *arena);
Arena get_scratch_arena(Arena *conflict);
