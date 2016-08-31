
all: decodificador

decodificador: decodificador.c
	gcc decodificador.c -o decodificador.exe

clean: 
	rm *.exe
