//xargs
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: %s command [args...]\n", argv[0]);
        exit(1);
    }
    char input[MAXARG]; //input is  
    read(0, input, MAXARG);
    //printf("标准化输入: %s\n",input);

    char *args[MAXARG];
    int argsc = 0;
    for (int i = 1; i < argc; i++) {
        args[argsc++] = argv[i];
        //printf("args[%d]:%s\n", argsc-1,args[argsc-1]);
        if (argv[i + 1] == 0) {
            break;
        }

    }

    char *p = input;
    for(int i=0; i<MAXARG; i++)
    {
        if(input[i]=='\n')
        {
            int pid = fork();            
            if(pid == 0)
            {   
                input[i]=0;
                //拼接一个argv出来，然后调用exec去执行
                args[argsc++] = p;
                args[argsc] = 0; // the end

                exec(argv[1], args);
                exit(0);
            }
            else if(pid>0)
            {
                p = &input[i + 1];// next instruction continues..
                wait(0);
            }
        }
    } 
    exit(0);
}
