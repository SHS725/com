#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN_LINE    100
#define LEN_HOSTNAME	30

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;

    char hostname[LEN_HOSTNAME + 1];
    memset(hostname, 0x00, sizeof(hostname));
    gethostname(hostname, LEN_HOSTNAME);

    while (true) {
        char *s;
        int len;
        char cwd[1024];
        char *my_command = "exit, echo, show_command, ls, date, mkdir, rmdir";	
        int size = sizeof(my_command)/sizeof(char);
        char *input;
        args[0] = NULL;                                                                         /* args[] 초기화 */

        printf("\x1b[33mSHS's SHELL \n");
        printf("\x1b[36musername: %s\n", getpwuid(getuid())->pw_name);
        printf("\x1b[36mhostname: %s\n",	 hostname);

        getcwd(cwd, sizeof(cwd));                                                               /* CWD 구현*/
        printf("\x1b[32mCWD is : %s\n\e[0m", cwd);
        
        
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }
        
        printf("[%s]\n", command);
        

        if(strstr(command, "exit")!= NULL){                                                 /* exit 구현 */
        	break;
        }
        
      

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        	
        else {  /* child */
        
        
        if(strstr(command, "echo")!= NULL){                                                /* echo 구현 */
			printf("\x1b[35m%s\n\e[0m",strstr(command, "echo")+5);
        }

        if(strstr(command, "show_command")!= NULL){                                 /* show_command 구현 */
			printf("%s\n", my_command);
        }

        if(strstr(command, "ls")!= NULL){                                                     /* ls 구현 */
        	input = strtok(command," ");
            input = strtok(NULL, " ");
            args[0] = "/bin/ls";
            args[1] = input;
		    input = strtok(NULL, " ");
	    	args[2] = input;
        }	

        if(strstr(command, "date")!= NULL){                                                 /* date 구현 */
			args[0] = "/bin/date";
        }
        

        if(strstr(command, "mkdir")!= NULL){                                                /* mkdir 구현 */
        	input = strtok(command," ");
            input = strtok(NULL, " ");
            args[0] = "/bin/mkdir";
            args[1] = input;
	        input = strtok(NULL, " ");
	        args[2] = input;
              
        }

        if(strstr(command, "rmdir")!= NULL){                                                /* rmdir 구현 */
        	input = strtok(command," ");
            input = strtok(NULL, " ");
            args[0] = "/bin/rmdir";
            args[1] = input;
	        input = strtok(NULL, " ");
        	args[2] = input;
              
        }
        input = strtok(command, " ");                                                    /* command not found 구현*/
        if(!strstr(my_command, input)){
			printf("%s : command not found (Try 'show_command')\n", command);                   
        }
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "\x1b[37mexecve failed\n\e[0m");   
                return 1;
            }
        }
    }
    return 0;
}
