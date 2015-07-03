.code32

.global outb
.global inw
.global inb
.global break_point

# void outb(u16int port, u8int value)
outb:
	movw 4(%esp), %dx
	movb 8(%esp), %al
	outb %al, %dx
	ret

# u8int inb(u16int port)
inb:
	movw 4(%esp), %dx
	inb %dx, %al
	ret

# u16int inw(u16int port)
inw:
	movw 4(%esp), %dx
	inw %dx, %ax
	ret

# void break_point()
break_point:
    ret
