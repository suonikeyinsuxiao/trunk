# 强制目标

## 1. 定义

	如果一个规则(rule_A)既没有依赖也没有命令，仅有目标(Targe_A)，并且目标名不冲突。那么，在执行这个规则的时候，目标总被认为是更新过的。如果这个目标(Target_A)作为另一个规则(rule_B)的依赖时，因为依赖总被认为更新过，那么依赖所在的规则中的命令总会被执行，即规则(rule_B)中的命令总会被执行。

## 2. 示例

### 2.1 强制执行示例

```makefile
target: FORCE
    gcc force.c -o target

clean:
    rm target

.PHONY: FORCE
FORCE:

```

如果将　**target: FORCE** 改为　**target:** ，在执行过一次make后，再执行时就会提示:***make: 'target' is up to date.***

![1536245055559](/tmp/1536245055559.png)

![1536245148093](/tmp/1536245148093.png)

### 2.2 强制目标名示例

强制目标的命名没有约束，只是习惯上用FORCE表示。其实，可以取任意名字。

```makefile
target: aaa
    gcc force.c -o target

clean:
    rm target

.PHONY: aaa
aaa:

```



![1536245539493](/tmp/1536245539493.png)