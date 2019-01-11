cmake_minimum_required(VERSION 2.6)
project(test03)
message("include dir:" ${INCLUDE_DIRECTORYIES})

#Find an external project, and load its settings
find_package(PCL 1.2 REQUIRED)
message("frank:" PCL_FOUND)
message("include dir:" ${PCL_INCLUDE_DIRS})

#添加头文件搜索路径
include_directories(${PCL_INCLUDE_DIRS})
message("include dir:" ${INCLUDE_DIRECTORYIES})

#添加库搜索路径
message("lib dir:" ${PCL_LIBRARY_DIRS})
link_directories(${PCL_LIBRARY_DIRS})

#添加宏定义
message("macro:" ${PCL_DEFINITIONS})
add_definitions(${PCL_DEFINITIONS})

message("PCL_ITEM:"${PCL_LIBRARIES})
list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")


add_executable(pcl_test pcl_test.cpp)

#指定链接的库
message("lib:" ${PCL_LIBRARIES})
target_link_libraries (pcl_test ${PCL_LIBRARIES})


