#include "myserver.h"

extern sqlite3 *pdb;

int create_table(sqlite3 *pdb) //创建用户信息表
{
	int ret;
	char *sql = NULL;
	char *errmsg = NULL;
	
	sql = "create table if not exists user(bkid integer not null primary key, name text not null, passward text not null, vip integer default 0);";
	
	ret = sqlite3_exec(pdb, sql, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("Create table fail: %s\n", sqlite3_errmsg(pdb));
		
		return -1;
	}
	
	return SQLITE_OK;
}

int create_link(Oclient **head) //创建链表
{
	*head = (Oclient *)malloc(sizeof(Oclient));
	if (NULL == *head)
	{
		return CREATE_FAILURE;
	}
	else
	{
		(*head)->next = NULL;
		return CREATE_SUCCESS;
	}
}

void release_link(Oclient **head) //销毁链表
{
	Oclient *p = *head;
	while (NULL != p)
	{
		*head = (*head)->next;
		free(p);
		p = *head;
	}
	p = NULL;
}

int create_newnode(Oclient **newnode) //创建一个新结点
{
	*newnode = (Oclient *)malloc(sizeof(Oclient));
	if (NULL == *newnode)
	{
		return CREATE_FAILURE;
	}
	else
	{
		(*newnode)->next = NULL;
		return CREATE_SUCCESS;
	}
}

int insert_node(Oclient *head, Oclient *newnode) //插入结点
{
	newnode->next = head->next;
	head->next = newnode;
	
	return INSERT_SUCCESS;
}

int is_bkid_exist(int bkid, sqlite3 *pdb) //判断bkid是否重复
{
	int ret;
	char sql[100];
	char **pResult = NULL;
	int colc, rowc;
	char *errmsg = NULL;
	
	sprintf(sql, "select * from user where bkid = %d;", bkid);
	ret = sqlite3_get_table(pdb, sql, &pResult, &rowc, &colc, &errmsg);
	if (SQLITE_OK != ret)
	{
		printf("Select fail: %s", sqlite3_errmsg(pdb));
		
		return -1;
	}
	sqlite3_free_table(pResult);
	if (0 == rowc)
	{
		return NOT_EXIST;
	}
	else
	{
		return IS_EXIST;
	}
}

void insert_info(Mnode *message, sqlite3 *pdb) //将注册的用户信息存入数据库中去
{
	int ret;
	char sql[100];
	char *errmsg = NULL;
	sprintf(sql, "insert into user (bkid, name, passward) values (%d, '%s', '%s');", message->uinfo.bkid, message->uinfo.name, message->uinfo.passward);

	ret = sqlite3_exec(pdb, sql, NULL, NULL, &errmsg);
	if (SQLITE_OK != ret)
	{
		printf("Insert values fail: %s", sqlite3_errmsg(pdb));
		
		return;
	}
	
	char *sqlx = NULL;
	char **pResult = NULL;
	int colc, rowc;
	sqlx = "select * from user;";
	if (SQLITE_OK != sqlite3_get_table(pdb, sqlx, &pResult, &rowc, &colc, &errmsg))
	{
		printf("Select fail: %s", sqlite3_errmsg(pdb));
		
		return;
	}
	if (0 == rowc)
	{
		printf("user为空！\n");
	}
	else
	{
		display(pResult, rowc, colc); //打印结果	
	}
	
	sprintf(message->msg, "注册成功!您的BKID:%d\n", message->uinfo.bkid);
	sqlite3_free_table(pResult);
}

void display(char **pResult, int rowc, int colc) //打印结果
{
	int i;
	for (i = 0; i < (rowc + 1) * colc; i++)
		{
			printf("%-25s", pResult[i]);
			if (((i + 1) % colc) == 0)
			{
				putchar(10);
			}
		}
}

int is_passward_right(Mnode *message, sqlite3 *pdb, Tnode *clientnode) //判断密码是否正确
{
	Oclient *tmp;
	tmp = clientnode->head->next;
	
	int ret;
	char sql[100];
	char *errmsg = NULL;
	char **pResult = NULL;
	int colc, rowc;
	
	sprintf(sql, "select name, passward, vip from user where bkid = %d;", message->uinfo.bkid);
	ret = sqlite3_get_table(pdb, sql, &pResult, &rowc, &colc, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("Select fail: %s", sqlite3_errmsg(pdb));
		
		return SQLITE_ERROR;
	}
	
	if (0 == rowc) //id未注册
	{
		strcpy(message->msg, "该BKID未注册!");
	}
	else
	{
		//登陆成功
		if (strcmp(pResult[4], message->uinfo.passward) == 0) 
		{
			message->flag_log = log_ok;
			
			//判断是否是vip
			if (strcmp(pResult[5], "1") == 0)
			{
				message->flag_vip = vip_ok;
			}
			
			//将当前信息存入链表结点中去
			strcpy(tmp->name, pResult[3]);
			while (tmp != NULL)
			{
				if (tmp->client_fd == clientnode->client_fd)
				{
					break;
				}
				tmp = tmp->next;
			}
			tmp->bkid = message->uinfo.bkid;
			insert_clientinfo(message, clientnode); //保存客户端信息然后打包发送
			
			//strcpy(message->msg, "登陆成功!");
		}
		else
		{
			message->flag_log = log_no;
			strcpy(message->msg, "密码错误!");
		}
	}
	
	sqlite3_free_table(pResult);
	
	return SQLITE_OK;
}

void msg_send_recv(Tnode *clientnode, sqlite3 * pdb) //收发消息
{
	int num, i, ret;
	Mnode message;
	bzero(&message, sizeof(Mnode));
	
	//接收消息
	num = recv(clientnode->client_fd, &message, sizeof(message), 0); 
	if (0 == num)
	{
		printf("|客户端%d下线！|\n", clientnode->client_fd);
		
		release_node(&message, clientnode); //销毁结点
		
		
		pthread_exit(NULL);
	}
	else
	{
		if (message.flag_reg == reg)
		{
			message.flag_reg = 0;
			do
			{
				message.uinfo.bkid = rand() % 10000; //随机分配bkid号;
				ret = is_bkid_exist(message.uinfo.bkid, pdb); //判断bkid是否重复
			}while(ret != NOT_EXIST);
			
			insert_info(&message, pdb); //将注册的用户信息存入数据库中去
			send(clientnode->client_fd, &message, sizeof(message), 0);
		}
		
		if (message.flag_log == log)
		{
		//	printf("message.flag_log = %d\n", message.flag_log);
			ret = is_passward_right(&message, pdb, clientnode); //判断密码是否正确
			if (ret != SQLITE_OK)
			{
				perror("判断出错!\n");
				
			}
			send(clientnode->client_fd, &message, sizeof(message), 0);
		}
		
		if (message.flag_gro == gro)
		{
			message.flag_gro = 0;
		//	insert_clientinfo(&message, clientnode->head); //保存客户端信息然后打包发送
			group_chat(clientnode, &message); //群聊
		}
		
		if (message.flag_pri == pri)
		{
			message.flag_pri = 0;
			//insert_clientinfo(&message, clientnode->head); //保存客户端信息然后打包发送
			private_chat(clientnode, &message); //私聊
		}
		
		if (message.flag_file == file) 
		{
			message.flag_file = file_ok;
			//insert_clientinfo(&message, clientnode->head); //保存客户端信息然后打包发送
			group_file(clientnode, &message); //将文件发送给每一个人
		}
		
		if (message.flag_vip == vip) //VIP权限申请
		{
			message.flag_vip = vip_ok;
			//insert_clientinfo(&message, clientnode->head); //保存客户端信息然后打包发送
			be_vip(&message, pdb); //数据库中更改权限
			send(clientnode->client_fd, &message, sizeof(message), 0);
		}
		
		if (message.flag_shut == shut) //禁言
		{
			message.flag_shut = 0;
			is_vip_shut(&message, pdb, clientnode); //判断对象是否是vip
		
		}
		
		if (message.flag_kick == kick) //踢人
		{
			message.flag_kick = 0;
			is_vip_kick(&message, pdb, clientnode); //判断对象是否是vip
		
		}
		
		if (message.flag_esc == esc) //客户端退出
		{
			int i = 0;
			message.flag_esc = 0;
			Oclient *tmp = NULL;
			tmp = clientnode->head->next;
	
			//客户端信息存入结点链表中去
			while (tmp != NULL)
			{
				if (tmp->client_fd == clientnode->client_fd)
				{
					tmp = tmp->next;
					continue;
				}
				message.cinfo[i].client_fd = tmp->client_fd;
				message.cinfo[i].bkid = tmp->bkid;
				strcpy(message.cinfo[i].name, tmp->name);
				i++;
				tmp = tmp->next;
			}
	
			char buff[MSGSIZE] = {0};
			bzero(message.msg, MSGSIZE);
			sprintf(buff, "%d号客户端的好友已退出聊天室！", clientnode->client_fd);
			strcpy(message.msg, buff);
			//将结点链表中的客户端信息发给每一个客户端
			tmp = clientnode->head->next;
			while (tmp != NULL)
			{
				if (tmp->client_fd == clientnode->client_fd)
				{
					tmp = tmp->next;
					continue;
				}
				send(tmp->client_fd, &message, sizeof(Mnode), 0);
				tmp = tmp->next;
			}
		}
		fflush(stdin);
		fflush(stdout);
	}
}

void release_node(Mnode *message, Tnode *clientnode) //销毁结点
{
	Oclient *head = NULL;
	Oclient *p1 = NULL;
	Oclient *p2 = NULL;
	int client_fd;
	
	head = clientnode->head;
	client_fd = clientnode->client_fd;
	
	p1 = head;
	p2 = head->next;
	
	if (NULL == p2)
	{
		 printf("Link is empty!\n");
	}
	else
	{
		while (p2->next != NULL)
		{
			if (p2->client_fd == client_fd)
			{
				break;
			}
			p1 = p2;
			p2 = p2->next;
		} 
		
		if (p2->client_fd == client_fd)
		{
			p1->next = p2->next;
			free(p2);
			p1 = NULL;
		}
		else
		{
			printf("！！客户端不存在！！\n");
		}
		
	}
	
	//insert_clientinfo(message, head);
}

void private_chat(Tnode *clientnode, Mnode *message) //私聊
{
	int num;
	Oclient *tmp = NULL;
	
	tmp = clientnode->head->next;
	while (tmp != NULL && tmp->client_fd != message->toseid)
	{
		tmp = tmp->next;
	}
	
	if (tmp != NULL)
	{
		char buff[MSGSIZE] = {0};
		printf("私聊：%s\n", message->msg);
		sprintf(buff, "%d号客户端的好友悄悄对你说:", clientnode->client_fd);
		strcat(buff, message->msg);
		strcpy(message->msg, buff);
		send(message->toseid, message, sizeof(Mnode), 0);
	}
	else
	{
		strcpy(message->msg, "该好友未在线!\n");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
	}
}

void group_chat(Tnode *clientnode, Mnode *message) //群聊
{
	Oclient *tmp;
	tmp = clientnode->head->next;
	char buff[MSGSIZE] = {0};
	printf("群聊：%s\n", message->msg);
	sprintf(buff, "收到来自客户端%d的消息:", clientnode->client_fd);
	strcat(buff, message->msg);
	strcpy(message->msg, buff);
	
	while (tmp != NULL)
	{
		if (clientnode->client_fd == tmp->client_fd)
		{
			tmp = tmp->next;
			
			continue;
		}
		send(tmp->client_fd, message, sizeof(Mnode), 0);
		tmp = tmp->next;
	}
}

void group_file(Tnode *clientnode, Mnode *message) //将文件发送给每一个人
{
	Oclient *tmp;
	tmp = clientnode->head->next;
	
	while (tmp != NULL)
	{
		if (clientnode->client_fd == tmp->client_fd)
		{
			tmp = tmp->next;
			
			continue;
		}
		send(tmp->client_fd, message, sizeof(Mnode), 0);
		tmp = tmp->next;
	}
}

void insert_clientinfo(Mnode *message, Tnode *clientnode) //保存客户端信息并发送
{
	int i = 0;
	Oclient *tmp;
	tmp = clientnode->head->next;
	
	//客户端信息存入结点链表中去
	bzero(message->cinfo, sizeof(Cinfo));
	while (tmp != NULL)
	{
		message->cinfo[i].client_fd = tmp->client_fd;
		message->cinfo[i].bkid = tmp->bkid;
		strcpy(message->cinfo[i].name, tmp->name);
		i++;
		tmp = tmp->next;
	}
	
	Mnode tmpmessage;
	bzero(&tmpmessage, sizeof(Mnode));
	memcpy(&tmpmessage, message, sizeof(Mnode));
	tmpmessage.flag_vip = 0;
	
	
	//将结点链表中的客户端信息发给每一个客户端
	tmp = clientnode->head->next;
	while (tmp != NULL)
	{
		if (clientnode->client_fd == tmp->client_fd)
		{
			send(clientnode->client_fd, message, sizeof(Mnode), 0);
			tmp = tmp->next;
			
			continue;
		}
		send(tmp->client_fd, &tmpmessage, sizeof(Mnode), 0);
		tmp = tmp->next;
	}
	
}

void be_vip(Mnode *message, sqlite3 *pdb) //数据库中更改权限
{
	int ret;
	char sql[100];
	char *errmsg = NULL;
	int colc, rowc;
	
	sprintf(sql, "update user set vip = 1 where bkid = %d;", message->uinfo.bkid);
	ret = sqlite3_exec(pdb, sql, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("update fail: %s", sqlite3_errmsg(pdb));
		
		return;
	}
}

void is_vip_shut(Mnode *message, sqlite3 *pdb, Tnode *clientnode) //判断对象是否是vip
{
	int ret;
	char sql[100];
	char *errmsg = NULL;
	char **pResult = NULL;
	int colc, rowc;
	printf("1**\n");
	Mnode tmpmessage;
	bzero(&tmpmessage, sizeof(Mnode));
	memcpy(&tmpmessage, message, sizeof(Mnode));
	tmpmessage.flag_vip = 0;
	tmpmessage.flag_chat = chat_no;
	
	sprintf(sql, "select vip from user where bkid = %d;", message->bkid);
	ret = sqlite3_get_table(pdb, sql, &pResult, &rowc, &colc, &errmsg);
	printf("2**\n");
	if (ret != SQLITE_OK)
	{
		printf("Select fail: %s", sqlite3_errmsg(pdb));
		
		return;
	}
	printf("3**\n");
	printf("rowc = %d\n", rowc);
	printf("colc = %d\n", colc);
	if (rowc == 0)
	{
		strcpy(message->msg, "该好友未在线!");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
	}
	if (strcmp(pResult[1], "1") == 0)
	{
		printf("4**\n");
		strcpy(message->msg, "该用户是VIP您无法禁言他");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
	}
	else
	{
		printf("5**\n");
		strcpy(message->msg, "禁言成功!");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
		send(message->toseid, &tmpmessage, sizeof(Mnode), 0);
	}
	printf("6**\n");
	sqlite3_free_table(pResult);
	printf("7**\n");
}

void is_vip_kick(Mnode *message, sqlite3 *pdb, Tnode *clientnode) //判断对象是否是vip
{
	int ret;
	char sql[100];
	char *errmsg = NULL;
	char **pResult = NULL;
	int colc, rowc;
	printf("1**\n");
	Mnode tmpmessage;
	bzero(&tmpmessage, sizeof(Mnode));
	memcpy(&tmpmessage, message, sizeof(Mnode));
	tmpmessage.flag_vip = 0;
	tmpmessage.flag_kick = kick_yes;
	
	sprintf(sql, "select vip from user where bkid = %d;", message->bkid);
	ret = sqlite3_get_table(pdb, sql, &pResult, &rowc, &colc, &errmsg);
	printf("2**\n");
	if (ret != SQLITE_OK)
	{
		printf("Select fail: %s", sqlite3_errmsg(pdb));
		
		return;
	}
	printf("3**\n");
	printf("rowc = %d\n", rowc);
	printf("colc = %d\n", colc);
	if (rowc == 0)
	{
		strcpy(message->msg, "该好友未在线!");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
	}
	if (strcmp(pResult[1], "1") == 0)
	{
		printf("4**\n");
		strcpy(message->msg, "该用户是VIP您无法踢出他");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
	}
	else
	{
		printf("5**\n");
		strcpy(message->msg, "踢出成功!");
		send(clientnode->client_fd, message, sizeof(Mnode), 0);
		send(message->toseid, &tmpmessage, sizeof(Mnode), 0);
	}
	printf("6**\n");
	sqlite3_free_table(pResult);
	printf("7**\n");
}
