NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c \
      coder.c \
      dongle_management.c \
      heap_1.c\
	  heap_2.c\
	  logger.c\
	  sim.c\
	  time_management.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	
%.o: %.c $(INCLUDE)	Makefile
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re