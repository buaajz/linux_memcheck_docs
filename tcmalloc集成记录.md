## tcmalloc概述
tcmalloc有两个版本。分别为：
1、https://google.github.io/tcmalloc/

2、https://github.com/gperftools/gperftools

第一个版本tcmalloc为主线，日常维护并应用于google，支持GWP-ASan工具。第二个版本tcmalloc是gperftools的一个基础组件。gperftools支持Heap-checker的内存检测工具。
两者区别可参考：https://github.com/google/tcmalloc/blob/master/docs/gperftools.md

gperftools介绍：
gperftool是Google开源的一款非常的性能分析工具集。主要由四个组件组成：
1、Tcmalloc内存分析器
2、Heap-profiler：Heap -profiler是内存监控器，可以随时知道内存的使用情况。
3、Heap-checker：Heap-checker是专门检测内存泄露的工具
4、Cpu-profiler：Cpu-profiler主要是通过采样的方式，给出一段时间内程序时间占用CPU时间进行统计和分析
## tcmalloc原理介绍
参考资料：
https://goog-perftools.sourceforge.net/doc/tcmalloc.html

https://blog.51cto.com/quantfabric/2568961

## gperftools Heap-checker工具内存检测
### gperftools编译
https://github.com/gperftools/gperftools/releases

下载最新源码包。
```
cd /home/code/
tar -zxvf gperftools-2.7.90.tar.gz
cd gperftools-2.7.90
autoreconf -vif
../configure --prefix=/usr/local/lib --disable-cpu-profiler --disable-heap-profiler --disable-debugalloc
make && make install
```

可根据需求打开关闭指定的模块。 google-perftools的组件，包括TCMalloc、heap-checker、heap-profiler和cpu-profiler共4个组件。
编译完成生成lib库在–prefix指定的路径，文件名：libtcmalloc.so。
### Heap-checker工具使用
1）静态链接
在编译选项的最后加入 /usr/local/lib/libtcmalloc.a 即可链接静态库
2）动态链接
在编译选项这里插入 -L/usr/local/lib -ltcmalloc 即可；
3）、重启进程即可使用
启动进程前执行 export LD_PRELOAD=/usr/local/lib/libtcmalloc.so即可。
```
export LD_PRELOAD=/usr/local/lib/libtcmalloc.so 
HEAPCHECK=normal ./main
```
HEAPCHECK除了nomal外，还可以选择strict或者draconian。
参考：https://github.com/gperftools/gperftools/blob/master/docs/heap_checker.html

## tcmalloc GWP-ASan工具内存检测
### tcmalloc编译
参考：https://google.github.io/tcmalloc/quickstart.html

编译工具链可以选择gcc或clang，clang可以下载：https://github.com/llvm/llvm-project/releases/tag/llvmorg-17.0.1

1、下载bazel，可以选择对应platform的版本：https://bazel.build/install/ubuntu

2、下载tcmalloc源码：https://google.github.io/tcmalloc/

3、编译tcmalloc：
gcc：
```
../bazel test //tcmalloc/...  --sandbox_debug
```
clang：
```
CC=/opt/data/tcmalloc_test/clang+llvm-14.0.6-aarch64-linux-gnu/bin/clang ../bazel test //tcmalloc/...  --sandbox_debug
```
### GWP-ASan工具使用
#### GWP-ASan开启
首先编译开启GWP-ASan的tcmalloc库：
*gwp-asan.cc*, *BUILD*


通过bazel编译上述代码：
CC=/media/caros/nvme1/wangqingyi02/0827/bd_tcmalloc/bazel/clang+llvm-14.0.6-aarch64-linux-gnu/bin/clang ../bazel-6.3.1-linux-arm64 build //examples:libgwp-asan-clang.so --cxxopt='-std=c++17' --sandbox_debug
下面是anp团队提供的linux_aarch64下的libgwp-asan-clang.so：
*libgwp-asan-clang*

### 运行时开启检测
通过LD_PERLOAD指定libgwp-asan-clang.so，开启GWP-ASan的tcmalloc库。
## 结论
gperftools的Heap-checker工具实测不能有效检测内存错误。
tcmalloc开启GWP-ASan，在linux-x86_64及linux_aarch64下，实测不能有效检测内存错误。
