#include "coldfire_decoder.h"


/* OPCODE MASKS */
#define COLDFIRE_MASK_ORI                         0xfff8
#define COLDFIRE_MASK_BITREV                      0xfff8
#define COLDFIRE_MASK_BTST_IMM                    0xffc0
#define COLDFIRE_MASK_BTST_REG                    0xf1c0
#define COLDFIRE_MASK_BCHG_IMM                    0xffc0
#define COLDFIRE_MASK_BCHG_REG                    0xf1c0
#define COLDFIRE_MASK_BCLR_IMM                    0xffc0
#define COLDFIRE_MASK_BCLR_REG                    0xf1c0
#define COLDFIRE_MASK_BSET_IMM                    0xffc0
#define COLDFIRE_MASK_BSET_REG                    0xf1c0
#define COLDFIRE_MASK_ANDI                        0xfff8
#define COLDFIRE_MASK_BYTEREV                     0xfff8
#define COLDFIRE_MASK_SUBI                        0xfff8
#define COLDFIRE_MASK_FF1                         0xfff8
#define COLDFIRE_MASK_ADDI                        0xfff8
#define COLDFIRE_MASK_BCHG                        0xffc0
#define COLDFIRE_MASK_EORI                        0xfff8
#define COLDFIRE_MASK_CMPI                        0xff38
#define COLDFIRE_MASK_MOVE                        0xc000
#define COLDFIRE_MASK_STLDSR                      0xffffffff
#define COLDFIRE_MASK_MOVEA                       0xc1c0
#define COLDFIRE_MASK_NEGX                        0xfff8
#define COLDFIRE_MASK_MOVE_FROM_SR                0xfff8
#define COLDFIRE_MASK_LEA                         0xf1c0
#define COLDFIRE_MASK_CLR                         0xff00
#define COLDFIRE_MASK_MOVE_FROM_CCR               0xfff8
#define COLDFIRE_MASK_NEG                         0xfff8
#define COLDFIRE_MASK_MOVE_TO_CCR                 0xffc0
#define COLDFIRE_MASK_NOT                         0xfff8
#define COLDFIRE_MASK_MOVE_TO_SR                  0xffc0
#define COLDFIRE_MASK_SWAP                        0xfff8
#define COLDFIRE_MASK_PEA                         0xffc0
#define COLDFIRE_MASK_EXT                         0xfe38
#define COLDFIRE_MASK_EXTB                        0xfe38
#define COLDFIRE_MASK_MOVEM                       0xfbc0
#define COLDFIRE_MASK_TST                         0xff00
#define COLDFIRE_MASK_TAS                         0xffc0
#define COLDFIRE_MASK_HALT                        0xffff
#define COLDFIRE_MASK_PULSE                       0xffff
#define COLDFIRE_MASK_ILLEGAL                     0xffff
#define COLDFIRE_MASK_MULU_L                      0xffc08fff
#define COLDFIRE_MASK_MULS_L                      0xffc08fff
#define COLDFIRE_MASK_DIVU_L                      0xffc08ff8
#define COLDFIRE_MASK_REMU_L                      0xffc08ff8
#define COLDFIRE_MASK_DIVS_L                      0xffc08ff8
#define COLDFIRE_MASK_REMS_L                      0xffc08ff8
#define COLDFIRE_MASK_SATS                        0xfff8
#define COLDFIRE_MASK_TRAP                        0xfff0
#define COLDFIRE_MASK_LINK                        0xfff80000
#define COLDFIRE_MASK_UNLK                        0xfff8
#define COLDFIRE_MASK_MOVE_TO_USP                 0xfff8
#define COLDFIRE_MASK_MOVE_FROM_USP               0xfff8
#define COLDFIRE_MASK_NOP                         0xffff
#define COLDFIRE_MASK_STOP                        0xffff
#define COLDFIRE_MASK_RTE                         0xffff
#define COLDFIRE_MASK_RTS                         0xffff
#define COLDFIRE_MASK_MOVEC                       0xffff0000
#define COLDFIRE_MASK_JSR                         0xffc0
#define COLDFIRE_MASK_JMP                         0xffc0
#define COLDFIRE_MASK_ADDQ                        0xf1c0
#define COLDFIRE_MASK_SCC                         0xf0f8
#define COLDFIRE_MASK_SUBQ                        0xf1c0
#define COLDFIRE_MASK_TPF                         0xfff8
#define COLDFIRE_MASK_BRA                         0xff00
#define COLDFIRE_MASK_BSR                         0xff00
#define COLDFIRE_MASK_BCC                         0xf000
#define COLDFIRE_MASK_MOVEQ                       0xf100
#define COLDFIRE_MASK_MVS                         0xf180
#define COLDFIRE_MASK_MVZ                         0xf180
#define COLDFIRE_MASK_OR                          0xf000
#define COLDFIRE_MASK_DIVU_W                      0xf1c0
#define COLDFIRE_MASK_DIVS_W                      0xf1c0
#define COLDFIRE_MASK_SUB                         0xf000
#define COLDFIRE_MASK_SUBX                        0xf1f8
#define COLDFIRE_MASK_SUBA                        0xf1c0
#define COLDFIRE_MASK_MAC_MAC                     0xf1b00113
#define COLDFIRE_MASK_MAC_EMAC                    0xf1300103
#define COLDFIRE_MASK_MAC_WITH_LOAD_MAC           0xf0c00103
#define COLDFIRE_MASK_MAC_WITH_LOAD_EMAC          0xf1000100
#define COLDFIRE_MASK_MAAAC_EMAC_B                0xf1300103
#define COLDFIRE_MASK_MSAAC_EMAC_B                0xf1300103
#define COLDFIRE_MASK_MSAC_MAC                    0xf1b00113
#define COLDFIRE_MASK_MSAC_EMAC                   0xf1300103
#define COLDFIRE_MASK_MSAC_WITH_LOAD_MAC          0xf1800110
#define COLDFIRE_MASK_MSAC_WITH_LOAD_EMAC         0xf1000100
#define COLDFIRE_MASK_MSSAC_EMAC_B                0xf1300103
#define COLDFIRE_MASK_MOVE_TO_ACC_MAC             0xffc0
#define COLDFIRE_MASK_MOVE_TO_ACC_EMAC            0xf9c0
#define COLDFIRE_MASK_MOVE_ACC_TO_ACC_EMAC        0xf9fc
#define COLDFIRE_MASK_MOVE_FROM_ACC_MAC           0xfff0
#define COLDFIRE_MASK_MOVE_FROM_ACC_EMAC          0xf9f0
#define COLDFIRE_MASK_MOVCLR_EMAC                 0xf9f0
#define COLDFIRE_MASK_MOVE_FROM_MACSR             0xfff0
#define COLDFIRE_MASK_MOVE_TO_MACSR               0xfff0
#define COLDFIRE_MASK_MOVE_FROM_MACSR_TO_CCR      0xffff
#define COLDFIRE_MASK_MOVE_TO_ACCEXT01_EMAC       0xffc0
#define COLDFIRE_MASK_MOVE_FROM_ACCEXT01_EMAC     0xfff0
#define COLDFIRE_MASK_MOVE_TO_MASK                0xffc0
#define COLDFIRE_MASK_MOVE_FROM_MASK              0xfff0
#define COLDFIRE_MASK_MOVE_TO_ACCEXT23_EMAC       0xffc0
#define COLDFIRE_MASK_MOVE_FROM_ACCEXT23_EMAC     0xfff0
#define COLDFIRE_MASK_MOV3Q                       0xf1c0
#define COLDFIRE_MASK_CMP                         0xf000
#define COLDFIRE_MASK_CMPA                        0xf000
#define COLDFIRE_MASK_EOR                         0xf1c0
#define COLDFIRE_MASK_AND                         0xf000
#define COLDFIRE_MASK_MULU_W                      0xf1c0
#define COLDFIRE_MASK_MULS_W                      0xf1c0
#define COLDFIRE_MASK_ADD                         0xf000
#define COLDFIRE_MASK_ADDX                        0xf1f8
#define COLDFIRE_MASK_ADDA                        0xf1c0
#define COLDFIRE_MASK_ASL                         0xf1d8
#define COLDFIRE_MASK_ASR                         0xf1d8
#define COLDFIRE_MASK_LSL                         0xf1d8
#define COLDFIRE_MASK_LSR                         0xf1d8
#define COLDFIRE_MASK_FMOVE_1                     0xffc0a000
#define COLDFIRE_MASK_FMOVE_2                     0xffc0e07f
#define COLDFIRE_MASK_FINT                        0xffc0a07f
#define COLDFIRE_MASK_FINTRZ                      0xffc0a07f
#define COLDFIRE_MASK_FSQRT                       0xffc0a000
#define COLDFIRE_MASK_FABS                        0xffc0a000
#define COLDFIRE_MASK_FNEG                        0xffc0a000
#define COLDFIRE_MASK_FDIV                        0xffc0a000
#define COLDFIRE_MASK_FADD                        0xffc0a000
#define COLDFIRE_MASK_FMUL                        0xffc0a000
#define COLDFIRE_MASK_FSUB                        0xffc0a000
#define COLDFIRE_MASK_FCMP                        0xffc0a07f
#define COLDFIRE_MASK_FTST                        0xffc0a07f
#define COLDFIRE_MASK_FBCC                        0xff80
#define COLDFIRE_MASK_FMOVE_TO_FPIAR              0xffc0ffff
#define COLDFIRE_MASK_FMOVE_TO_FPSR               0xffc0ffff
#define COLDFIRE_MASK_FMOVE_TO_FPCR               0xffc0ffff
#define COLDFIRE_MASK_FMOVE_FROM_FPIAR            0xffc0ffff
#define COLDFIRE_MASK_FMOVE_FROM_FPSR             0xffc0ffff
#define COLDFIRE_MASK_FMOVE_FROM_FPCR             0xffc0ffff
#define COLDFIRE_MASK_FMOVEM                      0xffc0df00
#define COLDFIRE_MASK_FNOP                        0xffc0ffff
#define COLDFIRE_MASK_FSAVE                       0xffc0
#define COLDFIRE_MASK_FRESTORE                    0xffc0
#define COLDFIRE_MASK_INTOUCH                     0xfff8
#define COLDFIRE_MASK_CPUSHL                      0xff38
#define COLDFIRE_MASK_WDDATA                      0xff00
#define COLDFIRE_MASK_WDEBUG                      0xffc0ffff

/* OPCODE IDENTIFIERS */
#define COLDFIRE_CODE_ORI                         0x80
#define COLDFIRE_CODE_BITREV                      0xc0
#define COLDFIRE_CODE_BTST_IMM                    0x800
#define COLDFIRE_CODE_BTST_REG                    0x100
#define COLDFIRE_CODE_BCLR_IMM                    0x880
#define COLDFIRE_CODE_BCLR_REG                    0x180
#define COLDFIRE_CODE_BSET_IMM                    0x8c0
#define COLDFIRE_CODE_BSET_REG                    0x1c0
#define COLDFIRE_CODE_BCHG_IMM                    0x840
#define COLDFIRE_CODE_BCHG_REG                    0x140
#define COLDFIRE_CODE_ANDI                        0x280
#define COLDFIRE_CODE_BYTEREV                     0x2c0
#define COLDFIRE_CODE_SUBI                        0x480
#define COLDFIRE_CODE_FF1                         0x4c0
#define COLDFIRE_CODE_ADDI                        0x680
#define COLDFIRE_CODE_EORI                        0xa80
#define COLDFIRE_CODE_CMPI                        0xc00
#define COLDFIRE_CODE_MOVE                        0x0
#define COLDFIRE_CODE_STLDSR                      0x40e746fc
#define COLDFIRE_CODE_MOVEA                       0x40
#define COLDFIRE_CODE_NEGX                        0x4080
#define COLDFIRE_CODE_MOVE_FROM_SR                0x40c0
#define COLDFIRE_CODE_LEA                         0x41c0
#define COLDFIRE_CODE_CLR                         0x4200
#define COLDFIRE_CODE_MOVE_FROM_CCR               0x42c0
#define COLDFIRE_CODE_NEG                         0x4480
#define COLDFIRE_CODE_MOVE_TO_CCR                 0x44c0
#define COLDFIRE_CODE_NOT                         0x4680
#define COLDFIRE_CODE_MOVE_TO_SR                  0x46c0
#define COLDFIRE_CODE_SWAP                        0x4840
#define COLDFIRE_CODE_PEA                         0x4840
#define COLDFIRE_CODE_EXT                         0x4800
#define COLDFIRE_CODE_EXTB                        0x4800
#define COLDFIRE_CODE_MOVEM                       0x48c0
#define COLDFIRE_CODE_TST                         0x4a00
#define COLDFIRE_CODE_TAS                         0x4ac0
#define COLDFIRE_CODE_HALT                        0x4ac8
#define COLDFIRE_CODE_PULSE                       0x4acc
#define COLDFIRE_CODE_ILLEGAL                     0x4afc
#define COLDFIRE_CODE_MULU_L                      0x4c000000
#define COLDFIRE_CODE_MULS_L                      0x4c000800
#define COLDFIRE_CODE_DIVU_L                      0x4c400000
#define COLDFIRE_CODE_REMU_L                      0x4c400000
#define COLDFIRE_CODE_DIVS_L                      0x4c400800
#define COLDFIRE_CODE_REMS_L                      0x4c400800
#define COLDFIRE_CODE_SATS                        0x4c80
#define COLDFIRE_CODE_TRAP                        0x4e40
#define COLDFIRE_CODE_LINK                        0x4e500000
#define COLDFIRE_CODE_UNLK                        0x4e58
#define COLDFIRE_CODE_MOVE_TO_USP                 0x4e60
#define COLDFIRE_CODE_MOVE_FROM_USP               0x4e68
#define COLDFIRE_CODE_NOP                         0x4e71
#define COLDFIRE_CODE_STOP                        0x4e72
#define COLDFIRE_CODE_RTE                         0x4e73
#define COLDFIRE_CODE_RTS                         0x4e75
#define COLDFIRE_CODE_MOVEC                       0x4e7b0000
#define COLDFIRE_CODE_JSR                         0x4e80
#define COLDFIRE_CODE_JMP                         0x4ec0
#define COLDFIRE_CODE_ADDQ                        0x5080
#define COLDFIRE_CODE_SCC                         0x50c0
#define COLDFIRE_CODE_SUBQ                        0x5180
#define COLDFIRE_CODE_TPF                         0x51f8
#define COLDFIRE_CODE_BRA                         0x6000
#define COLDFIRE_CODE_BSR                         0x6100
#define COLDFIRE_CODE_BCC                         0x6000
#define COLDFIRE_CODE_MOVEQ                       0x7000
#define COLDFIRE_CODE_MVS                         0x7100
#define COLDFIRE_CODE_MVZ                         0x7180
#define COLDFIRE_CODE_OR                          0x8000
#define COLDFIRE_CODE_DIVU_W                      0x80c0
#define COLDFIRE_CODE_DIVS_W                      0x81c0
#define COLDFIRE_CODE_SUB                         0x9000
#define COLDFIRE_CODE_SUBX                        0x9180
#define COLDFIRE_CODE_SUBA                        0x91c0
#define COLDFIRE_CODE_MAC_MAC                     0xa0000000
#define COLDFIRE_CODE_MAC_EMAC                    0xa0000000
#define COLDFIRE_CODE_MAC_WITH_LOAD_MAC           0xa0400001
#define COLDFIRE_CODE_MAC_WITH_LOAD_EMAC          0xa0000000
#define COLDFIRE_CODE_MAAAC_EMAC_B                0xa0000001
#define COLDFIRE_CODE_MSAAC_EMAC_B                0xa0000101
#define COLDFIRE_CODE_MSAC_MAC                    0xa0000100
#define COLDFIRE_CODE_MSAC_EMAC                   0xa0000100
#define COLDFIRE_CODE_MSAC_WITH_LOAD_MAC          0xa0800100
#define COLDFIRE_CODE_MSAC_WITH_LOAD_EMAC         0xa0000100
#define COLDFIRE_CODE_MSSAC_EMAC_B                0xa0000103
#define COLDFIRE_CODE_MOVE_TO_ACC_MAC             0xa100
#define COLDFIRE_CODE_MOVE_TO_ACC_EMAC            0xa100
#define COLDFIRE_CODE_MOVE_ACC_TO_ACC_EMAC        0xa110
#define COLDFIRE_CODE_MOVE_FROM_ACC_MAC           0xa180
#define COLDFIRE_CODE_MOVE_FROM_ACC_EMAC          0xa180
#define COLDFIRE_CODE_MOVCLR_EMAC                 0xa1c0
#define COLDFIRE_CODE_MOVE_FROM_MACSR             0xa980
#define COLDFIRE_CODE_MOVE_TO_MACSR               0xa980
#define COLDFIRE_CODE_MOVE_FROM_MACSR_TO_CCR      0xa9c0
#define COLDFIRE_CODE_MOVE_TO_ACCEXT01_EMAC       0xab00
#define COLDFIRE_CODE_MOVE_FROM_ACCEXT01_EMAC     0xab80
#define COLDFIRE_CODE_MOVE_TO_MASK                0xad00
#define COLDFIRE_CODE_MOVE_FROM_MASK              0xad80
#define COLDFIRE_CODE_MOVE_TO_ACCEXT23_EMAC       0xaf00
#define COLDFIRE_CODE_MOVE_FROM_ACCEXT23_EMAC     0xaf80
#define COLDFIRE_CODE_MOV3Q                       0xa140
#define COLDFIRE_CODE_CMP                         0xb000
#define COLDFIRE_CODE_CMPA                        0xb000
#define COLDFIRE_CODE_EOR                         0xb180
#define COLDFIRE_CODE_AND                         0xc000
#define COLDFIRE_CODE_MULU_W                      0xc0c0
#define COLDFIRE_CODE_MULS_W                      0xc1c0
#define COLDFIRE_CODE_ADD                         0xd000
#define COLDFIRE_CODE_ADDX                        0xd180
#define COLDFIRE_CODE_ADDA                        0xd1c0
#define COLDFIRE_CODE_ASL                         0xe180
#define COLDFIRE_CODE_ASR                         0xe080
#define COLDFIRE_CODE_LSL                         0xe188
#define COLDFIRE_CODE_LSR                         0xe088
#define COLDFIRE_CODE_FMOVE_1                     0xf2000000
#define COLDFIRE_CODE_FMOVE_2                     0xf2006000
#define COLDFIRE_CODE_FINT                        0xf2000001
#define COLDFIRE_CODE_FINTRZ                      0xf2000003
#define COLDFIRE_CODE_FSQRT                       0xf2000000
#define COLDFIRE_CODE_FABS                        0xf2000000
#define COLDFIRE_CODE_FNEG                        0xf2000000
#define COLDFIRE_CODE_FDIV                        0xf2000000
#define COLDFIRE_CODE_FADD                        0xf2000000
#define COLDFIRE_CODE_FMUL                        0xf2000000
#define COLDFIRE_CODE_FSUB                        0xf2000000
#define COLDFIRE_CODE_FCMP                        0xf2000038
#define COLDFIRE_CODE_FTST                        0xf200003a
#define COLDFIRE_CODE_FBCC                        0xf280
#define COLDFIRE_CODE_FMOVE_TO_FPIAR              0xf2008400
#define COLDFIRE_CODE_FMOVE_TO_FPSR               0xf2008800
#define COLDFIRE_CODE_FMOVE_TO_FPCR               0xf2009000
#define COLDFIRE_CODE_FMOVE_FROM_FPIAR            0xf200a400
#define COLDFIRE_CODE_FMOVE_FROM_FPSR             0xf200a800
#define COLDFIRE_CODE_FMOVE_FROM_FPCR             0xf200b000
#define COLDFIRE_CODE_FMOVEM                      0xf200d000
#define COLDFIRE_CODE_FNOP                        0xf3000000
#define COLDFIRE_CODE_FSAVE                       0xf340
#define COLDFIRE_CODE_FRESTORE                    0xf340
#define COLDFIRE_CODE_INTOUCH                     0xf428
#define COLDFIRE_CODE_CPUSHL                      0xf428
#define COLDFIRE_CODE_WDDATA                      0xfb00
#define COLDFIRE_CODE_WDEBUG                      0xfbc00003

// Effective addressing modes
#define COLDFIRE_ADDR_MODE_REG_DIRECT_DATA      0x00
#define COLDFIRE_ADDR_MODE_REG_DIRECT_ADDR      0x1
#define COLDFIRE_ADDR_MODE_REG_INDIR_ADDR       0x2
#define COLDFIRE_ADDR_MODE_REG_INDIR_ADDR_W_POSTINC 0x3
#define COLDFIRE_ADDR_MODE_REG_INDIR_ADDR_W_PREDEC 0x4
#define COLDFIRE_ADDR_MODE_REG_INDIR_ADDR_W_DISPL 0x5
#define COLDFIRE_ADDR_MODE_ADDR_REG_INDIR_W_SCALED_IDX_AND_DISPL 0x6
#define COLDFIRE_ADDR_MODE_PC_INDIR_W_DISPL     0x7
#define COLDFIRE_ADDR_MODE_PC_INDIR_W_SCALED_IDX_AND_DISPL 0x7
#define COLDFIRE_ADDR_MODE_ABS_DATA_SHORT       0x7
#define COLDFIRE_ADDR_MODE_ABS_DATA_LONG        0x7
#define COLDFIRE_ADDR_MODE_IMMEDIATE            0x7
// Effective Address register codes
#define COLDFIRE_EA_REG_0   0x0
#define COLDFIRE_EA_REG_1   0x1
#define COLDFIRE_EA_REG_2   0x2
#define COLDFIRE_EA_REG_3   0x3
#define COLDFIRE_EA_REG_4   0x4
#define COLDFIRE_EA_REG_5   0x5
#define COLDFIRE_EA_REG_6   0x6
#define COLDFIRE_EA_REG_7   0x7
#define COLDFIRE_EA_REG_PC_W_DISPL  0x2
#define COLDFIRE_EA_REG_PC_W_SCALED_IDX_AND_DISPL   0x3
#define COLDFIRE_EA_REG_ABS_DATA_SHORT  0x0
#define COLDFIRE_EA_REG_ABS_DATA_LONG   0x1
#define COLDFIRE_EA_REG_IMMEDIATE       0x4
// Effective Address order
#define COLDFIRE_EA_ORDER_REG_MODE  0x00
#define COLDFIRE_EA_ORDER_MODE_REG  0x01
// Lengths of words, longwords, and longlongwords
#define COLDFIRE_WORD   2
#define COLDFIRE_LONGWORD   4
#define COLDFIRE_LONGLONGWORD   6


void fmt_condition(uint8_t *out, int olen, uint8_t ccbyte)
{
    // Given condition byte provide condition code characters
    switch(ccbyte)
    {
    case 0x4: // cc
        snprintf(out,olen,"cc");
        break;
    case 0x5: // cs
        snprintf(out,olen,"cs");
        break;
    case 0x7: // eq
        snprintf(out,olen,"eq");
        break;
    case 0x1: // f
        snprintf(out,olen,"f");
        break;
    case 0xc: // ge
        snprintf(out,olen,"ge");
        break;
    case 0xe: // gt
        snprintf(out,olen,"gt");
        break;
    case 0x2: // hi
        snprintf(out,olen,"hi");
        break;
    case 0xf: // le
        snprintf(out,olen,"le");
        break;
    case 0x3: // ls
        snprintf(out,olen,"ls");
        break;
    case 0xd: // lt
        snprintf(out,olen,"lt");
        break;
    case 0xb: // mi
        snprintf(out,olen,"mi");
        break;
    case 0x6: // ne
        snprintf(out,olen,"ne");
        break;
    case 0xa: // pl
        snprintf(out,olen,"pl");
        break;
    case 0x0: // t
        snprintf(out,olen,"t");
        break;
    case 0x8: // vc
        snprintf(out,olen,"vc");
        break;
    case 0x9: // vs
        snprintf(out,olen,"vs");
        break;
    }
}

/* Format effective address
 * Returns number of extended words used (0, 1, 2, or -1 for incomplete)
 * 
 * order is either COLDFIRE_EA_ORDER_REG_MODE or COLDFIRE_EA_ORDER_MODE_REG depending
 * on effective address bit order.
 */
int fmt_eaddr(uint8_t *out,int olen, uint8_t eabyte, uint8_t order,uint64_t pc, 
            uint16_t extword1,uint16_t extword2,uint8_t HAS_32,uint8_t HAS_48,
            char opsz)
{
    // Format effective address given EA byte of instruction word
    uint8_t mode=0,reg=0;
    if(order == COLDFIRE_EA_ORDER_REG_MODE)
    {
        reg = (eabyte & 0x38)>>3; // xx111xxxb
        mode = eabyte & 7;     // xxxxx111b
    }
    else if(order == COLDFIRE_EA_ORDER_MODE_REG)
    {
        mode = (eabyte & 0x38)>>3; // xx111xxxb
        reg = eabyte & 7;     // xxxxx111b
    }

    if (mode == COLDFIRE_ADDR_MODE_REG_DIRECT_DATA){
        snprintf(out,olen,"d%x",reg);
        return 0;
    }
    else if (mode == COLDFIRE_ADDR_MODE_REG_DIRECT_ADDR ){
        snprintf(out,olen,"a%x",reg);
        return 0;
    }
    else if (mode == COLDFIRE_ADDR_MODE_REG_INDIR_ADDR ){
        snprintf(out,olen,"(a%x)",reg);
        return 0;
    }
    else if (mode == COLDFIRE_ADDR_MODE_REG_INDIR_ADDR_W_POSTINC ){
        snprintf(out,olen,"(a%x)+",reg);
        return 0;
    }
    else if (mode == COLDFIRE_ADDR_MODE_REG_INDIR_ADDR_W_PREDEC ){
        snprintf(out,olen,"-(a%x)",reg);
        return 0;
    }

    // EXTENDED MODES
    else if (mode == COLDFIRE_ADDR_MODE_REG_INDIR_ADDR_W_DISPL)
    {
        if(HAS_32)
            snprintf(out,olen,"(0x%x,a%x)",extword1,reg);
        else
            return -1; // incomplete
        return 1;
    }
    else if (mode == COLDFIRE_ADDR_MODE_ADDR_REG_INDIR_W_SCALED_IDX_AND_DISPL ){
        if(HAS_48)
        {
            uint8_t da = (extword1 & 0x8000)>0; // use Dx or Ax register (for 0 and 1 resp)
            uint8_t extreg = (extword1 & 0x7000)>>12; // index register number
            uint8_t wl = (extword1 & 0x800)>0; // word/longword; if 0, 8-bit extnension; if 1, next word is extension
            uint8_t scale = (extword1 & 0x600)>>9; // scale factor
            uint8_t disp8 = extword1 & 0xff;
            snprintf(out,olen,"()"); // TODO
        }
        else 
            return -1; // incomplete
        return 1;
    }
    else if (mode == COLDFIRE_ADDR_MODE_PC_INDIR_W_DISPL && reg == COLDFIRE_EA_REG_PC_W_DISPL){
        if(HAS_32)
            snprintf(out,olen,"(0x%x,pc)",extword1);
        else
            return -1; // incomplete
        return 1;
    }
    else if (mode == COLDFIRE_ADDR_MODE_PC_INDIR_W_SCALED_IDX_AND_DISPL && reg == COLDFIRE_EA_REG_PC_W_SCALED_IDX_AND_DISPL){
        //uint64_t ea = pc + Xi*ScaleFactor + d8_sign_extended;
        snprintf(out,olen,"()"); // TODO
    }
    else if (mode == COLDFIRE_ADDR_MODE_ABS_DATA_SHORT && reg == COLDFIRE_EA_REG_ABS_DATA_SHORT ){
        if(HAS_32)
            snprintf(out,olen,"(0x%x).w",extword1);
        else 
            return -1;
        return 1;
    }
    else if (mode == COLDFIRE_ADDR_MODE_ABS_DATA_LONG && reg == COLDFIRE_EA_REG_ABS_DATA_LONG){
        if(HAS_48)
            snprintf(out,olen,"(0x%x%02x).l",extword1,extword2);
        else 
            return -1;
        return 2;
    }
    else if (mode == COLDFIRE_ADDR_MODE_IMMEDIATE && reg == COLDFIRE_EA_REG_IMMEDIATE){
        if(opsz == 'b' && HAS_32)
        {
            snprintf(out,olen,"#0x%x",extword1 & 0xff);
            return 1;
        }
        else if(opsz == 'w' && HAS_32)
        {
            snprintf(out,olen,"#0x%x",extword1);
            return 1;
        }
        else if(opsz == 'l' && HAS_48)
        {
            if(extword1 != 0)
                snprintf(out,olen,"#0x%x%04x",extword1,extword2);
            else 
                snprintf(out,olen,"#0x%x",extword2);

            return 2;
        }
        else
            return -1;
        
    }
    return 0; // Success
}

/* decode_coldfire
 * @brief Convert bytes to a ColdFire ISA C instruction
 * 
 * @description
 * Use the first 2, 4, or 6 bytes of buffer buf (length of buf = len) starting at 
 * program counter pc, to decode an instruction according to ColdFire ISA_C.
 * Only partially implements MAC/EMAC and floating-point. Instruction addressing 
 * modes are not checked for validity; i.e. ColdFire instructions typically support
 * only a subset of the effective addressing modes, but this decoding method will
 * not throw an error.
 * 
 * @returns
 * Number of bytes occupied by instruction
 * 
 * TODO address 0x4a28 is causing a seg fault 
 * 0x4a24    66ec b682 6662 b882 665e 7218 e3aa ccbc 00ff ffff
 * 
 */
int decode_coldfire(const uint8_t *buf, const int BUF_LEN, int pc, uint8_t *asm_buf, const int ASM_BUF_LEN)
{
    // Decode coldfire instruction
    uint8_t HAS_16 = 0,HAS_32 = 0,HAS_48=0;
    uint16_t code16 = 0;
    uint64_t code32 = 0;
    uint64_t code48 = 0;
    code16 = ((uint16_t)(buf[pc])<<8) + ((uint16_t)(buf[pc+1]));
    HAS_16 = 1;
    if(pc+COLDFIRE_WORD < BUF_LEN){
        code32 = ((uint32_t)(buf[pc])<<24) + 
            ((uint32_t)(buf[pc+1])<<16) + 
            ((uint32_t)(buf[pc+2])<<8) + 
            ((uint32_t)(buf[pc+3]));
        HAS_32 = 1;
    }
    if(pc+COLDFIRE_LONGWORD < BUF_LEN){
        code48 = ((uint64_t)(buf[pc])<<(40)) + 
            ((uint64_t)(buf[pc+1])<<(32)) + 
            ((uint64_t)(buf[pc+2])<<(24)) + 
            ((uint64_t)(buf[pc+3])<<(16)) + 
            ((uint64_t)(buf[pc+4])<<(8)) + 
            ((uint64_t)(buf[pc+5]));
        HAS_48 = 1;
    }

    // Effective address formatted string
    uint8_t fmtted1[64] = {0};
    uint8_t fmtted2[64] = {0};
    const uint8_t flen = 64;

    /***********************
     * Opcodes in order of most significant mask bits 
     **********************/
    if( code48 && (code32 & COLDFIRE_MASK_STLDSR) == COLDFIRE_CODE_STLDSR){
        // handle instruction
        /* 1 word of data */ 
        snprintf(asm_buf,ASM_BUF_LEN,"stldsr #0x%x",code48 & 0x00000000ffff);
        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MOVEC) == COLDFIRE_CODE_MOVEC){
        // handle instruction
        uint8_t ad = (code32 & 0x8000) > 0;
        uint8_t Ry = (code32 & 0x7000) >> 12;
        uint16_t Rc = (code32 & 0xfff);
        char regt = ad ? 'a' : 'd';
        uint8_t ctrl_reg[9] = {0};
        switch(Rc)
        {
        case 0x2: // cacr
            snprintf(ctrl_reg,9,"cacr");
            break;
        case 0x3: // asid
            snprintf(ctrl_reg,9,"asid");
            break;
        case 0x4: // acr0
            snprintf(ctrl_reg,9,"acr0");
            break;
        case 0x5: // acr1
            snprintf(ctrl_reg,9,"acr1");
            break;
        case 0x6: // acr2
            snprintf(ctrl_reg,9,"acr2");
            break;
        case 0x7: // acr3
            snprintf(ctrl_reg,9,"acr3");
            break;
        case 0x8: // mmubar
            snprintf(ctrl_reg,9,"mmubar");
            break;
        case 0x801: // vbr
            snprintf(ctrl_reg,9,"vbr");
            break;
        case 0x80f: // pc
            snprintf(ctrl_reg,9,"pc");
            break;
        case 0xc00: // rombar0
            snprintf(ctrl_reg,9,"rombar0");
            break;
        case 0xc01: // rombar1
            snprintf(ctrl_reg,9,"rombar1");
            break;
        case 0xc04: // rambar0
            snprintf(ctrl_reg,9,"rambar0");
            break;
        case 0xc05: // rambar1
            snprintf(ctrl_reg,9,"rambar1");
            break;
        case 0xc0c: // mpcr
            snprintf(ctrl_reg,9,"mpcr");
            break;
        case 0xc0d: // edrambar
            snprintf(ctrl_reg,9,"edrambar");
            break;
        case 0xc0e: // secmbar
            snprintf(ctrl_reg,9,"secmbar");
            break;
        case 0xc0f: // mbar
            snprintf(ctrl_reg,9,"mbar");
            break;
        case 0xd02: // pcr1u0
            snprintf(ctrl_reg,9,"pcr1u0");
            break;
        case 0xd03: // pcr1l0
            snprintf(ctrl_reg,9,"pcr1l0");
            break;
        case 0xd04: // pcr2u0
            snprintf(ctrl_reg,9,"pcr2u0");
            break;
        case 0xd05: // pcr2l0
            snprintf(ctrl_reg,9,"pcr2l0");
            break;
        case 0xd06: // pcr3u0
            snprintf(ctrl_reg,9,"pcr3u0");
            break;
        case 0xd07: // pcr3l0
            snprintf(ctrl_reg,9,"pcr3l0");
            break;
        case 0xd0a: // pcr1u1
            snprintf(ctrl_reg,9,"pcr1u1");
            break;
        case 0xd0b: // pcr1l1
            snprintf(ctrl_reg,9,"pcr1l1");
            break;
        case 0xd0c: // pcr2u1
            snprintf(ctrl_reg,9,"pcr2u1");
            break;
        case 0xd0d: // pcr2l1
            snprintf(ctrl_reg,9,"pcr2l1");
            break;
        case 0xd0e: // pcr3u1
            snprintf(ctrl_reg,9,"pcr3u1");
            break;
        case 0xd0f: // pcr3l1
            snprintf(ctrl_reg,9,"pcr3l1");
            break;
        }
        snprintf(asm_buf,ASM_BUF_LEN,"movec %c%x,%s",regt,Ry,ctrl_reg); // Ry,Rc
        return COLDFIRE_LONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_NOP) == COLDFIRE_CODE_NOP){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"nop");
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_STOP) == COLDFIRE_CODE_STOP){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"stop");
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_RTE) == COLDFIRE_CODE_RTE){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"rte");
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_RTS) == COLDFIRE_CODE_RTS){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"rts");
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_HALT) == COLDFIRE_CODE_HALT){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"halt");
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_PULSE) == COLDFIRE_CODE_PULSE){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"pulse");
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_ILLEGAL) == COLDFIRE_CODE_ILLEGAL){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"illegal");
        return COLDFIRE_WORD; 
    }
    else if( (code32 & COLDFIRE_MASK_FNOP) == COLDFIRE_CODE_FNOP){
        // handle instruction
        snprintf(asm_buf,ASM_BUF_LEN,"fnop");
        return COLDFIRE_LONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_MACSR_TO_CCR) == COLDFIRE_CODE_MOVE_FROM_MACSR_TO_CCR){
        // handle instruction
        /* NO OPERANDS */ 
        snprintf(asm_buf,ASM_BUF_LEN,"move.l macsr,ccr");
        return COLDFIRE_WORD; 
    }
    else if( code48 && (code16 & COLDFIRE_MASK_ORI) == COLDFIRE_CODE_ORI){
        // handle instruction
        // last three bits are Dx, then longword of data (upper then lower)
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"ori.l #0x%x,d%x",data,Dx);
        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_BITREV) == COLDFIRE_CODE_BITREV){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"bitrev.l d%x",Dx);
        return COLDFIRE_WORD;
    }
    else if( (code16 & COLDFIRE_MASK_ANDI) == COLDFIRE_CODE_ANDI){
        // handle instruction
        // last three bits are Dx, then longword of data (upper then lower)
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"andi.l #0x%x,d%x",data,Dx);

        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_BYTEREV) == COLDFIRE_CODE_BYTEREV){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"byterev d%x",Dx);

        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_SUBI) == COLDFIRE_CODE_SUBI){
        // handle instruction
        // last three bits are Dx, then longword of data (upper then lower)
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"subi.l #0x%x,d%x",data,Dx);

        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_FF1) == COLDFIRE_CODE_FF1){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"ff1 #0x%x,d%x",data,Dx);
        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_ADDI) == COLDFIRE_CODE_ADDI){
        // handle instruction
        // last three bits are Dx, then longword of data (upper then lower)
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"addi.l #0x%x,d%x",data,Dx);

        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_EORI) == COLDFIRE_CODE_EORI){
        // handle instruction
        // last three bits are Dx, then longword of data (upper then lower)
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"eori.l #0x%x,d%x",data,Dx);

        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_CMPI) == COLDFIRE_CODE_CMPI){
        // handle instruction
        // last three bits are Dx, then longword of data (upper then lower)
        uint8_t Dx = code16 & 0x7;
        uint64_t data = code48 & 0xffffffff;
        snprintf(asm_buf,ASM_BUF_LEN,"cmpi.l #0x%x,d%x",data,Dx);

        return COLDFIRE_LONGLONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_INTOUCH) == COLDFIRE_CODE_INTOUCH){
        // handle instruction
        uint8_t Ax = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"intouch (a%x)",Ax);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_NEGX) == COLDFIRE_CODE_NEGX){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"negx.l d%x",Dx);
        
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_SR) == COLDFIRE_CODE_MOVE_FROM_SR){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.w sr,d%x",Dx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_CCR) == COLDFIRE_CODE_MOVE_FROM_CCR){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.w ccr,d%x",Dx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_NEG) == COLDFIRE_CODE_NEG){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"neg.l d%x",Dx);
        
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_SATS) == COLDFIRE_CODE_SATS){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"sats.l d%x",Dx);
        
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_UNLK) == COLDFIRE_CODE_UNLK){
        // handle instruction
        uint8_t Ax = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"unlk a%x",Ax);
        
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_USP) == COLDFIRE_CODE_MOVE_TO_USP){
        // handle instruction
        uint8_t Ay = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l a%x,usp",Ay); 
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_USP) == COLDFIRE_CODE_MOVE_FROM_USP){
        // handle instruction
        uint8_t Ax = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l usp,a%x",Ax);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_NOT) == COLDFIRE_CODE_NOT){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"not.l d%x",Dx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_TPF) == COLDFIRE_CODE_TPF){
        // handle instruction
        uint8_t mode = code16 & 0x7;
        if(mode == 2){
            // one extension word
            uint32_t extword = code32 & 0xffff;
            snprintf(asm_buf,ASM_BUF_LEN,"tpf.w #0x%x",extword);
            return COLDFIRE_LONGWORD; 
        }
        else if(mode == 3){
            // two extension words
            uint64_t extwords = code48 & 0xffffffff;
            snprintf(asm_buf,ASM_BUF_LEN,"tpf.l #0x%x",extwords);
            return COLDFIRE_LONGLONGWORD; 
        }
        else if(mode == 4){
            // no extension word
            snprintf(asm_buf,ASM_BUF_LEN,"tpf");
            return COLDFIRE_WORD; 
        }
        else{
            snprintf(asm_buf,ASM_BUF_LEN,"invalid");
            return COLDFIRE_WORD;
        }

    }
    else if( (code16 & COLDFIRE_MASK_SWAP) == COLDFIRE_CODE_SWAP){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"swap.w d%x",Dx);
        return COLDFIRE_WORD; 
    }
    else if( (code32 & COLDFIRE_MASK_LINK) == COLDFIRE_CODE_LINK){
        // handle instruction
        uint8_t Ay = code16 & 0x7;
        uint16_t displ = code32 & 0xff;
        snprintf(asm_buf,ASM_BUF_LEN,"link.w A%x,#0x#x",Ay,displ);
        return COLDFIRE_LONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_ACC_MAC) == COLDFIRE_CODE_MOVE_FROM_ACC_MAC){
        // handle instruction
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l ACC,%c%x",reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_MACSR) == COLDFIRE_CODE_MOVE_FROM_MACSR){
        // handle instruction
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l macsr,%c%x",reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_ACCEXT01_EMAC) == COLDFIRE_CODE_MOVE_FROM_ACCEXT01_EMAC){
        // handle instruction
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l accext01,%c%x",reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_TRAP) == COLDFIRE_CODE_TRAP){
        // handle instruction
        uint8_t vec = code16 & 0x8;
        snprintf(asm_buf,ASM_BUF_LEN,"trap #0x%x",vec);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_ACCEXT23_EMAC) == COLDFIRE_CODE_MOVE_FROM_ACCEXT23_EMAC){
        // handle instruction
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l accext23,%c%x",reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_MASK) == COLDFIRE_CODE_MOVE_FROM_MASK){
        // handle instruction
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l mask,%c%x",reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_MASK) == COLDFIRE_CODE_MOVE_TO_MASK){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,mask",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_ACCEXT23_EMAC) == COLDFIRE_CODE_MOVE_TO_ACCEXT23_EMAC){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,accext23",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_MACSR) == COLDFIRE_CODE_MOVE_TO_MACSR){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,macsr",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_CCR) == COLDFIRE_CODE_MOVE_TO_CCR){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'b');
        snprintf(asm_buf,ASM_BUF_LEN,"move.b %s,ccr",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_SR) == COLDFIRE_CODE_MOVE_TO_SR){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'w');
        snprintf(asm_buf,ASM_BUF_LEN,"move.w %s,sr",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_PEA) == COLDFIRE_CODE_PEA){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"pea.l %s",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_TAS) == COLDFIRE_CODE_TAS){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'b');
        snprintf(asm_buf,ASM_BUF_LEN,"tas.b %s",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_FSAVE) == COLDFIRE_CODE_FSAVE){
        // handle instruction 
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,0);
        snprintf(asm_buf,ASM_BUF_LEN,"fsave %s",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_FRESTORE) == COLDFIRE_CODE_FRESTORE){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,0);
        snprintf(asm_buf,ASM_BUF_LEN,"frestore %s",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_JSR) == COLDFIRE_CODE_JSR){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,0);
        snprintf(asm_buf,ASM_BUF_LEN,"jsr %s",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_JMP) == COLDFIRE_CODE_JMP){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,0);
        snprintf(asm_buf,ASM_BUF_LEN,"jmp %s",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_ACCEXT01_EMAC) == COLDFIRE_CODE_MOVE_TO_ACCEXT01_EMAC){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,accext01",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_TO_FPIAR) == COLDFIRE_CODE_FMOVE_TO_FPIAR){
        // handle instruction
        // TODO I'm guessing the effective address data will be in the second extword
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,fpiar",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_TO_FPSR) == COLDFIRE_CODE_FMOVE_TO_FPSR){
        // handle instruction
        // TODO I'm guessing the effective address data will be in the second extword
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,fpsr",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_TO_FPCR) == COLDFIRE_CODE_FMOVE_TO_FPCR){
        // handle instruction
        // TODO I'm guessing the effective address data will be in the second extword
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,fpiar",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_FROM_FPIAR) == COLDFIRE_CODE_FMOVE_FROM_FPIAR){
        // handle instruction
        // TODO I'm guessing the effective address data will be in the second extword
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l fpiar,%s",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_FROM_FPSR) == COLDFIRE_CODE_FMOVE_FROM_FPSR){
        // handle instruction
        // TODO I'm guessing the effective address data will be in the second extword
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l fpsr,%s",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_FROM_FPCR) == COLDFIRE_CODE_FMOVE_FROM_FPCR){
        // handle instruction
        // TODO I'm guessing the effective address data will be in the second extword
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l fpcr,%s",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_WDEBUG) == COLDFIRE_CODE_WDEBUG){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"wdebug.l %s",fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_MULU_L) == COLDFIRE_CODE_MULU_L){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t Dx = (extword1 & 0xffff)<<12;
        snprintf(asm_buf,ASM_BUF_LEN,"mulu.l %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_MULS_L) == COLDFIRE_CODE_MULS_L){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t Dx = (extword1 & 0x7000)>>12;
        snprintf(asm_buf,ASM_BUF_LEN,"muls.l %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_DIVU_L) == COLDFIRE_CODE_DIVU_L){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        // TODO CFPRM shows Register,Dx in TWO locations. Do we need to check that they
        // match?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t Dx = (extword1 & 0x7000)>>12;
        snprintf(asm_buf,ASM_BUF_LEN,"divu.l %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_REMU_L) == COLDFIRE_CODE_REMU_L){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t Dx = (extword1 & 0x7000)>>12;
        uint8_t Dw = extword1 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"remu.l %s,d%x:d%x",fmtted1,Dx,Dw);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_DIVS_L) == COLDFIRE_CODE_DIVS_L){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        // TODO CFPRM shows Register,Dx in TWO locations. Do we need to check that they
        // match or is instruction otherwise unique?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t Dx = (extword1 & 0x7000)>>12;
        snprintf(asm_buf,ASM_BUF_LEN,"divs.l %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_REMS_L) == COLDFIRE_CODE_REMS_L){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t Dx = (extword1 & 0x7000)>>12;
        uint8_t Dw = extword1 & 0x7;
        snprintf(asm_buf,ASM_BUF_LEN,"rems.l %s,d%x:d%x",fmtted1,Dx,Dw);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FMOVEM) == COLDFIRE_CODE_FMOVEM){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        uint8_t dr = (extword1 & 0x2000)>0;
        uint8_t reglist = extword1 & 0xff;
        if(dr == 0)
            snprintf(asm_buf,ASM_BUF_LEN,"fmovem.l #0x%x,%s",reglist,fmtted1);
        else if(dr == 1)
            snprintf(asm_buf,ASM_BUF_LEN,"fmovem.l %s,#0x%s",fmtted1,reglist);

        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code32 & COLDFIRE_MASK_FCMP) == COLDFIRE_CODE_FCMP){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        uint8_t rm = (extword1 & 0x4000)>0;
        uint8_t src_spec = (extword1 & 0x1c00)>>10;
        uint8_t dest_reg = (extword1 & 0x380)>>7;
        char datat = '\0';


        if(rm == 0)
            snprintf(asm_buf,ASM_BUF_LEN,"fcmp.d fp%x,fp%x",src_spec,dest_reg);
        else if(rm == 1)
        {
            switch(src_spec){
            case 0:
                datat = 'l';
                break;
            case 1:
                datat = 's';
                break;
            case 4:
                datat = 'w';
                break;
            case 5:
                datat = 'd';
                break;
            case 6:
                datat = 'b';
                break;
            default:
                snprintf(asm_buf,ASM_BUF_LEN,"invalid");
                return COLDFIRE_LONGWORD;
            }
            sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,datat);
            snprintf(asm_buf,ASM_BUF_LEN,"fcmp.%c %s,#0x%s",datat,fmtted1,dest_reg);
            return COLDFIRE_WORD*(2+sz_1); 
        }
    }
    else if( (code32 & COLDFIRE_MASK_FTST) == COLDFIRE_CODE_FTST){
        // handle instruction
        // TODO Organization in memory is two doublewords, and effective address after?
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        
        uint8_t rm = (extword1 & 0x4000)>0;
        uint8_t src_spec = (extword1 & 0x1c00)>>10;
        char datat = '\0';


        if(rm == 0)
            snprintf(asm_buf,ASM_BUF_LEN,"ftst.d fp%x",src_spec);
        else if(rm == 1)
        {
            switch(src_spec){
            case 0:
                datat = 'l';
                break;
            case 1:
                datat = 's';
                break;
            case 4:
                datat = 'w';
                break;
            case 5:
                datat = 'd';
                break;
            case 6:
                datat = 'b';
                break;
            default:
                snprintf(asm_buf,ASM_BUF_LEN,"invalid");
                return COLDFIRE_LONGWORD;
            }
            sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,datat);
            snprintf(asm_buf,ASM_BUF_LEN,"ftst.%c %s",datat,fmtted1);
            return COLDFIRE_WORD*(2+sz_1); 
        }
    }
    // TODO The rest of the floating point instructions...
    else if( (code32 & COLDFIRE_MASK_FMOVE_2) == COLDFIRE_CODE_FMOVE_2){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FINT) == COLDFIRE_CODE_FINT){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FINTRZ) == COLDFIRE_CODE_FINTRZ){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FMOVE_1) == COLDFIRE_CODE_FMOVE_1){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FSQRT) == COLDFIRE_CODE_FSQRT){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FABS) == COLDFIRE_CODE_FABS){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FNEG) == COLDFIRE_CODE_FNEG){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FDIV) == COLDFIRE_CODE_FDIV){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FADD) == COLDFIRE_CODE_FADD){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FMUL) == COLDFIRE_CODE_FMUL){
        // handle instruction
    }
    else if( (code32 & COLDFIRE_MASK_FSUB) == COLDFIRE_CODE_FSUB){
        // handle instruction
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_ACC_MAC) == COLDFIRE_CODE_MOVE_TO_ACC_MAC){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,acc",fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_FBCC) == COLDFIRE_CODE_FBCC){
        // handle instruction
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_CPUSHL) == COLDFIRE_CODE_CPUSHL){
        // handle instruction
        uint8_t cache = (code16 & 0xc)>>6;
        uint8_t Ax = code16 & 0x7;
        switch(cache)
        {
        case 0: // reserved
            snprintf(asm_buf,ASM_BUF_LEN,"invalid");
            break;
        case 1: // data cache (dc)
            snprintf(asm_buf,ASM_BUF_LEN,"cpushl bc,(a%x)",Ax);
            break;
        case 2: // instruction cache (ic)
            snprintf(asm_buf,ASM_BUF_LEN,"cpushl bc,(a%x)",Ax);
            break;
        case 3: // both caches (bc) or when device has instr cache but not data cache
            snprintf(asm_buf,ASM_BUF_LEN,"cpushl bc,(a%x)",Ax);
            break;
        }
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_CLR) == COLDFIRE_CODE_CLR){
        // handle instruction
        uint8_t size = (code16 & 0xc0)>>6;
        uint8_t eabyte = code16 & 0x3f;
        char sz_char = 0;
        if(size == 0x0) // byte
            sz_char='b';
        else if(size == 0x1) // word
            sz_char='w';
        else if(size == 0x2) // longword
            sz_char='l';
        else { snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD; }
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"clr.%c %s",sz_char,fmtted1);
        return COLDFIRE_WORD*(1+sz_1);
    }
    else if( (code16 & COLDFIRE_MASK_TST) == COLDFIRE_CODE_TST){
        // handle instruction
        uint8_t size = (code16 & 0xc0)>>6;
        uint8_t eabyte = code16 & 0x3f;
        char sz_char = 0;
        if(size == 0x0) // byte
            sz_char='b';
        else if(size == 0x1) // word
            sz_char='w';
        else if(size == 0x2) // longword
            sz_char='l';
        else { snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD; }
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"tst.%c %s",sz_char,fmtted1);
        return COLDFIRE_WORD*(1+sz_1);
    }
    else if( (code16 & COLDFIRE_MASK_WDDATA) == COLDFIRE_CODE_WDDATA){
        // handle instruction
        uint8_t size = (code16 & 0xc0)>>6;
        uint8_t eabyte = code16 & 0x3f;
        char sz_char = 0;
        if(size == 0x0) // byte
            sz_char='b';
        else if(size == 0x1) // word
            sz_char='w';
        else if(size == 0x2) // longword
            sz_char='l';
        else { snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD; }
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"wddata.%c %s",sz_char,fmtted1);
        return COLDFIRE_WORD*(1+sz_1);
    }
    else if( (code16 & COLDFIRE_MASK_BRA) == COLDFIRE_CODE_BRA){
        // handle instruction
        uint8_t disp8 = code16 & 0xff;
        if(disp8 == 0)
        {
            uint16_t disp16 = (code32&0xffff);
            snprintf(asm_buf,ASM_BUF_LEN,"bra.w 0x%x",pc+disp16);
            return COLDFIRE_LONGWORD;
        }
        else if(disp8 == 0xff)
        {
            uint32_t disp32 = (code48&0xffffffff);
            snprintf(asm_buf,ASM_BUF_LEN,"bra.l 0x%x",pc+disp32);
            return COLDFIRE_LONGLONGWORD;
        }
        else
        {
            snprintf(asm_buf,ASM_BUF_LEN,"bra.b 0x%x",pc+disp8);
            return COLDFIRE_WORD; 
        }
    }
    else if( (code16 & COLDFIRE_MASK_BSR) == COLDFIRE_CODE_BSR){
        // handle instruction
        uint8_t disp8 = code16 & 0xff;
        if(disp8 == 0)
        {
            uint16_t disp16 = (code32&0xffff);
            snprintf(asm_buf,ASM_BUF_LEN,"bsr.w 0x%x",pc+disp16);
            return COLDFIRE_LONGWORD;
        }
        else if(disp8 == 0xff)
        {
            uint32_t disp32 = (code48&0xffffffff);
            snprintf(asm_buf,ASM_BUF_LEN,"bsr.l 0x%x",pc+disp32);
            return COLDFIRE_LONGLONGWORD;
        }
        else
        {
            snprintf(asm_buf,ASM_BUF_LEN,"bsr.b 0x%x",pc+disp8);
            return COLDFIRE_WORD; 
        }
    }
    else if( (code16 & COLDFIRE_MASK_EXT) == COLDFIRE_CODE_EXT){
        // handle instruction
        uint8_t opmode = (code16 & 0x1c0)>>6;
        uint8_t Dx = code16 & 0x7;
        switch(opmode)
        {
        case 2: // byte to word
            snprintf(asm_buf,ASM_BUF_LEN,"ext.w d%x",Dx);
            break;
        case 3: // word to longword
            snprintf(asm_buf,ASM_BUF_LEN,"ext.l d%x",Dx);
            break;
        case 7: // byte to longword
            snprintf(asm_buf,ASM_BUF_LEN,"extb.l d%x",Dx);
            break;
        default:
            snprintf(asm_buf,ASM_BUF_LEN,"invalid");
        return COLDFIRE_WORD;

        }
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVEM) == COLDFIRE_CODE_MOVEM){
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        uint8_t dr = (code16 & 0x400)>0;
        if(HAS_32)
            extword1 = code32 & 0xffff;
        else
            snprintf(asm_buf,ASM_BUF_LEN,"invalid");
            return COLDFIRE_WORD;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        // TODO check effective addressing
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,'l');
        if(dr == 0)
            snprintf(asm_buf,ASM_BUF_LEN,"movem.l #%x,%s",extword1,fmtted1);
        else if(dr == 1)
            snprintf(asm_buf,ASM_BUF_LEN,"movem.l %s,#%x",fmtted1,extword1);
        return COLDFIRE_WORD*(2+sz_1); 
        // handle instruction
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_ACC_TO_ACC_EMAC) == COLDFIRE_CODE_MOVE_ACC_TO_ACC_EMAC){
        // handle instruction
        uint8_t ACCx = (code16 & 0x600)>>9;
        uint8_t ACCy = code16 & 0x3;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l acc%x,acc%x",ACCx,ACCy);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_FROM_ACC_EMAC) == COLDFIRE_CODE_MOVE_FROM_ACC_EMAC){
        // handle instruction
        uint8_t ACCx = (code16 & 0x600)>>9;
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"move.l acc%x,%c%x",ACCx,reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVCLR_EMAC) == COLDFIRE_CODE_MOVCLR_EMAC){
        // handle instruction
        uint8_t ACCx = (code16 & 0x600)>>9;
        char reg = (code16 & 8) ? 'a' : 'd';
        uint8_t REGx = code16 & 7;
        snprintf(asm_buf,ASM_BUF_LEN,"moveclr.l acc%x,%c%x",ACCx,reg,REGx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE_TO_ACC_EMAC) == COLDFIRE_CODE_MOVE_TO_ACC_EMAC){
        // handle instruction
        uint8_t ACC = (code16 & 0x600)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"move.l %s,acc%x",fmtted1,ACC);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_SUBX) == COLDFIRE_CODE_SUBX){
        // handle instruction
        uint8_t Dx = (code16 & 0xE00)>>9;
        uint8_t Dy = (code16 & 0x7);

        snprintf(asm_buf,ASM_BUF_LEN,"subx.l d%x,d%x",Dy,Dx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_ADDX) == COLDFIRE_CODE_ADDX){
        // handle instruction
        uint8_t Dx = (code16 & 0xE00)>>9;
        uint8_t Dy = (code16 & 0x7);

        snprintf(asm_buf,ASM_BUF_LEN,"addx.l d%x,d%x",Dy,Dx);
        
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_SCC) == COLDFIRE_CODE_SCC){
        // handle instruction
        uint8_t cond = (code16 & 0xff0000)>>8;
        uint8_t Dx = code16 & 0x7;
        uint8_t cc[2] = {0};
        fmt_condition(cc, 2, cond);
        snprintf(asm_buf,ASM_BUF_LEN,"s%s d%x",cc,Dx);
        return COLDFIRE_WORD; 
    }
    else if( HAS_32 && (code16 & COLDFIRE_MASK_BSET_IMM) == COLDFIRE_CODE_BSET_IMM){
        // handle instruction
        // This style of operation has size dependent on the destination effective
        // address. This means we need to check the effective address mode manually
        // before formatting the effective address. 
        // This is one of two BSET instructions. 
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t bit_number = extword1 & 0xff;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';

        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"bset.%c #0x%x,%s",sz_char,bit_number,fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( HAS_32 && (code16 & COLDFIRE_MASK_BTST_IMM) == COLDFIRE_CODE_BTST_IMM){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t bit_number = extword1 & 0xff;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"btst.%c #0x%x,%s",sz_char,bit_number,fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( HAS_32 && (code16 & COLDFIRE_MASK_BCHG_IMM) == COLDFIRE_CODE_BCHG_IMM){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t bit_number = extword1 & 0xff;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"bchg.%c #0x%x,%s",sz_char,bit_number,fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( HAS_32 && (code16 & COLDFIRE_MASK_BCLR_IMM) == COLDFIRE_CODE_BCLR_IMM){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t bit_number = extword1 & 0xff;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword2,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"bclr.%c #0x%x,%s",sz_char,bit_number,fmtted1);
        return COLDFIRE_WORD*(2+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_BTST_REG) == COLDFIRE_CODE_BTST_REG){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t Dy = (code16 & 0xe00)>>9;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"btst.%c d%x,%s",sz_char,Dy,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_BCHG_REG) == COLDFIRE_CODE_BCHG_REG){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t Dy = (code16 & 0xe00)>>9;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"bchg.%c d%x,%s",sz_char,Dy,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_BCLR_REG) == COLDFIRE_CODE_BCLR_REG){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t Dy = (code16 & 0xe00)>>9;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"bclr.%c d%x,%s",sz_char,Dy,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_BSET_REG) == COLDFIRE_CODE_BSET_REG){
        // handle instruction
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t Dy = (code16 & 0xe00)>>9;
        uint8_t eamode;
        char sz_char = '\0';
        eamode = (eabyte & 0x38)>>3; // xx111xxxb
        if(eamode == 0)
            sz_char = 'l';
        else
            sz_char = 'b';
        
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"bset.%c d%x,%s",sz_char,Dy,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_LEA) == COLDFIRE_CODE_LEA){
        // handle instruction
        uint8_t Ax = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"lea.l %s,a%x",fmtted1,Ax);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOV3Q) == COLDFIRE_CODE_MOV3Q){
        // handle instruction
        uint8_t imm = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        snprintf(asm_buf,ASM_BUF_LEN,"mov3q.l #0x%x,%s",imm,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_ADDQ) == COLDFIRE_CODE_ADDQ){
        // handle instruction
        uint8_t imm = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        snprintf(asm_buf,ASM_BUF_LEN,"addq.l #0x%x,%s",imm,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_SUBQ) == COLDFIRE_CODE_SUBQ){
        // handle instruction
        uint8_t imm = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        snprintf(asm_buf,ASM_BUF_LEN,"subq.l #0x%x,%s",imm,fmtted1);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_DIVU_W) == COLDFIRE_CODE_DIVU_W){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'w');
        snprintf(asm_buf,ASM_BUF_LEN,"divu.w %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_DIVS_W) == COLDFIRE_CODE_DIVS_W){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'w');
        snprintf(asm_buf,ASM_BUF_LEN,"divs.w %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_SUBA) == COLDFIRE_CODE_SUBA){
        // handle instruction
        uint8_t Ax = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"suba %s,a%x",fmtted1,Ax);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_ADDA) == COLDFIRE_CODE_ADDA){
        // handle instruction
        uint8_t Ax = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"suba %s,a%x",fmtted1,Ax);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_EOR) == COLDFIRE_CODE_EOR){
        // handle instruction
        uint8_t Dy = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        snprintf(asm_buf,ASM_BUF_LEN,"eor.l %s,d%x",fmtted1,Dy);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MULU_W) == COLDFIRE_CODE_MULU_W){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'w');
        snprintf(asm_buf,ASM_BUF_LEN,"muls.w %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MULS_W) == COLDFIRE_CODE_MULS_W){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'w');
        snprintf(asm_buf,ASM_BUF_LEN,"muls.w %s,d%x",fmtted1,Dx);
        return COLDFIRE_WORD*(1+sz_1);
    }
    else if( (code32 & COLDFIRE_MASK_MAC_MAC) == COLDFIRE_CODE_MAC_MAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MAC_EMAC) == COLDFIRE_CODE_MAC_EMAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MAC_WITH_LOAD_EMAC) == COLDFIRE_CODE_MAC_WITH_LOAD_EMAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MAAAC_EMAC_B) == COLDFIRE_CODE_MAAAC_EMAC_B){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSAAC_EMAC_B) == COLDFIRE_CODE_MSAAC_EMAC_B){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSAC_MAC) == COLDFIRE_CODE_MSAC_MAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSAC_EMAC) == COLDFIRE_CODE_MSAC_EMAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSAC_WITH_LOAD_MAC) == COLDFIRE_CODE_MSAC_WITH_LOAD_MAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSAC_WITH_LOAD_EMAC) == COLDFIRE_CODE_MSAC_WITH_LOAD_EMAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSAAC_EMAC_B) == COLDFIRE_CODE_MSAAC_EMAC_B){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MSSAC_EMAC_B) == COLDFIRE_CODE_MSSAC_EMAC_B){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code32 & COLDFIRE_MASK_MAC_WITH_LOAD_MAC) == COLDFIRE_CODE_MAC_WITH_LOAD_MAC){
        // handle instruction
        return COLDFIRE_LONGWORD; 
    }
    else if( (code16 & COLDFIRE_MASK_ASL) == COLDFIRE_CODE_ASL){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        uint8_t ir = (code16 & 0x20)>0;
        if(ir == 0)
        {
            uint8_t count = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"asl.l #0x%x,d%x",count,Dx);
        }
        else if(ir == 1)
        {
            uint8_t Dy = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"asl.l d%x,d%x",Dy,Dx);
        }
        return COLDFIRE_WORD;
    }
    else if( (code16 & COLDFIRE_MASK_ASR) == COLDFIRE_CODE_ASR){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        uint8_t ir = (code16 & 0x20)>0;
        if(ir == 0)
        {
            uint8_t count = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"asr.l #0x%x,d%x",count,Dx);
        }
        else if(ir == 1)
        {
            uint8_t Dy = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"asr.l d%x,d%x",Dy,Dx);
        }
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_LSL) == COLDFIRE_CODE_LSL){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        uint8_t ir = (code16 & 0x20)>0;
        if(ir == 0)
        {
            uint8_t count = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"lsl.l #0x%x,d%x",count,Dx);
        }
        else if(ir == 1)
        {
            uint8_t Dy = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"lsl.l d%x,d%x",Dy,Dx);
        }
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_LSR) == COLDFIRE_CODE_LSR){
        // handle instruction
        uint8_t Dx = code16 & 0x7;
        uint8_t ir = (code16 & 0x20)>0;
        if(ir == 0)
        {
            uint8_t count = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"lsr.l #0x%x,d%x",count,Dx);
        }
        else if(ir == 1)
        {
            uint8_t Dy = (code16 & 0xe00)>>9;
            snprintf(asm_buf,ASM_BUF_LEN,"lsr.l d%x,d%x",Dy,Dx);
        }
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_MVS) == COLDFIRE_CODE_MVS){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        char sz_char = ((code16 & 4)>0) ? 'w' : 'b';
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"mvs.%c %s,d%x",sz_char,fmtted1,Dx);
        return COLDFIRE_WORD*(1+sz_1);
    }
    else if( (code16 & COLDFIRE_MASK_MVZ) == COLDFIRE_CODE_MVZ){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        char sz_char = ((code16 & 4)>0) ? 'w' : 'b';
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        snprintf(asm_buf,ASM_BUF_LEN,"mvz.%c %s,d%x",sz_char,fmtted1,Dx);
        return COLDFIRE_WORD*(1+sz_1);
    }
    else if( (code16 & COLDFIRE_MASK_MOVEQ) == COLDFIRE_CODE_MOVEQ){
        // handle instruction
        uint8_t Dx = (code16 & 0xe00)>>9;
        uint8_t imm = code16 & 0xff;
        snprintf(asm_buf,ASM_BUF_LEN,"moveq.l #0x%x,d%x",imm,Dx);
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_BCC) == COLDFIRE_CODE_BCC){
        // handle instruction
        uint8_t cond = (code16 & 0xff0000)>>8;
        uint8_t disp8 = code16 & 0xff;
        uint8_t cc[2] = {0};
        fmt_condition(cc, 2, cond);
        if(disp8 == 0)
        {
            uint16_t disp16 = (code32&0xffff);
            snprintf(asm_buf,ASM_BUF_LEN,"b%s.w 0x%x",cc,pc+disp16);
            return COLDFIRE_LONGWORD;
        }
        else if(disp8 == 0xff)
        {
            uint32_t disp32 = (code48&0xffffffff);
            snprintf(asm_buf,ASM_BUF_LEN,"b%s.l 0x%x",cc,pc+disp32);
            return COLDFIRE_LONGLONGWORD;
        }
        else
        {
            snprintf(asm_buf,ASM_BUF_LEN,"b%s.b 0x%x",cc,pc+disp8);
            return COLDFIRE_WORD; 
        }
        return COLDFIRE_WORD; 
    }
    else if( (code16 & COLDFIRE_MASK_OR) == COLDFIRE_CODE_OR){
        // handle instruction
        uint8_t datareg = (code16 & 0xe00)>>9;
        uint8_t opmode = (code16 & 0x1c0)>>6;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension 
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}

        if(opmode == 2)
            snprintf(asm_buf,ASM_BUF_LEN,"or.l %s,d%x",fmtted1,datareg);
        else if(opmode == 6)
            snprintf(asm_buf,ASM_BUF_LEN,"or.l d%x,%s",datareg,fmtted1);
        else {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_SUB) == COLDFIRE_CODE_SUB){
        // handle instruction
        uint8_t datareg = (code16 & 0xe00)>>9;
        uint8_t opmode = (code16 & 0x1c0)>>6;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension 
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}

        if(opmode == 2)
            snprintf(asm_buf,ASM_BUF_LEN,"sub.l %s,d%x",fmtted1,datareg);
        else if(opmode == 6)
            snprintf(asm_buf,ASM_BUF_LEN,"sub.l d%x,%s",datareg,fmtted1);
        else {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_AND) == COLDFIRE_CODE_AND){
        // handle instruction
        // Two modes: <ea>y,Dx and Dy,<ea>x
        uint8_t datareg = (code16 & 0xe00)>>9;
        uint8_t opmode = (code16 & 0x1c0)>>6;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension 
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}

        if(opmode == 2)
            snprintf(asm_buf,ASM_BUF_LEN,"and.l %s,d%x",fmtted1,datareg);
        else if(opmode == 6)
            snprintf(asm_buf,ASM_BUF_LEN,"and.l d%x,%s",datareg,fmtted1);
        else {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_ADD) == COLDFIRE_CODE_ADD){
        // handle instruction
        uint8_t datareg = (code16 & 0xe00)>>9;
        uint8_t opmode = (code16 & 0x1c0)>>6;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension 
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,'l');
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}

        if(opmode == 2)
            snprintf(asm_buf,ASM_BUF_LEN,"add.l %s,d%x",fmtted1,datareg);
        else if(opmode == 6)
            snprintf(asm_buf,ASM_BUF_LEN,"add.l d%x,%s",datareg,fmtted1);
        else {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_CMP) == COLDFIRE_CODE_CMP){
        // handle instruction
        uint8_t datareg = (code16 & 0xe00)>>9;
        uint8_t opmode = (code16 & 0x1c0)>>6;
        char sz_char = '\0';
        if(opmode == 0)
            sz_char = 'b';
        else if(opmode == 1)
            sz_char = 'w';
        else if(opmode == 2)
            sz_char = 'l';
        else {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension 
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}

        snprintf(asm_buf,ASM_BUF_LEN,"cmp.%c %s,d%x",sz_char,fmtted1,datareg);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_CMPA) == COLDFIRE_CODE_CMPA){
        // handle instruction
        uint8_t Ax = (code16 & 0xe00)>>9;
        uint8_t opmode = (code16 & 0x1c0)>>6;
        char sz_char = '\0';
        if(opmode == 2)
            sz_char = 'w';
        else if(opmode == 7)
            sz_char = 'l';
        else {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension 
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}

        snprintf(asm_buf,ASM_BUF_LEN,"cmpa.%c %s,a%x",sz_char,fmtted1,Ax);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVEA) == COLDFIRE_CODE_MOVEA){
        // handle instruction
        uint8_t size = (code16 & 0x3000)>>12;
        uint8_t Ax = (code16 & 0xe00)>>9;
        uint8_t eabyte = (code16 & 0x3f);
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        char sz_char = 0;
        if(size == 0x3) // word
            sz_char='w';
        else if(size == 0x2) // longword
            sz_char='l';
        else { snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD; }
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        if(sz_1 == -1) {snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD;}
        
        snprintf(asm_buf,ASM_BUF_LEN,"movea.%c %s,a%x",sz_char,fmtted1,Ax);
        return COLDFIRE_WORD*(1+sz_1); 
    }
    else if( (code16 & COLDFIRE_MASK_MOVE) == COLDFIRE_CODE_MOVE){
        // handle instruction
        // move.sz <ea>y,<ea>x 
        // Note TWO effective addresses. Only instruction that does this. 
        uint8_t size = (code16 & 0x3000)>>12;
        uint8_t eabyte1 = (code16 & 0x0fc0)>>6;
        uint8_t eabyte2 = code16 & 0x3f;
        char sz_char = 0;
        if(size == 0x1) // byte
            sz_char='b';
        else if(size == 0x3) // word
            sz_char='w';
        else if(size == 0x2) // longword
            sz_char='l';
        else { snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD; }
        uint16_t extword1 = 0;
        uint16_t extword2 = 0;
        if(HAS_32)
            extword1 = code32 & 0xffff;
        if(HAS_48)
            extword2 = code48 & 0xffff;
        uint8_t sz_1 = 0; // fmtted1 number of extension words
        uint8_t sz_2 = 0; // fmtted2 number of extension words
        sz_1 = fmt_eaddr(fmtted1,flen,eabyte1,COLDFIRE_EA_ORDER_REG_MODE,pc,extword1,extword2,HAS_32,HAS_48,sz_char);
        sz_2 = fmt_eaddr(fmtted2,flen,eabyte2,COLDFIRE_EA_ORDER_MODE_REG,pc,extword1,extword2,HAS_32,HAS_48,sz_char);

        if(sz_1 >= 0 && sz_2 >= 0)
            snprintf(asm_buf,ASM_BUF_LEN,"move.%c %s,%s",sz_char,fmtted2,fmtted1);
        else { snprintf(asm_buf,ASM_BUF_LEN,"invalid"); return COLDFIRE_WORD; }
        return COLDFIRE_WORD*(1 + sz_1 + sz_2); // Return total size in bytes
    }

    else{
        snprintf(asm_buf,ASM_BUF_LEN,"invalid");
        return COLDFIRE_WORD;
    }
    return 0;
}

