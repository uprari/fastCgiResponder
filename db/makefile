INCLUDE = -I../util/h

.SUFFIXES: .o .c

SOURCE_DIR = src/
HEADER_DIR = h/

SRC = $(wildcard $(SOURCE_DIR)*.c)
OBJ = $(patsubst %.c, %.o, $(notdir $(SRC)))

all : $(OBJ)
	mv *.o obj/

%.o: $(SOURCE_DIR)%.c
	gcc -g -c  $(INCLUDE) -I$(HEADER_DIR) -I/usr/include/mysql/ $< -o $@
clean:
	rm -rf *.o

indent :
	indent -kr $(SOURCE_DIR)*.c $(HEADER_DIR)*.h
print:
	@echo $(SRC)
	@echo $(OBJ)
