函数原型:
    $(eval text)
    它的意思是 text 的内容将作为makefile的一部分而被make解析和执行。
    需要注意的是该函数在执行时会对它的参数进行两次展开，第一次展开是由函数本身完成，第二次是函数展开后的结果被作为makefile内容时由make解析时展开.

# 1.函数二次展开示例
```makefile
define MA
aa:aa.c
	gcc -g -o aa aa.c
endef

$(eval $(call MA) )

```

会产生一个这样的编译：gcc -g -o aa aa.c



# 2.模板示例

## 2.1 模板引出示例 
```makefile
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

#从上面可以看出,生成SERVER和CLIETN的两个规则基本上是一样的，那么可以使用更通用的规则表示

$(PROGRAMS):
	gcc $^ -o $@

```

## 2.2 call与eval组合注意事项

![1537088222239](/home/frank/Pictures/Selection_058.png)

![1537088250945](/home/frank/Pictures/Selection_059.png)

第８行　引用变量SERVER_OBJS前多了一个空格，显然是引用不到server.o
第９行　可以正常引用
第20,25行　由于29行eval还没有执行，所以为空
第27行　打印函数call调用后的返回值，可以看到依赖展开后　中间多了一个空格

## 2.3  call与eval组合完整示例
```makefile
CC=gcc
PROGRAMS=SERVER  CLIENT

SERVER_OBJS:=server.o 

CLIENT_OBJS:=client.o 

.PHONY: all
all: $(PROGRAMS)

#define PROGRAM_TEMPLATE
#$(1):$$($(1)_OBJS)
#ALL_OBJS += $$($(1)_OBJS)
#endef

define PROGRAM_TEMPLATE
$(strip $(1)):$$($(strip $(1))_OBJS)
ALL_OBJS += $$($(strip $(1))_OBJS)
endef

#$(info 111$(ALL_OBJS))

#下面两行的区别在于call函数中 逗号和参数[$(prog)]之间的空格.
#因此,PROGRAM_TEMPLATE更好的写法是去掉参数前面的空格，如果有的话
$(foreach prog, $(PROGRAMS), $(eval $(call PROGRAM_TEMPLATE, $(prog))))
#$(foreach prog, $(PROGRAMS), $(eval $(call PROGRAM_TEMPLATE,$(prog))))

#$(info 222-$(ALL_OBJS))

$(PROGRAMS):
	gcc $^ -o $@

clean:
	rm -f $(ALL_OBJS) $(PROGRAMS)


```
