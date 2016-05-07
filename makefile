CC=gcc
CFLAGS= -std=c99 -pedantic -Wall -g3

HWK4= /c/cs223/Hwk5

all:    Boggle
 
#####
# Instructions to make Boggle
#####

Boggle: Boggle.o getLine.o
	${CC} ${CFLAGS} -o Boggle Boggle.o getLine.o

Boggle.o: Boggle.c getLine.h
	${CC} ${CFLAGS} -c Boggle.c

getLine.o: getLine.c getLine.h
	${CC} ${CFLAGS} -c getLine.c



