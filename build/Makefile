#compiler
CC = gcc

#compiler flags
#
CFLAGS = -Wall #-Wextra -Werror

#binary name
BIN = userRequestHandler

#PROJECT PATH
PATH = /home/test/Project/BootCamp

#Header file path
INCLUDES = -I$(PATH)/include \
	   -I$(PATH)/responder/h \
	   -I$(PATH)/util/h 
#Source Files
SRC  = $(PATH)/responder/src \
       $(PATH)/util/src

OBJS = $(PATH)/responder/main.o \
       $(PATH)/util/util.o

.SUFFIXES: .c .O

$(BIN): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(BIN)

build : $(BIN)
	
$(OBJS)/%.o: $(SRC)/%.c
	$(CC) $(INCLUDES) -c $< -o $@ $(CFLAGS)
