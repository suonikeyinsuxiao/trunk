#函数原型 $(eval text)
#它的意思是 text 的内容将作为makefile的一部分而被make解析和执行。
#需要注意的是该函数在执行时会对它的参数进行两次展开，第一次展开是由函数本身完成，第二次是函数展开后的结果被作为makefile内容时由make解析时展开.

#这样一个makefile:

define MA
aa:aa.c
	gcc -g -o aa aa.c
endef

$(eval $(call MA) )

#会产生一个这样的编译：
#gcc -g -o aa aa.c
