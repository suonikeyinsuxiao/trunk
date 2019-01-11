# 1 什么是CMake?

Cmake是一个编译、构建工具。使用CMakeLists.txt来描述构建过程，可以生成标准的构建文件，如Makefile。一般先编写CMakeLists.txt，然后通过cmake来生成Makefile，最后执行make进行编译。

## 1.1 cmake安装

​	在ubuntu上安装cmake非常简单，执行sudo apt-get install cmake即可。如果想安装最新的cmake版本，就需要自己进行源码编译安装。源码下载路径:https://cmake.org/download。源码编译安装非常简单，这里就不再详细描述了。

## 1.2 查看cmake版本

cmake安装完成后，执行cmake --version，即可查看cmake的版本号。我的是3.5.1

# 2 CMake的基本使用流程

- 编写CMakeLists.txt
- 执行cmake .;一般在工程根目录下mkdir build;cd build;然后执行cmake ..;这样的目的是为了保证源码不会污染
- 执行make进行编译

# 3 CMake简单示例

这里给出一个简单的使用cmake进行构建的工程示例，目录结构如下:

test01
​    ├── build
​    ├── CMakeLists.txt
​    └── main.c

main.c文件如下:

```c
#include <stdio.h>

int main(int argc, char** argv)
{
	printf("hello cmake\n");

	return 0;
}
```

CMakeLists.txt内容如下:

```cmake
#设置cmake的最小版本
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#设置项目名称
project(test01)
#设置源文件
aux_source_directory(. dir_srcs)
#设置可执行程序
add_executable(test01 ${dir_srcs})

```

我这里mkdir 了build 目录，

cd build

cmake ..

make

最后就会在当前目录下看到生成的test01可执行程序。

接下来，对上例中的CMakeLists.txt的语法进行解释。

- CMakeLists.txt 的语法比较简单,由命令、注释和空格组成,其中**命令是不区分大小写**的,**符号"#"后面的内容被认为是注释**。

  > Call the [`cmake_minimum_required()`](https://cmake.org/cmake/help/v3.13/command/cmake_minimum_required.html#command:cmake_minimum_required) command at the beginning of the top-level `CMakeLists.txt` file even before calling the `project()` command. It is important to establish version and policy settings before invoking other commands whose behavior they may affect. See also policy [`CMP0000`](https://cmake.org/cmake/help/v3.13/policy/CMP0000.html#policy:CMP0000).

- cmake_minimun_required

  格式:cmake_minimum_required(VERSION \<min\>\[\...\<max\>\][FATAL_ERROR])

  设置该工程的cmake最低支持版本，注意"VERSION"不能写成小写，否则会报cmake_minimum_required called with unknown argument "version".

- project

  >```cmake
  >project(<PROJECT-NAME> [LANGUAGES] [<language-name>...])
  >project(<PROJECT-NAME>
  >        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
  >        [DESCRIPTION <project-description-string>]
  >        [HOMEPAGE_URL <url-string>]
  >        [LANGUAGES <language-name>...])
  >```
  >

- aux_source_directory

  > ```cmake
  > aux_source_directory(<dir> <variable>)
  > ```

  Find all source files in a directory.Collects the names of all the source files in the specified directory and stores the list in the <variable> provided. 

- add_executable

  >```cmake
  >add_executable(<name> [WIN32] [MACOSX_BUNDLE]
  >               [EXCLUDE_FROM_ALL]
  >               [source1] [source2 ...])
  >```

  将源文件编译成可执行程序，可执行程序的名称由<name>指定


补充:

cmake的[命令](https://cmake.org/cmake/help/v3.13/manual/cmake-commands.7.html)包含 scripting commands,project commands,ctest commands,deprecated commands。


