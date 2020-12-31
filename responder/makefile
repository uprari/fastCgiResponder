MYSQL_LIB_PATH = -I/usr/include/mysql
MYSQL_LIB = $(shell mysql_config --libs)
MYSQL_FLAGS = $(shell mysql_config --cflags)
MEMCACHE_LIB = memcached

.SUFFIXES: .o .c 

INCLUDE = -Ih/ \
	  -I../db/h/ \
	  -I../cache/h/ \
	  -I../util/h/ 

SOURCE_DIR = src/
SRC = $(wildcard $(SOURCE_DIR)*.c)

OBJ = $(patsubst %.c, %.o, $(notdir $(SRC)))

%.o : $(SOURCE_DIR)%.c
	
	gcc -g -c $(INCLUDE) $(MYSQL_LIB_PATH) $< -o $@

all: $(OBJ)
	
	mv *.o obj/
	
install :
	gcc -g $(MYSQL_FLAGS) obj/*.o ../db/obj/*.o ../util/util.o ../cache/obj/*.o -o test.exe $(MYSQL_LIB) -l$(MEMCACHE_LIB)
clean:
	find . -type f -name '*~' -delete
	rm -rf obj/*.o

indent :
	indent -kr src/*.c h/*.h
print:
	@echo $(OBJ)
	@echo $(SRC)
