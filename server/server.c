#include "myserver.h"

sqlite3 *pdb = NULL;

void *client_chart(void * arg) //线程函数
{
	Tnode clientnode;
	clientnode = *((Tnode *)arg);
	
	while (running)
	{
		msg_send_recv(&clientnode, pdb); //收发消息
	}
	close(clientnode.client_fd);
	
	return NULL;
}

int main()
{
	//定义变量
	int ret, opt;
	int listen_fd, client_fd;
	pthread_t tid;
	socklen_t server_len, client_len;
	struct sockaddr_in server_addr, client_addr;
	Oclient *head = NULL;
	Oclient *newnode = NULL;
	Tnode clientnode;
	
	//初始化变量
	server_len = sizeof(server_addr);
	client_len = sizeof(client_addr);
	bzero(&server_addr, server_len);
	bzero(&client_addr, client_len);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVERPORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	//创建数据库
	ret = sqlite3_open("database_user.db", &pdb);
	if(ret != SQLITE_OK)
	{
		perror("open database fail!");
		
		exit(EXIT_FAILURE);
	}
	
	//创建用户信息表
	ret = create_table(pdb);
	if (ret != SQLITE_OK) 
	{
		sqlite3_close(pdb);
		perror("create table fail!");
		
		exit(-1);
	}
	
	//创建链表
	ret = create_link(&head); 
	if (ret != CREATE_SUCCESS)
	{
		perror("create link fail!");
		
		exit(EXIT_FAILURE);
	}
	printf("1.create link success!\n");
	
	//将链表头结点的地址存入线程函数的参数中
	clientnode.head = head; 
	
	//创建监听套接字
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd < 0)
	{
		perror("socket fd fail!");
		
		exit(EXIT_FAILURE);
	}
	printf("2.socket fd success!\n");
	
	//允许重复绑定
	ret = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (ret < 0)
	{
		perror("setsockopt fail!");
		
		exit(EXIT_FAILURE);
	}
	printf("3.setsockopt success!\n");
	
	//绑定服务器地址
	ret = bind(listen_fd, (struct sockaddr *)&server_addr, server_len);
	if (ret < 0)
	{
		perror("bind fail!");
		
		exit(EXIT_FAILURE);
	}
	
	//服务器监听
	ret = listen(listen_fd, MAXLISTEN);
	if (ret < 0)
	{
		perror("listen fail!");
		
		exit(EXIT_FAILURE);
	}
	printf("4.listen success!\n");
	
	//并发接收客户端的连接
	while (running)
	{
		client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);
		ret = create_newnode(&newnode); //创建一个新结点准备插入到链表中去
		if (ret != CREATE_SUCCESS)
		{
			perror("create node fail!");
		
			exit(EXIT_FAILURE);
		}
		printf("5.create node success!\n");
		
		newnode->client_fd = client_fd; //客户端id存入新结点中去
		clientnode.client_fd = client_fd; //客户端id存入需要传给线程函数的参数中去
		ret = insert_node(head, newnode); //新结点插入到链表中去
		if (ret != INSERT_SUCCESS)
		{
			perror("insert node fail!");
		
			exit(EXIT_FAILURE);
		}
		
		printf("|||客户端 %d 已连接|||\n", client_fd);
		ret = pthread_create(&tid, NULL, client_chart, (void *)(&clientnode));
		if (ret < 0)
		{
			perror("pthread fail!");
		
			exit(EXIT_FAILURE);
		}
	}
	
	
	close(listen_fd); //关闭监听套接字
	sqlite3_close(pdb); //关闭数据库
	release_link(&head); //销毁链表
	printf("release link success!\n");
	
	return 0;
}
