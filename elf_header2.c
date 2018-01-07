#include <elf.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define P(s, ...) ({printf(s , ##__VA_ARGS__);puts("");})

typedef struct {
	Elf64_Half e;
	unsigned char *c;
}type;

static type types[] = {
	{ET_NONE, "Tanimsiz"},
	{ET_REL, "Yeniden Konumlanabilir"},
	{ET_EXEC, "Calistirilabilir"},
	{ET_DYN, "Paylasimli"},
	{ET_CORE, "Core Dosyasi"},
//	{ET_NUM, "Isletim Sistemi Tanimli"},
	{ET_LOOS, "Isletim Sistemi Tanimli Baslangic Adresi"},	//isletim sistemi dosyalari araligi
	{ET_HIOS, "Isletim Sistemi Tanimli Bitis Adresi"},
	{ET_LOPROC, "Islemci Tanimli Baslangic Adresi"},	//islemci dosyalarinin adresi
	{ET_HIPROC, "Isletim Tanimli Bitis Adresi"}
};

// asagidaki kod blogu istenirse basit for dongusuyle'de yazilabilirdi anlasilir
// ve ogretici olmasi icin switch blogu kullanilmistir
void
file_type(Elf64_Half e_type) {
	switch(e_type) {
		case ET_NONE:	P(types[0].c); break;
		case ET_REL:	P(types[1].c); break;
		case ET_EXEC:	P(types[2].c); break;
		case ET_DYN:	P(types[3].c); break;
		case ET_CORE:	P(types[4].c); break;
//		case ET_NUM:	P(types[5].c); break;
		case ET_LOOS:	P(types[5].c); break;
		case ET_HIOS:	P(types[6].c); break;
		case ET_LOPROC: P(types[7].c); break;
		case ET_HIPROC:	P(types[8].c); break;
	};
}

////////////////////e_ident tanimlayicisi 16 bayttir/////////////
typedef struct {
	int i;
	unsigned char c;
	unsigned char *s;
}magnum;

// ilk dort bayt icin veri tipleri 
// bunlar elf dosyasinin baslangic bitleri, taninmasi icin gereklidir
magnum magic_number[] = {
	{EI_MAG0, ELFMAG0, NULL},
	{EI_MAG1, ELFMAG1, NULL},
	{EI_MAG2, ELFMAG2, NULL},
	{EI_MAG3, ELFMAG3, NULL}
};
// lan duduk makarnasi struct yapisini niye kullanmadin
void
elf(unsigned char e[]) {
	if(e[EI_MAG0] == ELFMAG0)
		P("0x7F");

	if(e[EI_MAG1] == ELFMAG1)
		P("E");

	if(e[EI_MAG2] == ELFMAG2)
		P("L");

	if(e[EI_MAG3] == ELFMAG3)
		P("F");
}
///////////////////////////////////////////////////////////

// e_ident 5. bayt sinif tipi 
magnum class[] = {
	{EI_CLASS, ELFCLASSNONE, "Tanimsiz Sinif"},
	{EI_CLASS, ELFCLASS32, "32 bitlik dosya"},
	{EI_CLASS, ELFCLASS64, "64 bitlik dosya"},
	{EI_CLASS, ELFCLASSNUM, "Bilinmeyen sinif"}
};

void
class_control(unsigned char e[]) {
	switch(e[EI_CLASS]) {
		case ELFCLASSNONE:	P(class[0].s); break;
		case ELFCLASS32:	P(class[1].s); break;
		case ELFCLASS64:	P(class[2].s); break;
		case ELFCLASSNUM:	P(class[3].s); break;
	};
}
////////////////////////////////////////////////////////////

// e_ident 6. bayt data tipi
magnum data[] = {
	{EI_DATA, ELFDATANONE, "Hatali data"},
	{EI_DATA, ELFDATA2LSB, "Little endian"},
	{EI_DATA, ELFDATA2MSB, "Big endian"},
	{EI_DATA, ELFDATANUM, "Bilinmeyen data"}
};


void
data_control(unsigned char e[]) {
	switch(e[EI_DATA]) {
		case ELFDATANONE:	P(data[0].s); break;
		case ELFDATA2LSB:	P(data[1].s); break;
		case ELFDATA2MSB:	P(data[2].s); break;
		case ELFDATANUM:	P(data[3].s); break;
	};
}
//////////////////////////////////////////////////////////////

// e_ident 7. bayt versiyon
magnum version[] = {
	{EI_VERSION, 0, NULL} // 0 yerine (unsigned char) NULL yazilabilir ama uyari verir
};

void
version_control(unsigned char e[]) {
	P("Versiyon numarasi : %i", e[EI_VERSION]);
}
//////////////////////////////////////////////////////////////

// e_ident 8. bayt sistem turu
magnum os [] = {
	{EI_OSABI, ELFOSABI_NONE, "UNIX sistem"},
	{EI_OSABI, ELFOSABI_SYSV, "UNIX sistem"},
	{EI_OSABI, ELFOSABI_HPUX, "HP-UX"},
	{EI_OSABI, ELFOSABI_NETBSD, "NET-BSD"},
	{EI_OSABI, ELFOSABI_GNU, "GNU"},
	{EI_OSABI, ELFOSABI_LINUX, "GNU"},
	{EI_OSABI, ELFOSABI_SOLARIS, "SOLARIS"},
	{EI_OSABI, ELFOSABI_AIX, "IBM AIX"},
	{EI_OSABI, ELFOSABI_IRIX, "IRX"},
	{EI_OSABI, ELFOSABI_FREEBSD, "FREEBSD"},
	{EI_OSABI, ELFOSABI_TRU64, "TRU UNIX"},
	{EI_OSABI, ELFOSABI_MODESTO, "Novell Modesto"},
	{EI_OSABI, ELFOSABI_OPENBSD, "OPENBSD"},
	{EI_OSABI, ELFOSABI_ARM_AEABI, "ARM EABI"},
	{EI_OSABI, ELFOSABI_ARM, "ARM"},
	{EI_OSABI, ELFOSABI_STANDALONE, "Bagimsiz (embedded)"}
};

// e_ident 9. bayt abiversiyon 
magnum abiver [] = {
	{EI_ABIVERSION, 0, NULL}
};

magnum pad [] = {
	{EI_PAD, 0, NULL}
};

int
main() {
	FILE *f;
	Elf64_Ehdr *header;

	f = fopen("a.out", "r");

	if(f == NULL) {
		perror("");
		exit(errno);
	}

	header = (Elf64_Ehdr *) calloc(sizeof(Elf64_Ehdr), 1);

	if(header == NULL) {
		perror("");
		exit(errno);
	}

	fgets((char *)header, sizeof(Elf64_Ehdr), f);

	file_type(header->e_type);

	elf(header->e_ident);

	class_control(header->e_ident);

	data_control(header->e_ident);

	version_control(header->e_ident);

}


