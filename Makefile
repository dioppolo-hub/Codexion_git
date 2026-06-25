NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC_COMMON = main.c parcing1.c parcing2.c

all: $(NAME)

$(NAME): $(SRC_COMMON) $(SRC_PUSH)
	$(CC) $(CFLAGS) $(SRC_COMMON) -o $(NAME)

clean:
	rm -f $(SRC_COMMON) $(SRC_PUSH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re