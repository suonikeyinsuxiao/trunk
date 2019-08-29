# 1 变量的定义

> A **variable** is a name defined in a makefile to represent a string of text, called the variable’s
> value. These values are substituted by explicit request into targets, prerequisites, recipes,
> and other parts of the makefile. (In some other versions of make, variables are called macros.)

makefile中，变量就是一个名字，变量的值就是一个文本字符串。在makefile中的目标，依赖，命令或其他地方引用变量时，变量会被它的值替代。

Makefile 中变量和函数的展开(除规则命令行中的变量和函数以外),是在 make
读取 makefile 文件时进行的,这里的变量包括了使用“=”定义和使用指示符
“define”定义的

# 2 变量的特点

## 2.1 变量的值

>Variables can represent lists of file names, options to pass to compilers, programs to run,
>directories to look in for source files, directories to write output in, or anything else you can
>imagine.

变量的值可以是　文件名列表，编译选项列表，程序运行时的选项参数列表，搜索源文件的目录列表，编译输出的目录列表，或者其他你能够想到的需要表示的事物。

## 2.2 变量名的命名规则

### 2.2.1 变量名大小写敏感

‘hello’,'Hello','HELLO' 表示的是三个不同的变量。

### 2.2.2 变量名的命名规范

> It is traditional to use upper case letters in variable names, but we recommend using lower
> case letters for variable names that serve internal purposes in the makefile, and reserving
> upper case for parameters that control implicit rules or for parameters that the user should
> override with command options.

makefile中变量名传统的定义方式是使用大写字母。作为一个专业的程序员，不管写什么代码都要保持好的code规范。常用的做法是：对于makefile中内部定义的变量使用小写字母表示，对于一些参数变量使用大写字母表示(参数变量可能会被其他文件include)。当然，以上命名方式不是强制的，只是一种风格。

> A variable name may be any sequence of characters not containing ‘:’, ‘#’, ‘=’, or white-
> space. However, variable names containing characters other than letters, numbers, and
> underscores should be considered carefully, as in some shells they cannot be passed through
> the environment to a sub-make . Variable names beginning with ‘.’ and an uppercase letter may be given special meaning in future versions of make.

变量名不能包含 '**:**','**#**','**=**'或者空格，一般使用字母，数字，写划线来表示变量名。

#　3 变量的使用

一般使用"\$(var)"或者"\${var}"引用变量。

## 3.1 两种变量定义

### 3.1.1 递归展开变量

这种类型的变量是通过'='或者'define'指示符定义的。

> The value you specify is
> installed verbatim; if it contains references to other variables, these references are expanded
> whenever this variable is substituted (in the course of expanding some other string).

```makefile
aa = $(bb)
bb = $(cc)
cc = hello 

all:
    echo $(aa)

```

echo $(aa)中的变量aa会被展开成\$(bb),而变量bb又会被展开成\$(cc)，而变量cc最终展开成hello。

此种类型的变量的优点在于:可以引用其他的之前没有定义的变量。

```makefile
CFLAGS = $(inc_dirs) -O
inc_dirs = -Ia/include -Ib/include 
```

最终CFLAGS会被展开为"-Ia/include -Ib/include -O"，显然这是使用很简约方便的表达方式。但是，但是，但是，也会产生问题。

Q1: 无限递归

```makefile
CFLAGS = $(CFLAGS) -O

all:
    echo $(CFLAGS)

```

如果变量引用了自己，那就会出现无限递归，最终makefile的执行会报错(**makefile:2: *\** Recursive variable 'CFLAGS' references itself (eventually).  Stop**)退出。在多层次makefile架构中，会使编译异常。

Q2: 如果变量定义中使用了函数，会使make的执行效率降低或者会导致不可控的错误

###　3.1.2 直接展开变量

这种类型的变量通过':='来定义。

必须先定义，后引用;否则引用为空。

可避免无限递归问题。

```makefile
CFLAGS := $(CFLAGS) -O

all:
    echo $(CFLAGS)

```

echo $(CFLAGS)结果为　-O。

```makefile
CFLAGS = -Wall -O
CFLAGS := $(CFLAGS) -O

all:
    echo $(CFLAGS)

```

echo $(CFLAGS)结果为　-Wall -O -O。

```makefile
CFLAGS = -Wall -O
CFLAGS := $(CFLAGS) -O
CFLAGS := -O2

all:
    echo $(CFLAGS)

```

echo $(CFLAGS)结果为 -O2。

总结一下，在复杂的makefile中一般使用直接展开变量，尽量避免递归展开变量的使用。
直接展开变量的变量值在定义时,会对其他变量或函数的引用进行展开,所以变量定义完后,就是实际的字符串了,不再有任何引用.

## 3.2 '?='操作符

给未定义的变量赋默认值.

```makefile
aa ?= hello   

all:
    echo $(aa)

```

显示　hello

```makefile
aa := world   
aa ?= hello   
              
all:          
    echo $(aa)

```

显示　world

# 4. 自动化变量
\$@ 目标文件
\$< 第一个依赖文件
\$^ 所有依赖文件

