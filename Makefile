NAME = codexion

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

SRC_COMMON = main.c parcing1.c parcing2.c parcing_atoi.c \
heap1.c  heap2.c utils1.c utils2.c monitor.c coder_routine1.c \
coder_routine2 init.c


all: $(NAME)

$(NAME): $(SRC_COMMON) $(SRC_PUSH)
	$(CC) $(CFLAGS) $(SRC_COMMON) -o $(NAME)

clean:
	rm -f $(SRC_COMMON) $(SRC_PUSH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re