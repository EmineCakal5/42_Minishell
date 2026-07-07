#ifndef TOKENIZER_H
# define TOKENIZER_H

/* token.c */
t_token	*new_token(t_type type, char *value);
void	add_token(t_token **head, t_token *new);
int		word_quote_state(char *value);
char	*read_word(char *s, int *i);
t_token	*tokenize(char *s);

/* token_make.c */
int		handle_op_token(char *s, int *i, t_token **head);
int		handle_word_token(char *s, int *i, t_token **head);

/* token_helper.c */
int		is_empty(char c);
int		op(char a);
int		op_number(char *str, int i);
t_type	op_type(char *s, int i);

/* token_control.c */
int		pipe_control(char *str, int i);
int		double_token(char *str, int i);
int		after_token(char *s, int i);
char	*many_word_control(char *s, int *i, char c);

#endif
