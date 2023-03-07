
#include "hex.h"
/**
 * Return a string with exactly 2 hex digits 
 *
 * @param i Unsigned 8 bit int input
 *
 * @return Return a std::string
 **********************************************************************************/
std::string hex::to_hex8(uint8_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

/**
 * Return a string with exactly 8 hex digits 
 *
 * @param i Unsigned 32 bit int input
 *
 * @return Return a std::string
 **********************************************************************************/
std::string hex::to_hex32(uint32_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i;
    return os.str();
}

/**
 * Return a string with the format of 0x follow with 8 hex digits 
 *
 * @param i Unsigned 32 bit int input
 *
 * @return Return a std::string
 **********************************************************************************/
std::string hex::to_hex0x32(uint32_t i)
{
    return std::string("0x")+to_hex32(i);
}