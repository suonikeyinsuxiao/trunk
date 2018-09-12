# 伪目标

## 1. 伪目标的语法:

	在书写伪目标时,首先需要声明伪目标,然后再定义伪目标规则.

   ### 1.1 声明伪目标:
```makefile
.PHONY clean (这里声明clean是伪目标)
```

   ### 1.2 定义伪目标规则:
```makefile
clean:		(这里定义伪目标clean的规则,即伪目标的执行动作)
	rm *.c
```

## 2. 伪目标的作用:

### 	2.1 避免目标名与文件名冲突

		也即，如果指定了伪目标，那么伪目标一定会被执行。

### 	2.2 提高执行效率

		当一个目标被声明为　伪目标后，makefile在执行规则时不会去试图查找隐含规则来创建它。

## 3. 伪目标的示例:

### 3.1 文件名冲突

#### 3.1.1 当文件名冲突示例

假设，makefile 当前目录下有与伪目标clean 同名的文件clean，而makefile中没有定义伪目标clean。

```makefile
#.PHONY: clean
clean:
	rm temp
```

![1536240803361](/tmp/1536240803361.png)

从执行的结果可以看出，目标clean并未执行。

```makefile
.PHONY: clean
clean:
	rm temp
```

![1536241210474](/tmp/1536241210474.png)

从执行结果可以看出，伪目标clean执行了，temp文件被删除。

#### 3.1.2  多文件名冲突示例

```makefile
.PHONY: all clean prog1 prog2 prog3
all: prog1 prog2 prog3

prog1:
    gcc prog1/main.c -o p1

prog2:
    gcc prog2/main.c -o p2

prog3:
    gcc prog3/main.c -o p3

clean:
    rm p1 p2 p3

```

![1536243163506](/tmp/1536243163506.png)

如果将伪目标的声明改为　.PHONY: all clean，目标prog1,prog2,prog3将不会被执行。


## 

