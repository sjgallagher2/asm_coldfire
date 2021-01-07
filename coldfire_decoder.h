#ifndef COLDFIRE_DECODER_H
#define COLDFIRE_DECODER_H

#include <stdio.h>
#include <stdint.h>

int decode_coldfire(const uint8_t *buf, const int BUF_LEN, int pc, uint8_t *asm_buf, const int ASM_BUF_LEN);
void fmt_condition(uint8_t *out, int olen, uint8_t ccbyte);
int fmt_eaddr(uint8_t *out,int olen, uint8_t eabyte, uint8_t order, uint64_t pc, 
            uint16_t extword1,uint16_t extword2,uint8_t HAS_32,uint8_t HAS_48,
            char opsz);

#endif /* COLDFIRE_DECODER_H */