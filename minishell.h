#ifndef MINISHELL_H
# define MINISHELL_H

# include "executor/executor.h"
# include "signals/signals.h"
# include "builtins/builtins.h"
# include "utils/utils.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <ctype.h>
# include <readline/readline.h>
# include <readline/history.h>


/* ─────────────────────────────────────────────
   ENUMS
   ───────────────────────────────────────────── */

typedef enum e_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}	t_type;

typedef enum e_node_type
{
	LEAD_CMD,
	LEAF_PIPE
}	t_node_type;

/* ─────────────────────────────────────────────
   STRUCTS
   ───────────────────────────────────────────── */

typedef struct s_redir
{
	t_type			type;
	char			*target;
	struct s_redir	*next;
} 	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				heredoc_fd;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_token
{
	t_type			type;
	char			*value;
	int				quoted;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_node
{
	t_node_type		type;
	struct s_node	*left;
	struct s_node	*right;
	t_cmd			*cmd;
}	t_node;

/* ─────────────────────────────────────────────
   MODULE HEADERS
   ───────────────────────────────────────────── */

extern int	g_exit_status;

# include "tokenizer/tokenizer.h"
# include "parser/parser.h"
# include "expand/expand.h"
# include "utils/utils.h"

#endif
