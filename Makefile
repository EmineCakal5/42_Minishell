NAME    = minishell

# ── mevcut dosyalar ─────────────────────────────
SRCS    = main.c \
          tokenizer/token.c \
          tokenizer/token_make.c \
          tokenizer/token_helper.c \
          tokenizer/token_control.c \
          parser/parser.c \
          parser/parser_redir.c \
          parser/parser_utils.c \
          expand/expand.c \
          expand/expand_utils.c \
          utils/utils.c \
          utils/free.c \
            executor/executor.c \
            executor/exec_cmd.c \
         executor/exec_pipe.c \
           executor/exec_redir.c \
           executor/exec_heredoc.c \
           signals/signals.c \
               builtins/builtins.c \
               builtins/ft_pwd.c \
               builtins/ft_env.c \
               builtins/ft_exit.c \
               builtins/ft_export.c \
               builtins/ft_unset.c \
               builtins/ft_cd.c  \
               builtins/ft_echo.c 
          

OBJS    = $(SRCS:.c=.o)

CC      = cc
CFLAGS  = -Wall -Werror -Wextra
RM      = rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lreadline

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
