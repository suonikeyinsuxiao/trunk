"include"指示符告诉 make 暂停读取当前的 Makefile，而转去读取"include"指定的一个或者多个文件，完成以后再继续当前 Makefile 的读取。

为什么要include其他文件呢?

1. 对于一些通用的变量定义、通用规则，写在一个文件中，任意目录结构中的makefile想要使用这些通用的变量或规则时，include指定的文件就好了，而不用在每个makefile中又重写一遍。
2. 对于源文件自动生成依赖文件([makefile之目录搜索＆自动依赖](https://www.jianshu.com/p/d35a8291e2f4))时，将这些个依赖关系保存成文件，在需要使用时include进来，这样少了人为的干预，同时也减少的错误的发生。

include是怎样进行搜索的?

如果在当前目录下或者指定的绝对路径找不到目标文件，make将根据文件名进行查找:

a. 查找使用命令行选项 "-I" 指定的目录;

b. "/usr/gnu/include","/usr/local/include","/usr/include",如果这些目录存在的话;

当在这些目录下都没有找到“include”指定的文件时,make将会提示一个包含文件未找到的告警提示,但是不会立刻退出。而是继续处理Makefile的后续内容。当完成读取整个Makefile后,make将试图使用规则来创建通过指示符“include”指定的但未
找到的文件,当不能创建它时(没有创建这个文件的规则),make将提示致命错误并退出。

有时候，当被include的文件不重要时,也就是说是否include了，没太大关系，那么可以在include前加"-"，表示忽略该错误。

-include FILENAMES...

使用这种方式时，当所要包含的文件不存在时不会有错误提示、make也不会退出。