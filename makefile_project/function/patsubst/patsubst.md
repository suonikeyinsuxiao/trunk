格式：\$(patsubst pattern,replacement,text) 
名称：模式字符串替换函数——patsubst。
功能：查找text中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否符合模式pattern，如果匹配的话，则以replacement替换。
　　　这里，pattern可以包括通配符“%”，表示任意长度的字串。如果replacement中也包含“%”，那么，replacement中的这个“%”将是pattern中的那个“%”所代表的字串。（可以用“\”来转义，以“\%”来表示真实含义的“%”字符）
返回：函数返回被替换过后的字符串。
示例：
$(patsubst %.c,%.o, a.c b.c)
把字串“a.c b.c”符合模式[%.c]的单词替换成[%.o]，返回结果是“a.o b.o”

