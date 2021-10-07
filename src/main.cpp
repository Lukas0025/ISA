#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void print_help(char *prog) {
    printf("Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n\n", prog);
    printf("-r <file>           file to send\n");
    printf("-s <ip|hostname>    ip address/hostanme of listening client for file\n");
    printf("-l                  listen for files and save it to $(PWD)\n");
}

int main(int argc, char *argv[]) {
    char *send_file = NULL;
    char *send_addr = NULL;
    bool listen     = false;

    //get opts for parameters parse
    int opt;
    while ((opt = getopt(argc, argv, "r:s:ih")) != -1) {
        switch (opt) {
            case 'r':
                send_file = optarg;
                break;
            case 's':
                send_addr = optarg;
                break;
            case 'l':
                listen = true;
                break;
            case 'h':
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                fprintf(stderr, "Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    //Check of existence of right parameters for modes
    if (!listen && (send_addr == NULL || send_file == NULL)) {
        fprintf(stderr, "send mode and not -r or -s\n");
        fprintf(stderr, "Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n", argv[0]);
        exit(EXIT_FAILURE);
    } else if (listen && (send_addr != NULL || send_file != NULL)) {
        fprintf(stderr, "-l do not need -r or -s\n");
        fprintf(stderr, "Usage: %s  -r <file> -s <ip|hostname> [-l] [-h]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    //check if is IP or HOSTAME valid
    


    exit(EXIT_SUCCESS);
}