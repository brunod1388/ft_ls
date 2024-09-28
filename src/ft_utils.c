#include "ft_ls.h"
#include <errno.h>

char get_dir_type(mode_t mode) {
	if (S_ISDIR(mode)) return 'd';
	if (S_ISCHR(mode)) return 'c';
	if (S_ISBLK(mode)) return 'b';
	if (S_ISFIFO(mode)) return 'p';
	if (S_ISLNK(mode)) return 'l';
	if (S_ISSOCK(mode)) return 's';
	return '-';
}

void set_permissions(char *perms, mode_t mode) {
	perms[0] = get_dir_type(mode);

	perms[1] = (mode & S_IRUSR) ? 'r' : '-';
	perms[2] = (mode & S_IWUSR) ? 'w' : '-';
	perms[3] = (mode & S_IXUSR) ? 'x' : '-';

	perms[4] = (mode & S_IRGRP) ? 'r' : '-';
	perms[5] = (mode & S_IWGRP) ? 'w' : '-';
	perms[6] = (mode & S_IXGRP) ? 'x' : '-';

	perms[7] = (mode & S_IROTH) ? 'r' : '-';
	perms[8] = (mode & S_IWOTH) ? 'w' : '-';
	perms[9] = (mode & S_IXOTH) ? 'x' : '-';

	perms[10] = '\0';
}

void ft_error(const char *msg, const int exiting) {\
	ft_printf_fd(STDERR_FILENO, "(%serror%s) ft_ls: %s", COLOR_RED, COLOR_RESET, msg);
	if (errno)
		ft_printf_fd(STDERR_FILENO, ": %s", strerror(errno));
	ft_putchar_fd('\n', STDERR_FILENO);
	if (exiting) exit(1);
}