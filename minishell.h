#ifndef MINISHELL_H
# define MINISHELL_H

# include "types.h"
# include "executor/executor.h"
# include "signals/signals.h"
# include "builtins/builtins.h"
# include "utils/utils.h"
# include "tokenizer/tokenizer.h"
# include "parser/parser.h"
# include "expand/expand.h"

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_exit_status;

#endif
