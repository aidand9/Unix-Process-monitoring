#define _XOPEN_SOURCE 500  // Need this for readlink() to work (no clue why).


#include "process_handling.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <ctype.h>

void allProc(int per_proc_flag, int systemwide_flag, int vnode_flag, int composite_flag, int threshold){


    DIR* proc_dir = opendir("/proc");
    int uid = getuid();
    char threshold_storage[1000][1000];
    int threshold_counter = 0;

    struct dirent* proc_entry;
    char path[500];
    if(per_proc_flag == 1){
        printf("PID\tFD\n===============================================\n");
    } else if(systemwide_flag == 1){
        printf("PID\tFD\tFILENAME\n===============================================\n");            // Print table titles depending on flags
    } else if(vnode_flag == 1){
        printf("PID\tFD\tINODE\n===============================================\n");
    } else if(composite_flag == 1){
        printf("PID\tFD\tFILENAME\tINODE\n===============================================\n");
    }

    
    
    
    while ((proc_entry = readdir(proc_dir)) != NULL) {
     
        
        if(!isdigit(*(proc_entry->d_name))){
            continue;
        }
        
        

        
        
        if (atoi(proc_entry->d_name) != 0) {
       
            sprintf(path, "/proc/%s/status", proc_entry->d_name);
            FILE* status = fopen(path, "r");
            if (status == NULL) {
                continue;
            }


            char storage[500];


            for(int i = 0; i < 3; i++){                               // Various checks to make sure process is running, running under user, and making sure we have access to it
                fgets(storage, 500, status);
            }

            if(strstr(storage, "State:	R") == NULL && strstr(storage, "State:	S") == NULL && strstr(storage, "State:	D") == NULL && strstr(storage, "State:	T") == NULL){  // Based off lecture slides, processes with States R,S,D,T can be considered "running" for this assignment 
                continue;
            }

            
            

            for(int i = 0; i < 6; i++){

                fgets(storage, 500, status);
            } int pid_uid = (int) strtol(&storage[5], NULL, 10);

            fclose(status);

           
            if (pid_uid == uid) {
                
                sprintf(path, "/proc/%s/fd", proc_entry->d_name);
                DIR* fd_dir = opendir(path);
                if (fd_dir == NULL) {
                    continue;
                }
          
                struct dirent* fd_entry;
                int fd;
                char fd_path[1000];
                char link_path[1500];
                int l;
                char inode_path[1000];

                int counter = 0;

                while ((fd_entry = readdir(fd_dir)) != NULL) {
                    if (strcmp(fd_entry->d_name,".") != 0 && strcmp(fd_entry->d_name,"..") != 0) {
                        counter++;
                        
                        fd = (int) strtol(fd_entry->d_name, NULL, 10);
                        sprintf(fd_path, "/proc/%s/fd/%s", proc_entry->d_name, fd_entry->d_name);
                        l = readlink(fd_path, link_path, 1499);
                        link_path[l] = '\0';

                        sprintf(inode_path, "/proc/%s/fdinfo/%s", proc_entry->d_name, fd_entry->d_name);     // Get file descriptor, the link associated with it, and the inode

                        FILE *inode_file = fopen(inode_path, "r");

                        for(int i = 0; i < 4; i++){
                            fgets(storage, 500, inode_file);
                        } fclose(inode_file);
                        
                        long inode = strtol(&storage[4], NULL, 10);

                        if(per_proc_flag == 1){
                        printf("%s\t%d\n", proc_entry->d_name, fd);
                        } else if(systemwide_flag == 1){
                        printf("%s\t%d\t%s\n", proc_entry->d_name, fd, link_path);             // Print various lines depending on the flag
                        } else if(vnode_flag == 1){
                        printf("%s\t%d\t%ld\n", proc_entry->d_name, fd, inode);
                        } else if(composite_flag == 1){
                        printf("%s\t%d\t%s\t%ld\n", proc_entry->d_name, fd, link_path, inode);
                        }
                        
                        


                    }
                }
                closedir(fd_dir);
                if(threshold != -1 && (counter > threshold)){         

                    sprintf(threshold_storage[threshold_counter], "%s (%d)", proc_entry->d_name, counter);
                    threshold_counter++;
                            
                } 
            }
        }
    }
    closedir(proc_dir);
    
    if(threshold != -1){

        printf("Offending processes:\n");

        for(int i = 0; i < threshold_counter; i++){
            printf("%s\n", threshold_storage[i]);                   // If threshold is given, this will run
        } if(threshold_counter == 0) printf("None\n");


    }
    
    printf("===============================================\n");


}
