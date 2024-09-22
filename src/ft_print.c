#include "ft_ls.h"
#include <errno.h>

void print_dir(int fd, char *path, struct dirent *entry, t_options options) {
	struct stat statbuf;
	char fullPath[MAX_PATH_LENGTH] = {0};

	ft_fprintf(fullPath, "%s/%s", path, entry->d_name);

	if (lstat(fullPath, &statbuf) == -1) {
		char error[256] = {0};
		ft_strncat(error, "ft_ls: lstat:", 13);
		ft_strncat(error, entry->d_name, 256 -13 -1);
		// perror(error);
		// strerror(errno);
		return;
	}

	if (entry->d_name[0] == '.' && !(options & OPTION_a))
		return;
	if (options & OPTION_L)
		print_list(fd, entry, options, statbuf);
	else
		print_simple(fd, entry);
}

void print_list(int fd, struct dirent *entry, t_options options, struct stat statbuf) {
	struct passwd	*pw = getpwuid(statbuf.st_uid);
	struct group	*gr = getgrgid(statbuf.st_gid);
	char			*time = ctime(&statbuf.st_mtime);
	char 			formatted_permissions[256] = {0},
					formatted_size[32] = {0},
					formatted_time[32] = {0},
					formatted_name[MAX_PATH_LENGTH] = {0},
					perms[11] = {0};

	set_permissions(perms, statbuf);
	time[ft_strlen(time) - 1] = '\0';
	format_size(formatted_size, statbuf.st_size);
	format_time(formatted_time, time);
	format_perms(formatted_permissions, perms);
	ft_printf_fd(
		fd,
		"%s %s%s %s%2d %s%s %s %s%s %s%s\n",
		formatted_permissions,
		COLOR_GREEN,
		formatted_size,
		COLOR_BLUE,
		statbuf.st_nlink,
		COLOR_YELLOW,
		pw->pw_name,
		gr->gr_name,
		COLOR_BLUE,
		formatted_time,
		entry->d_name,
		COLOR_RESET
	);
}

void print_simple(int fd, struct dirent *entry) {
	ft_printf_fd(fd, "%s\n", entry->d_name);
}
