#include <stdio.h>
#include <pthread.h>
#include "request.h"
#include "io_helper.h"

char default_root[] = ".";

void *mythread(int fd) {
    printf("opening thread\n");
    request_handle(fd);
    close_or_die(fd);
    return NULL;
}

//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 
int main(int argc, char *argv[]) {
    int c;
    char *root_dir  = default_root;
    int port        = 10000;
    int threads     = 1;
    int buffers     = 1;
    char * schedalg = "FIFO";
    
    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1)
        switch (c) {
        case 'd':
            root_dir = optarg;
            printf("d: %s\n", root_dir);
            break;
        case 'p':
            port = atoi(optarg);
            printf("p: %d\n", port);
            break;
        case 't':
            threads = atoi(optarg);
            printf("t: %d\n", threads);
            break;
        case 'b':
            buffers = atoi(optarg);
            printf("b: %d\n", buffers);
            break;
        case 's':
            if (strcmp(optarg, "FIFO") == 0 || strcmp(optarg, "SFF") == 0) {
                schedalg = optarg;
                printf("s: %s\n", schedalg);
            } else {
                fprintf(stderr, "scheduling alg must be \"FIFO\" or \"SFF\"\n");
                exit(1);
            }
            break;
        default:
            fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t threads] [-b buffers] [-s scheduling algorithm]\n");
            exit(1);
        }

    //printf("d: %s, p: %d, t: %d, b: %d, s: %s", root_dir, port, threads, buffers, schedalg);
    // run out of this directory
    chdir_or_die(root_dir);

    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);

    while (1) {
        pthread_t p1;

        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);

        if (pthread_create(&p1, NULL, request_handle, conn_fd) != 0) {
            printf("Thread failure\n");
        } else {
            printf("Thread success\n");
        }
    }
    return 0;
}