# 1 cmake 消息打印

在c语言中，有printf来进行打印。那么cmake中有没有类似的函数或者命令呢？当然是有的。

## [message()](https://cmake.org/cmake/help/v3.13/command/message.html)

> ```cmake
> message([<mode>] "message to display" ...)
> ```

# 2 cmake 指定头文件搜索路径

通常gcc 编译是需要通过-I 来指定头文件搜索路径，那么，cmake怎样来指定呢?

## [include_directories()](https://cmake.org/cmake/help/v3.13/command/include_directories.html)

Add include directories to the build.

> ```cmake
> include_directories([AFTER|BEFORE] [SYSTEM] dir1 [dir2 ...])
> ```



## [target_include_directories()](https://cmake.org/cmake/help/v3.13/command/target_include_directories.html#command:target_include_directories)

Add include directories to a target.

> ```cmake
> target_include_directories(<target> [SYSTEM] [BEFORE]
>   <INTERFACE|PUBLIC|PRIVATE> [items1...]
>   [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
> ```

target_include_directories()必须在<target>被创建的命令后使用，如add_executable()或add_library()

# 3 cmake指定库的搜索路径

通常gcc 通过 -L 来指定第三方库的搜索路径，那么，cmake怎样来指定呢?

## [link_directories()](https://cmake.org/cmake/help/v3.13/command/link_directories.html)

Add directories in which the linker will look for libraries.

> ```cmake
> link_directories([AFTER|BEFORE] directory1 [directory2 ...])
> ```
>
> Relative paths given to this command are interpreted as relative to the current source directory.
>
> The directories are added to the [`LINK_DIRECTORIES`](https://cmake.org/cmake/help/v3.13/prop_dir/LINK_DIRECTORIES.html#prop_dir:LINK_DIRECTORIES) directory property for the current `CMakeLists.txt` file, converting relative paths to absolute as needed. **The command will apply only to targets created after it is called**.

## [target_link_directories()](https://cmake.org/cmake/help/v3.13/command/target_link_directories.html#command:target_link_directories)

Add link directories to a target.

> ```cmake
> target_link_directories(<target> [BEFORE]
>   <INTERFACE|PUBLIC|PRIVATE> [items1...]
>   [<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
> ```
>
> Specify the paths in which the linker should search for libraries when linking a given target. Each item can be an absolute or relative path, with the latter being interpreted as relative to the current source directory. These items will be added to the link command.
>
> The named `<target>` must have been created by a command such as [`add_executable()`](https://cmake.org/cmake/help/v3.13/command/add_executable.html#command:add_executable) or [`add_library()`](https://cmake.org/cmake/help/v3.13/command/add_library.html#command:add_library) and must not be an [ALIAS target](https://cmake.org/cmake/help/v3.13/manual/cmake-buildsystem.7.html#alias-targets).

# 4 cmake 指定宏定义

## [add_definitions()](https://cmake.org/cmake/help/v3.13/command/add_definitions.html)

Adds -D define flags to the compilation of source files.

> ```cmake
> add_definitions(-DFOO -DBAR ...)
> ```
>
> Adds definitions to the compiler command line for targets in the current directory and below (whether added before or after this command is invoked). This command can be used to add any flags, but it is intended to add preprocessor definitions.
>
> Note :This command has been superseded by alternatives:
>
> - Use [`add_compile_definitions()`](https://cmake.org/cmake/help/v3.13/command/add_compile_definitions.html#command:add_compile_definitions) to add preprocessor definitions.
> - Use [`include_directories()`](https://cmake.org/cmake/help/v3.13/command/include_directories.html#command:include_directories) to add include directories.
> - Use [`add_compile_options()`](https://cmake.org/cmake/help/v3.13/command/add_compile_options.html#command:add_compile_options) to add other options.

# 5 cmake 链接第三方库

gcc 在链接第三方库时，通常需要使用-l来链接。例如 -lpthread。cmake也有相应的命令来完成这功能。

## [link_libraries()](https://cmake.org/cmake/help/v3.13/command/link_libraries.html)

Link libraries to all targets added later.

> ```cmake
> link_libraries([item1 [item2 [...]]]
>                [[debug|optimized|general] <item>] ...)
> ```
>
> Specify libraries or flags to use when linking any targets created later in the current directory or below by commands such as [`add_executable()`](https://cmake.org/cmake/help/v3.13/command/add_executable.html#command:add_executable) or [`add_library()`](https://cmake.org/cmake/help/v3.13/command/add_library.html#command:add_library). See the [`target_link_libraries()`](https://cmake.org/cmake/help/v3.13/command/target_link_libraries.html#command:target_link_libraries) command for meaning of arguments.
>
> Note:The [`target_link_libraries()`](https://cmake.org/cmake/help/v3.13/command/target_link_libraries.html#command:target_link_libraries) command should be preferred whenever possible. Library dependencies are chained automatically, so directory-wide specification of link libraries is rarely needed.

## [target_link_libraries()](https://cmake.org/cmake/help/v3.13/command/target_link_libraries.html#id1)

Specify libraries or flags to use when linking a given target and/or its dependents.

> ```cmake
> target_link_libraries(<target> ... <item>... ...)
> ```



# 6 cmake 生成静态库或动态库

## [add_library()](https://cmake.org/cmake/help/v3.13/command/add_library.html)

Add a library to the project using the specified source files.

### [Normal Libraries](https://cmake.org/cmake/help/v3.13/command/add_library.html#id2)

> ```cmake
> add_library(<name> [STATIC | SHARED | MODULE]
>             [EXCLUDE_FROM_ALL]
>             [source1] [source2 ...])
> ```