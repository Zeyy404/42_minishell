cc:= CC
CCFLAGS:= -Wall -Werror -Wextra

LIBFT:= libft/libft.a

FILES:= tokenizer/token_list.c \
		tokenizer/word_list.c \
		tokenizer/token_syntax.c \
		tokenizer/token_utils.c \
		tokenizer/tokenize_core.c \
		tokenizer/tokenize_ops.c \
		tokenizer/tokenize_quote.c \
		tokenizer/tokenize_word.c \
		parser/parse_utils.c \
		parser/parse.c \
		env/env.c \
		expander/expand_utils.c \
		expander/expander.c \
		execution/find_path.c \
		execution/execute_utils.c \
		execution/execute_builtins.c \
		execution/execute_redirects.c \
		execution/execution.c \
		builtins/builtin_cd.c \
		builtins/builtin_echo.c \
		builtins/builtin_exit.c \
		builtins/builtin_export.c \
		builtins/builtin_export_utils.c \
		builtins/builtin_pwd.c \
		builtins/builtin_unset.c \
		builtins/builtin_env.c \
		builtins/builtin_utils.c \
		signals/signals.c \
		main.c

HEADER:= minishell.h
NAME:= minishell
OFILES:= $(FILES:.c=.o)

BONUS:= 
OBONUS:= $(BONUS:.c=.o)
BHEADER:= 
BNAME:=

all: $(LIBFT) $(NAME)

debug: CFLAGS += -fsanitize=address -ggdb3
debug: all

%.o: %.c $(HEADER) $(BHEADER)
	$(CC) $(CCFLAGS) -c $< -o $@

$(LIBFT):
	make -C libft

$(NAME): $(OFILES)
	$(CC) $(CCFLAGS) $(OFILES) $(LIBFT) -o $(NAME) -lreadline -L/opt/vagrant/embedded/lib

bonus: $(OBONUS)
	$(CC) $(CCFLAGS) $(OBONUS)  -o $(BNAME)
clean:
	make clean -C libft
	rm -f $(OFILES) $(OBONUS)

fclean: clean
	make fclean -C libft
	rm -f $(NAME) $(BNAME)

re: fclean all
.PHONY: all clean fclean re