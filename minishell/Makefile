NAME := minishell
CC := cc
CFLAGS := -Wall -Werror -Wextra -g -fsanitize=address
LDFLAGS := -lreadline
RM := rm -fr
LIBFTDIR := libft
LIBFT := $(LIBFTDIR)/libft.a
INCLUDES := -I includes -I $(LIBFTDIR)

SRC := src/main/main.c \
		src/executor/executor.c \
		src/liberations/liberations.c \
		src/errors/lexer_errors.c \
		src/parser/mock_parser.c \
		src/parser/lexer.c \
		src/parser/lexer_utils.c \
		src/parser/expansion.c \
		src/parser/operator_handlers.c \
		src/parser/quote_handlers.c \
		src/builtins/builtins.c \
		src/builtins/echo.c \
		src/builtins/env.c \
		src/builtins/exit.c \
		src/builtins/pwd.c \
		src/builtins/cd.c \
		src/utils/utils.c \
		src/utils/resolve_path.c

OBJS := $(SRC:.c=.o)

LIBS := $(LIBFT)

all: makelibft $(NAME)

makelibft:
	make -C $(LIBFTDIR)

%.o: %.c Makefile includes/minishell.h
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "Compiling: $(notdir $<)"

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@echo "Final linking: $(NAME) created."

clean:
	$(RM) $(OBJS)
	make -C $(LIBFTDIR) clean
	@echo "Object files and intermediate compilations removed."

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFTDIR) fclean
	@echo "Executables removed."

re: fclean all

PHONY: all clean fclean re
