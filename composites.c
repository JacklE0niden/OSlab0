#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include <stdarg.h>

int log_stdout(uint i) {
    /*
    Description: Redirect stdout to a log file named i.log.
    Example:
        - log_stdout(1); // Redirect the stdout to 1.log and return 0.
    Parameters:
        - i (uint): A number
    Return:
        - 0 (int)
    */
    char log_name[15] = "0.log";
    // Your code here
    uint base = 1, i_temp;
    if (i != 0) {
        for (base = 0, i_temp = i; i_temp != 0; ++base, i_temp /= 10); //i=1,base=1;i=22,base=2
        for (uint base_temp = 0, i_temp = i; i_temp != 0; ++base_temp, i_temp /= 10) {
            printf("%d",base_temp);
            log_name[base - base_temp - 1] = '0' + i_temp % 10;
        }
        strcpy(log_name + base, ".log");
    }
    close(1);
    if (open(log_name, O_CREATE | O_WRONLY) != 1) {
        fprintf(2, "log_stdout: open failed\n");
        return -1;
    }
    // End
    return 0;
}

void sub_process(int p_left[2], int i) {

    if (log_stdout(i) < 0) {
        fprintf(2, "composites: log_stdout %d failed\n", i);
        exit(1);
    }
    char m, prime;
    int num_read = 0, p_right[2], pid = 0;
    // int i;
    close(p_left[1]);
    read(p_left[0], &prime, 1);
    printf("prime %d\n", prime);   
    pipe(p_right);
    while (1) {
        if(read(p_left[0], &m, 1) <= 0)
        {
            close(p_left[0]);
            close(p_right[1]);
            break; // 没有数据可读
        }
        else{
            num_read++;

            if(num_read == 1)pid = fork();
            if(pid == 0)//子进程
            {
                sub_process(p_right, i+1);               
            }
            else if (pid > 0){
                close(p_right[0]);
                if (m % prime != 0) {//这个阶段仍然是质数
                    write(p_right[1], &m, 1);
                }
                else {
                    printf("composite %d\n", m);
                }
            }
        }
    }
    wait(0);//如何等待一个进程的子孙进程全部退出
    exit(0);
}

void composites() {
    /*
    Description:
        - A generating process can feed the numbers 2, 3, 4, ..., 35 into the left end of the pipeline: the first process in the line eliminates the multiples of 2, the second eliminates the multiples of 3, the third eliminates the multiples of 5, and so on:
                +---------+    +---------+     +---------+     +---------+
            -2->| print 2 |    |         |     |         |     |         |
            -3->|         |-3->| print 3 |     |         |     |         |
            -4->| print 4 |    |         |     |         |     |         |
            -5->|         |-5->|         |- 5->| print 5 |     |         |
            -6->| print 6 |    |         |     |         |     |         |
            -7->|         |-7->|         |- 7->|         |- 7->| print 7 |
            -8->| print 8 |    |         |     |         |     |         |
            -9->|         |-9->| print 9 |     |         |     |         |
                +---------+    +---------+     +---------+     +---------+
        - Be careful to close file descriptors that a process doesn't need, because otherwise your program will run xv6 out of resources before the first process reaches 35.
        - Once the first process reaches 35, it should wait until the entire pipeline terminates, including all children, grandchildren, &c. Thus the main composites process should only exit after all the output has been printed, and after all the other composites processes have exited.
        - You should create the processes in the pipeline only as they are needed.
    Example:
        - sub_process(4); // Run the 4th sub_process.
    Parameters:
    Return:
        - (void)
    */
    int p_right[2], pid, i = 0;
    int num = 2;
    pipe(p_right);
    pid = fork();
    if(pid == 0){//子进程中，启动sub_process
        sub_process(p_right, i);
    }
    else if(pid > 0)//父进程中，把2到35feed进子进程
    {
        while(num <= 35)
        {
            write(p_right[1], &num, 1);
            // printf("%d\n",num);
            num++;
        }
        close(p_right[1]);
        wait(0);
    }
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(2, "Usage: composites\n");
        exit(1);
    }
    composites();
    exit(0);
}
