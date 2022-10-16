int open_file(char path[4096]);
int recursive_remove(char path[4096]);
int print_permissions(struct stat st);
char *remove_path(char path[4096]);
char *combine_file_path(char path[4096], char file[256]);
char *readable_file_size(double size, char *buf);
