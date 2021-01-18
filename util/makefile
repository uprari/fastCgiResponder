include ../build/top.mk

all :
	gcc $(gdb) $(DEFS) -c -Ih/ src/*.c -o util.o

indent:
	indent -kr src/*.c h/*.h

clean:
	rm -rf *.o 
	find . -type f -name '*~' -delete
