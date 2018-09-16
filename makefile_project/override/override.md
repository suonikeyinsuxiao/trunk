
# override指示符
    通常在执行 make 时,如果通过命令行定义了一个变量,那么它将替代在 Makefile中出现的同名变量的定义。
就是说,对于一个在 Makefile 中使用常规方式(使用“=”、“:=”或者“define”)定义的变量,我们可以在执行 make 时通过命令行方式重新指定这个变量的值,命令行指定的值将替代出现在 Makefile 中此变量的值。
如果不希望命令行指定的变量值替代在 Makefile 中的变量定义,那么我们需要在 Makefile 中使用指示符“override”来对这个变量进行声明。

# override作用
1. 保护makefile中定义的变量的值;
2. 提供一种在makefile中增加或者修改命令行参数的方式;

实际情况下,我们经常会有这种需求:通过命令行指定一些附加的参数选项,对于一些通用的参数选项在makefile中指定.

## 1 保护makefile中定义变量值示例

### 1.1 没有使用override的情况

make命令行指定的变量值将会覆盖makefile中定义的同名的变量值
```makefile
SRCS := A.c B.c C.c

all:
	@echo "SRCS: " $(SRCS)
```


### 1.2 使用override的情况

make命令行指定的变量值将不会覆盖makefile中定义的同名的变量值，所以override有保护makefile中变量值不被命令行参数修改的作用。
```makefile
override SRCS := A.c B.c C.c

all:
	@echo "SRCS: " $(SRCS)
```

![运行结果](/home/frank/Pictures/Selection_062.png)

## 2 修改makefile中定义变量值的示例
```makefile
#使用override进行追加的变量的原来指定的值不会被命令行参数覆盖,而且会追加命令行指定的值
override CFLAGS += -g

all:
	@echo $(CFLAGS)
```
![运行结果](/home/frank/Pictures/Selection_063.png)
