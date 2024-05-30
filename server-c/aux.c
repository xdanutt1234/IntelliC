#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <spawn.h>
#include <wait.h>
#include <fcntl.h>
#include "aux.h"

#define TEMP_FILE_NAME_LENGTH 18
#define TEMP_FILE_PATH_LENGTH 5
#define C_COMPILE_COMMAND_LENGTH (18 + TEMP_FILE_NAME_LENGTH + TEMP_FILE_PATH_LENGTH + TEMP_FILE_NAME_LENGTH)
#define C_EXECUTE_COMMAND_LENGTH (24 + TEMP_FILE_NAME_LENGTH)

void compile_program(struct CompilationParams *arg, char *comp_buffer, char *exec_buffer) {
    int tmp_fd;
    char tmp_filename[TEMP_FILE_PATH_LENGTH + TEMP_FILE_NAME_LENGTH + 1];
    snprintf(tmp_filename, 24, "/tmp/%s_%s_XXXXXX", arg->studID, arg->exerciseID);
    tmp_fd = mkstemp(tmp_filename);
    if (tmp_fd < 0) {
        perror("Failed to create temporary file");
        return;
    } else if (write(tmp_fd, arg->buffer, arg->buffer_length - 1) < 0) {
        perror("Failed to write on temporary file");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    char name[TEMP_FILE_NAME_LENGTH + 1];
    memcpy(name, tmp_filename + 5, TEMP_FILE_NAME_LENGTH);
    name[TEMP_FILE_NAME_LENGTH] = '\0';

    int compilation_command_pipe[2];
    if (pipe(compilation_command_pipe) < 0) {
        perror("Failed to open compilation command pipe");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    posix_spawn_file_actions_t action;
    posix_spawn_file_actions_init(&action);

    if (posix_spawn_file_actions_addclose(&action, compilation_command_pipe[0]) < 0) {
        perror("addclose 1");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    if (posix_spawn_file_actions_adddup2(&action, compilation_command_pipe[1], STDOUT_FILENO) < 0) {
        perror("addup2 1");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    if (posix_spawn_file_actions_adddup2(&action, STDOUT_FILENO, STDERR_FILENO) < 0) {
        perror("adddup2 2");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    if (posix_spawn_file_actions_addclose(&action, compilation_command_pipe[1]) < 0) {
        perror("addclose 2");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    pid_t comp_pid;
    int errPS;
    char *envp[] = { "PATH=/bin:/usr/bin", NULL };
    switch (arg->lang[0]) {
    case 'C':
        printf("C\n");
        char o_file[BUFFER_SIZE];
        snprintf(o_file, sizeof(o_file), "-ocompiled-programs/%s", tmp_filename + TEMP_FILE_PATH_LENGTH);
        char *command_path = "/usr/bin/gcc";
        char *command_and_args[] = { "gcc", o_file, "-xc", tmp_filename, NULL };
        errPS = posix_spawn(&comp_pid, command_path, &action, NULL, command_and_args, envp);
        break;
    }
    if (errPS < 0) {
        perror("Failed to posix spawn");
        close(tmp_fd);
        unlink(tmp_filename);
        return;
    }

    printf("PID of compilation spawn: %jd\n", (intmax_t)comp_pid);
    int s, status;
    int compilation_failed = 1;
    do {
        s = waitpid(comp_pid, &status, WUNTRACED | WCONTINUED);
        if (s < 0) {
            perror("waitpid");
            close(tmp_fd);
            unlink(tmp_filename);
        }
        printf("Compilation spawn status: ");
        if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
            if (WEXITSTATUS(status) == 0) {
                compilation_failed = 0;
            }
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    close(tmp_fd);
    unlink(tmp_filename);

    if (compilation_failed) {
        ssize_t num_bytes = read(compilation_command_pipe[0], comp_buffer, BUFFER_SIZE);
        if (num_bytes < 0) {
            perror("Could't read from compilation pipe'");
        }
        if (num_bytes < BUFFER_SIZE) {
            comp_buffer[num_bytes] = '\0';
        } else {
            comp_buffer[BUFFER_SIZE - 1] = '\0';
        }
        printf("compilation output: %s\n", comp_buffer);
        return;
    }

    close(compilation_command_pipe[0]);
    close(compilation_command_pipe[1]);
    posix_spawn_file_actions_destroy(&action);

    int in_pipe[2], out_pipe[2];
    if (pipe(in_pipe) < 0 || pipe(out_pipe) < 0) {
        perror("Error execution pipes");
        return;
    }
    int flags = fcntl(out_pipe[0], F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return;
    }
    flags |= O_NONBLOCK;
    if (fcntl(out_pipe[0], F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        return;
    }

    posix_spawn_file_actions_t action2;
    posix_spawn_file_actions_init(&action2);

    if (posix_spawn_file_actions_addclose(&action2, in_pipe[1]) < 0) {
        perror("close in_pipe OUT");
        return;
    }
    if (posix_spawn_file_actions_addclose(&action2, out_pipe[0])) {
        perror("close out pipe IN");
        return;
    }
    if (
        posix_spawn_file_actions_adddup2(&action2, in_pipe[0], STDIN_FILENO) < 0) {
        perror("dup2 in pipe IN");
        return;
    }
    if (posix_spawn_file_actions_adddup2(&action2, out_pipe[1], STDOUT_FILENO) < 0) {
        perror("dup2 out pipe OUT stdout");
        return;
    }
    if (posix_spawn_file_actions_adddup2(&action2, out_pipe[1], STDERR_FILENO) < 0) {
        perror("dup2 out pipe OUT stderr");
        return;
    }
    if (posix_spawn_file_actions_addclose(&action2, in_pipe[0]) < 0) {
        perror("close in pipe IN");
        return;
    }
    if (posix_spawn_file_actions_addclose(&action, out_pipe[1])) {
        perror("close out pipe WRITE");
        return;
    }

    int exec_pid;
    switch (arg->lang[0]) {
    case 'C':
        printf("C\n");
        char o_file[BUFFER_SIZE];
        snprintf(o_file, sizeof(o_file), "/home/alex/info/temp-pc/compiled-programs/%s", name);
        printf("command: \"%s\"\n", o_file);
        char *command_and_args[] = { o_file, NULL };
        errPS = posix_spawn(&exec_pid, o_file, &action2, NULL, command_and_args, NULL);
        break;
    }
    posix_spawn_file_actions_destroy(&action2);
    if (errPS < 0) {
        perror("Failed to posix spawn");
        return;
    }

    if (write(in_pipe[1], arg->input, arg->input_length) < 0) {
        perror("Failed to write to in_pipe");
        return;
    }

    printf("PID of execution spawn: %jd\n", (intmax_t)exec_pid);
    do {
        s = waitpid(exec_pid, &status, WUNTRACED | WCONTINUED);
        if (s < 0) {
            perror("waitpid");
            return;
        }
        printf("Execution spawn status: ");
        if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
            if (WEXITSTATUS(status) == 0) {
                compilation_failed = 0;
            }
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    int num_bytes = read(out_pipe[0], exec_buffer, BUFFER_SIZE);
    if (num_bytes < 0) {
        perror("Could't read from compilation pipe'");
        return;
    }
    if (num_bytes < BUFFER_SIZE) {
        exec_buffer[num_bytes] = '\0';
    } else {
        exec_buffer[BUFFER_SIZE - 1] = '\0';
    }

    close(in_pipe[1]);
    close(out_pipe[0]);
}
