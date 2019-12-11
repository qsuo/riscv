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
#include <chrono>


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
        err(EX_NOINPUT, " open \"%s \" failed ", name);

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

/*
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
    uint32_t num_executed = 0;
    auto t_start = std::chrono::high_resolution_clock::now();

    //riscv.hart.pc = 65620;
    printf("0x%x\n", riscv.hart.pc);
    while(true)
    {
        //std::cout << std::hex << riscv.hart.pc << "\n";
        //std::cerr << riscv.hart.pc << "\n\n";
        if(riscv.hart.pc > riscv.memory.getSize())
            break;
        uint32_t encoding = fetch(riscv.hart.pc);
        Instruction instr = decode(encoding);
        execute(instr);
        num_executed++;
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> t_total = t_end - t_start;

    std::cout << "Total instructions: " << num_executed << std::endl
              << "Total time: " << t_total.count() << " ms" << std::endl;
}
*/

BasicBlock* Simulator::getBasicBlock()
{
    BasicBlock* bb = NULL;
    if(map.find(riscv.hart.pc) != map.end())
    {
        bb = map.at(riscv.hart.pc);

        return bb;
    }
    else
    {
        bb = new BasicBlock;
        bb->start = riscv.hart.pc;
        uint32_t addr = riscv.hart.pc;
        for(int i = 0; i < BasicBlock::BLOCK_SIZE; i++)
        {
            uint32_t encoding = 0;
            if(addr > riscv.memory.getSize())
                return NULL;
            riscv.memory.read(addr, &encoding, 4);
                           
            Instruction* instr = decoder.decode(encoding);

            bb->add(instr);

            if(instr->type == decoder.BRANCH || instr->type == decoder.JAL)
            {
                //uint32_t new_pc = hart.getPc() + (instr.imm << 1);
                addr = addr + (instr->imm << 1);
                break;
            }
            else if(instr->type == decoder.JALR)
            {
                addr = (instr->rs1 + instr->imm) & ~0x1u;
                break;
            }
            else
                addr += 4;
        }
        bb->next = addr;
        map[bb->start] = bb; 
    }
    return bb;
}

Simulator::~Simulator()
{
    for(auto bb : map)
        delete bb.second;
}

void Simulator::execute(BasicBlock* bb)
{
    bb->execute(riscv);
}



