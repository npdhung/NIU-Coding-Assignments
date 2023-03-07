#include "cpu_single_hart.h"

/**
 * Simulate the execution of a single-hart CPU.
 * @param exec_limit reperesents the machine exucution limit.
 ***********************************************************************************/
void cpu_single_hart::run(uint64_t exec_limit)
{
    regs.set(2, mem.get_size());
    if (exec_limit == 0)
    {
        do 
        tick(); 
        while (is_halted() == false);
    }
    else
    {
        do tick();
        while (is_halted() == false && get_insn_counter() < exec_limit);
    }
    if (is_halted()) std::cout << "Execution terminated. Reason: " 
      << get_halt_reason() << std::endl;
    std::cout << get_insn_counter() << " instructions executed" << std::endl;
}