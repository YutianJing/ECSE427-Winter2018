
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void run(char *cmd, int fd[]) {
    char *args[2];
    args[0] = cmd;
    args[1] = NULL;

    dup2(fd[1], 1);          //change the stdOut to the output from pipe
    execvp(cmd, args);       //execute the command from the input of method "run"
}

int main(){
    //create pipe here
    int fd[2];
    pipe(fd);

    if(fork() == 0){
        //Child : execute ls using execvp
        int stdOut = dup(1);                    //save stdOut
        run("ls", fd);                          //run ls command
        dup2(stdOut, 1);                        //restore stdOut
    }
    else{
        //Parent : print output from ls here
        char buff[1024];                        //create buffer
        int stdIn = dup(0);                     //save stdIn
        dup2(fd[0], 0);                         //change the stdIn to the input from pipe
        //read(fd[0], buff, sizeof(buff));
        read(0, buff, sizeof(buff));            //move the data read by fd[0] to buffer
        printf("%s", buff);                     //print buffer content
        dup2(stdIn, 0);                         //restore stdIn
    }
    return 0;
}
