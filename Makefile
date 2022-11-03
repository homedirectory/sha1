main:
	gcc -g -o main main.c hash.c hash.h
test:
	gcc -g -o test test.c hash.c hash.h
gui:
	gcc -g -o gui gui.c hash.c hash.h `pkg-config --cflags --libs gtk4`
