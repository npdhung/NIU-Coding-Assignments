//***************************************************************************
//
//  CSCI463 - Assignment 3
//
//  Author: Hung Nguyen (Z1924897)
//
//  Memory Simulator
//
//***************************************************************************
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
	mem.dump();

/*
	if (!mem.load_file(argv[optind]))
		usage();
	
	// Print out memory dump.
	mem.dump();
	cout << mem.get_size() << endl;
	cout << hex::to_hex32(mem.get8(0)) << endl;
	cout << hex::to_hex32(mem.get16(0)) << endl;
	cout << hex::to_hex32(mem.get32(0)) << endl;
	cout << hex::to_hex0x32(mem.get8(0)) << endl;
	cout << hex::to_hex0x32(mem.get16(0)) << endl;
	cout << hex::to_hex0x32(mem.get32(0)) << endl;
	cout << hex::to_hex8(mem.get8(0)) << endl;
	cout << hex::to_hex8(mem.get16(0)) << endl;
	cout << hex::to_hex8(mem.get32(0)) << endl;

	cout << hex::to_hex0x32(mem.get32(0x1000)) << endl;

	mem.set8(0x10, 0x12);
	mem.set16(0x14, 0x1234);
	mem.set32(0x18, 0x87654321);


	cout << hex::to_hex0x32(mem.get8_sx(0x0f)) << endl;
	cout << hex::to_hex0x32(mem.get8_sx(0x7f)) << endl;
	cout << hex::to_hex0x32(mem.get8_sx(0x80)) << endl;
	cout << hex::to_hex0x32(mem.get8_sx(0xe3)) << endl;

	cout << hex::to_hex0x32(mem.get16_sx(0xe0)) << endl;
	cout << hex::to_hex0x32(mem.get32_sx(0xe0)) << endl;

	// Print out memory dump.
	mem.dump();
*/
	return 0;
}
