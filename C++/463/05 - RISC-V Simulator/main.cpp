//***************************************************************************
//
//  CSCI463 - Assignment 5
//
//  Author: Hung Nguyen (Z1924897)
//
//  RISC-V Simulator
//
//***************************************************************************
#include "rv32i_decode.h"
#include "memory.h"
#include "hex.h"
#include "cpu_single_hart.h"
#include <stdio.h>
#include <unistd.h>

using std::cerr;
using std::cout;
using std::endl;

/**
 * Verify the command-line. 
 *
 * Print an "Usage" error message and terminates the program if it doesn't meet 
 * input requirements. 
 ***********************************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
	cerr << "    -d show disassembly before program execution" << endl;
	cerr << "    -i show instruction printing during execution" << endl;
	cerr << "    -l maximum number of instructions to exec" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	cerr << "    -r show register printing during execution" << endl;
	cerr << "    -z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}

/**
 * Decode and print each instruction in a loop. 
 * @param mem 
 ***********************************************************************************/
static void disassemble(const memory& mem)
{
	for (u_int pc = 0; pc < mem.get_size();)
	{
    	cout << hex::to_hex32(pc) << ": " << hex::to_hex32(mem.get32(pc)) << "  ";
		cout << rv32i_decode::decode(pc,mem.get32(pc)) << endl;
    	pc += 4; 
	}
}

/**
 * Simulate a computer system memory from valid input argument. 
 * @param argc Number of arguments passed to the program.
 * @param argv Argument vector, each string will be one of the arguments. 
 ***********************************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100;	// default memory size is 0x100
	int opt;
	uint64_t exec_limit;
	int d_flag = 0;
	int i_flag = 0;
	int r_flag = 0;
	int z_flag = 0;
	while ((opt = getopt(argc, argv, "dil:m:rz")) != -1)
	{
		switch(opt)
		{
			case 'd': 	d_flag = 1; break;
			case 'i': 	i_flag = 1; break;
			case 'l': 	exec_limit = atoi(optarg); break;
			case 'm':	
				{
					std::istringstream iss(optarg);
					iss >> std::hex >> memory_limit;
				}	
				break;
			case 'r':	r_flag = 1;	break;
			case 'z':	z_flag = 1;	break;
			case '?':
			default:	usage();
		}
	}
	if (optind >= argc)	usage();	// missing filename

	memory mem(memory_limit);
	if (!mem.load_file(argv[optind])) usage();
	if (d_flag) disassemble(mem);
	
	cpu_single_hart cpu(mem);
	if (i_flag)
	{
		cpu.set_show_instructions(true);
		if (r_flag) cpu.set_show_registers(true);
	}	
	cpu.reset();
	cpu.run(exec_limit);
	if (z_flag)	{cpu.dump(); mem.dump();}
	return 0;
}
