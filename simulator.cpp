#include <functional>
#include <elf.h>
#include <gelf.h>
#include "../memory.h"
#include <err.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "simulator.h"

void Simulator::load(const char* name)
{
    int fd = 0;
    Elf *e;
    size_t n;
    GElf_Phdr phdr;
    GElf_Shdr shdr;
    FILE* elf = fopen(name, "rb");

    if (elf_version(EV_CURRENT) == EV_NONE)
        errx(EX_SOFTWARE, " ELF library initialization failed: %s ", elf_errmsg(-1));

    if ((fd = open(name, O_RDONLY, 0)) < 0)
        err(EX_NOINPUT, " open \"%s \" failed ", "tmp");

    if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
        errx(EX_SOFTWARE, " elf_begin () failed: %s . ", elf_errmsg(-1));

    if (elf_kind(e) != ELF_K_ELF)
        errx(EX_DATAERR, " \"%s \" is not an ELF object . ", "tmp");

    if (elf_getphdrnum(e, &n) != 0)
        errx(EX_DATAERR, " elf_getphdrnum() failed: %s. ", elf_errmsg(-1));

    for (int k = 0; k < n; k++)
    {
        if (gelf_getphdr(e, k, &phdr) != &phdr)
            errx(EX_SOFTWARE, " getphdr() failed: %s . ",
                 elf_errmsg(-1));
        
        if (phdr.p_type == PT_LOAD)
        {
            fseek(elf, phdr.p_offset, SEEK_SET);
            uint8_t* buf = (uint8_t*)calloc(phdr.p_memsz, sizeof(uint8_t));
            fread(buf, sizeof(uint8_t), phdr.p_filesz, elf);
            riscv.memory.write(phdr.p_vaddr, buf, phdr.p_memsz);
            free(buf);
        }

    }

    Elf_Scn* scn = NULL;

    while ((scn = elf_nextscn(e, scn)) != NULL) {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB) {
            break;
        }
    }

    uint32_t startPc = 0;
    int count = 0;
    Elf_Data* data = elf_getdata(scn, NULL);
    count = shdr.sh_size / shdr.sh_entsize;
    for (int ii = 0; ii < count; ++ii)
    {
        GElf_Sym sym;
        gelf_getsym(data, ii, &sym);
        if(!strcmp(elf_strptr(e, shdr.sh_link, sym.st_name), "main"))
            startPc = sym.st_value;
    }

    riscv.hart.pc = startPc;
}

uint32_t Simulator::fetch(address_t address)
{
    uint32_t tmp = 0;
    riscv.memory.read(address, &tmp, 4);
    return tmp;
}

Instruction Simulator::decode(uint32_t encoding)
{
    return decoder.decode(encoding);
}

void Simulator::execute(const Instruction& instr)
{
    std::invoke(instr.executor, riscv, instr);
}

void Simulator::run()
{
    //riscv.hart.pc = 65620;
    while(true)
    {

        std::cerr << riscv.hart.pc << "\n\n";
        uint32_t encoding = fetch(riscv.hart.pc);
        if(encoding == 0)
        {
            riscv.hart.pc += 4;
            break;
        }
        Instruction instr = decode(encoding);
        execute(instr);
    }
}
