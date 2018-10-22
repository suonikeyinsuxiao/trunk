cc_library(
    name = "hello_func",
    srcs = ["func.cpp"],
    hdrs = ["func.hpp"],
)

cc_binary(
    name = "hello_world",
    srcs = ["hello_world.cpp"],
    deps = [
        ":hello_func",
    ],
)
