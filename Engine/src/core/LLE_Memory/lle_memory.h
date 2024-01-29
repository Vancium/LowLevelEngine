#pragma once

#include "defines.h"

// Used to systematically catagorize memory allocations
typedef enum memory_tag {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_MAX_TAGS
} memory_tag;

LLAPI void initialize_memory();
LLAPI void destroy_memory();

LLAPI void* lle_allocate(u64 size, memory_tag tag);

LLAPI void lle_free(void* block, u64 size, memory_tag tag);

LLAPI void* lle_zero_memory(void* block, u64 size);

LLAPI void* lle_copy_memory(void* dest, const void* src, u64 size);

LLAPI void* lle_set_memory(void* dest, i32 value, u64 size);

LLAPI char* get_memory_usage_str();
