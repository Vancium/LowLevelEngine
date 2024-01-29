#include "lle_memory.h"

#include "core/Log/log.h"

#include "platform/platform.h"

#include <string.h>
#include <stdio.h>

struct memory_data {
    u64 total_allocation;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static struct memory_data data;

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      "
};


void initialize_memory() {
    platform_zero_memory(&data, sizeof(data));
}

void destroy_memory() {

}

void* lle_allocate(u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        LLE_WARN("lle_allocate called using MEMORY_TAG_UNKNOWN. Re-Class this memory allocation");
    }

    data.total_allocation += size;
    data.tagged_allocations[tag] += size;
    
    // TODO revisit memory alignment
    // Memory returned from this function will atomatically be zerod
    void* block = platform_allocate(size, FALSE);
    platform_zero_memory(block, size);

    return block;
}

void lle_free(void* block, u64 size, memory_tag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        LLE_WARN("lle_free called using MEMORY_TAG_UNKNOWN. Re-Class this memory deallocation");
    }
    data.total_allocation -= size;
    data.tagged_allocations[tag] -= size;

    // TODO memory algnment
    platform_free(block, FALSE);
}

void* lle_zero_memory(void* block, u64 size) {
    platform_zero_memory(block, size);
}

void* lle_copy_memory(void* dest, const void* src, u64 size) {
    platform_copy_memory(dest, src, size);
}

void* lle_set_memory(void* dest, i32 value, u64 size) {
    platform_set_memory(dest, value, size);
}

char* get_memory_usage_str() {
    // uses base 2 instead of base 10 to more technically represent memory
    const u64 gibebyte = 1024 * 1024 * 1024;  
    const u64 mibebyta = 1024 * 1024;
    const u64 kibebyta = 1024;

    char buffer[8000] = "System memory use (tagged):\n";

    u64 offset = strlen(buffer);

    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        char unit[4] = "XiB";
        float amount = 1.0f;

        if (data.tagged_allocations[i] >= gibebyte) {
            unit[0] = 'G';
            amount = data.tagged_allocations[i] / (float)gibebyte;
        } else if (data.tagged_allocations[i] >= mibebyta) {
            unit[0] = 'M';
            amount = data.tagged_allocations[i] / (float)mibebyta;
        } else if (data.tagged_allocations[i] >= kibebyta) {
            unit[0] = 'K';
            amount = data.tagged_allocations[i] / (float)kibebyta;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)data.tagged_allocations[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;

    }

    char* out_string = strdup(buffer);
    return out_string;



}
