# 目录搜索

在一个大工程中，一般会将源文件和中间生成文件放在不同的目录，而且不会污染源码所在的目录。当需要编译不同目录下的源文件时，就需要指定路径，那么怎样让路径的表示以及源文件的引用更加灵活。就要用到目录搜索功能。

## VPATH

VPATH:指定依赖文件的搜索目录，当规则的依赖文件在当前目录不存在时，make会在此变量所指定目录下去寻找这些依赖文件。

```makefile
#VPATH = src

all: VPATH.o
    @echo compile done

%.o: %.c
    gcc -c $< -o $@

```

![1536619511612](/tmp/1536619511612.png)

依赖文件通常你可以指定全路径，那么执行make时能找到依赖文件，就会编译出对应的目标文件。但是在一个大工程中，源码的目录结构复杂，每个依赖文件都带上绝对路径，makefile看起来会非常繁琐。上面给出的例子中，只有一个依赖文件，或许你可以这样写all: src/VPATH.o, 但是当依赖文件变多，目录结构变复杂，或者需要更改目录结构时，makefile文件的改动很大。但是如果使用VPATH指定搜索目录，那么只需要关注依赖文件的文件名，然后在VPATH中指定搜索路径即可。

## vpath

vpath: make的一个关键字，和VPATH的功能类似，都是指定依赖文件的搜索目录。

使用的三种方法:

vpath PATTERN DIRECTORIES

为所有符合PATTERN的文件指定搜索目录DIRECTORIES.多个目录使用空格或冒号(:)分开

vpath PATTERN

清除之前　符合 PATTERN 的文件设置路径　

vpath

清除所有的已被设置的文件搜索路径

```makefile
#为.c文件指定搜索目录
vpath %.c src

all: vpath.o
    @echo compile done

%.o: %.c
    gcc -c $< -o $@

```



```makefile
#为.c文件指定搜索目录
vpath %.c src
#为.h文件指定搜索目录
vpath %.h include

all: vpath.o
    @echo compile done

%.o: %.c
    gcc -MD -I include -c $< -o $@

clean:
    rm *.o *.d

```

makefile vpath 中出现的.h文件，仅限于在makefile中有效。对于源文件#include的头文件仍需要使用-I + directory 指定搜索目录。

如果上例子中，将 -I include　去掉，那么make会报“src/vpath.c:2:19: fatal error: vpath.h: No such file or directory”的错误。

## 自动生成依赖关系

-MD　自动生成目标文件的依赖关系，并保存在*.d文件中。

```makefile
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

```

修改src/vpath.c前，该源文件是#include "vpath.h"的。生成的vpath.d的内容是:vpath.o: src/vpath.c /usr/include/stdc-predef.h include/vpath.h

修改src/vpath.c后，即将源文件中的#include "vpath.h"注释掉。生成的vpath.d的内容是:vpath.o: src/vpath.c /usr/include/stdc-predef.h

显然少了 include/vpath.h.那使用-MD有什么作用呢。

试想，当在源文件中加入或者删除头文件时，如果不使用MD自动生成依赖关系，就需要手动的修改makefile中显示的依赖关系，显然这是一种低效的方式且容易出错。

gcc 的　"-MD"选项将自动寻找源文件中include的头文件，并生成文件的依赖关系，保存在*.d文件中。

```makefile
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

objdir:
    @echo "create obj directory"
    -mkdir $(objdir)

$(objdir)/%.o: %.c
    gcc -MD $(INCDIR) -c $< -o $@

all: $(objdir) $(objlist)
    @echo compile done

clean:
    rm $(objdir)/*.o $(objdir)/*.d $(objdir) -rf

```

