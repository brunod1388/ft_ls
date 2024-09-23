#include "ft_ls.h"

struct s_dir_data *new_dir_data(struct dirent *entry, char *path) {
	struct s_dir_data *data = ft_calloc(1, sizeof(struct s_dir_data));
	if (!data) {
		ft_error("malloc in new_dir_data", 0);
		return NULL;
	}
	data->name = ft_strdup(entry->d_name);
	if (!data->name) {
		clear_dir_data(&data);
		ft_error("malloc in new_dir_data", 0);
		return NULL;
	}
	char full_path[MAX_PATH_LENGTH] = {0};
	ft_fprintf(full_path, "%s/%s", path, entry->d_name);
	struct stat statbuf;
	if (lstat(full_path, &statbuf) == -1) {
		ft_error("lstat in new_dir_data", 0);
	}
	set_permissions(data->perms, statbuf);
	data->size = statbuf.st_size;
	data->time = statbuf.st_mtime;
	data->nlink = statbuf.st_nlink;
	data->owner = ft_strdup(getpwuid(statbuf.st_uid)->pw_name);
	data->group = ft_strdup(getgrgid(statbuf.st_gid)->gr_name);
	if (!data->owner || !data->group) {
		clear_dir_data(&data);
		ft_error("malloc in new_dir_data", 0);
		return NULL;
	}
	return data;
}

void clear_dir_data(t_dir_data *data) {
	if (!data)
		return;
	if (data->name)
		free(data->name);
	if (data->owner)
		free(data->owner);
	if (data->group)
		free(data->group);
	free(data);
}