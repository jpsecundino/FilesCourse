all: 
	gcc programaTrab3.c fileManagement.c listaOrdenada.c indexFileManagement.c ioMain.c -o programaTrab3 -g -Wall
run:
	./programaTrab3
remove:
	rm *.bin
	rm *.out
	rm *.index
	rm *.in
	nautilus .
test1:
	
