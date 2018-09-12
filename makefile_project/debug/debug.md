# makefile 调试

## 1. 添加调试信息

执行到error时会中断，warning不中断makefile的执行， info不打印当前makefile名和行号。 

$(warning "some text") --- warning 不中断makefile的执行，打印warning中的信息，并打印当前makefile文件名和行号。

$(info "some text")　--- info 打印 \$info 中的信息。

$(error TEXT…)　---包含warning的功能，同时会中断makefile的执行并退出。

##　2. 使用示例

	### 2.1 简单使用 



![1536325246536](/tmp/1536325246536.png)

![1536325027834](/tmp/1536325027834.png)

