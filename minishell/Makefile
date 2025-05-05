NAME := minishell
CC := cc
CFLAGS := -Wall -Werror -Wextra -g #-fsanitize=address
LDFLAGS := -lreadline
RM := rm -fr
LIBFTDIR := libft
LIBFT := $(LIBFTDIR)/libft.a
INCLUDES := -I includes -I $(LIBFTDIR)

SRC := src/main/main.c \
		src/main/main_utils.c \
		src/main/main_utils1.c \
		src/executor/executor.c \
		src/executor/executor_utils1.c \
		src/executor/executor_utils2.c \
		src/executor/executor_utils3.c \
		src/executor/redirections.c \
		src/executor/heredoc.c \
		src/executor/heredoc_utils.c \
		src/liberations/liberations.c \
		src/errors/lexer_errors.c \
		src/parser/mock_parser.c \
		src/parser/lexer.c \
		src/parser/lexer_utils.c \
		src/parser/expansion.c \
		src/parser/operator_handlers.c \
		src/parser/quote_handlers.c \
		src/parser/parser.c \
		src/parser/parser_utils1.c \
		src/parser/parser_utils2.c \
		src/parser/parser_validation.c \
		src/parser/heredoc_utils.c \
		src/signals/signals.c \
		src/signals/signals_utils.c \
		src/builtins/builtins.c \
		src/builtins/echo.c \
		src/builtins/env.c \
    	src/builtins/export.c \
	  	src/builtins/unset.c \
		src/builtins/exit.c \
		src/builtins/pwd.c \
		src/builtins/cd.c \
   		src/utils/builtin_utils.c \
    	src/utils/find_command_path.c \
		src/utils/resolve_path.c \
		src/utils/export_utils.c \
    	src/env/env_utils.c \
	  	src/env/env_init.c \
		src/env/env_modify.c \
	  	src/env/env_to_array.c 

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
