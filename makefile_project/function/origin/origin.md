origin
    函数的作用是告诉你变量是哪里来的，其出生状况如何，他并不改变变量。

函数语法:
	\$(origin <variable>)
	<variable> 为变量的名字，而不是引用，所以一般没有"\$"字符在前。

origin 函数通过返回值来告诉你 <variable> 的出生情况。下面用实例说明：

1. 当从来未定义过该变量时，origin 函数返回 "undefined" 。
如下面的 Makefile 代码：
all:
    @echo $(origin V)

运行输出：
$ make
undefined

2. 如果该变量为环境变量，那么返回 "enviroment" 。
如下面的 Makefile 代码：
all:
    @echo $(origin USER)

运行输出：
$ make
environment
其中 USER 这个变量为系统定义的当前用户，使用 env 命令可以看到。

3. 如果变量是个默认定义，那么返回 "default"。
如下面的 Makefile 代码：
all:
    @echo $(origin CC)

运行输出：
$ make
default

4. 如果一个变量被定义在 Makefile 文件中，那么返回 "file" 。
如下面的 Makefile 代码：
V := 1
all:
    @echo $(origin V)

运行输出：
$ make
file

5. 如果变量来自命令行，那么返回 "command line" 。
如下面的 Makefile 代码：
all:
    @echo $(origin MyVar)

运行方法：
$ make MyVar="Are you ok?"
command line

6. 如果变量被 override 被重新定义过，那么返回 "override"。
如下面的 Makefile 代码：
override SHELL = /bin/sh
all:
    @echo $(origin SHELL)

运行输出：
$ make
override

上面，SHELL 原本是个环境变量，但在 Makefile 里被 override 指示符重定义过。

7. 如果变量是自动化变量(如 $@, $< 等)，那么返回 "automatic" 。
如下面的 Makefile 代码：
all:
    @echo $(origin @)

运行输出：
$ make
automatic

