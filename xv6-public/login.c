#include "user.h"
#include "stat.h"
#include "types.h"
#include "fcntl.h"

//char *argv[] = { "sh", 0 };

int main()
{
	int wpid, pid, fd;
	for(;;){
		char username[15];
		char password[15];
		char buf[31];
		char rbuf[31];
		for (int i=0; i<31; i++){
			buf[i]='0';
		}
		fd = open("userlist", O_RDONLY);
		if (fd == -1){
			close(fd);
			strcpy(buf, "root");
			strcpy(&buf[15], "1234");

			fd = open("userlist", O_CREATE | O_RDWR);
			if (fd == -1)
				printf(1, "create error\n");
			write(fd, buf, sizeof(buf));
			close(fd);
		}
		printf(1, "Username: ");
		gets(username, sizeof(username));
		username[strlen(username)-1] = 0;
		printf(1, "Password: ");
		gets(password, sizeof(password));
		password[strlen(password)-1] = 0;
		
		fd = open("userlist", O_RDONLY);
		int isexist = 0;
		while(read(fd, rbuf, sizeof(rbuf))>0){
			char tmp1[15];
			char tmp2[15];
			strcpy(tmp1, rbuf);
			strcpy(tmp2, rbuf+15);

			if (!strcmp(tmp1, username) && !strcmp(tmp2, password)){
				isexist = 1;
				//strcpy(argv[1], username);
    			pid = fork();
    			if(pid < 0){
      				printf(1, "init: fork failed\n");
      				exit();
    			}
    			if(pid == 0){
    				char *argv[] = { 0, 0 };
    				strcpy(argv[0], username);
      				exec("sh", argv);
      				printf(1, "init: exec sh failed\n");
      				exit();
    			}
    			while((wpid=wait()) >= 0 && wpid != pid)
      				printf(1, "zombie!\n");
			}
		}
		if (!isexist)
			printf(1, "Wrong login informatation\n");
		close(fd);
		wait();
	}
}