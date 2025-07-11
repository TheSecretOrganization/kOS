#pragma once

#include <stdint.h>

#define PAGE_DIR_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024
#define PAGE_ENTRIES_PER_TABLE 1024
#define PAGE_SIZE 0x1000u

/**
 * This structure is used to define entries in both the page directory and page
 * table. The interpretation of the fields depends on the context:
 * - **For PDEs**:
 *   - `frame_addr`: Points to a page table (4 KiB aligned).
 *   - `ps`: Set to 0, indicating the entry maps to a page table.
 * - **For PTEs**:
 *   - `frame_addr`: Points to a 4 KiB page frame (4 KiB aligned).
 *   - `ps`: Represents the Page Attribute Table.
 *
 */
typedef struct {
	uint32_t present : 1;
	uint32_t rw : 1;
	uint32_t user : 1;
	uint32_t pwt : 1;
	uint32_t pcd : 1;
	uint32_t accessed : 1;
	uint32_t dirty : 1;
	uint32_t ps : 1;
	uint32_t global : 1;
	uint32_t availabe : 3;
	uint32_t frame_addr : 20;
} __attribute__((packed)) page_entry_t;

void paging_setup();
