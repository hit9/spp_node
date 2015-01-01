NAME = spp_test

INC = ./

SRC = ./test.c ./spp.c ./buf.c

CC = cc

OBJ = $(SRC:c=o)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -g -o $(NAME) -I$(INC)

test: $(NAME)
	touch $(NAME).log
	env MALLOC_TRACE=$(NAME).log ./$(NAME)
	@echo "memory leaks check result:"
	mtrace $(NAME) ${NAME}.log
	rm $(NAME).log

clean:
	rm -rf *.o $(NAME)
