demo４示例　链接第三方动态库，静态库。

使用cc_import链接外部库。原理是通过cc_import规则导入第三方库，然后cc_binary再依赖cc_import规则的target，也即依赖第三方库。

# demo4目录树

├── link
│   ├── BUILD
│   ├── include
│   │   ├── func.h
│   │   └── static.h
│   ├── lib
│   │   ├── libfunc.so　　由shared_lib生成
│   │   └── libstatic.a　　由static_lib生成
│   └── main.c
├── README.md
├── shared_lib
│   ├── BUILD    动态库的目标名必须写成libfoo.so的形式
│   ├── func.c
│   └── func.h
├── static_lib
│   ├── BUILD 静态库的目标名可以省略lib*.a
│   ├── static.c
│   └── static.h
└── WORKSPACE

先分别将shared_lib和static_lib生成对应的动态库和静态库，然后在link包中链接这两个库，最后有main.c调用库中的API。

# static_lib BUILD

```bash
cc_library(             
    name = "static",
    srcs = ["static.c"],
    hdrs = ["static.h"],
    linkstatic = True,
)
	
```

# shared_lib BUILD

```bash
cc_binary(                                        
    name = "libfunc.so",
    #name = "func",#必须写成libfunc.so,否则编译不过
    srcs = ["func.h","func.c"],
    linkshared = True,
)
	
```

# link BUILD

```bash
cc_import(                             
    name = "func",
    hdrs = ["include/func.h"],
    shared_library = "lib/libfunc.so",
)

cc_import(
    name = "static",
    hdrs = ["include/static.h"],
    static_library = "lib/libstatic.a",
)

cc_binary(
    name = "link_static_shared",
    srcs = ["main.c"],
    deps = [":func",":static"],
    copts = ["-Ilink/include"],
)

```



参考:

https://docs.bazel.build/versions/master/be/c-cpp.html#cc_import

