#include "registerfile.h"

/**
 * Initialize register x0 to zero, and all other registers to 0xf0f0f0f0.
 ***********************************************************************************/
void registerfile::reset()
{
    registers.push_back(0); 
    for (int32_t i = 1; i < 32; ++i) registers.push_back(0xf0f0f0f0);
}

/**
 * Dump the content of the registers.
 * @param hdr string that will be printed at the beginning of output lines.
 ***********************************************************************************/
void registerfile::dump(const std::string& hdr) const
{
    
    for (int32_t i = 0; i < 32; i++)
    {
        std::ostringstream os;
        os << "x" << i;
        if (i%8 == 0)
        {
            if (i) std::cout << std::endl;
            std::cout << hdr << std::right << std::setw(3) << os.str();
        }
        else if (i%4 == 0) std::cout << " ";
        std::cout << " " << to_hex32(registers.at(i));
    }
    std::cout << std::endl;
}

/**
 * Assign the value val to register r if r is not x0.
 * @param r a general-purpose register.
 * @param val a 32-bit integer value.
 ***********************************************************************************/
void registerfile::set(uint32_t r, int32_t val)
{
    if (r != 0) registers.at(r) = val;
}

/**
 * Return the value of a register r and return zero for register x0.
 ***********************************************************************************/
int32_t registerfile::get(uint32_t r) const
{
    return r ? registers.at(r) : 0;
}