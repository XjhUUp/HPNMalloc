/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */

/*
 *  This file contains an ISA-portable PIN tool for tracing memory accesses.
 */

#include <stdio.h>
#include "pin.H"

FILE *trace;

// Print a memory read record
VOID RecordMemRead(VOID *ip, VOID *addr) {
     //fprintf(trace, "%p: R %p\n", ip, addr); 
}

// Print a memory write record
VOID RecordMemWrite(VOID *ip, VOID *addr)
{

     //fprintf(trace, "%p: W s%p\n", ip, addr);
    if ((uint64_t)addr >= 0x600000000000 && (uint64_t)addr < 0x601000000000)
    {

       //fprintf(trace, "%p: W %p\n", ip, addr);
        printf("%p\n",addr);    
    }else if ((uint64_t)addr >= 0x610000000000 && (uint64_t)addr < 0x611000000000)
    {

       //fprintf(trace, "%p: W %p\n", ip, addr);
        printf("%p\n",addr);    
    }else if ((uint64_t)addr >= 0x620000000000 && (uint64_t)addr < 0x621000000000)
    {

       //fprintf(trace, "%p: W %p\n", ip, addr);
        printf("%p\n",addr);    
    }else if ((uint64_t)addr >= 0x630000000000 && (uint64_t)addr < 0x631000000000)
    {

       //fprintf(trace, "%p: W %p\n", ip, addr);
        printf("%p\n",addr);    
    }
    // else if ((uint64_t)addr >= 0x670000000000 && (uint64_t)addr < 0x670001000000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }else if ((uint64_t)addr >=0x671000000000 && (uint64_t)addr < 0x671001000000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }else if ((uint64_t)addr >= 0x672000000000 && (uint64_t)addr <0x672001000000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }else if ((uint64_t)addr >= 0x673000000000 && (uint64_t)addr < 0x673001000000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }
    // else if ((uint64_t)addr >= 0x674000000000 && (uint64_t)addr < 0x674000100000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }else if ((uint64_t)addr >=0x675000000000 && (uint64_t)addr < 0x675000100000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }else if ((uint64_t)addr >= 0x676000000000 && (uint64_t)addr < 0x676000100000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }else if ((uint64_t)addr >=0x677000000000 && (uint64_t)addr < 0x677000100000)
    // {

    //    //fprintf(trace, "%p: W %p\n", ip, addr);
    //     printf("%p\n",addr);    
    // }
     //fprintf(trace, "%p: W s%p\n", ip, addr);
}

// Is called for every instruction and instruments reads and writes
VOID Instruction(INS ins, VOID *v)
{
    // Instruments memory accesses using a predicated call, i.e.
    // the instrumentation is called iff the instruction will actually be executed.
    //
    // On the IA-32 and Intel(R) 64 architectures conditional moves and REP
    // prefixed instructions appear as predicated instructions in Pin.
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Iterate over each memory operand of the instruction.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        if (INS_MemoryOperandIsRead(ins, memOp))
        {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead, IARG_INST_PTR, IARG_MEMORYOP_EA, memOp,
                                     IARG_END);
        }
        // Note that in some architectures a single memory operand can be
        // both read and written (for instance incl (%eax) on IA-32)
        // In that case we instrument it once for read and once for write.
        if (INS_MemoryOperandIsWritten(ins, memOp))
        {
            INS_InsertPredicatedCall(ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite, IARG_INST_PTR, IARG_MEMORYOP_EA, memOp,
                                     IARG_END);
        }
    }
}

VOID Fini(INT32 code, VOID *v)
{
    fprintf(trace, "#eof\n");
    fclose(trace);
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */

INT32 Usage()
{
    PIN_ERROR("This Pintool prints a trace of memory addresses\n" + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    if (PIN_Init(argc, argv))
        return Usage();

    trace = fopen("pinatrace.out", "w");

    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();

    return 0;
}
