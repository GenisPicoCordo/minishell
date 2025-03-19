NAME := minishell
CC := cc
CFLAGS := -Wall -Werror -Wextra -g
RM := rm -f
INCLUDES := -I includes

SRC := src/main.c

OBJS := $(SRC:.c=.o)

all: $(NAME)

%.o: %.c Makefile includes/minishell.h
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiling: $(notdir $<)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "Final linking: $(NAME) created."

clean:
	$(RM) $(OBJS)
	@echo "Object files and intermediate compilations removed."

fclean: clean
	$(RM) $(NAME)
	@echo "Executables removed."

re: fclean all

PHONY: all clean fclean re