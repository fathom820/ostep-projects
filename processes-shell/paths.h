int paths_run(char **args);
void paths_set(char **newpath, int len);
void paths_init();
extern int path_len;
extern int children;
extern int pids[256];
extern char *output_file;