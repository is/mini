#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>


#define MINI_VERSION "0.0.5"

#define ARRAY_LEN(x)  (sizeof(x) / sizeof((x)[0]))


// 检查文件是否可执行
int is_executable(const char *filepath) {
    struct stat st;
    
    // 获取文件状态
    if (stat(filepath, &st) != 0) {
        return 0; // 无法获取文件状态，认为不可执行
    }
    
    // 检查是否为普通文件且具有执行权限
    return S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR);
}

// 执行单个文件
void execute_file(const char *filepath) {
    pid_t pid = fork();
    
    if (pid == 0) {
        // 子进程
        execl(filepath, filepath, (char *)NULL);
        // 如果execl返回，说明执行失败
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
    } else {
        // fork失败
        perror("fork failed");
    }
}

// 扫描目录并执行所有可执行文件
void scan_and_execute(const char *dir_path) {
    DIR *dir;
    struct dirent *entry;
    
    // 打开目录
    dir = opendir(dir_path);
    if (!dir) {
        printf("[mini] %s is not existed\n", dir_path);
        return;
    }
    
    // 遍历目录项
    while ((entry = readdir(dir)) != NULL) {
        // 跳过.和..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // 构造完整路径
        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);
        
        // 检查是否可执行
        if (is_executable(full_path)) {
            printf("[mini] executing: %s\n", full_path);
            fflush(stdout);
            execute_file(full_path);
        }
    }
    
    // 关闭目录
    closedir(dir);
}

int main(int argc, char **argv) {
    printf("mini -- " MINI_VERSION "\n");
    fflush(stdout);
    // 通过环境变量MINI_DIR设置目录，缺省值是/.mini.d
    const char *mini_dir = getenv("MINI_DIR");
    if (mini_dir == NULL) {
        mini_dir = "/.mini.d";
    }
    
    // 首先扫描并执行指定目录下的所有可执行文件
    scan_and_execute(mini_dir);
    
    sigset_t parent_sigset;
    sigfillset(&parent_sigset);

    unsigned int i;
    int signals_for_mini[] = {SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGABRT, SIGTRAP, SIGSYS, SIGTTIN, SIGTTOU};
    for (i = 0; i < ARRAY_LEN(signals_for_mini); i++) {
        sigdelset(&parent_sigset, signals_for_mini[i]);
    }
    sigprocmask(SIG_SETMASK, &parent_sigset, NULL);
    
    siginfo_t sig;
    printf("[mini] boot\n");
    fflush(stdout);

    while(1) {
        errno = 0;
        int sig_result = sigtimedwait(&parent_sigset, &sig, NULL);

        if (errno) {
            perror("sigtimedwait failed");
            break;
        }
        
        if (sig_result == -1) {
            perror("sigtimedwait failed");
            break;
        }
        
        printf("receive signal: %d\n", sig.si_signo);
        fflush(stdout);


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

    printf("[mini] exit\n");
    exit(0);
}
