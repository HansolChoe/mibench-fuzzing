#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <signal.h>

#define BUFFER_SIZE 4096
#define FILE_NAME_SIZE 1024

#ifdef GCOV
#if __GNUC__ < 11
    extern void __gcov_flush();
#else
    extern void __gcov_dump();
#endif

void sighandler(int sig) {
#if __GNUC__ < 11
    __gcov_flush();
#else
    __gcov_dump();
#endif
    fprintf(stderr, "sig: %d", sig);
    exit(sig);
}
#endif

extern int susan_main(int argc, char *argv[]);

int main(int argc, char *argv[]) {
#ifdef GCOV
    signal(SIGSEGV, sighandler);
    signal(SIGFPE, sighandler);
    signal(SIGTERM, sighandler);
    signal(SIGABRT, sighandler);
#endif
    if (argc != 2) {
        printf("Usage: driver <input_file>\n");
        return 1;
    }

    // 퍼징 입력을 읽어옵니다.
    char data[BUFFER_SIZE] = {0};
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        perror("Failed to open file");
        return 1;
    }
    size_t n = fread(data, 1, BUFFER_SIZE - 1, f);  // -1 for null terminator
    fclose(f);

    if (n < 1) {
        printf("No data read from the input file\n");
        return 1;
    }

    char *cmd = strtok(data, "\n"); // 첫 번째 줄만 읽어옵니다.
    char *remaining_data = strtok(NULL, ""); // 나머지 데이터를 읽어옵니다.

    char test_file_name[FILE_NAME_SIZE];
    snprintf(test_file_name, sizeof(test_file_name), "/tmp/%s", basename(argv[1]));

    // 나머지 데이터를 테스트 파일에 씁니다.
    f = fopen(test_file_name, "wb");
    if (f == NULL) {
        perror("Failed to open file");
        return 1;
    }
    if (fwrite(remaining_data, 1, strlen(remaining_data), f) != strlen(remaining_data)) {
        perror("Failed to write to file");
        return 1;
    }
    fclose(f);
    
    char output_file_name[FILE_NAME_SIZE];
    snprintf(output_file_name, sizeof(output_file_name), "/tmp/%s_out", basename(argv[1]));

    char **cmd_line = malloc(sizeof(char *) * (BUFFER_SIZE / 2)); // 최대 토큰 개수를 대비하여 메모리를 할당합니다.
    cmd_line[0] = "./susan";
    cmd_line[1]= test_file_name;
    cmd_line[2] = output_file_name;

    // 공백에 따라 cmd를 나눕니다.
    int index = 3;
    char *token = strtok(cmd, " ");
    while (token != NULL) {
        cmd_line[index++] = token;
        token = strtok(NULL, " ");
    }
    cmd_line[index] = NULL; // 마지막에 NULL 포인터를 추가합니다.

    susan_main(index, cmd_line);  // susan_main 함수를 직접 호출

    free(cmd_line);  // 할당한 메모리를 해제합니다.

    return 0;
}
