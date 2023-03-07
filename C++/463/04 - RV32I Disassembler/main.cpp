//***************************************************************************
//
//  CSCI463 - Assignment 4
//
//  Author: Hung Nguyen (Z1924897)
//
//  RV32I Disassembler
//
//***************************************************************************
#include "rv32i_decode.h"
#include "memory.h"
#include "hex.h"
#include <stdio.h>
#include <unistd.h>

using namespace std;

/**
 * Verify the command-line. 
 *
 * Print an "Usage" error message and terminates the program if it doesn't meet 
 * input requirements. 
 *
 ***********************************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	exit(1);
}

/**
 * Decode and print each instruction in a loop. 
 *
 * @param mem 
 *
 ***********************************************************************************/
static void disassemble(const memory& mem)
{
	for (u_int pc = 0; pc < mem.get_size();)
	{
     std::cout << hex::to_hex32(pc) << ": " << hex::to_hex32(mem.get32(pc)) << "  ";
	 std::cout << rv32i_decode::decode(pc,mem.get32(pc)) << std::endl;
     pc += 4; 
	}
}

/**
 * Simulate a computer system memory from valid input argument. 
 *
 * @param argc Number of arguments passed to the program.
 * @param argv Argument vector, each string will be one of the arguments. 
 *
 * @return 0
 *
 * @bug User needs to enters the correct commands for the program to work.
 *      Example: ./EXACUTABLE -m HEX-MEM-SIZE FILENAME.IN
 ***********************************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100;	// default memory size is 0x100

	int opt;
	while ((opt = getopt(argc, argv, "m:")) != -1)
	{
		switch(opt)
		{
		case 'm':
			{
				std::istringstream iss(optarg);
				iss >> std::hex >> memory_limit;
			}
			break;

		default:
			usage();
		}
	}

	if (optind >= argc)
		usage();	// missing filename

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind]))
		usage();
	
	disassemble(mem);
	mem.dump();
	
	return 0;
}
