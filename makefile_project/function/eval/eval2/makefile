PROGRAMS := SERVER CLIENT

SERVER_OBJS := server.o 

CLIENT_OBJS := client.o 

.PHONY: all
all: $(PROGRAMS)

SERVER: $(SERVER_OBJS)
ALL_OBJS += $(SERVER_OBJS)

$(info $(ALL_OBJS))

CLIENT: $(CLIENT_OBJS)
ALL_OBJS += $(CLIENT_OBJS)

$(info $(ALL_OBJS))

#从上面可以看出,生成SERVER和CLIETN的两个规则可以使用更通用的规则表示

$(PROGRAMS):
	gcc $^ -o $@
