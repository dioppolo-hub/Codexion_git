NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC_COMMON = *.c

SRC_PUSH = main.c

OBJ_COMMON = $(SRC_COMMON:.c=.o)

OBJ_PUSH = $(SRC_PUSH.c=.o)

all: $(NAME)

$(NAME): $(OBJ_COMMON) $(OBJ_PUSH)
	$(CC) $(CFLAGS) $(OBJ_COMMON) $(OBJ_PUSH) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ_COMMON) $(OBJ_PUSH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re