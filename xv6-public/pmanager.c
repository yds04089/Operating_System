#include "user.h"
#include "stat.h"
#include "types.h"
#include "fcntl.h"

int
getcmd(char *buf, int nbuf)
{
  printf(2, "> ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}
 
void
panic(char *s)
{
  printf(2, "%s\n", s);
  exit();
}

int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork error\n");
  return pid;
}

int main()
{
	if (getadmin("2016024766")<0)
		panic("not administratot mode");
	int cnt = 0;
	printf(2, "[Process Manager]\n\n");
	static char buf[100];
	while(getcmd(buf, sizeof(buf)) >= 0){
		buf[strlen(buf)-1] = 0;
		if (buf[0]=='l' && buf[1]=='i' && buf[2]=='s' && buf[3]=='t'){
			if (buf[4]!=0){
				printf(2, "wrong input\n");
			}
			print_process();
		}
		else if (buf[0]=='k' && buf[1]=='i' && buf[2]=='l' && buf[3]=='l'){
			char* tmp = 0; 
			strcpy(tmp, buf+5);
			int pid = atoi(tmp);
			if (pid == 0){
				printf(2, "kill failed!\n");
				continue;	
			}
			if(!kill(pid))
				printf(2, "kill success!\n");
			else
				printf(2, "kill failed!\n");
		}
		else if (buf[0]=='e' && buf[1]=='x' && buf[2]=='e' && buf[3]=='c' && buf[4]=='u' && buf[5]=='t' && buf[6]=='e'){
			char* path = strchr(buf, ' '); 
			strcpy(path, path+1);
			char *tmp_sz = 0;
			strcpy(tmp_sz, path);
			tmp_sz = strchr(tmp_sz, ' ');
			strcpy(tmp_sz, tmp_sz+1);
			char* tmp = strchr(path, ' ');
			*tmp = 0;
			int stacksize = atoi(tmp_sz);
			if (fork1()==0){
				if (exec2(path, &path, stacksize)==-1){
					printf(2, "execute %s failed\n", path);
				}
				exit();
			}
			cnt++;
		}
		else if (buf[0]=='m' && buf[1]=='e' && buf[2]=='m' && buf[3]=='l' && buf[4]=='i' && buf[5]=='m'){
			char* tmp1 = strchr(buf, ' '); 
			strcpy(tmp1, tmp1+1);
			char *tmp2 = 0;
			strcpy(tmp2, tmp1);
			tmp2 = strchr(tmp2, ' ');
			strcpy(tmp2, tmp2+1);
			char* tmp3 = strchr(tmp1, ' ');
			*tmp3 = 0;
			int pid = atoi(tmp1);
			int limit=atoi(tmp2);
			if(setmemorylimit(pid, limit)==0)
				printf(2, "setting memory limit success!\n");
			else
				printf(2, "setting memory limit failed!\n");
		}
		else if (buf[0]=='e' && buf[1]=='x' && buf[2]=='i' && buf[3]=='t'){
			break;
		}
		else{
			printf(2, "command is wrong.\n");
		}
		printf(2, "\n");
	}
	while(cnt--)
		wait();
	exit();
	return 0;
}