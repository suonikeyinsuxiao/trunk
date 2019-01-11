#设置cmake的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#设置项目名称
project(test01)
#设置源文件
aux_source_directory(. dir_srcs)
#设置可执行程序
add_executable(test02 ${dir_srcs})
