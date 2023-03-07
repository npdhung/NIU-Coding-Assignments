#include "rv32i_hart.h"
#include <cassert>

/**
 * Reset the RV32I object and the register file.
 ***********************************************************************************/
void rv32i_hart::reset()
{
	pc = 0;
	regs.reset();
	insn_counter = 0;
	halt = false;
	halt_reason = "none";
}

/**
 * Print out the entire state of the hart, including content of the register and
 * value of the PC register.
 * @param hdr the prefix string for each printed line.
 ***********************************************************************************/
void rv32i_hart::dump(const std::string& hdr) const
{
	regs.dump(hdr);
	std::cout << hdr << std::setw(4) << std::right << "pc " << to_hex32(pc)
	  << std::endl;
}

/**
 * Simulate the execution of one instruction.
 * @param hdr the prefix string for each printed line of output.
 ***********************************************************************************/
void rv32i_hart::tick(const std::string& hdr)
{
	if (is_halted()) return;
	// Simulate the execution of one single instruction
	if (show_registers) dump(hdr);
	if (pc%4) {halt = true; halt_reason = "PC alignment error";	return;}
	++insn_counter;
	// Fetch an insn from the memory at the address in the pc register
	uint32_t insn = mem.get32(pc);
	if (show_instructions)
	{
		// Print hdr, pc register, and 32-bit fetched insn (in hex)
		std::cout << hdr << to_hex32(pc) << ": " << to_hex32(insn) << "  ";
		exec(insn, &std::cout);
	}
	else exec(insn, nullptr);
}

/**
 * Execute a given RV32I instruction.
 * This function uses the get_xxx() methods to extract the needed instruction fields 
 * to decode the instruction and invokes the associated exec_xxx() helper function.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
	uint32_t opcode = get_opcode(insn);
	switch (opcode)
	{
		case opcode_lui: 	exec_lui(insn, pos); return;
		case opcode_auipc: 	exec_auipc(insn, pos); return;
		case opcode_jal: 	exec_jal(insn, pos); return;
		case opcode_jalr: 	exec_jalr(insn, pos); return;
 		

		case opcode_btype:  
            switch (get_funct3(insn))
            {
                case funct3_beq:	exec_btype(insn, pos, 'e'); return;	
                case funct3_bne:	exec_btype(insn, pos, 'n'); return;
                case funct3_blt:	exec_btype(insn, pos, 'l'); return;
                case funct3_bge:	exec_btype(insn, pos, 'g'); return;
                case funct3_bltu:	exec_btype(insn, pos, 'u'); return;
                case funct3_bgeu:	exec_btype(insn, pos, 'o'); return;
                default:            exec_illegal_insn(insn, pos); return;
            }
            assert(0 && "unrecognized funct3 of btype");
		
		case opcode_load_imm:
            switch (get_funct3(insn))
            {
                case funct3_lb:		exec_itype_load(insn, pos, 'b'); return;
                case funct3_lh:		exec_itype_load(insn, pos, 'h'); return;
                case funct3_lw:		exec_itype_load(insn, pos, 'w'); return;
                case funct3_lbu:	exec_itype_load(insn, pos, 'u'); return;
                case funct3_lhu:	exec_itype_load(insn, pos, 'v'); return;
                default:            exec_illegal_insn(insn, pos); return;
            }
            assert(0 && "unrecognized funct3 of load_imm");
		
		case opcode_stype:
            switch (get_funct3(insn))
            {
                case funct3_sb:	    exec_stype(insn, pos, 'b'); return;
                case funct3_sh:	    exec_stype(insn, pos, 'h'); return;
                case funct3_sw:	    exec_stype(insn, pos, 'w'); return;
                default:            exec_illegal_insn(insn, pos); return;
            }
            assert(0 && "unrecognized funct3 of stype");

		case opcode_alu_imm:
            switch (get_funct3(insn))
            {
                case funct3_add:	exec_itype_alu(insn, pos, 'i'); return;
                case funct3_slt:	exec_itype_alu(insn, pos, 's'); return;
                case funct3_sltu:	exec_itype_alu(insn, pos, 'u'); return;
                case funct3_xor:	exec_itype_alu(insn, pos, 'x'); return;
				case funct3_or:		exec_itype_alu(insn, pos, 'o'); return;
                case funct3_and:	exec_itype_alu(insn, pos, 'n'); return;
                case funct3_sll:	exec_itype_alu(insn, pos, 'l'); return;
                case funct3_srx:	
                    switch(get_funct7(insn))
                    {
                        case funct7_srl: exec_itype_alu(insn, pos, 'r'); return;
                        case funct7_sra: exec_itype_alu(insn, pos, 'a'); return;
                        default: exec_illegal_insn(insn, pos); return;
                    }
                    assert(0 && "unrecognized funct7 of i-type srx");
                default: exec_illegal_insn(insn, pos); return;
            }
            assert(0 && "unrecognized funct3 of itype alu");
		
		case opcode_rtype:
            switch (get_funct3(insn))
            {
				case funct3_add:	
					switch (get_funct7(insn))
                    {
                        case funct7_add: exec_rtype(insn, pos, 'd'); return;
                        case funct7_sub: exec_rtype(insn, pos, 't'); return;
						default: 	exec_illegal_insn(insn, pos); return;
                    }	
				case funct3_sll:	exec_rtype(insn, pos, 'l'); return;
				case funct3_slt:	exec_rtype(insn, pos, 'e'); return;
				case funct3_sltu:	exec_rtype(insn, pos, 'u'); return;
				case funct3_xor:	exec_rtype(insn, pos, 'x'); return;
				case funct3_srx:
                switch (get_funct7(insn))
                    {
                        case funct7_srl: exec_rtype(insn, pos, 'r'); return;
                        case funct7_sra: exec_rtype(insn, pos, 'a'); return;
                        default: 	exec_illegal_insn(insn, pos); return;
                    }	
				case funct3_or:		exec_rtype(insn, pos, 'o'); return;
				case funct3_and:	exec_rtype(insn, pos, 'n'); return;
				default:            exec_illegal_insn(insn, pos); return;
			}
            assert(0 && "unrecognized funct3 of rtype");

		case opcode_system:
            switch (insn)
            {
                case insn_ecall: 	exec_ecall(insn, pos);return;
                case insn_ebreak: 	exec_ebreak(insn, pos);return;
                default:
					switch (get_funct3(insn))
                    {
                        case funct3_csrrw:	exec_csrrx(insn, pos, 'w'); return;
                        case funct3_csrrs:	exec_csrrx(insn, pos, 's'); return;
                        case funct3_csrrc:	exec_csrrx(insn, pos, 'c'); return;
                        case funct3_csrrwi:	exec_csrrx(insn, pos, 'i'); return;
                        case funct3_csrrsi:	exec_csrrx(insn, pos, '0'); return;
                        case funct3_csrrci:	exec_csrrx(insn, pos, '1'); return;
                        default: exec_illegal_insn(insn, pos); return;
                    }
                    assert(0 && "unrecognized funct3 of system type insn");
			}  	

		default: exec_illegal_insn(insn, pos); return;
	}
}

/**
 * Simulate the execution of an illegal instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_illegal_insn(uint32_t insn , std::ostream* pos)
{
	if (pos) *pos << render_illegal_insn(insn);
	halt = true;
	halt_reason = "Illegal instruction";
}

/**
 * Simulate the execution of the 'lui' instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_lui(uint32_t insn , std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn) << 12;
	if (pos)
	{
		std::string s = render_lui(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u) << std::endl;
	}
	regs.set(rd, imm_u);
	pc += 4;
}

/**
 * Simulate the execution of the 'auipc' instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_u = get_imm_u(insn) << 12;
	int32_t val = pc + imm_u;
	if (pos)
	{
		std::string s = render_auipc(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + "  
		  << hex::to_hex0x32(imm_u) << " = " << hex::to_hex0x32(val) << std::endl;
	}
	regs.set(rd, val);
	pc += 4;
}

/**
 * Simulate the execution of the 'jal' instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_jal(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	int32_t imm_j = get_imm_j(insn);
	int32_t val = pc + imm_j;
	if (pos)
	{
		std::string s = render_jal(pc, insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) << ",  pc = "  
		  << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j) << " = "
		  << hex::to_hex0x32(val) << std::endl;
	}
	regs.set(rd, pc + 4);
	pc += imm_j;
}

/**
 * Simulate the execution of the 'jalr' instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream *pos)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t val = (regs.get(rs1) + imm_i) & ~1;
	if (pos)
	{
		std::string s = render_jalr(insn);
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc + 4) 
		  << ",  pc = ("  << hex::to_hex0x32(imm_i) << " + " 
		  << hex::to_hex0x32(regs.get(rs1)) << ") & " << hex::to_hex0x32(~1) << " = " 
		  << hex::to_hex0x32(val) << std::endl;
	}
	regs.set(rd, pc + 4);
	pc = val;
}

/**
 * Simulate the execution of a b-type instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 * @param mnemonic a character representing a b-type instruction ('e' for beq, 'n' 
 * for bne, 'l' for blt, 'g' for bge, 'u' for bltu, and 'o' for bgeu).
 ***********************************************************************************/
void rv32i_hart::exec_btype(uint32_t insn, std::ostream *pos, const char mnemonic)
{
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_b = get_imm_b(insn);
	int32_t val = 0;
	std::string s;
	std::string cond_str;
	uint32_t u_rs1 = regs.get(rs1);
	uint32_t u_rs2 = regs.get(rs2);
	switch (mnemonic)
	{
		case 'e':
			s = render_btype(pc, insn, "beq");
			val = pc + ((regs.get(rs1) == regs.get(rs2)) ? imm_b : 4);
			cond_str = " == ";
			break;
		case 'n':
			s = render_btype(pc, insn, "bne");
			val = pc + ((regs.get(rs1) != regs.get(rs2)) ? imm_b : 4);
			cond_str = " != ";
			break;
		case 'l':
			s = render_btype(pc, insn, "blt");
			val = pc + ((regs.get(rs1) < regs.get(rs2)) ? imm_b : 4);
			cond_str = " < ";
			break;
		case 'g':
			s = render_btype(pc, insn, "bge");
			val = pc + ((regs.get(rs1) >= regs.get(rs2)) ? imm_b : 4);
			cond_str = " >= ";
			break;
		case 'u':
			s = render_btype(pc, insn, "bltu");
			val = pc + ((u_rs1 < u_rs2) ? imm_b : 4);
			cond_str = " <U ";
			break;
		case 'o':
			s = render_btype(pc, insn, "bgeu");
			val = pc + ((u_rs1 >= u_rs2) ? imm_b : 4);
			cond_str = " >=U ";
			break;
		default: break;
	}
	if (pos)
	{
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << "pc += ("  << hex::to_hex0x32(regs.get(rs1)) << cond_str 
		  << hex::to_hex0x32(regs.get(rs2)) << " ? " << hex::to_hex0x32(imm_b) 
		  << " : 4) = " << hex::to_hex0x32(val) << std::endl;
	}
	pc = val;
}

/**
 * Simulate the execution of a i-type load instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 * @param mnemonic a character representing the i-type load instruction ('b' for lb,
 * 'h' for lh, 'w' for lw, 'u' for lbu, and v' for lhu).
 ***********************************************************************************/
void rv32i_hart::exec_itype_load(uint32_t insn, std::ostream *pos, const char mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t val = 0;
	std::string s;
	std::ostringstream os; // hold part of string for print out
	switch (mnemonic)
	{
		case 'b':
			s = render_itype_load(insn, "lb");
			val = mem.get8_sx(regs.get(rs1) + imm_i);
			os << "sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_i) << "))";
			break;
		case 'h':
			s = render_itype_load(insn, "lh");
			val = mem.get16_sx(regs.get(rs1) + imm_i);
			os << "sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_i) << "))";
			break;
		case 'w':
			s = render_itype_load(insn, "lw");
			val = mem.get32_sx(regs.get(rs1) + imm_i);
			os << "sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_i) << "))";
			break;
		case 'u':
			s = render_itype_load(insn, "lbu");
			val = mem.get8(regs.get(rs1) + imm_i);
			os << "zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_i) << "))";
			break;
		case 'v':
			s = render_itype_load(insn, "lhu");
			val = mem.get16(regs.get(rs1) + imm_i);
			os << "zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_i) << "))";
			break;
		default: break;
	}
	if (pos)
	{
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << os.str() << " = " 
		  << hex::to_hex0x32(val) << std::endl;
	}
	regs.set(rd, val);
	pc += 4;
}

/**
 * Simulate the execution of a s-type instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 * @param mnemonic a character representing the s-type instruction ('b' for sb, 'h' 
 * for sh, and 'w' for sw).
 ***********************************************************************************/
void rv32i_hart::exec_stype(uint32_t insn, std::ostream *pos, const char mnemonic)
{
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t imm_s = get_imm_s(insn);
	int32_t val = 0;
	std::string s;
	std::ostringstream os; // hold part of string for print out
	switch (mnemonic)
	{
		case 'b':
			s = render_stype(insn, "sb");
			val = regs.get(rs2) & 0x000000ff;
			os << "m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_s) << ")";
			mem.set8(regs.get(rs1) + imm_s,val);
			break;
		case 'h':
			s = render_stype(insn, "sh");
			val = regs.get(rs2) & 0x0000ffff;
			os << "m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_s) << ")";
			mem.set16(regs.get(rs1) + imm_s,val);
			break;
		case 'w':
			s = render_stype(insn, "sw");
			val = regs.get(rs2);
			os << "m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " 
			  << hex::to_hex0x32(imm_s) << ")";
			mem.set32(regs.get(rs1) + imm_s,val);
			break;
		default: break;
	}
	if (pos)
	{
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << os.str() << " = " << hex::to_hex0x32(val) << std::endl;
	}
	pc += 4;
}

/**
 * Simulate the execution of a i-type alu instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 * @param mnemonic a character representing the i-type alu instruction ('i' for addi,
 * 's' for slti, 'u' for sltiu, 'x' for xori, 'o' for ori, 'n' for andi, 'l' for 
 * slli, 'r' for srli, and 'a' for srai).
 ***********************************************************************************/
void rv32i_hart::exec_itype_alu(uint32_t insn, std::ostream *pos, const char mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	int32_t imm_i = get_imm_i(insn);
	int32_t val = 0;
	uint32_t u_rs1 = regs.get(rs1);
	uint32_t u_imm_i = imm_i;
	std::string s;
	std::ostringstream os; // hold part of string for print out
	uint32_t msb;
	switch (mnemonic)
	{
		case 'i':
			s = render_itype_alu(insn, "addi", imm_i);
			val = regs.get(rs1) + imm_i;
			os << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i);
			break;
		case 's':
			s = render_itype_alu(insn, "slti", imm_i);
			val = (regs.get(rs1) < imm_i) ? 1 : 0;
			os << "(" << hex::to_hex0x32(regs.get(rs1)) << " < " << imm_i << ") ? 1 : 0";
			break;
		case 'u':
			s = render_itype_alu(insn, "sltiu", imm_i);
			val = (u_rs1 < u_imm_i) ? 1 : 0;
			os << "(" << hex::to_hex0x32(u_rs1) << " <U " << u_imm_i << ") ? 1 : 0";
			break;
		case 'x':
			s = render_itype_alu(insn, "xori", imm_i);
			val = regs.get(rs1) ^ imm_i;
			os << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(imm_i);
			break;
		case 'o':
			s = render_itype_alu(insn, "ori", imm_i);
			val = regs.get(rs1) | imm_i;
			os << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(imm_i);
			break;
		case 'n':
			s = render_itype_alu(insn, "andi", imm_i);
			val = regs.get(rs1) & imm_i;
			os << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(imm_i);
			break;
		case 'l':
			s = render_itype_alu(insn, "slli", imm_i%XLEN);
			val = regs.get(rs1) << imm_i%XLEN;
			os << hex::to_hex0x32(regs.get(rs1)) << " << " << imm_i%XLEN;
			break;
		case 'r':
			s = render_itype_alu(insn, "srli", imm_i%XLEN);
			val = regs.get(rs1);
			for (uint8_t i = 0; i < imm_i%XLEN; ++i)
			{
				val >>= 1;	// Shift right 1 bit
				val &= 0x7fffffff; // Turn MSB to 0
			}
			os << hex::to_hex0x32(regs.get(rs1)) << " >> " << imm_i%XLEN;
			break;
		case 'a':
			s = render_itype_alu(insn, "srai", imm_i%XLEN);
			val = regs.get(rs1);
			msb = val & 0x80000000; // Get MSB
			for (uint8_t i = 0; i < imm_i%XLEN; ++i)
			{
				val >>= 1;	// Shift right 1 bit
				if (msb) val |= 0x80000000; // Copy the MSB
				else val &= 0x7fffffff;
			}
			os << hex::to_hex0x32(regs.get(rs1)) << " >> " << imm_i%XLEN;
			break;
		default: break;
	}
	if (pos)
	{
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << os.str() << " = " 
		  << hex::to_hex0x32(val) << std::endl;
	}
	regs.set(rd, val);
	pc += 4;
}

/**
 * Simulate the execution of a r-type instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 * @param mnemonic a character representing the r-type instruction ('d' for add, 't' 
 * for sub, 'l' for sll, 'e' for slt, 'u' for sltu, 'x' for xor, 'r' for srl, 'a'
 * for sra, 'o' for or, and 'n' for and).
 ***********************************************************************************/
void rv32i_hart::exec_rtype(uint32_t insn, std::ostream *pos, const char mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t rs2 = get_rs2(insn);
	int32_t val = 0;
	uint32_t u_rs1 = regs.get(rs1);
	uint32_t u_rs2 = regs.get(rs2);
	std::string s;
	std::ostringstream os; // hold part of string for print out
	uint32_t msb;
	switch (mnemonic)
	{
		case 'd':
			s = render_rtype(insn, "add");
			val = regs.get(rs1) + regs.get(rs2);
			os << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(regs.get(rs2));
			break;
		case 't':
			s = render_rtype(insn, "sub");
			val = regs.get(rs1) - regs.get(rs2);
			os << hex::to_hex0x32(regs.get(rs1)) << " - " << hex::to_hex0x32(regs.get(rs2));
			break;
		case 'l':
			s = render_rtype(insn, "sll");
			val = regs.get(rs1) << regs.get(rs2)%XLEN;
			os << hex::to_hex0x32(regs.get(rs1)) << " << " << regs.get(rs2)%XLEN;
			break;
		case 'e':
			s = render_rtype(insn, "slt");
			val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
			os << "(" << hex::to_hex0x32(regs.get(rs1)) << " < " 
			  << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0";
			break;
		case 'u':
			s = render_rtype(insn, "sltu");
			val = (u_rs1 < u_rs2) ? 1 : 0;
			os << "(" << hex::to_hex0x32(u_rs1) << " <U " << hex::to_hex0x32(u_rs2) 
			  << ") ? 1 : 0";
			break;
		case 'x':
			s = render_rtype(insn, "xor");
			val = regs.get(rs1) ^ regs.get(rs2);
			os << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(regs.get(rs2));
			break;
		case 'r':
			s = render_rtype(insn, "srl");
			val = regs.get(rs1);
			for (uint8_t i = 0; i < regs.get(rs2)%XLEN; ++i)
			{
				val >>= 1;	// Shift right 1 bit
				val &= 0x7fffffff; // Turn MSB to 0
			}
			os << hex::to_hex0x32(regs.get(rs1)) << " >> " << regs.get(rs2)%XLEN;
			break;
		case 'a':
			s = render_rtype(insn, "sra");
			val = regs.get(rs1);
			msb = val & 0x80000000; // Get MSB
			for (uint8_t i = 0; i < regs.get(rs2)%XLEN; ++i)
			{
				val >>= 1;	// Shift right 1 bit
				if (msb) val |= 0x80000000; // Copy the MSB
				else val &= 0x7fffffff;
			}
			os << hex::to_hex0x32(regs.get(rs1)) << " >> " << regs.get(rs2)%XLEN;
			break;
		case 'o':
			s = render_rtype(insn, "or");
			val = regs.get(rs1) | regs.get(rs2);
			os << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(regs.get(rs2));
			break;
		case 'n':
			s = render_rtype(insn, "and");
			val = regs.get(rs1) & regs.get(rs2);
			os << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(regs.get(rs2));
			break;
		default: break;
	}
	if (pos)
	{
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << os.str() << " = " 
		  << hex::to_hex0x32(val) << std::endl;
	}
	regs.set(rd, val);
	pc += 4;
}

/**
 * Simulate the execution of a csrrx instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 * @param mnemonic a character representing the csrrx instruction ('w' for csrrw, 's' 
 * for csrrs, 'c' for csrrc, 'i' for csrrwi, '0' for csrrsi, and '1' for csrrci).
 * @note only the csrrs instruction is implemented at this point. Other csrrx
 * instructions will throw the "Illegal CSR in CSRRS instruction" error.
 ***********************************************************************************/
void rv32i_hart::exec_csrrx(uint32_t insn, std::ostream *pos, const char mnemonic)
{
	uint32_t rd = get_rd(insn);
	uint32_t csr = get_csr(insn);
	uint32_t rs1 = get_rs1(insn);
	uint32_t val = 0;
	std::string s;
	switch (mnemonic)
	{
		case 's':
			s = render_csrrx(insn, "csrrs");
			// The instruction will not write to CSR if rs1 = x0
			if (rs1) val = csr | regs.get(rs1);
			break;
		default: 
			halt = true;
			halt_reason = "Illegal CSR in CSRRS instruction";
			break;
	}
	if (pos)
	{
		*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
		*pos << "// " << render_reg(rd) << " = " << val << std::endl;
	}
	regs.set(rd, val);
	// The instruction will not write to CSR if rs1 = x0
	if (rs1) 
	{
		// Write the CSR value to the memory using mem.set_32().
		// Have to do this because we don't have a method to write a 12 bit value.
		uint32_t csr_w; 
		// Extract last 20 bit from the instruction which is the value of memory at
		// the current address.
		csr_w = insn & 0x000fffff;
		csr_w |= val << 20; // Add 12 bit new CSR value to form a 32 bit value
		mem.set32(pc, csr_w); // Write back to the memory at the address of CSR
	}
	pc += 4;
}

/**
 * Simulate the execution of an ebreak instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos)
{
	if (pos)
		{
			std::string s = render_ebreak(insn);
			*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
			*pos << "// HALT" << std::endl;
		}
	halt = true;
	halt_reason = "EBREAK instruction";
	
}

/**
 * Simulate the execution of an ebreak instruction.
 * @param insn a 32-bit instruction.
 * @param pos a pointer to the output stream.
 ***********************************************************************************/
void rv32i_hart::exec_ecall(uint32_t insn, std::ostream *pos)
{
	if (pos)
		{
			std::string s = render_ecall(insn);
			*pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
			*pos << "// HALT" << std::endl;
		}
	halt = true;
	halt_reason = "ECALL instruction";
}