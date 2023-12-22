#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>

int main(int argc, char *argv[]) {
        if (argc != 4) {
                fprintf(stderr, "Incorrect number of arguments\n");
                return 1;
        }

        int n_proc;
        int nval;
        int stime;

        if (!(n_proc = atoi(argv[1]))) {
                fprintf(stderr, "Failed to convert to int\n");
                return 1;
        }

        nval = atoi(argv[2]);

        if (!(stime = atoi(argv[3]))) {
                fprintf(stderr, "Failed to convert to int\n");
                return 1;
        }

        if ((n_proc < 0) || (n_proc > 64)) {
                fprintf(stderr, "Arguments must be greater than or equal to zero\n");
                return 1;
        }

        if ((nval < -19) || (nval > 20)) {
                fprintf(stderr, "Nice value out of bounds\n");
                return 1;
        }

        if ((stime < 0) || (stime > 100)) {
                fprintf(stderr, "Outside range for sleep time (0ms - 100ms)");
                return 1;
        }


        struct rusage usage;
        int    pid[n_proc];

        for (int i = 0; i < n_proc; i++) {
                pid[i] = fork();
                if (pid[i] == -1) { fprintf(stderr, "Fork failed"); return 1; }
                if (pid[i] == 0) {


                        if (i%2 == 0) { nice(nval); }

                        while (1) { ; }
                }
        }

        sleep(stime);

        for (int i = 0; i < n_proc; i++) {
                kill(pid[i], SIGKILL);
        }

        for (int i = 0; i < n_proc; i++) {
                int status;
                wait3(&status, 0, &usage);

                long total_proc_stime = usage.ru_stime.tv_sec*1000000 + usage.ru_stime.tv_usec;
                long total_proc_utime = usage.ru_utime.tv_sec*1000000 + usage.ru_utime.tv_usec;
                long total_proc_time = total_proc_stime + total_proc_utime;

                fprintf(stdout, "%ld\n", total_proc_time);
        }

        return 0;
}