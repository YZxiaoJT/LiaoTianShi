#include "myclient.h"

void menu1() //登陆注册界面
{
	/* system("clear");
	sleep(1);
	printf("\033[%dC",33);
	printf("---欢迎来到BK聊天室---\n");
	printf("\033[%dC",36);
	printf("---(log)登录 ---\n");
	printf("\033[%dC",36);
	printf("---(reg)注册 ---\n");
	printf("\033[%dC",36);
	printf("---(pri)私聊 ---\n");
	printf("\033[%dC",36);
	printf("---(gro)群聊 ---\n"); */
	
	system("clear");
//	sleep(1);
	printf("\033[?25h");
	printf("\033[0m");
	printf("\033[%d;%dH",0, 0);
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
	printf("\033[%d;%dH密码:",8, 25);

	
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
	printf("\033[%d;%dH",14, 2);
	
}

void menu2(Mnode *message, int vipflag) //聊天界面
{
	//system("clear");
	int i = 0;
	printf("\033[0m");
	printf("\033[%d;%dH",0, 0);
	char *star = "*****************************************************************************************";
	printf("%s\n", star);
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
	
	if (vipflag == vip_ok)
	{
		printf("\033[%d;%dH",3, 2);
		printf("\033[40;35m您已成为尊贵的VIP用户!\033[0m");
	}
	
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
	i = 0;
	while (message->cinfo[i].client_fd != 0)
	{
		printf("%-6d%8d%10s", message->cinfo[i].client_fd, message->cinfo[i].bkid, message->cinfo[i].name);
		printf("\033[%d;%dH",7 + i, 55);
		i++;
	}
	//bzero(message->cinfo, sizeof(Cinfo));
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
	printf("\033[%d;%dH",29, 2);
	fflush(stdout);
}
