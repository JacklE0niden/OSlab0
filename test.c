#include <stdio.h>
#include <string.h>
#include <stdint.h>
#define uint uint32_t
int main(){

    int i=22;
    uint base = 1, i_temp;
    char log_name[15] = "0.log";
    if (i != 0) {
        for (base = 0, i_temp = i; i_temp != 0; ++base, i_temp /= 10); //i=1,base=1;i=22,base=2
        for (uint base_temp = 0, i_temp = i; i_temp != 0; ++base_temp, i_temp /= 10) {
            printf("%d",base_temp);
            log_name[base - base_temp - 1] = '0' + i_temp % 10;
        }
        strcpy(log_name + base, ".log");
    }
    return 0;
}