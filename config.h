/* Colors */

#define PRIMARY_FG "\x1B[31m"
#define SECONDARY_FG "\x1B[35m"

#define DIRECTORY_FG "\x1B[32m"
#define SYMLINK_FG "\x1B[36m"

#define SELECTED_FG "\x1B[30m"
#define SELECTED_BG "\x1B[47m"

#define RESET "\x1B[0m"
#define CLEAR "\e[1;1H\e[2J"

/* Keybinds */

#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'

#define UP2 'k'
#define DOWN2 'j'
#define LEFT2 'h'
#define RIGHT2 'l'

#define SELECT 32 // space
#define REMOVE 'r'
#define COPY 'c'
#define MOVE 'm' // this is rename as well
#define SYMLINK 'y'
#define MAKE_FILE 't' // t as in touch
#define MAKE_DIR 'T'

#define TEXT_OPEN 'e'
#define MEDIA_OPEN 'v'
#define IMAGE_OPEN 'i'
#define DOCUMENT_OPEN 'n'
#define BROWSER_OPEN 'b'
#define EXECUTE 'x'

#define SEARCH '/'
#define OPEN 'o'
#define UNHIDE 'u'
#define JUMP_FIRST 'g'
#define JUMP_LAST 'G'

#define QUIT 'q'
#define NORMAL_RESET 27 // escape

/* Software to open files with */

#define TEXT_EDITOR "vim"
#define MEDIA_PLAYER "mpv"
#define IMAGE_VIEWER "feh"
#define DOCUMENT_READER "firefox"
#define BROWSER "firefox"
