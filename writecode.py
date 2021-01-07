# mnemonics in order of restrictiveness
mnes = [
    'STLDSR',
    'MOVEC',
    'NOP',
    'STOP',
    'RTE',
    'RTS',
    'HALT',
    'PULSE',
    'ILLEGAL',
    'MOVE_FROM_MACSR_TO_CCR',
    'ORI',
    'BITREV',
    'ANDI',
    'BYTEREV',
    'SUBI',
    'FF1',
    'ADDI',
    'EORI',
    'CMPI',
    'INTOUCH',
    'NEGX',
    'MOVE_FROM_SR',
    'MOVE_FROM_CCR',
    'NEG',
    'SATS',
    'UNLK',
    'MOVE_TO_USP',
    'MOVE_FROM_USP',
    'NOT',
    'TPF',
    'SWAP',
    'LINK',
    'MOVE_FROM_ACC_MAC',
    'MOVE_FROM_MACSR',
    'MOVE_TO_MACSR',
    'MOVE_FROM_ACCEXT01_EMAC',
    'TRAP',
    'MOVE_FROM_ACCEXT23_EMAC',
    'MOVE_FROM_MASK',
    'MOVE_TO_MASK',
    'MOVE_TO_ACCEXT23_EMAC',
    'BTST',
    'BCHG',
    'BCLR',
    'BSET',
    'MOVE_TO_CCR',
    'MOVE_TO_SR',
    'PEA',
    'TAS',
    'FSAVE',
    'FRESTORE',
    'JSR',
    'JMP',
    'MOVE_TO_ACCEXT01_EMAC',
    'FMOVE_TO_FPIAR',
    'FMOVE_TO_FPSR',
    'FMOVE_TO_FPCR',
    'FMOVE_FROM_FPIAR',
    'FMOVE_FROM_FPSR',
    'FMOVE_FROM_FPCR',
    'FNOP',
    'WDEBUG',
    'MULU_L',
    'MULS_L',
    'DIVU_L',
    'REMU_L',
    'DIVS_L',
    'REMS_L',
    'FMOVEM',
    'FCMP',
    'FTST',
    'FMOVE_2',
    'FINT',
    'FINTRZ',
    'FMOVE_1',
    'FSQRT',
    'FABS',
    'FNEG',
    'FDIV',
    'FADD',
    'FMUL',
    'FSUB',
    'MOVE_TO_ACC_MAC',
    'FBCC',
    'CPUSHL',
    'CLR',
    'TST',
    'WDDATA',
    'BRA',
    'BSR',
    'EXT',
    'EXTB',
    'MOVEM',
    'MOVE_ACC_TO_ACC_EMAC',
    'MOVE_FROM_ACC_EMAC',
    'MOVCLR_EMAC',
    'MOVE_TO_ACC_EMAC',
    'SUBX',
    'ADDX',
    'SCC',
    'BSET',
    'BTST',
    'BCH',
    'BCLR',
    'LEA',
    'MOV3Q',
    'ADDQ',
    'SUBQ',
    'DIVU_W',
    'DIVS_W',
    'SUBA',
    'ADDA',
    'EOR',
    'MULU_W',
    'MULS_W',
    'MAC_MAC',
    'MAC_EMAC',
    'MAC_WITH_LOAD_EMAC',
    'MAAAC_EMAC_B',
    'MSAAC_EMAC_B',
    'MSAC_MAC',
    'MSAC_EMAC',
    'MSAC_WITH_LOAD_MAC',
    'MSAC_WITH_LOAD_EMAC',
    'MSAAC_EMAC_B',
    'MSSAC_EMAC_B',
    'MAC_WITH_LOAD_MAC',
    'ASL',
    'ASR',
    'LSL',
    'LSR',
    'MVS',
    'MVZ',
    'MOVEQ',
    'BCC',
    'OR',
    'SUB',
    'AND',
    'ADD',
    'CMP',
    'CMPA',
    'MOVEA',
    'MOVE'                  
]
opcodes = {
    'STLDSR'                :'0100000011100111'+'0100011011111100',
    'MOVEC'                 :'0100111001111011'+'xxxxxxxxxxxxxxxx',
    'NOP'                   :'0100111001110001',
    'STOP'                  :'0100111001110010',
    'RTE'                   :'0100111001110011',
    'RTS'                   :'0100111001110101',
    'HALT'                  :'0100101011001000',
    'PULSE'                 :'0100101011001100',
    'ILLEGAL'               :'0100101011111100',
    'MOVE_FROM_MACSR_TO_CCR':'1010100111000000',
    'ORI'                   :'0000000010000xxx',
    'BITREV'                :'0000000011000xxx',
    'ANDI'                  :'0000001010000xxx',
    'BYTEREV'               :'0000001011000xxx',
    'SUBI'                  :'0000010010000xxx',
    'FF1'                   :'0000010011000xxx',
    'ADDI'                  :'0000011010000xxx',
    'EORI'                  :'0000101010000xxx',
    'CMPI'                  :'00001100xx000xxx',
    'INTOUCH'               :'1111010000101xxx',
    'NEGX'                  :'0100000010000xxx',
    'MOVE_FROM_SR'          :'0100000011000xxx',
    'MOVE_FROM_CCR'         :'0100001011000xxx',
    'NEG'                   :'0100010010000xxx',
    'SATS'                  :'0100110010000xxx',
    'UNLK'                  :'0100111001011xxx',
    'MOVE_TO_USP'           :'0100111001100xxx',
    'MOVE_FROM_USP'         :'0100111001101xxx',
    'NOT'                   :'0100011010000xxx',
    'TPF'                   :'0101000111111xxx', # + 2 opt. immediate words
    'SWAP'                  :'0100100001000xxx',
    'LINK'                  :'0100111001010xxx'+'xxxxxxxxxxxxxxxx',
    'MOVE_FROM_ACC_MAC'     :'101000011000xxxx',
    'MOVE_FROM_MACSR'       :'101010011000xxxx',
    'MOVE_TO_MACSR'         :'101010011000xxxx',
    'MOVE_FROM_ACCEXT01_EMAC':'101010111000xxxx',
    'TRAP'                  :'010011100100xxxx',
    'MOVE_FROM_ACCEXT23_EMAC':'101011111000xxxx',
    'MOVE_FROM_MASK'        :'101011011000xxxx',
    'MOVE_TO_MASK'          :'1010110100xxxxxx',
    'MOVE_TO_ACCEXT23_EMAC' :'1010111100xxxxxx',
    'BTST'                  :'0000100000xxxxxx',
    'BCHG'                  :'0000100001xxxxxx',
    'BCLR'                  :'0000100010xxxxxx',
    'BSET'                  :'0000100011xxxxxx',
    'MOVE_TO_CCR'           :'0100010011xxxxxx',
    'MOVE_TO_SR'            :'0100011011xxxxxx',
    'PEA'                   :'0100100001xxxxxx',
    'TAS'                   :'0100101011xxxxxx',
    'FSAVE'                 :'1111001101xxxxxx',
    'FRESTORE'              :'1111001101xxxxxx',
    'JSR'                   :'0100111010xxxxxx',
    'JMP'                   :'0100111011xxxxxx',
    'MOVE_TO_ACCEXT01_EMAC' :'1010101100xxxxxx',
    'FMOVE_TO_FPIAR'        :'1111001000xxxxxx'+'1000010000000000',
    'FMOVE_TO_FPSR'         :'1111001000xxxxxx'+'1000100000000000',
    'FMOVE_TO_FPCR'         :'1111001000xxxxxx'+'1001000000000000',
    'FMOVE_FROM_FPIAR'      :'1111001000xxxxxx'+'1010010000000000',
    'FMOVE_FROM_FPSR'       :'1111001000xxxxxx'+'1010100000000000',
    'FMOVE_FROM_FPCR'       :'1111001000xxxxxx'+'1011000000000000',
    'FNOP'                  :'1111001100xxxxxx'+'0000000000000000',
    'WDEBUG'                :'1111101111xxxxxx'+'0000000000000011',
    'MULU_L'                :'0100110000xxxxxx'+'0xxx000000000000',
    'MULS_L'                :'0100110000xxxxxx'+'0xxx100000000000',
    'DIVU_L'                :'0100110001xxxxxx'+'0xxx000000000xxx',
    'REMU_L'                :'0100110001xxxxxx'+'0xxx000000000xxx',
    'DIVS_L'                :'0100110001xxxxxx'+'0xxx100000000xxx',
    'REMS_L'                :'0100110001xxxxxx'+'0xxx100000000xxx',
    'FMOVEM'                :'1111001000xxxxxx'+'11x10000xxxxxxxx',
    'FCMP'                  :'1111001000xxxxxx'+'0x0xxxxxx0111000',
    'FTST'                  :'1111001000xxxxxx'+'0x0xxxxxx0111010',
    'FMOVE_2'               :'1111001000xxxxxx'+'011xxxxxx0000000',
    'FINT'                  :'1111001000xxxxxx'+'0x0xxxxxx0000001',
    'FINTRZ'                :'1111001000xxxxxx'+'0x0xxxxxx0000011',
    'FMOVE_1'               :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FSQRT'                 :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FABS'                  :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FNEG'                  :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FDIV'                  :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FADD'                  :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FMUL'                  :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'FSUB'                  :'1111001000xxxxxx'+'0x0xxxxxxxxxxxxx',
    'MOVE_TO_ACC_MAC'       :'1010000100xxxxxx',
    'FBCC'                  :'111100101xxxxxxx', # + displacement,
    'CPUSHL'                :'11110100xx101xxx',
    'CLR'                   :'01000010xxxxxxxx',
    'TST'                   :'01001010xxxxxxxx',
    'WDDATA'                :'11111011xxxxxxxx',
    'BRA'                   :'01100000xxxxxxxx', # + up to 2 displacement words
    'BSR'                   :'01100001xxxxxxxx', # + up to 2 displacement words
    'EXT'                   :'0100100xxx000xxx',
    'EXTB'                  :'0100100xxx000xxx',
    'MOVEM'                 :'01001x0011xxxxxx',
    'MOVE_ACC_TO_ACC_EMAC'  :'10100xx1000100xx',
    'MOVE_FROM_ACC_EMAC'    :'10100xx11000xxxx',
    'MOVCLR_EMAC'           :'10100xx11100xxxx',
    'MOVE_TO_ACC_EMAC'      :'10100xx100xxxxxx',
    'SUBX'                  :'1001xxx110000xxx',
    'ADDX'                  :'1101xxx110000xxx',
    'SCC'                   :'0101xxxx11000xxx',
    'BSET'                  :'0000xxx111xxxxxx',
    'BTST'                  :'0000xxx100xxxxxx',
    'BCH'                   :'0000xxx101xxxxxx',
    'BCLR'                  :'0000xxx110xxxxxx',
    'LEA'                   :'0100xxx111xxxxxx',
    'MOV3Q'                 :'1010xxx101xxxxxx',
    'ADDQ'                  :'0101xxx010xxxxxx',
    'SUBQ'                  :'0101xxx110xxxxxx',
    'DIVU_W'                :'1000xxx011xxxxxx',
    'DIVS_W'                :'1000xxx111xxxxxx',
    'SUBA'                  :'1001xxx111xxxxxx',
    'ADDA'                  :'1101xxx111xxxxxx',
    'EOR'                   :'1011xxx110xxxxxx',
    'MULU_W'                :'1100xxx011xxxxxx',
    'MULS_W'                :'1100xxx111xxxxxx',
    'MAC_MAC'               :'1010xxx00x00xxxx'+'xxxxxxx0xxx0xx00',
    'MAC_EMAC'              :'1010xxx0xx00xxxx'+'xxxxxxx0xxxxxx00',
    'MAC_WITH_LOAD_EMAC'    :'1010xxx0xxxxxxxx'+'xxxxxxx0xxxxxxxx',
    'MAAAC_EMAC_B'          :'1010xxx0xx00xxxx'+'xxxxxxx0xxxxxx01',
    'MSAAC_EMAC_B'          :'1010xxx0xx00xxxx'+'xxxxxxx0xxxxxx11',
    'MSAC_MAC'              :'1010xxx00x00xxxx'+'xxxxxxx1xxx0xx00',
    'MSAC_EMAC'             :'1010xxx0xx00xxxx'+'xxxxxxx1xxxxxx00',
    'MSAC_WITH_LOAD_MAC'    :'1010xxx01xxxxxxx'+'xxxxxxx1xxx0xxxx',
    'MSAC_WITH_LOAD_EMAC'   :'1010xxx0xxxxxxxx'+'xxxxxxx1xxxxxxxx',
    'MSAAC_EMAC_B'          :'1010xxx0xx00xxxx'+'xxxxxxx1xxxxxx01',
    'MSSAC_EMAC_B'          :'1010xxx0xx00xxxx'+'xxxxxxx1xxxxxx11',
    'MAC_WITH_LOAD_MAC'     :'1010xxxx01xxxxxx'+'xxxxxxx0xxxxxx01',
    'ASL'                   :'1110xxxx10x00xxx',
    'ASR'                   :'1110xxxx10x00xxx',
    'LSL'                   :'1110xxxx10x01xxx',
    'LSR'                   :'1110xxxx10x01xxx',
    'MVS'                   :'0111xxx10xxxxxxx',
    'MVZ'                   :'0111xxx11xxxxxxx',
    'MOVEQ'                 :'0111xxx0xxxxxxxx',
    'BCC'                   :'0110xxxxxxxxxxxx', # + up to 2 displacement words
    'OR'                    :'1000xxxxxxxxxxxx',
    'SUB'                   :'1001xxxxxxxxxxxx',
    'AND'                   :'1100xxxxxxxxxxxx',
    'ADD'                   :'1101xxxxxxxxxxxx',
    'CMP'                   :'1011xxxxxxxxxxxx',
    'CMPA'                  :'1011xxxxxxxxxxxx',
    'MOVEA'                 :'00xxxxx001xxxxxx',
    'MOVE'                  :'00xxxxxxxxxxxxxx',
}

mask = ''
hcode = ''
for mne in mnes:
    code = opcodes[mne]
    mask = hex(int(''.join([str(int(i != 'x')) for i in code]),2))
    hcode = hex(int(code.replace('x','0'),2))

    mask_s = 'COLDFIRE_MASK_'+mne
    code_s = 'COLDFIRE_CODE_'+mne
    if len(code) == 16:
        print( 'else if( (code16 & '+ mask_s + ') == '+code_s+'){\n    // handle instruction')
        print('    asm_buf=fmt_instr("'+mne.lower()+'",left,right,data);\n    pc+=4; \n}')
    elif len(code) == 32:
        print( 'else if( (code32 & '+ mask_s + ') == '+code_s+'){\n    // handle instruction')
        print('    asm_buf"'+mne.lower()+'";\n    pc+=8; \n}' )


