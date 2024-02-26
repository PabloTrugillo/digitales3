#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include<sys/wait.h> 
#define E "0123456789"
#define AA "/tmp/CC"
int a,x,c,f;
char d[10];
int main(){
mkfifo(AA, 0777);

x=fork();
switch (x) {
case 0:
c = open(AA, O_WRONLY , 0);
write(c, E,sizeof(E));
close(c);
exit(0);
break;
default:
a = open(AA, O_RDONLY, 0);
f = read(a, d, sizeof(d));
write(STDOUT_FILENO, d, f);
break;
} 
wait(NULL);
close(a);
unlink(AA);
exit(0);
}
