demo3 使用bazel编译静态库

# demo3目录树

├── README.md
├── static
│   ├── BUILD
│   ├── static.c
│   └── static.h
└── WORKSPACE

# BUILD

```bash
cc_library(
    name = "static",
    srcs = ["static.c","static.h"],
    linkstatic = True,
)
```



本例中，包名即目录名static，cc_library规则的name也为static即目标名,所以在当前根目录下执行bazel build //static:static

bazel自动加上lib 和 .a，最终生成liibstatic.a

![Selection_071](/home/frank/Pictures/Selection_071.png)

![Selection_070](/home/frank/Pictures/Selection_070.png)

再简单的验证一下静态库是否有效

vi main.c

调用print_static函数

gcc test.c libstatic.a -o a.out 编译得到a.out（会有编译警告,因为没有包含头文件）

执行a.out，即可看到打印"I'm static"

