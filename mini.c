#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#define ARRAY_LEN(x)  (sizeof(x) / sizeof((x)[0]))

int main(int argc, char **argv) {
    sigset_t parent_sigset;
    sigfillset(&parent_sigset);

    int i;
    int signals_for_mini[] = {SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGABRT, SIGTRAP, SIGSYS, SIGTTIN, SIGTTOU};
    for (i = 0; i < ARRAY_LEN(signals_for_mini); i++) {
        sigdelset(&parent_sigset, signals_for_mini[i]);
    }
    sigprocmask(SIG_SETMASK, &parent_sigset, NULL);
    
    siginfo_t sig;

    while(1) {
        sigtimedwait(&parent_sigset, &sig, NULL);
        if (errno) {
            break;
        }
        printf("receive signal: %d\n", sig.si_signo);

        if (sig.si_signo == SIGINT) {
            break;
        }

        if (sig.si_signo == SIGTERM) {
            break;
        }

        if (sig.si_signo == SIGCHLD) {
            pid_t current_pid;
	        int current_status;

            while(1) {
                current_pid = waitpid(-1, &current_status, WNOHANG);
                if (current_pid <= 0) {
                    break;
                }
            }
        }
    }

    exit(0);
}
