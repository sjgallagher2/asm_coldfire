/* 
 * Coldfire processor support for radare2 disassembly mode
 * Copyright 2021 - sjgallagher2
 *
 * This only needs to add a few instructions (floating-point, privileged, mov3q, etc) as
 * most of the instructions are covered by capstone, see e.g. this blob:
 * https://github.com/radareorg/radare2/blob/b8f37cb0c3ba53a4ad3d330d1ac0eeb967abd19d/libr/asm/p/asm_m68k_cs.c
 */

#include <stdio.h>
#include <string.h>
#include <r_types.h>
#include <r_lib.h>
#include <r_asm.h>
#include "coldfire_decoder.h"


// Main disassembly method
static int disassemble(RAsm *a, RAsmOp *op, const ut8 *buf, int len)
{
    /* @brief
     * Motorola ColdFire disassembler (general)
     * Based on CFPRM rev. 3
     *
     * @params
     * a        Pointer to struct containing system config parameters
     * op       Pointer to struct containing op code size, bitsize, payload (=size-opsize), buffers
     *          Note: op is set by this function, needs size, buf_asm at minimum
     * buf      Byte array for block (default block size is 258 B)
     *          Note: current offset is given by a->pc
     * len      Length of file block
     * 
     * op->buf_asm is the assembly string as it would appear in an assembly program,
     * in proper syntax. See e.g. r_asm_op_set_asm()
     */
    if(len < 2) return -1;
    uint8_t buf_asm[80] = {0};
    int ret = decode_coldfire(buf,len,0,buf_asm,80);
    if(ret > 0){
//        snprintf(op->buf_asm,R_ASM_BUFSIZE,"%s %s",
//                cmd.instr, cmd.operands);
        r_asm_op_set_asm(op,buf_asm);
    }
    return op->size = ret;
}

// Plugin struct
RAsmPlugin r_asm_plugin_coldfire = {
    .name = "coldfire",
    .license = "LGPL3",
    .desc = "Motorola Coldfire disassembly plugin",
    .arch = "coldfire",
    .bits = 32,
    .endian = R_SYS_ENDIAN_LITTLE,
    .disassemble = &disassemble
};

#ifndef R2_PLUGIN_INCORE
R_API RLibStruct radare_plugin = {
    .type = R_LIB_TYPE_ASM,
    .data = &r_asm_plugin_coldfire,
    .version = R2_VERSION
};
#endif



