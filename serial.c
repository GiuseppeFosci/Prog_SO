#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
	int serial_port = open("/dev/ttyACM1", O_RDWR);
	struct termios tty;
	if(tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return 1;
	}
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;
	tty.c_cflag |= CREAD | CLOCAL;
	
	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO;
	tty.c_lflag &= ~ECHOE;
	tty.c_lflag &= ~ECHONL;
	tty.c_lflag &= ~ISIG;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);
	
	tty.c_oflag &= ~OPOST;
	tty.c_oflag &= ~ONLCR;
	
	tty.c_cc[VTIME]=10;
	tty.c_cc[VMIN]=0;
	
	cfsetispeed(&tty, B9600);
	cfsetospeed(&tty, B9600);
	
	if(tcsetattr(serial_port, TCSANOW, &tty) != 0){
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return 1;
	}
	unsigned char carattere;
	scanf("%c", &carattere);
	unsigned char msg[] = {carattere, '\n'};
	char read_buf[256];
	memset(&read_buf, '\0', sizeof(msg));
	sleep(4);
	printf("quello che hai scritto è: %s\n", msg);
	int num_bytes = write(serial_port, msg, sizeof(msg));
	if(num_bytes < 0){
		printf("Error writing: %s", strerror(errno));
		return 1;
	}
	sleep(1);
	read(serial_port, &read_buf, sizeof(msg));
	if(num_bytes < 0) {
		printf("Error reading: %s", strerror(errno));
		return 1;
	}
	unsigned char output[4];
	int idx = 0;
	while(msg[0]=='1'){
		read(serial_port, &read_buf, sizeof(unsigned char));
		output[idx]=*read_buf;
		idx++;
		if(*read_buf=='\n' || *read_buf=='\0'){
			output[idx]='\0';
			idx=0;
			printf("Misurazione: %s", output);
			memset(&output, '\0', sizeof(output));
		}	
	}
	close(serial_port);
	return 0;
}
