include ../build/top.mk

SRC_DIR = src/
HEADER_DIR = h/

INCLUDE = -I../util/h/

SRC = $(wildcard $(SRC_DIR)*.c)
OBJ = $(patsubst %.c, %.o, $(notdir $(SRC)))

all	:	$(OBJ)
		mv *.o obj/
%.o : $(SRC_DIR)%.c
		gcc $(gdb) $(DEFS) -c -I$(HEADER_DIR) $(INCLUDE) $< -o $@

clean:
		rm -rf *.o
		find . -name "*~" -delete

indent:
		indent -kr $(HEADER_DIR)*.h $(SRC_DIR)*.c
print :
		@echo $(SRC)
		@echo $(OBJ)
