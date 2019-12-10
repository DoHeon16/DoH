#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
//#include<sys/soundcard.h>
#include<errno.h>
#include<fcntl.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>
void fileReader(char* file);
void printLine(int fd);
void checkLetter(int n,char* _buf, char* input);
int readLine(int fd, char* buf, int nbyte);
void calTypingCount(double playTime);
int size=0;
int checktime=1;
struct timeval startTime, endTime, limitTime;
const int TIME_LIMIT_1=300;
int totalTyping=0;
int typingCount=0;
void menu(){
	int choice=0;
	char num[2];
	char name[8]="Text";
	double playTime;
	int flag=1;
	while(flag){
		
		totalTyping =0;
		typingCount =0;
		printf("\n1. Play\n2. End\n\n==> ");
		scanf("%d", &choice);
		__fpurge(stdin);
		switch(choice){
			case 1:
				printf("\n다음 노래 중 하나를 선택해주세요\n\n1. Taylor Swift - 22\n2. Sia - Alive\n\n==>  ");

				scanf("%c",num);
				strcat(name,num);
				strcat(name,".txt");

				printf("제한시간 : 5분\n");
				//printf(name);
				gettimeofday(&startTime,NULL);
				fileReader(name);
				gettimeofday(&endTime,NULL);
				playTime=(endTime.tv_sec-startTime.tv_sec)+((endTime.tv_usec-startTime.tv_usec)/1000000);
				if(checktime){
					printf("%f초\n",playTime);
				}
				else{
					printf("\n----------- 제한 시간 종료 ----------\n");
				}

				
				printf("오타수 : %d\n",size);
				calTypingCount(playTime);
				break;
			default:
				flag=0;
				printf("\n종료합니다.\n");
				exit(1);
		}
		memset(name,'\0',sizeof(name));
		memset(num,'\0',sizeof(num));
		name[0]='T';name[1]='e';name[2]='x';name[3]='t';
		checktime=1;
	}
}
void calTypingCount(double playTime){
	//printf("%f \n%d\n%d\n", playTime, totalTyping, typeErrorNum);
	double time = (double)60 / playTime;
	typingCount = time*(totalTyping - size);
	//printf("playTime : %f\n",playTime);
	//printf("total : %d, errornum : %d\n", totalTyping, typeErrorNum);
	
	printf("\n분당 평균 타자 속도 : %d 타수\n", typingCount);
}
void main(){
	sigset_t newsigset,oldset;

	if((sigemptyset(&newsigset)==-1)||(sigaddset(&newsigset,SIGINT)==-1)){
		perror("Failed to initialize the signal set\n");
	}
	else if(sigprocmask(SIG_BLOCK,&newsigset,&oldset)==-1){
		perror("Failed to block signal set\n");
	}
		
	menu();
	
	sigprocmask(SIG_SETMASK,&oldset, NULL);
	return;
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
	
	int n=0;
	while(1){
		gettimeofday(&limitTime, NULL);
		if(limitTime.tv_sec<startTime.tv_sec+TIME_LIMIT_1) {
			int check=readLine(fd,_buf,sizeof(_buf));
			//printf("check : %d\n",check);
		
			if(check==-1){
				printf("Can't ReadLine\n");
				return;
			}
			if(check==0){
				break;
			}
			totalTyping+=strlen(_buf)-1;
			printf("%s",_buf);
			__fpurge(stdin);
		
			//printf("_buf : %ld\n",strlen(_buf));
			//scanf("%s",input);
			//printf("_buf size : %ld\n",strlen(_buf));
		 
			gets(input);
		
			//printf("input : %ld\n",strlen(input));	
			//printf("%s\n",input);
		
			__fpurge(stdin);
	
			//printf("input size : %ld\n",strlen(input));
		
			if(strcmp(_buf,input)>0){
				n=strlen(_buf);
			}
			else if(strcmp(_buf,input)<0){
			//	size+=(strlen(input)-strlen(_buf));
				n=strlen(input);
			}
	
			checkLetter(strlen(_buf),_buf,input);
			memset(_buf,'\0',sizeof(_buf));	
		}
		else{
			checktime=0;
			break;
		}		
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
void checkLetter(int n,char* _buf, char* input){
	
	if(strcmp(_buf,input)){
		
		for(int i=0;i<n-1;i++){
			
			//input이 먼저 끝날 때
			if(n==strlen(_buf)&&input[i]=='\n'){
				size+=(strlen(_buf)-1-i);
				break;
			}
			//공백과 문자 처리
			if((input[i]==' '&&_buf[i]!=' ')||
					(input[i]!=' '&&_buf[i]==' ')){
				printf("공백비교\n");
			
				size++;
				continue;
			}

			//일반적 비교
			if(strncmp(_buf+i,input+i,1))
			{
				size++;
				continue;
			}	

		}
	}
	//size--;
	printf("size : %d\n",size);
}
