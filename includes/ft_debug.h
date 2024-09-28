#ifndef FT_DEBUG_H
#define FT_DEBUG_H

#include <dirent.h> // opendir readdir closedir
#include <sys/stat.h> // stat lstat
#include <pwd.h> // getpwuid
#include <stdio.h> // printf

void print_dirent(struct dirent *dirent);
void print_stat(struct stat *statbuf);
void print_DIR(DIR *dir);

const char* get_dirent_type_string(unsigned char d_type);

#endif