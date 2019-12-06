#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/soundcard.h>
#include<errno.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
void fileReader(char* file);
void printLine(int fd);
void checkLetter(char* _buf, char* input);
int readLine(int fd, char* buf, int nbyte);
int size=0;
/*void menu(){
	int num, choice=1;
	while(choice){
		printf("\n1. Play\n2. End\n\n==>");
		scanf_S("%d", &choice);
		switch(choice){
			case 1:
				printf("111111");
				break;
			default:
				exit(1);
		}
	}
}*/
void main(int argc, char *argv[]){
	struct timeval startTime, endTime;
	double playTime;
	char* name;
	name="Text1.txt";
	gettimeofday(&startTime, NULL);

	fileReader(name);

	gettimeofday(&endTime, NULL);
	playTime=(endTime.tv_sec-startTime.tv_sec)+((endTime.tv_usec-startTime.tv_usec)/1000000);
	printf("%f초 \n",playTime);
	printf("오타수 : %d\n",size);
	return 0;;
}
void fileReader(char* file){//파일 가져오기
	int fd;
	char buf[60];
	fd=open(file, O_RDONLY);
	if(fd==-1){
		printf("File Open Error\n");
		return;
	}

	//오타수 초기화
	size=0;
	printLine(fd);
	close(fd);
}
void printLine(int fd){
	char _buf[80];
	char input[80];
	while(1){
		
		int check=readLine(fd,_buf,sizeof(_buf));
		printf("check : %d\n",check);
		if(check==-1){
			printf("Can't ReadLine\n");
			return;
		}
		if(check==0){
			break;
		}
		printf("%s",_buf);
		scanf("%s",input);
		__fpurge(stdin);
		checkLetter(_buf,input);
		memset(_buf,'\0',sizeof(_buf));	
					
	}

}
int readLine(int fd, char* buf, int nbyte){
	int numread=0;
	int returnval;

	while(numread<nbyte-1){
		returnval=read(fd, buf+numread,1);
		if((returnval==-1)&&(errno==EINTR)) continue;//interrupt error
		if((returnval==0)&&(numread==0)) return 0;//EOF
		if((returnval ==0))break;
		if(returnval==-1) return -1;//error
		numread++;
		if(buf[numread-1]=='\n'){
			buf[numread]='\0';
			return numread;
		}
	}
		errno = EINVAL;
		return -1;

}
void checkLetter(char* _buf, char* input){
	if(strcmp(_buf,input)){
		for(int i=0;i<sizeof(_buf);i++){
			if(_buf[i]!=input[i]){
				size++;
			}
		}
	}
}
