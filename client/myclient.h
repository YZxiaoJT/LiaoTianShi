#ifndef _MYCLIENT_H_
#define _MYCLIENT_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <errno.h>
#include <semaphore.h>
#include <curses.h>

#define SERVERPORT 9527
#define MSGSIZE 1024
#define MAXLISTEN 1000
#define running 1

enum ret_values{CREATE_FAILURE, CREATE_SUCCESS, INSERT_SUCCESS};
enum flag_values{log = 100, reg, pri, gro, file, save, vip, shut, kick, esc, log_ok, log_no, file_ok, vip_ok, chat_no, kick_yes};

typedef struct user_info //用户信息
{
	int bkid;
	char name[20];
	char passward[20];
}User;

typedef struct client_info //客户端信息
{
	int client_fd;
	int bkid;
	char name[20];
}Cinfo;

typedef struct Msg_node //客户端发送的数据包
{
	int flag_reg; //注册
	int flag_log; //登录
	int flag_pri; //私聊
	int flag_gro; //群聊
	int flag_file; //发送文件
	int flag_save; //查看记录
	int flag_vip; //成为会员
	int flag_shut; //禁言
	int flag_kick; //踢人
	int flag_esc; //退出
	int flag_chat; //聊天权限
	int toseid; //目标端口号;
	int bkid; //目标id
	User uinfo; //用户信息
	char path[100]; //文件名
	char msg[MSGSIZE]; //传送的消息内容
	Cinfo cinfo[MAXLISTEN]; //各个客户端信息
}Mnode;

void menu1(); //登陆注册界面
void menu2(Mnode *message, int vipflag); //聊天界面

#endif
