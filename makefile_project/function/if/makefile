#if 函数的语法是:
#$(if <condition>,<then-part> )
#或
#$(if <condition>,<then-part>,<else-part> )
#<condition>参数是if的表达式，如果其返回的为非空字符串，那么这个表达式就相当于返回真，于是，<then-part>会被计算，否则<else-part>会被计算
#
#if函数的返回值是，
#     如果<condition>为真（非空字符串），那个<then-part>会是整个函数的返回值，
#     如果<condition>为假（空字符串），那么<else-part>会是整个函数的返回值，此时如果<else-part>没有被定义，那么，整个函数返回空字串。

SRC_DIR := src

#if函数---设置默认值
#如果变量SRC_DIR的值不为空,则将SRC_DIR指定的目录作为SUBDIR子目录;否则将/home/src作为子目录
SUBDIR += $(if $(SRC_DIR) $(SRC_DIR),/home/src)

all:
	@echo $(SUBDIR)
