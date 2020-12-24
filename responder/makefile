MYSQL_LIB_PATH = -I/usr/include/mysql

INCLUDE = -I/h/ \
	  -I../db/h/ \
	  -I../util/h/ 


all:
	gcc -c $(INCLUDE) $(MYSQL_LIB_PATH) src/*.c -o main.o

install :
	gcc main.o ../db/db.o ../util/util.o -o test.exe

