
#为.c文件指定搜索目录
vpath %.c src

all: vpath.o
	@echo compile done

%.o: %.c
	gcc -c $< -o $@
