include ../build/top.mk

MYSQL_LIB_PATH = -I/usr/include/mysql
MYSQL_LIB = $(shell mysql_config --libs)
MYSQL_FLAGS = $(shell mysql_config --cflags)
MEMCACHE_LIB = memcached

SHARED_LIB = -lfcgi
SHARED_PATH = -Wl,-rpath /usr/local/lib

.SUFFIXES: .o .c 

INCLUDE = -Ih/ \
	  -I../db/h/ \
	  -I../cache/h/ \
	  -I../util/h/ \
		-I../include/\

SOURCE_DIR = src/
SRC = $(wildcard $(SOURCE_DIR)*.c)

OBJ = $(patsubst %.c, %.o, $(notdir $(SRC)))

%.o : $(SOURCE_DIR)%.c
	
	gcc $(gdb) $(DEFS) -c $(INCLUDE) $(MYSQL_LIB_PATH) $< -o $@

all: $(OBJ)
	
	mv *.o obj/
	
install :
	gcc -g $(MYSQL_FLAGS) obj/*.o ../db/obj/*.o ../util/util.o ../cache/obj/*.o -o test.exe $(MYSQL_LIB) -l$(MEMCACHE_LIB) $(SHARED_PATH) $(SHARED_LIB)
clean:
	find . -type f -name '*~' -delete
	rm -rf obj/*.o
	rm -rf *.i
indent :
	indent -kr src/*.c h/*.h
print:
	@echo $(OBJ)
	@echo $(SRC)

macro : $(SRC)
		gcc -E $(INCLUDE) $(MYSQL_LIB_PATH)  $(SRC) > main.i
