#ifndef REGISTERFILE_H
#define REGISTERFILE_H
#include "rv32i_decode.h"
#include <vector>

/**
 * Store the state of the general-purpose registers of one RISC-V hart.
 ***********************************************************************************/
class registerfile : public hex
{
    public:
        void reset();
        void set(uint32_t r, int32_t val);
        int32_t get(uint32_t r) const;
        void dump(const std::string &hdr="") const;
    private:
        std::vector<int32_t> registers;
};
#endif