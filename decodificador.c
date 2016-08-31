#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
int mascaraRs =     0x03E00000;
int mascaraRt =     0x001F0000;
int mascaraRd =     0x0000F800;
int mascaraShamt =  0x000007C0;
int mascaraFunct =  0x0000003F;

int mascaraImmediate =  0x0000FFFF;
int mascaraAddress =   0x03FFFFFF;

char * registers[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
      


int getOpCode(int ir) {
	printf("opcode: %d\n ", (ir & mascaraOpCode) >> 26);
	return (ir & mascaraOpCode) >> 26;
}

int getRs(int ir) {
	return (ir & mascaraRs) >> 21;
}

int getRt(int ir) {
	return (ir & mascaraRt) >> 16;
}

int getRd(int ir) {
	return (ir & mascaraRd) >> 11;
}

int getShamt(int ir) {
	return (ir & mascaraShamt) >> 6;
}

int getFunct(int ir) {
	return (ir & mascaraFunct);
}

int getImmediate(int ir) {
	return (ir & mascaraImmediate);
}

int getAddress(int ir) {
	return (ir & mascaraAddress);
}

void bin_prnt_byte(int x)
{
   int n;
   for(n=0; n<32; n++)
   {
      if((x & 0x80) !=0)
      {
         printf("1");
      }
      else
      {
         printf("0");
      }
      if (n==3)
      {
         printf(" "); /* insert a space between nybbles */
      }
      x = x<<1;
   }
}

// Converte um char * representando um binário para inteiro.
int fromBinary(char *s) {
  int teste = (int) strtol(s, 0, 2);
  // printf("valor: %d - %X - %b", teste, teste, teste);
  
  bin_prnt_byte(teste);
  printf("\n");
  
  return teste;
}


// Decodifica uma instrução.
void decodificar(int ir) {
    switch (getOpCode(ir)) {
        case 0 : // 000000
                switch (getFunct(ir)) {
                    case 32 : // 100000 -> add.
                                printf("add ");
                                printf("%s, ", registers[getRd(ir)]);
                                printf("%s, ", registers[getRs(ir)]);
                                printf("%s\n", registers[getRt(ir)]);
                                break;
                    case 34 : // 100010 -> sub.
                                printf("sub ");
                                printf("%s, ", registers[getRd(ir)]);
                                printf("%s, ", registers[getRs(ir)]);
                                printf("%s\n", registers[getRt(ir)]);
                                break;
                }
                
                break;
    
        default : // outros casos.
                  printf("Instrução não implementada. \n");
       
    }
}
    
int main(int argc,char *argv[]){
   int i;
   int ir;
   if(argc < 2){
       printf("Uso:\n ./decodificador arquivo.b\n");
       return(0);
   }
   
   printf("Argumentos:\n");
   printf("argc = %d\n", argc);
   for(i=0;i<argc;i++)
       printf("argv[%d] = %s\n", i, argv[i]);
       
   FILE *arquivo= fopen(argv[1], "r");
   size_t len= 32;  // tamanho da linha.
   char *linha= malloc(len);

   if (!arquivo) {
        perror(argv[1]);
        exit(1);
   }
   
   while (getline(&linha, &len, arquivo) > 0){
        printf("Linha: %s", linha);
        ir = fromBinary(linha);
        printf("Inteiro: %d\n", ir);
        decodificar(ir);
   }
   
   if (linha)
    free(linha);
  
   fclose(arquivo);
   return 0;    
}
