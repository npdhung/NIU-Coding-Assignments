#include "memory.h"
#include "hex.h"

/**
 * @brief Rounds the size input and allotcate memory size.
 *
 * @param s Memory size input.
 * 
 **********************************************************************************/
memory::memory(uint32_t s)
{
    // round the length up, align to 4-byte boundary, mod-16
    size = (s + 15)&0xfffffff0; 
    mem.resize(size, 0xa5);     // fill memory with hex value a5
}

/**
 * @brief A clean up routine.
 *
 **********************************************************************************/
memory::~memory()
{
    mem.clear();
}

/**
 * @brief Check if the given address i does not represent an element that is 
 * present in the mem vector. 
 *
 * @param i Input address
 * @return True if i is an illegal address.
 * 
 **********************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
    if (i <= size-1)
        return false; 
    else 
    {
        std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
        return true;
    }
}

/**
 * @brief Returns the size of the number of bytes in the simulated memory. 
 *
 **********************************************************************************/
uint32_t memory::get_size() const 
{
     return size;
}

 /**
 * @brief Verifies the address and will call check_illegal().
 *
 * @param addr Input address, used to checking if is in a valid range. 
 *
 * @return true, will return the value of the bytes from the simulated memory.
 * @return 0, if addr is not in valid range. 
 **********************************************************************************/
uint8_t memory::get8(uint32_t addr) const
{
    if(!check_illegal(addr))
        return mem.at(addr);
    else 
        return 0;
}

/**
 * This function will call get8() twice, grabbing 2 bytes and doing a combination with little-endian. 
 * 
 * @param addr Input address, used for the conversion in little-endian.  
 *
 * @return x Returns 16-bit value.   
 **********************************************************************************/
uint16_t memory::get16(uint32_t addr) const
{
    uint16_t x;
    x = get8(addr) | get8(addr + 1) << 8;
    // Get 8 bits at address addr or with (8 bits from address addr+1
    // shift left 8 bits) -> little-endian
    return x;
}

/**
 * This function will call get16() twice, grabbing 2 bytes and doing a combination with little-endian. 
 * 
 * @param addr Input address, used for the conversion in little-endian.  
 *
 * @return x Returns 32-bit value.   
 **********************************************************************************/
uint32_t memory::get32(uint32_t addr) const 
{
    uint32_t x;
    x = get16(addr) | get16(addr + 2) << 16;
    // Get 32 bits value using little-endian (reverse order)
    return x;
}

/**
 * @brief Calls get8() to return a sign-extended value using a cast.
 *
 * @param addr Input address. 
 *
 * @return x, Will return the sign extention value from a 32-bit sign int.
 **********************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const
{
    uint32_t val;
    val = memory::get8(addr);
    if ((val & 0x80) == 0x80)
    {
        // If the MSB == 1 then sign extend the first 24 bits
        return (val | 0xffffff00);
    }
    else 
        return val;    
}
/**
 * @brief Calls get8() to return a sign-extended value using a cast.
 *
 * @param addr Input address. 
 *
 * @return x, Will return the sign extention value from a 16-bit sign int.
 **********************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const
{
    uint32_t val;
    val = memory::get16(addr);
    if ((val & 0x8000) == 0x8000)
    {
        // If the MSB == 1 then sign extend the first 16 bits
        return (val | 0xffff0000);
    }
    else 
        return val;    
}
/**
 * @brief Calls get8() to return a sign-extended value using a cast.
 *
 * @param addr Input address. 
 *
 * @return x, Will return the sign extention value from a 32-bit sign int.
 **********************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const
{
    return memory::get32(addr);
}

 /**
 * @brief Calls check_illegal() to verify the arguments of the address. Pass the given data
 * if the address is valid. Otherwise, discard the data and return to caller.
 *
 * @param addr Input address. 
 * @param val The data value to be passed to the valid address.
 **********************************************************************************/
void memory::set8(uint32_t addr, uint8_t val)
{
    if(!check_illegal(addr))
        mem.at(addr) = val;
}

 /**
 * @brief Calls set8() twice to store the given val in litte-endian order into the 
 * simulated memory starting at the address given in the adde argument.
 *
 * @param addr Input address. 
 * @param val The data value to be passed to the valid address.
 **********************************************************************************/
void memory::set16(uint32_t addr, uint16_t val)
{
    uint8_t val_8 = val >> 8;
    set8(addr + 1, val_8);
    set8(addr, val);
}

 /**
 * @brief Calls set16() twice to store the given val in litte-endian order into the 
 * simulated memory starting at the address given in the adde argument.
 *
 * @param addr Input address. 
 * @param val The data value to be passed to the valid address.
 **********************************************************************************/
void memory::set32(uint32_t addr, uint32_t val)
{
    uint16_t val_16 = val >> 16;
    set16(addr + 2, val_16);
    set16(addr, val);
}

 /**
 * @brief Dumps the content of the simulated memory.
 * 
 **********************************************************************************/
void memory::dump() const
{
    char ascii[17];
    ascii[16] = 0;
    for(uint32_t i=0; i < size; i++)
    {
        if(i%16 == 0)
        {
            if(i != 0)
            std::cout << " *" << ascii << "*" << std::endl;
            std::cout << hex::to_hex32(i) << ":";
        }
        uint8_t ch = get8(i);
        std::cout << (i%16 == 8?"  ":" ") << hex::to_hex8(ch);
        ascii[i%16] = isprint(ch) ? ch : '.';
    }
    std::cout << " *" << ascii << "*" << std::endl;
}

 /**
 * @brief Reads a file in binary mode and read its content into the simulated memory.   
 * @param fname Input file name  
 *
 * @return false, if the file can't be found or if the address is not valid.
 * @return true,  if file loads OK and then close the file.
 **********************************************************************************/
bool memory::load_file(const std::string &fname)
{
    uint8_t i;
    std::ifstream infile(fname, std::ios::in|std::ios::binary);
    if (!infile)
    {
        std::cout << "Can't open file '" << fname << "' for reading!" << std::endl;
        return false;
    }
    infile >> std::noskipws;
    for (uint32_t addr = 0; infile >> i; ++addr)
    {
        if(!check_illegal(addr))
            set8(addr,i);
        else
        {
            std::cerr << "Program too big!" << std::endl;
            return false;
        }
    }
    return true;
}