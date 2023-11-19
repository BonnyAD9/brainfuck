#ifndef ANSI_COLORS_H_INCLUDED
#define ANSI_COLORS_H_INCLUDED

#define ESC "\x1b"
#define CSI ESC "["

#define RESET CSI "0m"

#define SET_BOLD         CSI "1m"
#define SET_FAINT        CSI "2m"
#define SET_ITALIC       CSI "3m"
#define SET_UNDERLINE    CSI "4m"
#define SET_BLINKING     CSI "5m"
#define SET_INVERSE      CSI "7m"
#define SET_HIDDEN       CSI "8m"
#define SET_STRIKETROUGH CSI "9m"

#define RESET_BOLD         CSI "22m"
#define RESET_FAINT        CSI "22m"
#define RESET_ITALIC       CSI "23m"
#define RESET_UNDERLINE    CSI "24m"
#define RESET_BLINKING     CSI "25m"
#define RESET_INVERSE      CSI "27m"
#define RESET_HIDDEN       CSI "28m"
#define RESET_STRIKETROUGH CSI "29m"

#define SET_DOUBLE_UNDERLINE   CSI "21m"
#define RESET_DOUBLE_UNDERLINE CSI "24m"

#define FG_BLACK        CSI "30m"
#define FG_DARK_RED     CSI "31m"
#define FG_DARK_GREEN   CSI "32m"
#define FG_DARK_YELLOW  CSI "33m"
#define FG_DARK_BLUE    CSI "34m"
#define FG_DARK_MAGENTA CSI "35m"
#define FG_DARK_CYAN    CSI "36m"
#define FG_GRAY         CSI "37m"
#define FG_DEFAULT      CSI "39m"

#define BG_BLACK        CSI "40m"
#define BG_DARK_RED     CSI "41m"
#define BG_DARK_GREEN   CSI "42m"
#define BG_DARK_YELLOW  CSI "43m"
#define BG_DARK_BLUE    CSI "44m"
#define BG_DARK_MAGENTA CSI "45m"
#define BG_DARK_CYAN    CSI "46m"
#define BG_GRAY         CSI "47m"
#define BG_DEFAULT      CSI "49m"

#define FG_DARK_GRAY CSI "90m"
#define FG_RED       CSI "91m"
#define FG_GREEN     CSI "92m"
#define FG_YELLOW    CSI "93m"
#define FG_BLUE      CSI "94m"
#define FG_MAGENTA   CSI "95m"
#define FG_CYAN      CSI "96m"
#define FG_WHITE     CSI "97m"

#define BG_DARK_GRAY CSI "100m"
#define BG_RED       CSI "101m"
#define BG_GREEN     CSI "102m"
#define BG_YELLOW    CSI "103m"
#define BG_BLUE      CSI "104m"
#define BG_MAGENTA   CSI "105m"
#define BG_CYAN      CSI "106m"
#define BG_WHITE     CSI "107m"

#define FG_RGB(r, g, b) CSI "38;2;" #r ";" #g ";" #b "m"
#define BG_RGB(r, g, b) CSI "48;2;" #r ";" #g ";" #b "m"

#endif // ANSI_COLORS_H_INCLUDED
