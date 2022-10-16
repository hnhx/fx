#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "utils.h"
#include "config.h"

int main(int argc, char *argv[])
{
    static struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    struct winsize w;
    struct stat st;
    
    struct dirent *entry;

    char c = -1;

    char buf[10];
    
    char software[256];
    char entry_name[256];
    char search_term[256];
    char last_entry_name[256] = { 0 };
    
    char path[4096];
    char new_path[4096];
    char temp_path[4096];
    char symlink_path[4096];
    char current_entry[4096];
    char entry_to_work_with[4096] = { 0 };

    char command[5120];

    char tags[9][4096] = { 0 };

    char selected_entries[1024][1024] = { 0 };

    int chr;
    int rows;
    int entry_count;

    int hidden = 0;
    int position = 0;
    int current_tag = 0;

    double size;
    double file_size_sum;

    DIR *dir;

    FILE *fp, *fptr1, *fptr2;

    if (argc == 1)
    {
      getcwd(path, 4096);
    }
    else
    {
      strcpy(path, argv[1]);
    }

    dir = opendir(path);
    if (dir == NULL)
    {
       printf("This directory doesn't exist! (%s)\n", path);
       return 1;
    }

    do
    {
      switch (c)
      {
        case 'a': // enter the parent directory
        case 'h':
            if (strcmp(path, "/") == 0)
            {
                break;
            }
            strcpy(last_entry_name, remove_path(path));
            strcpy(temp_path, dirname(path));
            strcpy(path, temp_path);
            break;

        case 'w': // go up
        case 'k':
            if (position)
            {
              position--;
            }
            break;

        case 's': // go down
        case 'j':
             if (entry_count - 1 > position)
             {
               position++;
             }
             break;

       case 'd': // enter a directory or open a file
       case 'l':
            stat(current_entry, &st);

	    if (S_ISDIR(st.st_mode))
	    {
		    strcpy(path, current_entry);
		    position = 0;
	    }
	    else
            {
		puts(SECONDARY_FG "\n\nOpen file with:\n");
		puts(SECONDARY_FG "[" PRIMARY_FG "1" SECONDARY_FG"] Text editor");
		puts(SECONDARY_FG "[" PRIMARY_FG "2" SECONDARY_FG"] Media player");
		puts(SECONDARY_FG "[" PRIMARY_FG "3" SECONDARY_FG"] Image viewer");
		puts(SECONDARY_FG "[" PRIMARY_FG "4" SECONDARY_FG"] Document reader");
		puts(SECONDARY_FG "[" PRIMARY_FG "5" SECONDARY_FG"] Browser");
		puts(SECONDARY_FG "\n[" PRIMARY_FG "6" SECONDARY_FG"] Execute it");	

		c = getchar();
		switch (c)
	        {
			case '1':
				strcpy(software, text_editor);
				break;
			case '2':
				strcpy(software, media_player);
				break;
			case '3':
				strcpy(software, image_viewer);
				break;
			case '4':
				strcpy(software, document_reader);
				break;	
			case '5':
				strcpy(software, browser);
				break;	
			case '6':
				strcpy(software, current_entry);
				break;
		}
		
		if (strcmp(software, current_entry) == 0)
		{
			strcpy(command, software);
		}
		else
		{		
	      		sprintf(command, "%s %s", software, current_entry);
		}
		
		system(command);

		printf(RESET);
	    }
	    break;

       case 'g': // go to the top of the directory
	    position = 0;
	    break;

       case 'G': // go to the bottom of the directory
	    position = entry_count - 1;
	    break;

       case 'm': // move or rename an entry
       case 'y': // create a symlink
       case 'c': // copy a file
       case 'r': // remove an entry
	    if (selected_entries[0][0])
	    {
		    if (c == 'r')
		    {
			    printf(SECONDARY_FG"\n\nAre you sure that you want to remove the selected entries? (" PRIMARY_FG "y" SECONDARY_FG "/" PRIMARY_FG "N" SECONDARY_FG ")" RESET " ");
			    if (getchar() != 'y')
			    {
				    break;
			    }
		    }


		    for (int i = 0; 1024 > i; i++)
		    {
			    if (selected_entries[i][0] == 0)
			    {
				    break;
			    }

			    strcpy(entry_to_work_with, selected_entries[i]);

			    if (selected_entries[1][0] == 0 && c != 'r')
			    {
				    printf(SECONDARY_FG "\n\nUse a different name? (" PRIMARY_FG "y" SECONDARY_FG "/" PRIMARY_FG "N" SECONDARY_FG ")" RESET " ");

				    if (getchar() == 'y')
				    {
					    printf(SECONDARY_FG "\nEntry name:" RESET " " PRIMARY_FG);
					    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
					    scanf("%255s", entry_name);
					    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

					    strcpy(temp_path, entry_name);
				    }
				    else
				    {
					    strcpy(temp_path, remove_path(entry_to_work_with));
				    }
			    } 
			    else
			    {
				    strcpy(temp_path, remove_path(entry_to_work_with));
			    }

			    printf(RESET);

			    strcpy(new_path, combine_file_path(path, temp_path));

			    switch (c)
			    {
				    case 'm':
					    rename(entry_to_work_with, new_path);
					    break;
				    case 'y':
					    symlink(entry_to_work_with, new_path);
					    break;
				    case 'c':

					    if (strcmp(entry_to_work_with, new_path) == 0)
					    {
						    break;
					    }

					    fptr1 = fopen(entry_to_work_with, "r");
					    if (fptr1 != NULL)
					    {
						    fptr2 = fopen(new_path, "w");
						    chr = fgetc(fptr1);
						    while (chr != EOF)
						    {
							    fputc(chr, fptr2);
							    chr = fgetc(fptr1);
						    }

						    fclose(fptr2);
					    }

					    fclose(fptr1);
					    break;
				    case 'r':
					    stat(entry_to_work_with, &st);

	    				    if (S_ISDIR(st.st_mode))
	                                    {
						recursive_remove(entry_to_work_with);
				            }
					    else
				            {
					    	remove(entry_to_work_with);
				            }
					    break;
			    }
		    }
		    position = 0;
		    memset(selected_entries, 0, 1024 * 1024);
	    }
	    break;

       case 't': // create a new file
       case 'T': // create a new directory
	    printf(SECONDARY_FG "\n\nEntry name:" RESET " " PRIMARY_FG);
	    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	    scanf("%255s", entry_name);
	    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	    strcpy(temp_path, combine_file_path(path, entry_name));

	    printf(RESET);

	    if (c == 't')
	    {
		    fp = fopen(temp_path, "w");
		    fclose(fp);
	    }
	    else
	    {
		    mkdir(temp_path, 0755);
	    }

	    break;

       case '/': // filter entry names
	    printf(SECONDARY_FG "\n\nSearch entry:" RESET " " PRIMARY_FG);
	    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	    scanf("%255s", search_term);
	    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	    position = 0;
	    break;

       case 'o': // open a given path
	    printf(SECONDARY_FG "\n\nPath to open:" RESET " " PRIMARY_FG);
	    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	    scanf("%255s", temp_path);
	    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	    if (strcmp(temp_path, "..") == 0 || strcmp(temp_path, ".") == 0)
	    {
		    break;
	    }

	    dir = opendir(temp_path);
	    if (dir != NULL)
	    {
		    strcpy(path, temp_path);
		    position = 0;
	    }
	    break;


       case 'u': // unhide hidden entries
	    if (hidden)
	    {
		    hidden = 0;
	    }
	    else
	    {
		    hidden = 1;
	    }

	    position = 0;
	    break;

       case '1' ... '9': // switch between tags
	    current_tag = c - '0' - 1;

	    strcpy(temp_path, tags[current_tag]);

	    if (temp_path[0])
	    {
		    strcpy(path, temp_path);
	    }
	    else
	    {
		    strcpy(tags[current_tag], path);
	    }

	    position = 0;
	    break;

       case 32: // space, select entries
	   for (int i = 0; 1024 > i; i++)
	   {
		   if (selected_entries[i][0])
	           {
			  if (strcmp(selected_entries[i], current_entry) == 0)
		          {
			      for (int j = i; 1024 > j; j++)
			      {
				      if (selected_entries[j][0])
				      {
				      	strcpy(selected_entries[j], selected_entries[j + 1]);
				      }
				      else
				      {
					memset(selected_entries[j - 1], 0, 1024);
				      }
			      }
			      break;
		          }
	           }
		   else
		   {
			   strcpy(selected_entries[i], current_entry);
			   break;
	           }
           }
	   break;

       case 27: // escape, resets everything
           position = 0;
           
	   memset(selected_entries, 0, 1024 * 1024);
           memset(search_term, 0, 256);
           memset(last_entry_name, 0, 256);

           break;
      }

      printf(CLEAR);

      for (int i = 0; 9 > i; i++)
      {
        if (current_tag == i)
        {
          printf("[" SECONDARY_FG  "%d" RESET "] ", i + 1);
        }
        else
        {
          printf("[%d] ", i + 1);
        }
      }

      printf(SECONDARY_FG "\n\nPath: " PRIMARY_FG "%s\n" RESET, path); 

      if (selected_entries[0][0])
      {
	 
	   printf(SECONDARY_FG "Selected entries: [ ");
      	   for (int i = 0; 1024 > i; i++)
      	   {
		   if (selected_entries[i][0])
	           {
			   printf(PRIMARY_FG "%s" SECONDARY_FG ", " RESET, remove_path(selected_entries[i]));
		   }
       	   }
	   printf(SECONDARY_FG "]\n");
      }

      printf(RESET);

      if (hidden)
      {
        puts(SECONDARY_FG "Hidden files:" PRIMARY_FG " ON" RESET);
      }

      printf("\n");

      entry_count = 0;
      file_size_sum = 0;

      strcpy(tags[current_tag], path);
      dir = opendir(path);

      while ((entry = readdir(dir)) != NULL)
      {

        strcpy(entry_name, entry->d_name);

        if (strcmp(entry_name, "..") == 0 || strcmp(entry_name, ".") == 0)
        {
            continue;
        }

        if (!hidden && entry_name[0] == '.')
        {
            continue;
        }

          strcpy(temp_path, combine_file_path(path, entry_name));

	  for (int i = 0; 1024 > i; i++)
          {
		if (strcmp(selected_entries[i], temp_path) == 0)
		{
		   printf(SECONDARY_FG);
		}
          }

          if (strcmp(entry_name, last_entry_name) == 0)
          {
              position = entry_count;
              memset(last_entry_name, 0, 256);
              printf(SELECTED_BG SELECTED_FG);
          }
          else if (search_term[0] && 
	           strcasestr(entry_name, search_term))
          {
              position = entry_count;
              memset(search_term, 0, 256);
              printf(SELECTED_BG SELECTED_FG);
          }
          else if (entry_count == position && last_entry_name[0] == 0)
          {
            printf(SELECTED_BG SELECTED_FG);
          }

          if (position == entry_count)
          {
              strcpy(current_entry, temp_path);
          }

          stat(temp_path, &st);
          size = st.st_size;

          if(entry->d_type == DT_REG)
          {
            file_size_sum += size;
          }
	  
	  ioctl(0, TIOCGWINSZ, &w);
	  rows = (w.ws_row / 2) - 7;

          if ((position + rows > entry_count 
	      && entry_count > position - rows))
          {
            if (entry->d_type == DT_DIR)
            {
               printf(DIRECTORY_FG "[%s]\n", entry_name);
            }
            else if(entry->d_type == DT_LNK)
            {
               readlink(temp_path, symlink_path, 4096);
               printf(SYMLINK_FG "[%s] -> [%s]\n", entry_name, symlink_path);
               memset(symlink_path, 0, 4096);
            }
            else
            {
               printf("[%s] [%s]\n", entry_name, readable_file_size(size, buf));
            }
          }

	 entry_count++;

         printf(RESET);

      }

      if (entry_count == 0)
      {
         puts("Empty directory...");
      }

     printf(SECONDARY_FG "\nPermissions: " PRIMARY_FG);
     stat(current_entry, &st);
     print_permissions(st);
     printf(RESET SECONDARY_FG "Last changed: " PRIMARY_FG "%s" RESET, ctime(&st.st_ctime));

     printf(RESET SECONDARY_FG "\nNumber of entries: " PRIMARY_FG "%d" RESET, entry_count);
     printf(SECONDARY_FG "\nFile size sum: " PRIMARY_FG "%s" RESET,  readable_file_size(file_size_sum, buf));

    } while((c=getchar()) != 'q');

    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

    return 0;
}
