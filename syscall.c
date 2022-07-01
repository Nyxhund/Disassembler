#include "stdint.h"
#include "stdlib.h"
#include "inttypes.h"
#include "unistd.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "fcntl.h"
#include "a.out.h"
#include "stdio.h"
#include "utility.h"
#include "syscall.h"

void syscall()
{
	message* m = (mem + *getRegister16(0x03));
	switch (m->m_type)
	{
		case 4:
			fprintf(stderr, "<write(%x, 0x%04x, %i)", m->m1_i1, m->m1_p1, m->m1_i2);
			ssize_t written = write(m->m1_i1, &mem[m->m1_p1], m->m1_i2);
			fprintf(stderr, " => %li>\n", written);
			setRegister16(0, 0x00);
			m->m_type = written;
			break;
		
		case 1:
			fprintf(stderr, "<exit(%d)>\n", m->m1_i1);
			exit(m->m1_i1);
			break;
	}
}
