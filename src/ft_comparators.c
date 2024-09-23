#include "ft_ls.h"

int dir_data_name_cmp(const void *a, const void *b) {
	return ft_strcmp_ci(((t_dir_data *)a)->name, ((t_dir_data *)b)->name);
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