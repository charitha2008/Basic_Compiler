Bin = ./bin/
Lib = ./lib/
Src = ./src/

ALL: y.tab.c lex.yy.c y.tab.o calc3i.exe
y.tab.c: ./lexyacc-code/calc3.y
	bison -y -d ./lexyacc-code/calc3.y -o ./lexyacc-code/y.tab.c

lex.yy.c:	./lexyacc-code/calc3.l
	flex -o ./lexyacc-code/lex.yy.c ./lexyacc-code/calc3.l

y.tab.o:	./lexyacc-code/y.tab.o ./lexyacc-code/lex.yy.o
	gcc -c ./lexyacc-code/y.tab.c -I./lexyacc-code/ -o ./lexyacc-code/lex.yy.c

calc3i.exe: ./lexyacc-code/y.tab.o ./lexyacc-code/lex.yy.o ./lexyacc-code/calc3i.c
	gcc ./lexyacc-code/y.tab.o ./lexyacc-code/lex.yy.o ./lexyacc-code/calc3i.c -o bin/calc3i.exe


lex.yy.o: lex.yy.c
	gcc -c  ./lexyacc-code/lex.yy.c -I./lexyacc-code/ -o ./lexyacc-code/lex.yy.o
