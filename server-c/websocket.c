#include <stdint.h>
#include <stdio.h>
#include "aux.h"
#include <string.h>
#include <unistd.h>
#include <wsserver/ws.h>

int parseMsg(const unsigned char *msg, uint64_t size, struct CompilationParams *params) {
    if (size < STUDENT_ID_LENGTH + EXERCISE_ID_LENGTH + 4) {
        printf("message to short\n");
        return -1;
    } else {
        int offset = -1;
        int count = 0;
        for (uint64_t i = 0; i < size; i++) {
            if (msg[i] == '@') {
                int len = i - offset - 1;
                switch (count) {
                case 0:
                    if (len != STUDENT_ID_LENGTH) {
                        printf("Invalid student id length %d (correct %d)\n", len, STUDENT_ID_LENGTH);
                        return -1;
                    }
                    memcpy(params->studID, msg + offset + 1, len);
                    params->studID[len] = '\0';
                    break;
                case 1:
                    if (len != EXERCISE_ID_LENGTH) {
                        printf("Invalid exercise id length %d (correct %d)\n", len, EXERCISE_ID_LENGTH);
                        return -1;
                    }
                    memcpy(params->exerciseID, msg + offset + 1, len);
                    params->exerciseID[len] = '\0';
                    break;
                case 2:
                    if (len != 1) {
                        printf("Invalid language length %d (correct 1)\n", len);
                        return -1;
                    }
                    memcpy(params->lang, msg + offset + 1, len);
                    params->lang[len] = '\0';
                    break;
                case 3:
                    memcpy(params->buffer, msg + offset + 1, len);
                    params->buffer[len] = '\0';
                    params->buffer_length = len + 1;
                    break;
                case 4:
                    memcpy(params->input, msg + offset + 1, len);
                    params->input[len] = '\0';
                    params->input_length = len + 1;
                    break;
                }
                offset = i;
                count++;
            }
        }
    }
    return 0;
}

void onopen(ws_cli_conn_t *client) {
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection opened, addr: %s\n", cli);
}

void onclose(ws_cli_conn_t *client) {
    char *cli;
    cli = ws_getaddress(client);
    printf("Connection closed, addr: %s\n", cli);
}

void onmessage(ws_cli_conn_t *client, const unsigned char *msg, uint64_t size, int type) {
    printf("%s\n", msg);
    struct CompilationParams params;
    if (parseMsg(msg, size, &params) < 0) {
        perror("couldnt parse message");
        ws_close_client(client);
    }

    printf("Student: \"%s\"\nExercise: \"%s\"\nLanguage: \"%s\"\nProgram:\n\"%s\"\n",
        params.studID, params.exerciseID, params.lang, params.buffer);

    char comp_buffer[BUFFER_SIZE], exec_buffer[BUFFER_SIZE];
    compile_program(&params, &comp_buffer[0], &exec_buffer[0]);

    printf("compilation output: %s\n", comp_buffer);
    printf("execution output: %s\n", exec_buffer);

    ws_sendframe_txt(client, comp_buffer);
    ws_sendframe_txt(client, exec_buffer);
}

int main(void) {
    /*
     * Main loop, this function never* returns.
     *
     * *If the .thread_loop is != 0, a new thread is created
     * to handle new connections and ws_socket() becomes
     * non-blocking.
     */
    ws_socket(&(struct ws_server) {
        /*
         * Bind host, such as:
         * localhost -> localhost/127.0.0.1
         * 0.0.0.0   -> global IPv4
         * ::        -> global IPv4+IPv6 (Dual stack)
         */
        .host = "localhost",
        .port = 8080,
        .thread_loop = 0,
        .timeout_ms = 1000,
        .evs.onopen = &onopen,
        .evs.onclose = &onclose,
        .evs.onmessage = &onmessage });

    return (0);
}
