OBJS=server.c myserver.c
server:$(OBJS)
	gcc $(OBJS) -o server -lsqlite3 -lpthread
.PHONY:clean
clean:
	rm server