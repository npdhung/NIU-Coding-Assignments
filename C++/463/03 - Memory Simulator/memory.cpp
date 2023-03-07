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
    size = (s + 15)&0xfffffff0; // round the length up, mod-16
    mem.resize(size, 0xa5);
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
 * @brief Returns the size of the number of bytes in the simulated memory. 
 *
 * @param i Input address
 * 
 **********************************************************************************/
bool memory::check_illegal(uint32_t i) const
{
// Change to get_size ...    
    if (i <= size)
        return true; 
    else 
    {
        std::cout << "WARNING: Address out of range: " << hex::to_hex0x32(i) << std::endl;
        return 0;
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
    if(check_illegal(addr))
        return mem[addr];
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
    return (int32_t)memory::get8(addr);
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
    return (int32_t)memory::get16(addr);
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
    if(check_illegal(addr))
        mem[addr] = val;
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
    set8(addr + 1, val >> 8);
    set8(addr, val);
    mem[addr] = val;
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
    set16(addr + 1, val >> 8);
    set16(addr + 2, val >> 16);
    mem[addr] = val;
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
    char x;
    uint8_t i;
    int cnt = 0; 
    std::ifstream infile(fname, std::ios::in|std::ios::binary);
    if (!infile)
    {
        std::cout << "Can't open file '" << fname << "' for reading!" << std::endl;
        return false;
    }
    while(infile.get(x))    
    {                            
      if(check_illegal(cnt))
        mem[cnt] = x;
      else
      {
        std::cerr << "Program too big!" << std::endl;
        return false;
      }
      cnt++;
    }
    // May need to put this before
    infile >> std::noskipws;
    for (uint32_t addr = 0; infile >> i; ++addr)
    {
        return false;
    }
    return true;
}