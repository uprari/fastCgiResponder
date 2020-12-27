MYSQL_LIB_PATH = -I/usr/include/mysql


.SUFFIXES: .o .c 

INCLUDE = -Ih/ \
	  -I../db/h/ \
	  -I../util/h/ 
SRC = ./src

OBJ = main.o testUtil.o

%.o : $(SRC)/%.c
	
	gcc -g -c $(INCLUDE) $(MYSQL_LIB_PATH) $< -o $@

all: $(OBJ)
	
	mv *.o obj/
	
install :
	gcc -g obj/*.o ../db/db.o ../util/util.o -o test.exe
clean:
	rm -rf obj/*.o
