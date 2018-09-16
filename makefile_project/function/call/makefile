#call函数是唯一一个可以创建定制化参数函数的引用函数。
#支持对自定义函数的引用;
#支持将一个变量定义为一个复杂的表达式，用call函数根据不同的参数对它进行展开来过去不同的结果;
#
#函数语法:
#	$(call VARIABLE,PARAM,PARAM,...)
#
#函数功能:
#	在执行时,将它的参数"PARAM"依次赋给临时变量"$(1)","$(2)".call对参数的数目没有限制，也可以没有参数值。
#	最后再对VARIABLE展开后的表达式进行处理.
#函数返回值:
#	VARIABLE展开后的表达式的值
#函数说明:
#	1. 变量VARIABLE在定义时只能定义为递归展开式;
#	2. call函数中对VARIABLE的调用,直接给函数或变量名就好了，不要用"$";
#	3. 多个PARAM使用逗号分割开,且逗号和PARAM之间不能有空格，否则会导致解析异常;(PS:我自己在这个挖过坑,因为习惯c编码时，在逗号后面加一个空格)



#变量的引用

#变量定义为直接展开式
#如果这里将VARIABLE1定义为直接展开式,最终调用call函数后，返回值为空
#我的理解是,直接展开式在定义时将$(1)和$(2)展开,而此时它们的值为空,所以变量的值为空.当执行到call函数时，尽管带上了需要传递的参数,但PARAM($(1)和$(2))之前已经被展开了,故此时已经取不到传递进来的参数值.
VARIABLE1 := $(2) $(1)

$(info 1-$(VARIABLE1))

aa=$(call VARIABLE1,hello,world)

$(info 1-$(aa))

#变量定义为递归展开式
VARIABLE1 = $(2) $(1)

$(info 2-$(VARIABLE1))

aa=$(call VARIABLE1,hello,world)

$(info 2-$(aa))

#这里变量类型的定义再罗嗦一下,所谓的直接展开式,即在定义的时候立即展开;递归展开式,在定义的时候其实相当于声明,只有在实际引用时才展开.


#函数的引用

#不带参数的函数引用
define FUNC1
$(info echo 3-"hello")
endef
$(call FUNC1)

#带参数的函数引用
define FUNC1
$(info echo 4-$(1) $(2))
endef

$(call FUNC1,hello,wolrd)

#逗号和参数之间有空格的示例
define FUNC1
$(info echo 5-$(1) $(2))
endef

$(call FUNC1, hello, wolrd)
#从执行的结果会发现,逗号和参数之间的空格也被作为参数的一部分.在这个例子中可能看不出明显的负面影响,下面我在举个栗子.

all:
	@echo Done
