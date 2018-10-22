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
    #同一个package内的资源引用时,可以省去包名
    deps = [":func",":static"],
    copts = ["-Ilink/include"],
)

