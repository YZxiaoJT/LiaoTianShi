#include "myclient.h"

int logflag = 0;
int vipflag = 0;
int chatflag = 0;
int menuflag = 1;
int i;
Mnode message;
//聊天记录消息存储文件
FILE *read_fp; 
FILE *write_fp;
char readbuff[MSGSIZE * 4];
char writebuff[MSGSIZE * 4];

void *read_thread(void *arg) //读线程函数
{
	
	int client_fd;
	int num;
	
	client_fd = *((int *)arg);
	
	while (running)
	{
		sleep(1);
		num = recvfrom(client_fd, &message, sizeof(message), 0, NULL, NULL);
		if (0 == num)
		{
			printf("服务器已关闭!\n");
			
			pthread_exit(NULL);
		}
		if (message.flag_log == log_ok)
		{
			message.flag_log = 0;
			logflag = log_ok;
			menuflag = 2;
		}
		if (message.flag_vip == vip_ok)
		{
			message.flag_vip = 0;
			vipflag = vip_ok;
		}
		if (message.flag_chat == chat_no)
		{
			message.flag_chat = 0;
			chatflag = chat_no;
			printf("\033[%d;%dH",5, 2);
			printf("您已被禁言!");
			fflush(stdout);
		}
		if (message.flag_kick == kick_yes)
		{
			system("clear");
			printf("\033[%d;%dH",5, 2);
			printf("您已被踢出聊天室!\n");
			fflush(stdout);

			exit(EXIT_SUCCESS);
		}
		
		if (message.flag_file == file_ok)
		{
			message.flag_file = 0;
			FILE *fromfile_fd;
			if ((fromfile_fd = fopen(message.path, "w")) == NULL)
			{
				perror("fopen file error!");
		
				exit(EXIT_FAILURE);
			}
			fwrite(message.msg, MSGSIZE, 1, fromfile_fd);
			fclose(fromfile_fd);
			printf("\033[%d;%dH",5, 2);
			printf("收到了一个文件:\"%s\"", message.path);
			printf("\033[%d;%dH",29, 2);
			fflush(stdout);
			bzero(message.msg, MSGSIZE);
			
			continue;
		}
		
		if (logflag != log_ok)
		{
			printf("\033[%d;%dH",12, 35);
			printf("%s", message.msg);
			fflush(stdout);
			bzero(message.msg, MSGSIZE);
		}
		else
		{
			/* printf("\033[2J");
			menu2(&message); //聊天界面 */
			//printf("vipflag = %d", vipflag);
			if (vipflag == vip_ok)
			{
				printf("\033[%d;%dH",3, 2);
				printf("\033[40;35m您已成为尊贵的VIP用户!\033[0m");
				fflush(stdout);
			}
			printf("\033[%d;%dH",5, 2);
			printf("%s", message.msg);
			fflush(stdout);
			
			strcat(readbuff, message.msg); //将收到的消息存入数组
			
			//fflush(stdout);
			/* printf("\033[%d;%dH", 6, 55);
			for (i = 0; i < 15; i++)
			{
				printf("\033[K");
				printf("\033[%d;%dH",6 + i, 89);
				printf("*");
				printf("\033[%d;%dH",7 + i, 55);
			} */
			
			printf("\033[%d;%dH",5, 45);
			printf("在线人员(客户端号   BKID       昵称)\n");
			printf("\033[%d;%dH",6, 55);
			i = 0;
			while (message.cinfo[i].client_fd != 0)
			{
				printf("%-6d%8d%10s", message.cinfo[i].client_fd, message.cinfo[i].bkid, message.cinfo[i].name);
				printf("\033[%d;%dH",7 + i, 55);
				i++;
			}
			
			printf("\033[%d;%dH",29, 2);
			fflush(stdout);
			bzero(message.msg, MSGSIZE);
		}
		
	}
	
	pthread_exit(NULL);
}

void *write_thread(void *arg) //写线程函数
{
	char choice[10];
	int client_fd;
	int i;
	
	client_fd = *((int *)arg);
	
	while (running)
	{	
		sleep(1);
		bzero(message.msg, MSGSIZE);
		if (logflag == 0)
		{
			
			menu1(); //登陆注册界面
		}
		if (menuflag == 2)
		{
			system("clear");
			menuflag == 0;
		}
		if (logflag == log_ok)
		{
			/* for (i = 0; i < 15; i++)
			{
				printf("\033[K");
				printf("\033[%d;%dH",6 + i, 89);
				printf("*");
				printf("\033[%d;%dH",7 + i, 55);
			} */
			//printf("\033[2J");
			menu2(&message, vipflag); //聊天界面
			printf("\033[%d;%dH",5, 45);
			printf("在线人员(客户端号   BKID       昵称)\n");
			printf("\033[%d;%dH",6, 55);
			i = 0;
			while (message.cinfo[i].client_fd != 0)
			{
				printf("%-6d%8d%10s", message.cinfo[i].client_fd, message.cinfo[i].bkid, message.cinfo[i].name);
				printf("\033[%d;%dH",7 + i, 55);
				i++;
			}
			
			/* printf("\033[%d;%dH",5, 45);
			printf("在线人员(客户端号   BKID       昵称)\n");
			printf("\033[%d;%dH",6, 55);
			i = 0;
			while (message.cinfo[i].client_fd != 0)
			{
				printf("%-6d%8d%10s", message.cinfo[i].client_fd, message.cinfo[i].bkid, message.cinfo[i].name);
				printf("\033[%d;%dH",7 + i, 55);
				i++;
			} */
			
			printf("\033[%d;%dH",29, 2);
			fflush(stdout);
			//bzero(message.cinfo, sizeof(Cinfo));
		}
		
		scanf("%s", &choice);
		//getchar();
		
//登陆
		if (strcmp(choice, "log") == 0)
		{
			message.flag_log = log;
			printf("\033[%d;%dH",5, 35);
			//printf("请输入你的BKID:\n");
			scanf("%d", &message.uinfo.bkid);
			printf("\033[%d;%dH",8, 35);
			scanf("%s", message.uinfo.passward);
			//printf("请输入你的密码:\n");
			/* char c;
			int i = 0;
			while ((c = getch()) != '\r')
			{
				message.uinfo.passward[i] = c;
				putchar('*');
				i++;
				if (i >= 10)
				{
					break;
				}
				
			} */
			
			send(client_fd,&message, sizeof(message), 0);		
		}

//注册
		else if (strcmp(choice, "reg") == 0)
		{
			message.flag_reg = reg;
			while (running)
			{
				//system("clear");
				printf("\033[%d;%dH昵称:",5, 25);
				printf("\033[%d;%dH",5, 35);
				//printf("请输入你的昵称(10个字符以内):\n");
				scanf("%s", message.uinfo.name);
				if (strlen (message.uinfo.name) > 10)
				{
					printf("昵称不符合规范!\n");
					
					continue;
				}
				printf("\033[%d;%dH",8, 35);
				//printf("请输入你的密码(10个字符以内):\n");
				scanf("%s", message.uinfo.passward);
				if (strlen(message.uinfo.passward) > 10)
				{
					printf("密码不符合规范!\n");
					
					continue;
				}
				
				break;
			}	
			//menu1();
			send(client_fd,&message, sizeof(message), 0);
		}

//私聊
		else if (strcmp(choice, "pri") == 0)
		{
			if (chatflag == chat_no)
			{
				printf("您已被禁言!无法使用该功能!");
				fflush(stdout);
				
				continue;
			}
			
			char buff[MSGSIZE];
			message.flag_pri = pri;
			if (logflag != log_ok)
			{
				printf("请先登录您的账号!\n");
			}
			else
			{
				printf("\033[%d;%dH",30, 2);
				printf("请输入好友的客户端ID:");
				scanf("%d", &message.toseid);
				printf("\033[%d;%dH",31, 2);
				printf("请输入您要私发的消息:");
				scanf(" %[^\n]", message.msg);
			}
			
			strcat(writebuff, message.msg);  //将发送的消息存入数组
			send(client_fd,&message, sizeof(message), 0);
		}
		
//群聊
		else if (strcmp(choice, "gro") == 0)
		{
			if (chatflag == chat_no)
			{
				printf("您已被禁言!无法使用该功能!");
				fflush(stdout);
				
				continue;
			}
			
			message.flag_gro = gro;
			if (logflag != log_ok)
			{
				printf("请先登录您的账号!\n");
			}
			else
			{
				//bzero(message.msg, MSGSIZE);
				printf("\033[%d;%dH",30, 2);
				printf("请输入您要群发的消息:");
				scanf("   %[^\n]", message.msg);
			}
			
			strcat(writebuff, message.msg); //将发送的消息存入数组
			send(client_fd,&message, sizeof(message), 0);
		}
		
//发送文件
		else if (strcmp(choice, "file") == 0)
		{
			message.flag_file = file;
			FILE *file_fd;
			char ch;
			if (logflag != log_ok)
			{
				printf("请先登录您的账号!\n");
			}
			else
			{
				printf("\033[%d;%dH",30, 2);
				printf("请输入文件路径:");
				scanf("%s", message.path);
				if ((file_fd = fopen(message.path, "r")) == NULL)
				{
					perror("fopen file error!");
		
					exit(EXIT_FAILURE);
				}
				ch = fgetc(file_fd);
				int i = 0;
				//bzero(message.msg, MSGSIZE);
				while (ch != EOF)
				{
					message.msg[i] = ch;
					i++;
					ch = fgetc(file_fd);
				}
				fclose(file_fd);
			}
			
			send(client_fd,&message, sizeof(message), 0);
		}
		
//查询聊天记录
		else if (strcmp(choice, "save") == 0)
		{
			char rbuff[MSGSIZE * 4] = {0};
			char wbuff[MSGSIZE * 4] = {0};
			fseek(read_fp, 0, SEEK_END);
			fseek(write_fp, 0, SEEK_END);
			fwrite(readbuff, strlen(readbuff), 1, read_fp);
			fwrite(writebuff, strlen(writebuff), 1, write_fp);
			
			fseek(read_fp, 0, SEEK_SET);
			fseek(write_fp, 0, SEEK_SET);
			fread(rbuff, sizeof(rbuff), 1, read_fp);
			fread(wbuff, sizeof(wbuff), 1, write_fp);
			system("clear");
			printf("发送的消息:\n");
			fputs(wbuff, stdout);
			printf("\n收到的消息:\n");
			fputs(rbuff, stdout);
			fflush(stdout);
			
			/* char buff[MSGSIZE];
			fread(buff, MSGSIZE, 1, savefile_fd);
			fputs(buff, stdout);
			fflush(stdout); */
		}
		
//成为会员
		else if (strcmp(choice, "vip") == 0)
		{
			message.flag_vip = vip;
			send(client_fd,&message, sizeof(message), 0);
		}
		
//禁言
		else if (strcmp(choice, "shut") == 0)
		{
			//printf("vipflag = %d\n", vipflag);
			if (vipflag != vip_ok)
			{
				printf("请先成为VIP再使用此功能!");
				fflush(stdout);
				
				continue;
			}
			message.flag_shut = shut;
			printf("请输入禁言用户的客户端端号:");
			scanf("%d", &message.toseid);
			printf("请输入禁言用户的BKID:");
			scanf("%d", &message.bkid);
			send(client_fd,&message, sizeof(message), 0);
		}
		
//踢人
		else if (strcmp(choice, "kick") == 0)
		{
			if (vipflag != vip_ok)
			{
				printf("请先成为VIP再使用此功能!");
				fflush(stdout);
				
				continue;
			}
			message.flag_kick = kick;
			printf("请输入踢出用户的客户端号:");
			scanf("%d", &message.toseid);
			printf("请输入踢出用户的BKID:");
			scanf("%d", &message.bkid);
			send(client_fd,&message, sizeof(message), 0);
		}
		else if (strcmp(choice, "esc") == 0)
		{
			message.flag_esc = esc;
			send(client_fd,&message, sizeof(message), 0);
			system("clear");
			printf("\033[%d;%dH",5, 35);
			printf("\033[40;31m正在退出!\n\033[0m");
			//printf("正在退出!\n");
			sleep(1);
			system("clear");
			exit(EXIT_SUCCESS);
		}
		else
		{
			fflush(stdin);
			continue;	
		}
		fflush(stdin);
	}
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	bzero(&message, sizeof(Mnode));
	bzero(readbuff, sizeof(readbuff));
	bzero(writebuff, sizeof(writebuff));
	
	if ((read_fp = fopen("readhistory.txt", "a+")) == NULL)
	{
			perror("fopen readhistory error!");
		
			exit(EXIT_FAILURE);
	}
	if ((write_fp = fopen("writehistory.txt", "a+")) == NULL)
	{
			perror("fopen writehistory error!");
		
			exit(EXIT_FAILURE);
	}
	
	//IP通过命令行参数传入
	if (2 != argc)
	{
		printf("usage:client <IP address>\n");
		
        exit(EXIT_FAILURE);
	}
	
	//定义变量
	int ret;
	int client_fd;
	socklen_t addrlen;
	struct sockaddr_in server_addr;
	pthread_t tid_read, tid_write;
	
	//初始化变量
	addrlen = sizeof(server_addr);
	bzero(&server_addr, addrlen);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	
	//创建套接字
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0)
	{
		perror("socket fail!");
		
		exit(EXIT_FAILURE);
	}
	//printf("1.socket success!\n");
	
	//连接服务器
	ret = connect(client_fd, (struct sockaddr *)&server_addr, addrlen);
	if (ret < 0)
	{
		perror("connect fail!");
		
		exit(EXIT_FAILURE);
	}
	//printf("2.connect success!\n");
	
	//创建写线程
	ret = pthread_create(&tid_write, NULL, write_thread, (void *)(&client_fd));
	if (ret < 0)
	{
		perror("create read_thread fail!");
		
		exit(EXIT_FAILURE);
	}
	
	//创建读线程
	ret = pthread_create(&tid_read, NULL, read_thread, (void *)(&client_fd));
	if (ret < 0)
	{
		perror("create read_thread fail!");
		
		exit(EXIT_FAILURE);
	}
	
	pthread_detach(tid_write);
    pthread_join(tid_read,NULL);
	
	close(client_fd);
	fclose(read_fp);
	fclose(write_fp);
	
	return 0;
}
