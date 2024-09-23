#include "ft_ls.h"
#include <errno.h>

char get_dir_type(struct stat statbuf) {
	if (S_ISDIR(statbuf.st_mode)) return 'd';
	if (S_ISCHR(statbuf.st_mode)) return 'c';
	if (S_ISBLK(statbuf.st_mode)) return 'b';
	if (S_ISFIFO(statbuf.st_mode)) return 'p';
	if (S_ISLNK(statbuf.st_mode)) return 'l';
	if (S_ISSOCK(statbuf.st_mode)) return 's';
	return '-';
}

void set_permissions(char *perms, struct stat statbuf) {
	perms[0] = get_dir_type(statbuf);

	perms[1] = (statbuf.st_mode & S_IRUSR) ? 'r' : '-';
	perms[2] = (statbuf.st_mode & S_IWUSR) ? 'w' : '-';
	perms[3] = (statbuf.st_mode & S_IXUSR) ? 'x' : '-';

	perms[4] = (statbuf.st_mode & S_IRGRP) ? 'r' : '-';
	perms[5] = (statbuf.st_mode & S_IWGRP) ? 'w' : '-';
	perms[6] = (statbuf.st_mode & S_IXGRP) ? 'x' : '-';

	perms[7] = (statbuf.st_mode & S_IROTH) ? 'r' : '-';
	perms[8] = (statbuf.st_mode & S_IWOTH) ? 'w' : '-';
	perms[9] = (statbuf.st_mode & S_IXOTH) ? 'x' : '-';

	perms[10] = '\0';
}

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