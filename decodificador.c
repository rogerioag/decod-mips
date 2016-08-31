#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "debug.h"

#define _GNU_SOURCE // necessário porque getline() é extensão GNU

// Campos do formato de instrução.
// opcode  := ir(31 downto 26)
// rs      := ir(25 downto 21);
// rt      := ir(20 downto 16);
// rd      := ir(15 downto 11);
// shamt   := ir(10 downto  6);
// imm     := ir(15 downto  0);
// address := ir(25 downto  0);

int mascaraOpCode = 0xFC000000;
int mascaraRs = 0x03E00000;
int mascaraRt = 0x001F0000;
int mascaraRd = 0x0000F800;
int mascaraShamt = 0x000007C0;
int mascaraFunct = 0x0000003F;

int mascaraImmediate = 0x0000FFFF;
int mascaraAddress = 0x03FFFFFF;

char *registers[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2",
                       "$a3",   "$t0", "$t1", "$t2", "$t3", "$t4", "$t5",
                       "$t6",   "$t7", "$s0", "$s1", "$s2", "$s3", "$s4",
                       "$s5",   "$s6", "$s7", "$t8", "$t9", "$k0", "$k1",
                       "$gp",   "$sp", "$fp", "$ra"};

void bin_prnt_byte(int x) {
  int n;
  printf("bin_prnt_byte begin x: %u\n");
  for (n = 0; n < 32; n++) {
    if ((x & 0x80000000) != 0) {
      printf("1");
    } else {
      printf("0");
    }
    if (n % 4 == 3) {
      printf(" "); /* insert a space between nybbles */
    }
    x = x << 1;
  }
  printf("\nbin_prnt_byte end.\n");
}

void bin_prnt_int(int x)
{
   int hi, lo;
   hi=(x>>8) & 0xff;
   lo=x&0xff;
   bin_prnt_byte(hi);
   printf(" ");
   bin_prnt_byte(lo);
}


unsigned int getOpCode(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int opcode = ((ir & mascaraOpCode) >> 26);
  TRACE("Opcode: %u\n ", opcode);
  return opcode;
}

unsigned int getRs(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int rs = (ir & mascaraRs) >> 21;
  TRACE("Rs: %u\n ", rs);
  return rs;
}

unsigned int getRt(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int rt = (ir & mascaraRt) >> 16;
  TRACE("Rt: %u\n ", rt);
  return rt;
}

unsigned int getRd(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int rd = (ir & mascaraRd) >> 11;
  TRACE("Rd: %u\n ", rd);
  return rd;
}

int getShamt(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int shamt = (ir & mascaraShamt) >> 6;
  TRACE("Shamt: %u\n ", shamt);
  return shamt;
}

unsigned int getFunct(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int funct = (ir & mascaraFunct);
  TRACE("funct: %u\n ", funct);
  return funct;
}

int getImmediate(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int imm = (ir & mascaraImmediate);
  TRACE("imm: %u\n ", imm);
  return imm;
}

unsigned int getAddress(unsigned int ir) {
  PRINT_FUNC_NAME;
  unsigned int address = (ir & mascaraAddress);
  TRACE("address: %u\n ", address);
  return address; 
}

// Converte um char * representando um binário para inteiro.
int intFromBinary(char *s) {
  PRINT_FUNC_NAME;
  int inteiro = (int) strtol(s, 0, 2);
  TRACE("valor inteiro %d - %X - %b\n", inteiro, inteiro, inteiro);
  bin_prnt_byte(inteiro);
  printf("\n");

  return inteiro;
}

// Decodifica uma instrução.
void decodificar(unsigned int ir) {
  PRINT_FUNC_NAME;
  switch (getOpCode(ir)) {
    case 0: { // 000000, Aritmética.
      switch (getFunct(ir)) {
        case 32: { // 100000 -> add, R-Type.
          printf("add ");
          printf("%s, ", registers[getRd(ir)]);
          printf("%s, ", registers[getRs(ir)]);
          printf("%s\n", registers[getRt(ir)]);
          break;
        }

        case 34: { // 100010 -> sub, R-Type.
          printf("sub ");
          printf("%s, ", registers[getRd(ir)]);
          printf("%s, ", registers[getRs(ir)]);
          printf("%s\n", registers[getRt(ir)]);
          break;
        }
      }
      break;
    }
    case 35: // 100011 (lw), I-Type. lw rt, imm(rs)
      printf("lw ");
      printf("%s, ", registers[getRt(ir)]);
      printf("%d", getImmediate(ir));
      printf("(%s)\n", registers[getRs(ir)]);
      break;

    default: // outros casos.
      printf("Instrução não implementada. \n");
  }
}

int main(int argc, char *argv[]) {
  PRINT_FUNC_NAME;
  int i;
  unsigned int ir;
  if (argc < 2) {
    printf("Uso:\n ./decodificador arquivo.b\n");
    return (0);
  }

  printf("Argumentos:\n");
  printf("argc = %d\n", argc);
  for (i = 0; i < argc; i++)
    printf("argv[%d] = %s\n", i, argv[i]);

  FILE *arquivo = fopen(argv[1], "r");
  size_t len = 32; // tamanho da linha.
  char *linha = malloc(len);

  if (!arquivo) {
    perror(argv[1]);
    exit(1);
  }

  while (getline(&linha, &len, arquivo) > 0) {
    printf("Linha: %s", linha);
    // teste: bin_prnt_byte(0x8C130004);
    ir = intFromBinary(linha);
    printf("IR: %u\n", ir);
    decodificar(ir);
  }

  if (linha) {
    free(linha);
  }

  fclose(arquivo);

  return 0;
}
