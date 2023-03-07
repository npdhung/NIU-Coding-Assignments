#ifndef RV32I_HART_H
#define RV32I_HART_H
#include "rv32i_decode.h"
#include "registerfile.h"
#include "memory.h"

/**
 * Represent the execution unit of a RV32I hart.
 ***********************************************************************************/
class rv32i_hart : public rv32i_decode
{
public:
    rv32i_hart(memory& m) : mem (m) { }
    void set_show_instructions(bool b) {show_instructions = b;}
    void set_show_registers(bool b) {show_registers = b;}
    bool is_halted () const {return halt ;}
    const std::string & get_halt_reason () const {return halt_reason;}
    uint64_t get_insn_counter () const {return insn_counter;}
    void set_mhartid(int i) {mhartid = i;}

    void tick(const std::string& hdr ="");
    void dump(const std::string& hdr ="") const;
    void reset() ;

private:
    static constexpr int instruction_width = 35;
    void exec(uint32_t insn , std::ostream *);
    void exec_illegal_insn(uint32_t insn , std::ostream *);
    void exec_lui(uint32_t insn , std::ostream *);
    void exec_auipc(uint32_t insn , std::ostream *);
    void exec_jal(uint32_t insn , std::ostream *);
    void exec_jalr(uint32_t insn , std::ostream *);
    void exec_btype(uint32_t insn , std::ostream *, const char mnemonic);
    void exec_itype_load(uint32_t insn , std::ostream *, const char mnemonic);
    void exec_stype(uint32_t insn , std::ostream *, const char mnemonic);
    void exec_itype_alu(uint32_t insn , std::ostream *, const char mnemonic);
    void exec_rtype(uint32_t insn , std::ostream *, const char mnemonic);
    void exec_ecall(uint32_t insn , std::ostream *);
    void exec_ebreak(uint32_t insn , std::ostream *);
    void exec_csrrx(uint32_t insn , std::ostream *, const char mnemonic);
    
    void exec_slt(uint32_t insn , std::ostream *);
    
    bool halt = {false};
    bool show_instructions = {false};
    bool show_registers = {false};
    std::string halt_reason = {"none"};
    
    uint64_t insn_counter = {0};
    uint32_t pc = {0};
    uint32_t mhartid = {0};

protected:
    registerfile regs;
    memory& mem;

};
#endif
