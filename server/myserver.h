#ifndef _MYSERVER_H_
#define _MYSERVER_H_

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <errno.h>

#define SERVERPORT 9527
#define MSGSIZE 1024
#define MAXLISTEN 1000
#define running 1

enum ret_values{CREATE_FAILURE, CREATE_SUCCESS, INSERT_SUCCESS, NOT_EXIST, IS_EXIST};
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

typedef struct Online_client //客户端id链表
{
	int client_fd;
	int bkid;
	char name[20];
	struct Online_client *next;
}Oclient;

typedef struct Thread_node //线程函数的参数
{
	int client_fd;
	Oclient *head;
}Tnode;

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

int create_table(sqlite3 *pdb); //创建用户信息表
int create_link(Oclient **head); //创建链表
void release_link(Oclient **head); //销毁链表
int create_newnode(Oclient **newnode); //创建一个新结点
int insert_node(Oclient *head, Oclient *newnode); //插入结点
void msg_send_recv(Tnode *clientnode, sqlite3 * pdb); //收发消息
void release_node(Mnode *message, Tnode *clientnode); //销毁结点
void private_chat(Tnode *clientnode, Mnode *message); //私聊
void group_chat(Tnode *clientnode, Mnode *message); //群聊
int is_bkid_exist(int bkid, sqlite3 *pdb); //判断bkid是否重复
void insert_info(Mnode *message, sqlite3 *pdb); //将注册的用户信息存入数据库中去
int is_passward_right(Mnode *message, sqlite3 *pdb, Tnode *clientnode); //判断密码是否正确
void group_file(Tnode *clientnode, Mnode *message); //将文件发送给每一个人
void insert_clientinfo(Mnode *message, Tnode *clientnode); //保存客户端信息
void be_vip(Mnode *message, sqlite3 *pdb); //数据库中更改权限
void is_vip_shut(Mnode *message, sqlite3 *pdb, Tnode *clientnode); //判断对象是否是vip
void is_vip_kick(Mnode *message, sqlite3 *pdb, Tnode *clientnode); //判断对象是否是vip
void display(char **pResult, int rowc, int colc); //打印结果

#endif
