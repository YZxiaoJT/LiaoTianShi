OBJS=client.c myclient.c
client:$(OBJS)
	gcc $(OBJS) -o client -lsqlite3 -lpthread -lcurses
.PHONY:clean
clean:
	rm client
	