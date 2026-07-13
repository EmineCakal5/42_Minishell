NAME    = minishell

SRCS    = main.c \
          tokenizer/token.c \
          tokenizer/token_make.c \
          tokenizer/token_helper.c \
          tokenizer/token_control.c \
          parser/parser.c \
          parser/parser_cmd.c \
          parser/parser_debug.c \
          parser/parser_redir.c \
          parser/parser_pipe.c \
          parser/parser_utils.c \
          expand/expand.c \
          expand/expand_value.c \
          expand/expand_quote.c \
          expand/expand_dollar.c \
          expand/expand_env.c \
          expand/expand_append.c \
          expand/expand_heredoc.c \
          expand/expand_split.c \
          utils/utils.c \
          utils/input.c \
          utils/free.c \
          utils/env_utils.c \
          utils/ft_string.c \
          utils/ft_mem.c \
          utils/ft_char.c \
          executor/executor.c \
          executor/exec_cmd.c \
          executor/exec_error.c \
          executor/exec_builtin.c \
          executor/exec_path.c \
          executor/exec_path_utils.c \
          executor/exec_pipe.c \
          executor/exec_redir.c \
          executor/exec_heredoc.c \
          executor/exec_heredoc_prep.c \
          executor/exec_heredoc_io.c \
          signals/signals.c \
          signals/signals_heredoc.c \
          builtins/builtins.c \
          builtins/builtins_utils.c \
          builtins/ft_pwd.c \
          builtins/ft_env.c \
          builtins/ft_exit.c \
          builtins/ft_export.c \
          builtins/ft_export_utils.c \
          builtins/ft_unset.c \
          builtins/ft_cd.c \
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
