
CC=gcc
CXX=g++

all: decodificador

decodificador: decodificador.c
	${CC} $(OPTIONS) decodificador.c -o decodificador.exe

clean: 
	rm *.exe

info:
	@echo "For compiling with DEBUG messages, use:"
	@echo "make OPTIONS=\"-DVERBOSE -DDEBUG\""