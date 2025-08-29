CC = gcc
FLAGS = -Wall -g

TARGET = VMTranslater
SRCS = main.c  Parser.c CodeWriter.c VMTranslater.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) -o $(TARGET) $(OBJS)  

%.o: %.c %.h
	$(CC) $(FLAGS) -c $< -o $@ 

main.o: main.c VMTranslater.h Parser.h CodeWriter.h
	$(CC) $(FLAGS) -c main.c -o main.o 

clean: 
	rm -f $(OBJS) $(TARGET)

# Mark that all and clean are not files but cmd
.PHONY: all clean
