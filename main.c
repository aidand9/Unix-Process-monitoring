#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




#include "process_handling.h"
#include "specific_process.h"



int main(int argc, char **argv){

    int composite_flag = 1;
    int vnode_flag = 0;
    int per_proc_flag = 0;     // deafult flags
    int systemwide_flag = 0;
    int threshold = -1;
    int specific_proc_flag = 0;
    int pid;
    int composite_modified_flag = 0;
    char *temp;



    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "--per-process") == 0){
            per_proc_flag = 1;
            if(vnode_flag == 0 && systemwide_flag == 0 && composite_modified_flag == 0 ) composite_flag = 0;
        }
        else if(strcmp(argv[i], "--systemWide") == 0){
            systemwide_flag = 1;
            if(vnode_flag == 0 && per_proc_flag == 0 && composite_modified_flag == 0 ) composite_flag = 0;
        }
        else if(strcmp(argv[i], "--Vnodes") == 0){
            vnode_flag = 1;
            if(per_proc_flag == 0 && systemwide_flag == 0 && composite_modified_flag == 0 ) composite_flag = 0;    // basic logic for handling command line arguments
        }
        else if(strcmp(argv[i],"--composite") == 0){
            composite_flag = 1;
            composite_modified_flag = 1;
        }
        else if(strstr(argv[i], "--threshold=") != NULL){
            temp = &argv[i][0] + 12;
            threshold = (int) strtol(temp, NULL, 10);
            

        }
        else if(isdigit(argv[i][0]) != 0){
            pid = (int) strtol(argv[i], NULL, 10);
            specific_proc_flag = 1;
        }
    }

    if(specific_proc_flag == 1){


        if(per_proc_flag) specificProc(pid,1,0,0,0, threshold);
        if(systemwide_flag) specificProc(pid,0,1,0,0, threshold);
        if(vnode_flag) specificProc(pid,0,0,1,0, threshold);
        if(composite_flag) specificProc(pid,0,0,0,1, threshold);


        


    } else if(specific_proc_flag == 0){


            

        if(per_proc_flag) allProc(1,0,0,0, threshold);
        if(systemwide_flag) allProc(0,1,0,0, threshold);
        if(vnode_flag) allProc(0,0,1,0, threshold);
        if(composite_flag) allProc(0,0,0,1, threshold);      


    }


    return 0;
}





