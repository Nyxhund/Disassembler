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
#include "errno.h"

void syscall()
{
	message* m = (message *) (mem + *getRegister16(0x03));
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

		case 54:
			fprintf(stderr, "<ioctl(%d, 0x%04x, 0x%04x)>\n", *((uint16_t*)(mem + *getRegister16(0x03) + 4)), *((uint16_t*)(mem + *getRegister16(0x03) + 8)), *((uint16_t*)(mem + *getRegister16(0x03) + 18)));
			errno = EINVAL;
			setRegister16(0x00, 0x00);
			m->m_type = -errno;
			break;

		case 17:
			setRegister16(0x00, 0x00);

			uint16_t addr = *((uint16_t*)(mem + *getRegister16(0x03) + 10));
			fprintf(stderr, "<brk(0x%04x) => ", addr);

			if (addr >= ((*getRegister16(0x04) & ~0x3ff) - 0x400))
			{
				errno = ENOMEM;
				if (interpret) fprintf(stderr, "ENOMEM>\n");
				m->m_type = -errno;
			}
			else {
				uint16_t brksize = addr;
				if (interpret) fprintf(stderr, "0>\n");
				m->m_type = 0;
				*((uint16_t*)(mem + *getRegister16(0x03) + 18)) = brksize;
			}
			break;
	}
}

