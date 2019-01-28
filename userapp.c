#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define BUFSIZE 50
void checkerror(int res)
{
	if(res<=0){
		perror("Error:");
		exit(EXIT_FAILURE);
	}
}
int main(int argc, char const *argv[])
{
	int fifo_id_2,fifo_id_1;
	char buff[BUFSIZE];
	fifo_id_1 = open("/dev/rtf10",O_WDONLY);
	checkerror(fifo_id_1);
	fifo_id_2=open("/dev/rtf11",O_RDONLY);
	checkerror(fifo_id_2);
	write(fifo_id_1,"Hello From user space!",strlen("Hello From user space!"));
	read(fifo_id_2,buff,BUFSIZE);
	printf("%s\n",buff);
	close(fifo_id_1);
	close(fifo_id_2);
	return 0;
}