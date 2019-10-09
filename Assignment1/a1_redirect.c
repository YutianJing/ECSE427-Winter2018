#include <stdio.h>
#include <fcntl.h> // for open
#include <unistd.h> // for close

int main(){
printf("First : Print to stdout\n");                            //print to stdOut

int stdOut = dup(1);                                            //save stdOut

int fd = open("redirect_out.txt", O_CREAT|O_RDWR, 0666);        //creat file with read/write authority
close(1);                                                       //close the stdOut file descriptor
dup(fd);                                                        //move fd to the stdOut file descriptor port

printf ("Second : Print to redirect_out.txt\n");                //print to file
close(fd);                                                      //close handle fd
dup2(stdOut, 1);                                                //replace the output file descriptor with default stdOut

printf ("Third : Print to stdout\n");                           //print to stdOut

return 0;
}