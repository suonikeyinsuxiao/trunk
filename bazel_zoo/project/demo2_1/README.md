# demo2_1目录树

├── app
│   ├── BUILD
│   ├── hello_world.cpp
│   └── lib
│       ├── BUILD
│       ├── func.cpp
│       └── func.hpp
├── README.md
└── WORKSPACE

我们知道子目录下再创建一个BUILD文件，那么该子目录也是一个Package。

app/BUILD:

```bash
cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cpp"],
    deps = ["//app/lib:hello_func",],
)
```

hello_world目标需要调用func1()函数，所以需要依赖lib包。

lib/BUILD:

```bash
cc_library(
    name = "hello_func",
    srcs = ["func.cpp"],
    hdrs = ["func.hpp"],
    visibility = ["//app:__pkg__"],
)
```

> The `visibility` attribute on a rule controls whether the rule can be used by other packages. Rules are always visible to other rules declared in the same package.

visibility属性相当于设置规则的可见域，使其他规则能够使用本规则。这里的“//app:\_\_pkg\_\_”表明app包可以访问本规则。