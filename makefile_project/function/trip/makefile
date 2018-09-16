#$(strip <string> )
#名称：去空格函数——strip。
#功能：去掉<string>字串中开头和结尾的空字符,并将中间的多个连续空字符(如果有的话)合并为一个空字符。
#返回：返回被去掉空格的字符串值。
#说明: 空字符包括　空格,tab等不可显示的字符

#把字串" abc"开头的空格去掉,结果是"abc"。
str1 :=    abc
str2 := abc      
str3 := a	b  c      

all:
	@echo bound$(strip $(str1))bound
	@echo bound$(strip $(str2))bound
	@echo bound$(strip $(str3))bound

