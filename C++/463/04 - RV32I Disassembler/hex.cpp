
#include "hex.h"
/**
 * Return a string with exactly 2 hex digits (or 8 bits)
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
    return std::string ("0x")+to_hex32(i);
    // Append "0x" at the beggining of string to_hex32(i)
}

/**
 * Return a string beginning with 0x, followed by 5 hex digits representing the 
 * 20 LSBs of the i argument
 *
 * @param i Unsigned 32 bit int input
 *
 * @return Return a std::string
 **********************************************************************************/
std::string hex::to_hex0x20(uint32_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(5) << i;
    return std::string ("0x")+os.str();
}

/**
 * Return a string beginning with 0x, followed by 3 hex digits representing the 
 * 12 LSBs of the i argument
 *
 * @param i Unsigned 32 bit int input
 *
 * @return Return a std::string
 **********************************************************************************/
std::string hex::to_hex0x12(uint32_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(3) << i;
    return std::string ("0x")+os.str();
}