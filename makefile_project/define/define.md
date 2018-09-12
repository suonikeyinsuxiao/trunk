# define&endef

## 1. 命令包(canned recipes)&多行变量(muti-line variables)

>The **define** directive is followed on the same line by the name of the variable being
>defined and an (optional) assignment operator, and nothing more. The value to give the
>variable appears on the following lines. The end of the value is marked by a line containing
>just the word **endef**.Aside from this difference in syntax, define works just like any other
>variable definition. The variable name may contain function and variable references, which
>are expanded when the directive is read to find the actual variable name to use.
>
>You may omit the variable assignment operator if you prefer. If omitted, make assumes
>it to be ‘=’ and creates a recursively-expanded variable. When using a ‘+=’ operator, the value is appended to the previous value as with any other append operation: with a single space separating the old and new
>values.
>
>You may nest define directives: make will keep track of nested directives and report
>an error if they are not all properly closed with endef. Note that lines beginning with
>the recipe prefix character are considered part of a recipe, so any define or endef strings
>appearing on such a line will not be considered make directives.

本质上就是通过define directive来定义一个变量，而这个变量包含一组命令，而这一组命令经常会被多个地方使用到，那么通过对该变量的引用，就能相应召开这一组命令。就像c语言的函数一样。也可以当作是makefile中“函数"的定义。需要注意的是该变量名不能与其他变量名冲突。

### 1.1 示例

#### 1.1.1 命令包的简单使用---自定义函数

```makefile
all:                    
    @$(cmd)

define cmd
    echo "test define 1"
    echo "test define 2"
    echo "test define 3"
endef

```

cmd　是命令包的名字，在define 和　endef　间的部分即是命令主体。

![1536333578008](/tmp/1536333578008.png)

从执行结果可以看出，命令包变量cmd被引用展开后，被执行。上图中第一次make的结果中打印了命令本身，而第二次make的结果中只打印了命令结果。**@**符号表示不回显命令本身。