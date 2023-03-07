#include "rv32i_decode.h"
#include <cassert>
#include <iomanip>

/**
 * Return a std::string containing the disassembled instruction text.
 * @param addr A 32-bit memory address.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    switch (get_opcode(insn))
    {
        case opcode_lui:            return render_lui(insn);
        case opcode_auipc:          return render_auipc(insn);
        case opcode_jal:            return render_jal(addr, insn);
        case opcode_jalr:           return render_jalr(insn);

        case opcode_btype:  
            switch (get_funct3(insn))
            {
                case funct3_beq:    return render_btype(addr, insn, "beq");
                case funct3_bne:    return render_btype(addr, insn, "bne");
                case funct3_blt:    return render_btype(addr, insn, "blt");
                case funct3_bge:    return render_btype(addr, insn, "bge");
                case funct3_bltu:   return render_btype(addr, insn, "bltu");
                case funct3_bgeu:   return render_btype(addr, insn, "bgeu");
                default:            return render_illegal_insn(insn);
            }
            assert(0 && "unrecognized funct3 of btype");
        
        case opcode_load_imm:
            switch (get_funct3(insn))
            {
                case funct3_lb:	    return render_itype_load(insn, "lb");
                case funct3_lh:	    return render_itype_load(insn, "lh");
                case funct3_lw:	    return render_itype_load(insn, "lw");
                case funct3_lbu:	return render_itype_load(insn, "lbu");
                case funct3_lhu:	return render_itype_load(insn, "lhu");
                default:            return render_illegal_insn(insn);
            }
            assert(0 && "unrecognized funct3 of load_imm");

        case opcode_stype:
            switch (get_funct3(insn))
            {
                case funct3_sb:	    return render_stype(insn, "sb");
                case funct3_sh:	    return render_stype(insn, "sh");
                case funct3_sw:	    return render_stype(insn, "sw");
                default:            return render_illegal_insn(insn);
            }
            assert(0 && "unrecognized funct3 of stype");
        
        case opcode_alu_imm:
            switch (get_funct3(insn))
            {
                case funct3_add:	return render_itype_alu(insn, "addi", get_imm_i(insn));
                case funct3_sll:    return render_itype_alu(insn, "slli", get_imm_i(insn)%XLEN);
                case funct3_slt:	return render_itype_alu(insn, "slti", get_imm_i(insn));
                case funct3_sltu:	return render_itype_alu(insn, "sltiu", get_imm_i(insn));
                case funct3_xor:	return render_itype_alu(insn, "xori", get_imm_i(insn));
                case funct3_srx:	
                    switch(get_funct7(insn))
                    {
                        case funct7_srl: 
                            return render_itype_alu(insn, "srli", get_imm_i(insn)%XLEN);
                        case funct7_sra: 
                            return render_itype_alu(insn, "srai", get_imm_i(insn)%XLEN);
                        default: return render_illegal_insn(insn);
                    }
                    assert(0 && "unrecognized funct7 of i-type srx");
                case funct3_or:	    return render_itype_alu(insn, "ori", get_imm_i(insn));
                case funct3_and:	return render_itype_alu(insn, "andi", get_imm_i(insn));
                default:            return render_illegal_insn(insn);
            }
            assert(0 && "unrecognized funct3 of alu_imm type");
        
        case opcode_rtype:
            switch (get_funct3(insn))
            {
                case funct3_add:
                    switch (get_funct7(insn))
                    {
                        case funct7_add: return render_rtype(insn, "add");
                        case funct7_sub: return render_rtype(insn, "sub");
                        default: return render_illegal_insn(insn);
                    }	
                case funct3_sll:	return render_rtype(insn, "sll");
                case funct3_slt:	return render_rtype(insn, "slt");
                case funct3_sltu:	return render_rtype(insn, "sltu");
                case funct3_xor:	return render_rtype(insn, "xor");
                case funct3_srx:
                switch (get_funct7(insn))
                    {
                        case funct7_srl: return render_rtype(insn, "srl");
                        case funct7_sra: return render_rtype(insn, "sra");
                        default: return render_illegal_insn(insn);
                    }	
                case funct3_or:	    return render_rtype(insn, "or");
                case funct3_and:	return render_rtype(insn, "and");
                default:            return render_illegal_insn(insn);
            }
            assert(0 && "unrecognized funct3 of rtype");
        
        case opcode_system:
            switch (insn)
            {
                case insn_ecall: return render_ecall(insn);
                case insn_ebreak: return render_ebreak(insn);
                default:
                    switch (get_funct3(insn))
                    {
                        case funct3_csrrw:	return render_csrrx(insn, "csrrw");
                        case funct3_csrrs:	return render_csrrx(insn, "csrrs");
                        case funct3_csrrc:	return render_csrrx(insn, "csrrc");
                        case funct3_csrrwi:	return render_csrrxi(insn, "csrrwi");
                        case funct3_csrrsi:	return render_csrrxi(insn, "csrrsi");
                        case funct3_csrrci:	return render_csrrxi(insn, "csrrci");
                        default:            return render_illegal_insn(insn);
                    }
                    assert(0 && "unrecognized funct3 of system type insn");
            }

        default: return render_illegal_insn(insn);
    }
}

/**
 * Get opcode for a 32-bit instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    // Extract last 7s bit of the instruction
    return (insn & 0x0000007f);
}

/**
 * Get funct3 field of a 32-bit instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    // Extract bit 12 - 14 of the instruction
    return (insn & 0x00007000) >> 12;
}

/**
 * Get funct7 field of a 32-bit instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
    // Extract bit 25 - 31 of the instruction
    return (insn & 0xfe000000) >> 25;
}

/**
 * Get rd field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return ((insn & 0x00000f80) >> 7); 
}

/**
 * Get rs1 field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return ((insn & 0x000f8000) >> 15); 
}

/**
 * Get rs2 field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return ((insn & 0x01f00000) >> 20); 
}

/**
 * Get the imm_u field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    return ((insn & 0xfffff000)>>12); 
}

/**
 * Get the imm_j field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    int32_t imm_j = (insn & 0x7fe00000);    // Get bit 1 - 10 from insn
    imm_j >>= 20;   // Move bit 1 - 10 to position and let bit 0 = 0
    imm_j |= (insn & 0x00100000) >> 9; // Extract bit 20 of insn into bit 11 of imm_j
    imm_j |= (insn & 0x000ff000); // Extract bit 12-19 of insn to bit 12-19 of imm_j
    // Sign extend for bit 20-31 of imm_j
    if (insn & 0x80000000)      // If bit 31 of insn is 1
		imm_j |= 0xfff00000;    // Placing sign bit to [31:20] 
    return imm_j; 
}

/**
 * Get the imm_i field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    int32_t imm_i = (insn & 0xfff00000) >> 20;  // Get bit 0 - 11 from insn
    // Sign extend for bit 12-31 of imm_i
    if (insn & 0x80000000)      // If bit 31 of insn is 1
		imm_i |= 0xfffff000;    // Placing sign bit to [31:12] 
    return imm_i; 
}

/**
 * Get the imm_b field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    // Get bit 1 - 4 of imm_b from insn, let bit 0 of imm_b equal 0
    int32_t imm_b = (insn & 0x00000f00) >> 7;
    imm_b |= (insn & 0x7e000000) >> 20; // Get bit 5 - 10 of imm_b from insn
    imm_b |= (insn & 0x00000080) << 4;  // Get bit 11 of imm_b from insn
    // Sign extend for bit 12-31 of imm_b
    if (insn & 0x80000000)      // If bit 31 of insn is 1
		imm_b |= 0xfffff000;    // Placing sign bit to [31:12] 
    return imm_b; 
}

/**
 * Get the imm_s field of the instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    int32_t imm_s = (insn & 0x00000f80) >> 7;   // Get bit 0 - 4 of imm_s
    imm_s |= (insn & 0xfe000000) >> 20; // Get bit 5 - 11 of imm_b from insn
    // Sign extend for bit 12-31 of imm_b
    if (insn & 0x80000000)      // If bit 31 of insn is 1
		imm_s |= 0xfffff000;    // Placing sign bit to [31:12] 
    return imm_s; 
}

/**
 * Helper function to get csr from csrx type of system instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
int32_t rv32i_decode::get_csr(uint32_t insn)
{
    int32_t csr = (insn & 0xfff00000) >> 20;
    return csr; 
}

/**
 * Return a message for illegal instruction. 
 **********************************************************************************/
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    (void)insn; // Temporary fix unused parameter which may be needed for next Assgn
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

/**
 * Return the disassembled instruction text for lui instruction. 
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) <<","<< to_hex0x20(imm_u);
    return os.str();
}

/**
 * Return the disassembled instruction text for auipc instruction. 
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd) <<","<< to_hex0x20(imm_u);
    return os.str();
}

/**
 * Return the disassembled instruction text for jal instruction. 
 * @param addr Address of the intruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t prel_21 = get_imm_j(insn)+addr;
    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) <<","<< to_hex0x32(prel_21);
    return os.str();
}

/**
 * Return the disassembled instruction text for jalr instruction. 
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << ","
        << render_base_disp(get_imm_i(insn), get_rs1(insn));
    return os.str();
}

/**
 * Return the disassembled instruction text for b_type instruction. 
 * @param addr Address of the intruction.
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 =   get_rs1(insn);
    uint32_t rs2 =   get_rs2(insn);
    int32_t  imm_b = get_imm_b(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) <<  render_reg(rs1) << "," 
        << render_reg(rs2) << "," << to_hex0x32(imm_b + addr);
    return os.str();
}

/**
 * Return the disassembled instruction text for i-type load instruction. 
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 =  get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
        << render_base_disp(imm_i, rs1);
	return os.str();
}

/**
 * Return the disassembled instruction text for s-type instruction. 
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 =  get_rs1(insn);
    uint32_t rs2 =  get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rs2) << "," 
        << render_base_disp(imm_s, rs1);
	return os.str();
}

/**
 * Return the disassembled instruction text for i-type load instruction. 
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 =  get_rs1(insn);
    std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1)
        << "," << imm_i;
	return os.str();
}

/**
 * Return the disassembled instruction text for r-type instruction. 
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 =  get_rs1(insn);
    uint32_t rs2 =  get_rs2(insn);
    std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
        << render_reg(rs1) << "," << render_reg(rs2);
	return os.str();
}

/**
 * Return the disassembled instruction text for ecall system instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn)
{
	(void)insn; // Temporary fix unused parameter which may be needed for next Assgn
	return "ecall";
}

/**
 * Return the disassembled instruction text for ebreak system instruction.
 * @param insn A 32-bit intruction.
 **********************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
	(void)insn; // Temporary fix unused parameter which may be needed for next Assgn
	return "ebreak";
}

/**
 * Return the disassembled instruction text for csrrx type of system instruction. 
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t csr =  get_csr(insn);
    uint32_t rs1 =  get_rs1(insn);
    std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
        << to_hex0x12(csr) << "," << render_reg(rs1);
	return os.str();
}

/**
 * Return the disassembled instruction text for csrrxi type of system instruction. 
 * @param insn A 32-bit intruction.
 * @param mnemonic The mnemonic characters of the intruction.
 **********************************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t csr =  get_csr(insn);
    uint32_t zimm =  get_rs1(insn);
    std::ostringstream os;
	os << render_mnemonic(mnemonic) << render_reg(rd) << "," 
        << to_hex0x12(csr) << "," << zimm;
	return os.str();
}

/**
 * Render into a std:string the name of the register with the given number r
 * @param r Integer number.
 **********************************************************************************/
std::string rv32i_decode::render_reg(int r)
{
	std::ostringstream os;
	os << std::right << "x" << r;
	return os.str();
}

/**
 * Return the instruction mnemonic with proper space padding on the right side to make it
 * mnemonic_width characters long. 
 * @param m Input mnemonic instruction.
 **********************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &m)
{
	std::ostringstream os;
	os << std::left << std::setw(mnemonic_width) << m;
	return os.str();
}

/**
 * Render into a string the operands of the form imm(register)
 * @param base Input base register.
 * @param disp Input displacement from the base register.
 **********************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp)
{
	std::ostringstream os;
	os << std::dec << static_cast<int>(base) << "(x" << disp << ")";
	return os.str();
}
