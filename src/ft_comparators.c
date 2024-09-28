#include "ft_ls.h"

int dir_data_name_cmp(const void *a, const void *b) {
#if EXA
	return ft_strcmp_ci(((t_dir_data *)a)->name, ((t_dir_data *)b)->name);
#else
	return ft_strcmp(((t_dir_data *)a)->name, ((t_dir_data *)b)->name);
#endif
}

int dir_data_name_cmp_reverse(const void *a, const void *b) {
	return dir_data_name_cmp(b, a);
}

int dir_data_time_cmp(const void *a, const void *b) {
	return ((t_dir_data *)a)->time - ((t_dir_data *)b)->time;
}

int dir_data_time_cmp_reverse(const void *a, const void *b) {
	return dir_data_time_cmp(b, a);
}

int comp_path(const char *queue_path_to_compare, const char *path_to_find) {
	char path_buffer[MAX_PATH_LENGTH] = {0};
	ft_strcat(path_buffer, path_to_find);
	ft_strcat(path_buffer, "/");
	return ft_strncmp(path_buffer, queue_path_to_compare, ft_strlen(path_buffer));
}