
incdirs := include include/src2
srcdirs := src src2

#指定源文件列表(由vpath处理路径问题)
srcs := vpath.c src2.c

#指定中间文件目录
objdir := obj

#指定目标文件列表
objlist := $(patsubst %.c, $(objdir)/%.o, $(srcs))

#指定gcc头文件路径
INCDIR := $(patsubst %, -I%, $(incdirs))

#为.c文件指定搜索目录
vpath %.c $(srcdirs)
#为.h文件指定搜索目录
vpath %.h $(incdirs)

.PHONY: all clean

all: $(objdir) $(objlist)
	@echo compile done

$(objdir):
	@echo "create obj directory"
	-mkdir $(objdir)

$(objdir)/%.o: %.c 
	gcc -MD $(INCDIR) -c $< -o $@

clean:
	rm $(objdir)/*.o $(objdir)/*.d $(objdir) -rf
