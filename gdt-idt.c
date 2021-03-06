#include "gdt-idt.h"

gdt_entry_t gdt_entries[6];
gdt_ptr_t gdt_ptr;

idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;

tss_entry_t tss_entry;

extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);
extern void isr128(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);

extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

static void gdt_set(u32int idx, u32int base, u32int limit, u8int access, u8int gran)
{
	gdt_entries[idx].base_low	 = (base & 0xFFFF);
	gdt_entries[idx].base_middle = (base >> 16) & 0xFF;
	gdt_entries[idx].base_high	 = (base >> 24) & 0xFF;

	gdt_entries[idx].limit_low	 = (limit & 0xFFFF);
	gdt_entries[idx].granularity = (limit >> 16) & 0x0F;

	gdt_entries[idx].granularity |= gran & 0xF0;
	gdt_entries[idx].access		 = access;
}

static void write_tss(u32int idx, u32int ss0, u32int esp0)
{
	u32int base = (u32int)&tss_entry;
	u32int limit = base + sizeof(tss_entry_t);

	gdt_set(idx, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0, sizeof(tss_entry_t));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;

	tss_entry.cs = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
}

void set_kernel_stack(u32int stack)
{
	tss_entry.esp0 = stack;
}

void init_gdt()
{
	gdt_ptr.size = (sizeof(gdt_entry_t) * 6) - 1;
	gdt_ptr.base = (u32int)&gdt_entries;

	gdt_set(0, 0, 0, 0, 0);					// Null segment
	gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);	// Code segment
	gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF);	// Data segment
	gdt_set(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);	// User mode code segment
	gdt_set(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);	// User mode data segment
	write_tss(5, 0x10, 0x0);

	gdt_flush((u32int)&gdt_ptr);
	tss_flush();
}

static void idt_set(u8int idx, u32int base, u16int sel, u8int flags)
{
	idt_entries[idx].base_low  = base & 0xFFFF;
	idt_entries[idx].base_high = (base >> 16) & 0xFFFF;

	idt_entries[idx].sel 	 = sel;
	idt_entries[idx].always0 = 0;

	idt_entries[idx].flags = flags;
}

void init_idt()
{
	idt_ptr.size = sizeof(idt_entry_t) * 256 - 1;
	idt_ptr.base = (u32int)&idt_entries;

	// Master PIC
	outb(0x20, 0x11);
	outb(0x21, 0x20);
	outb(0x21, 0x04);
	outb(0x21, 0x01);
	outb(0x21, 0x00);
	// Slave PIC
	outb(0xA0, 0x11);
	outb(0xA1, 0x28);
	outb(0xA1, 0x02);
	outb(0xA1, 0x01);
	outb(0xA1, 0x00);

	idt_set(0, (u32int)isr0, 0x08, 0x8E);
	idt_set(1, (u32int)isr1, 0x08, 0x8E);
	idt_set(2, (u32int)isr2, 0x08, 0x8E);
	idt_set(3, (u32int)isr3, 0x08, 0x8E);
	idt_set(4, (u32int)isr4, 0x08, 0x8E);
	idt_set(5, (u32int)isr5, 0x08, 0x8E);
	idt_set(6, (u32int)isr6, 0x08, 0x8E);
	idt_set(7, (u32int)isr7, 0x08, 0x8E);
	idt_set(8, (u32int)isr8, 0x08, 0x8E);
	idt_set(9, (u32int)isr9, 0x08, 0x8E);
	idt_set(10, (u32int)isr10, 0x08, 0x8E);
	idt_set(11, (u32int)isr11, 0x08, 0x8E);
	idt_set(12, (u32int)isr12, 0x08, 0x8E);
	idt_set(13, (u32int)isr13, 0x08, 0x8E);
	idt_set(14, (u32int)isr14, 0x08, 0x8E);
	idt_set(15, (u32int)isr15, 0x08, 0x8E);
	idt_set(16, (u32int)isr16, 0x08, 0x8E);
	idt_set(17, (u32int)isr17, 0x08, 0x8E);
	idt_set(18, (u32int)isr18, 0x08, 0x8E);
	idt_set(19, (u32int)isr19, 0x08, 0x8E);
	idt_set(20, (u32int)isr20, 0x08, 0x8E);
	idt_set(21, (u32int)isr21, 0x08, 0x8E);
	idt_set(22, (u32int)isr22, 0x08, 0x8E);
	idt_set(23, (u32int)isr23, 0x08, 0x8E);
	idt_set(24, (u32int)isr24, 0x08, 0x8E);
	idt_set(25, (u32int)isr25, 0x08, 0x8E);
	idt_set(26, (u32int)isr26, 0x08, 0x8E);
	idt_set(27, (u32int)isr27, 0x08, 0x8E);
	idt_set(28, (u32int)isr28, 0x08, 0x8E);
	idt_set(29, (u32int)isr29, 0x08, 0x8E);
	idt_set(30, (u32int)isr30, 0x08, 0x8E);
	idt_set(31, (u32int)isr31, 0x08, 0x8E);

	idt_set(32, (u32int)irq0, 0x08, 0x8E);
	idt_set(33, (u32int)irq1, 0x08, 0x8E);
	idt_set(34, (u32int)irq2, 0x08, 0x8E);
	idt_set(35, (u32int)irq3, 0x08, 0x8E);
	idt_set(36, (u32int)irq4, 0x08, 0x8E);
	idt_set(37, (u32int)irq5, 0x08, 0x8E);
	idt_set(38, (u32int)irq6, 0x08, 0x8E);
	idt_set(39, (u32int)irq7, 0x08, 0x8E);

	idt_set(40, (u32int)irq8, 0x08, 0x8E);
	idt_set(41, (u32int)irq9, 0x08, 0x8E);
	idt_set(42, (u32int)irq10, 0x08, 0x8E);
	idt_set(43, (u32int)irq11, 0x08, 0x8E);
	idt_set(44, (u32int)irq12, 0x08, 0x8E);
	idt_set(45, (u32int)irq13, 0x08, 0x8E);
	idt_set(46, (u32int)irq14, 0x08, 0x8E);
	idt_set(47, (u32int)irq15, 0x08, 0x8E);

	idt_set(128, (u32int)isr128, 0x08, 0xEE);

	idt_flush((u32int)&idt_ptr);
}
