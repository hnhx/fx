/* Colors */

#define PRIMARY_FG "\x1B[31m"
#define SECONDARY_FG "\x1B[35m"

#define DIRECTORY_FG "\x1B[32m"
#define SYMLINK_FG "\x1B[36m"

#define SELECTED_FG "\x1B[30m"
#define SELECTED_BG "\x1B[47m"

#define RESET "\x1B[0m"
#define CLEAR "\e[1;1H\e[2J"

/* Software to open files with */

const char text_editor[256] = "nano";
const char media_player[256] = "mpv";
const char image_viewer[256] = "feh";
const char document_reader[256] = "firefox";
const char browser[256] = "firefox";
