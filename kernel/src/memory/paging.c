#include "paging.h"
#include "frame.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

extern void enable_paging(page_entry_t* page_directory);

static page_entry_t* page_directory = NULL;

static page_entry_t* init_page_table() {
	page_entry_t* page_table = (page_entry_t*)alloc_frame();
	if ((page_frame_t)page_directory == FRAME_ALLOC_ERROR)
		return NULL; // TODO: Handle this error

	for (uint32_t i = 0; i < PAGE_TABLE_ENTRIES; i++) {
		page_table[i].present = 1;
		page_table[i].rw = 1;
		page_table[i].user = 0;
		page_table[i].ps = 0;
		page_table[i].frame_addr = i;
	}

	return page_table;
}

void paging_setup() {
	page_directory = (page_entry_t*)alloc_frame();
	if ((page_frame_t)page_directory == FRAME_ALLOC_ERROR)
		return; // TODO: Handle this error

	page_entry_t* first_page_table = init_page_table();

	for (size_t i = 0; i < PAGE_DIR_ENTRIES; i++) {
		page_directory[i].rw = 1;
	}

	page_directory[0].present = 1;
  page_directory[0].rw = 1;
	page_directory[0].user = 0;
	page_directory[0].ps = 0;
	page_directory[0].frame_addr = ((uint32_t)first_page_table) >> 12;

	enable_paging(page_directory);
}
