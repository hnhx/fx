#define _XOPEN_SOURCE 500

#include <ftw.h>
#include <grp.h>
#include <pwd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int external_open(char path[4096], const char *software)
{
	char command[4352];


	if (strcmp(path, software) != 0)
	{
		sprintf(command, "%s \"%s\"", software, path);
	}
	else
	{
		strcpy(command, software);
	}

	system(command);
	return 0;
}

int print_permissions(struct stat st)
{
    mode_t m = st.st_mode;
    struct passwd *pw = getpwuid(st.st_uid);
    struct group  *gr = getgrgid(st.st_gid);

    putchar( m & S_IRUSR ? 'r' : '-' );
    putchar( m & S_IWUSR ? 'w' : '-' );
    putchar( m & S_IXUSR ? 'x' : '-' );
    putchar( m & S_IRGRP ? 'r' : '-' );
    putchar( m & S_IWGRP ? 'w' : '-' );
    putchar( m & S_IXGRP ? 'x' : '-' );
    putchar( m & S_IROTH ? 'r' : '-' );
    putchar( m & S_IWOTH ? 'w' : '-' );
    putchar( m & S_IXOTH ? 'x' : '-' );

    printf(" %s %s \n", pw->pw_name, gr->gr_name);

    return 0;
}

int recursive_remove(char path[4096])
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

char *remove_path(char path[4096])
{
    static char last_entry_name[256];

    strcpy(last_entry_name, path);
    char *token = strtok(last_entry_name, "/");
    while( token != NULL )
    {
        strcpy(last_entry_name, token);
        token = strtok(NULL, "/");
    }

    free(token);

    return last_entry_name;
}

char *combine_file_path(char path[4096], char file[256])
{
      static char combined_path[4096];

      strcpy(combined_path, path);
      if (strcmp(path, "/") != 0)
      {
        strcat(combined_path, "/");
      }
      strcat(combined_path, file);

      return combined_path;
}

char *readable_file_size(double size, char *buf)
{
    int i = 0;

    const char* units[] = {"B", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"};
    while (size > 1024) {
        size /= 1024;
        i++;
    }

    sprintf(buf, "%.*f %s", i, size, units[i]);

    return buf;
}
