char * cmdops_read_line(char *line);
char ** cmdops_split(char *line, char *delim);
char ** cmdops_get_redirect(char *line);
extern int cmd_args_len;
void remove_spaces(char *s);