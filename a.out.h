#ifndef _AOUT_H
#define _AOUT_H

struct exec {
	unsigned char a_magic[2];
	unsigned char a_flags;
	unsigned char a_cpu;
	unsigned char a_hdrlen;
	unsigned char a_unused;
	unsigned short a_version;
	int		a_text;
	int 		a_data;
	int 		a_bss;
	int		a_entry;
	int 		a_total;
	int		a_syms;

	//int 		a_trsize;
	//int 		a_drsize;
	//int 		a_tbase;
	//int		a_dbase;
};


#endif
