#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

typedef struct {
	u32int present	: 1;
	u32int rw		: 1;
	u32int user		: 1;
	u32int accessed	: 1;
	u32int dirty	: 1;
	u32int unused	: 7;
	u32int frame	: 20;
} page_t;

typedef struct {
	page_t pages[1024];
} page_table_t;

typedef struct {
	page_table_t *tables[1024];
	u32int tablesPhysical[1024];
	u32int physicalAddr;
} page_directory_t;

page_directory_t *kernel_directory = 0;
page_directory_t *current_directory = 0;

u32int *frames;
u32int nframes;

#define INDEX_FROM_BIT(a) (a/32)
#define OFFSET_FROM_BIT(a) (a%32)

void init_paging();
void switch_page_directory(u32int pageDirPhysAddr);
page_t *get_page(u32int address, int make, page_directory_t *dir);
void page_fault(registers_t regs);

#endif
