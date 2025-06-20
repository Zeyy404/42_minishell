cc:= CC
CCFLAGS:= -Wall -Werror -Wextra

LIBFT:= libft/libft.a

FILES:= tokenizer/token_list.c \
		tokenizer/token_syntax.c \
		tokenizer/token_utils.c \
		tokenizer/tokenize_core.c \
		tokenizer/tokenize_ops.c \
		tokenizer/tokenize_quote.c \
		tokenizer/tokenize_word.c \
		env/env.c \
		main.c

HEADER:= minishell.h
NAME:= minishell
OFILES:= $(FILES:.c=.o)

BONUS:= 
OBONUS:= $(BONUS:.c=.o)
BHEADER:= 
BNAME:=

all: $(LIBFT) $(NAME)


%.o: %.c $(HEADER) $(BHEADER)
	$(CC) $(CCFLAGS) -c $< -o $@

$(LIBFT):
	make -C libft

$(NAME): $(OFILES)
	$(CC) $(CCFLAGS) $(OFILES) $(LIBFT) -o $(NAME) -lreadline

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