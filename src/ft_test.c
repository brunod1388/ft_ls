#include "ft_ls.h"
#include <errno.h>

const char* get_dirent_type_string(unsigned char d_type) {
    switch (d_type) {
        case DT_UNKNOWN: return "Unknown";
        case DT_FIFO:    return "FIFO";
        case DT_CHR:     return "Character Device";
        case DT_DIR:     return "Directory";
        case DT_BLK:     return "Block Device";
        case DT_REG:     return "Regular File";
        case DT_LNK:     return "Symbolic Link";
        case DT_SOCK:    return "Socket";
        case DT_WHT:     return "Whiteout";
        default:         return "Invalid Type";
    }
}

void ft_error(const char *msg, const int exiting) {\
	ft_printf_fd(STDERR_FILENO, "(%serror%s) ft_ls: %s", COLOR_RED, COLOR_RESET, msg);
	if (errno)
		ft_printf_fd(STDERR_FILENO, ": %s", strerror(errno));
	ft_putchar_fd('\n', STDERR_FILENO);
	if (exiting) exit(1);
}