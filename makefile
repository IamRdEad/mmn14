SOURCES = main.c pre_asm.c asm_part1.c asm_part2.c structs/dictionary.c  handle_instructions1.c handle_commands1.c handle_instructions2.c handle_commands2.c utils.c base_trans.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = data.h structs/dictionary.h
TARGET  = main
CC      = gcc
FLAGS	= -g -c -Wall -pedantic -ansi
CFLAGS  = -g -Wall -pedantic -ansi
LDFLAGS = -lm


$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

main.o: main.c $(HEADERS)
	$(CC) $(FLAGS) main.c 

dictionary.o: structs/dictionary.c structs/dictionary.h
	$(CC) $(FLAGS) dictionary.c 

utils.o: utils.c $(HEADERS)
	$(CC) $(FLAGS) utils.c 

pre_asm.o: pre_asm.c $(HEADERS)
	$(CC) $(FLAGS) pre_asm.c 

asm_part1.o: asm_part1.c $(HEADERS)
	$(CC) $(FLAGS) asm_part1.c 

asm_part2.o: asm_part2.c $(HEADERS)
	$(CC) $(FLAGS) asm_part2.c 

handle_instructions1.o: handle_instructions1.c $(HEADERS)
	$(CC) $(FLAGS) handle_instructions1.c 

handle_commands1.o: handle_commands1.c $(HEADERS)
	$(CC) $(FLAGS) handle_commands1.c 

handle_instructions2.o: handle_instructions2.c $(HEADERS)
	$(CC) $(FLAGS) handle_instructions2.c 

handle_commands2.o: handle_commands2.c $(HEADERS)
	$(CC) $(FLAGS) handle_commands2.c 

base_trans.o: base_trans.c $(HEADERS)
	$(CC) $(FLAGS) base_trans.c 
	
	
clean:
	rm -f $(OBJECTS) *.am *.ext *.ob *.ent
