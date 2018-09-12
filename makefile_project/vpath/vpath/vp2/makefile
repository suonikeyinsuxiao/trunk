
#为.c文件指定搜索目录
vpath %.c src
#为.h文件指定搜索目录
vpath %.h include

all: vpath.o
	@echo compile done

%.o: %.c 
	#gcc -MD -c $< -o $@
	gcc -MD -I include -c $< -o $@

clean:
	rm *.o *.d
