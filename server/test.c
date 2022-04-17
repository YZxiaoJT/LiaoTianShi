/* #include <stdio.h>
#include <string.h>

/* typedef struct user_info //用户信息
{
	int bkid;
	char name[20];
	char passward[20];
}User; */

/* int main()
{ */
	/* User user1;
	bzero(&user1, sizeof(User));
	user1.bkid = 5;
	strcpy(user1.passward, "123");
	User user2;
	memcpy(&user2, &user1, sizeof(User));
	
	printf("%s\n", user2.passward); */
	
	/* system("clear");
	printf("\033[0m");
	char *star = "*****************************************************************************************";
	printf("%s\n", star);
	int i;
	for (i = 0; i < 15; i++)
	{
		printf("*");
		printf("\033[%dC",87);
		printf("*\n");
	}
	printf("\033[%d;%dH",16, 0);
	printf("%s\n", star);
	
	printf("\033[%d;%dHBKID:",5, 25);
	printf("\033[%d;%dH密码:",9, 25);

	
	printf("\033[%d;%dH",2, 33);
	printf("\033[40;32m---欢迎来到BK聊天室---\033[0m");
	printf("\033[%d;%dH",4, 35);
	printf("\033[40;32m______________________\033[0m");
	printf("\033[%d;%dH",5, 34);
	printf("\033[40;32m|\033[0m");
	printf("\033[%d;%dH",5, 57);
	printf("\033[40;32m|\033[0m");
	printf("\033[%d;%dH",6, 35);
	printf("\033[40;32m----------------------\033[0m");
	
	printf("\033[%d;%dH",7, 35);
	printf("\033[40;32m______________________\033[0m");
	printf("\033[%d;%dH",8, 34);
	printf("\033[40;32m|\033[0m");
	printf("\033[%d;%dH",8, 57);
	printf("\033[40;32m|\033[0m");
	printf("\033[%d;%dH",9, 35);
	printf("\033[40;32m----------------------\033[0m");
	
	printf("\033[%d;%dH",13, 2);
	printf("\033[40;31m                        --(reg)注册"); 
	printf("--(log)登录");
	printf("--(esc)退出--");
	printf("\033[0m");
	printf("\033[%d;%dH",14, 2); */
	
/* 	sleep(1);
	system("clear");
	printf("\033[0m");
	char *star = "*****************************************************************************************";
	printf("%s\n", star);
	int i;
	for (i = 0; i < 33; i++)
	{
		printf("*");
		printf("\033[%dC",87);
		printf("*\n");
	}
	printf("\033[%d;%dH",34, 0);
	printf("%s\n", star);
	
	printf("\033[%d;%dH",2, 33);
	printf("\033[40;32m---欢迎来到BK聊天室---\033[0m");
	printf("\033[%d;%dH",3, 2);
	printf("\033[40;35m您已成为尊贵的VIP用户!");
	
	printf("\033[%d;%dH",4, 2);
	printf("\033[40;33m______________________________________\033[0m");
	printf("\033[%d;%dH",5, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",6, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",7, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",8, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",9, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",10, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",11, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",12, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",13, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",14, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",15, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",16, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",17, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",18, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",18, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",19, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",20, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",21, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",22, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",23, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",24, 40);
	printf("\033[40;33m|\033[0m");
	printf("\033[%d;%dH",25, 2);
	printf("\033[40;33m______________________________________\033[0m");
	
	printf("\033[%d;%dH",5, 45);
	printf("在线人员(客户端号   BKID       昵称)\n");
	printf("\033[%d;%dH",6, 55);
	for (i = 0; i < 5; i++)
	{
		printf("%-6d%8d%10s", 56, 2777, "bbk");
		printf("\033[%d;%dH",7+i, 55);
	}
	printf("\033[%d;%dH", 6, 55);
	for (i = 0; i < 3; i++)
	{
		printf("\033[K");
		printf("\033[%d;%dH",6 + i, 89);
		printf("*");
		printf("\033[%d;%dH",7 + i, 55);
	}
	
	

	
	printf("\033[%d;%dH",27, 2);
	printf("\033[40;31m  --(pri)私聊"); 
	printf("--(gro)群聊");
	printf("--(file)发送文件");
	printf("--(save)保存记录");
	printf("--(vip)成为会员");
	printf("--(esc)退出--");
	printf("\033[%d;%dH",28, 2);
	printf("\033[40;35m                             --(shut)禁言");
	printf("--(kick)踢人--");	
	printf("\033[0m");
	printf("\033[%d;%dH",29, 2); */

/* 	FILE * fd;
	fopen("test.txt", "a+");
	
	printf("1\n");
	//fseek(fd, 0, SEEK_SET);
	printf("2\n");
	char *buff1 = "abc";
	printf("3\n");
	fwrite(buff1, sizeof(buff1), 1, fd);
	printf("4\n");
//	fseek(fd, 0, SEEK_SET);
	char *buff2 = "123";
	fwrite(buff2, sizeof(buff2), 1, fd);
	fclose(fd); */
	
	

/* 	return 0;
}
 */ 
 
 
 
 
 #include <stdio.h>
 #include <string.h>
 
int main ()
{
   /* FILE *fp;
   char str1[] = "This is runoob.com";
 
   fp = fopen( "file.txt" , "a+" );
   fseek(fp, 0, SEEK_END);
   fwrite(str1, sizeof(str1) , 1, fp );
   char str2[] = "haahhahahah";
   fseek(fp, 0, SEEK_SET);
   fwrite(str2, sizeof(str2) , 1, fp );
   fclose(fp); */
   
   char ch[81];
	
	scanf("%[^\n]", ch);
	printf("%d\n", strlen(ch));
	printf("%s\n", ch);
	
	return 0;
  
   return(0);
}
