## 参考资料
[查内存泄漏试试AScan](https://www.jianshu.com/p/9e85345e500b)
https://gcc.gnu.org/onlinedocs/gcc-13.1.0/gcc/Instrumentation-Options.html
https://github.com/google/sanitizers/wiki/AddressSanitizer
https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer
Address Sanitizer是谷歌的快速的内存错误检测工具，它非常快只拖慢程序2倍左右的速度。
可以支持的内存检测：
* Use after free
* Heap buffer overflow
* Stack buffer overflow
* Global buffer overflow
* Use after return
* Use after scope
* Initialization order bugs
* Memory leaks
## asan原理
ASAN工具主要由两部分组成：
编译器插桩模块
运行时库
运行时库：（libasan.so.x）会接管malloc和``free函数。malloc执行完后，已分配内存的前后（称为“红区”）会被标记为“中毒”状态，而释放的内存则会被隔离起来（暂时不会分配出去）且也会被标记为“中毒”状态。
编译器插桩模块：加了ASAN相关的编译选项后，代码中的每一次内存访问操作都会被编译器修改为如下方式：
编译前:
```
*address = ...;  // or: ... = *address;
```
编译后：
```
if (IsPoisoned(address)) {
ReportError(address, kAccessSize, kIsWrite);
}
*address = ...;  // or: ... = *address;
```
### 基本原理
1. 程序申请的每8bytes内存映射到1byte的shadown内存上
2. 因为malloc返回的地址都是基于8字节对齐的，所以每8个字节实际可能有以下几个状态
case 1：8个字节全部可以访问，例如char* p = new char[8]; 将0写入到这8个字节对应的1个字节的shadow内存；
case 2：前1<=n<8个字节可以访问, 例如char* p = new char[n], 将数值n写入到相应的1字节的shadow内存，尽管这个对象实际只占用5bytes，malloc的实现里[p+5, p+7]这尾部的3个字节的内存也不会再用于分配其他对象，所以通过指针p来越界访问最后3个字节的内存也是被允许的
3. asan还会在程序申请的内存的前后，各增加一个redzone区域（n * 8bytes），用来解决overflow/underflow类问题
4. free对象时，asan不会立即把这个对象的内存释放掉，而是写入1个负数到该对象的shadown内存中，即将该对象成不可读写的状态， 并将它记录放到一个隔离区(book keeping)中, 这样当有野指针或use-after-free的情况时，就能跟进shadow内存的状态，发现程序的异常；一段时间后如果程序没有异常，就会再释放隔离区中的对象
5. 编译器在对每个变量的load/store操作指令前都插入检查代码，确认是否有overflow、underflow、use-after-free等问题
### Shadow Memory 地址算法
现在，我们有了更好的映射方式。但又有一个问题出现：如何通过一个地址计算出与之对应的映射地址呢？
正如上面提到的，由于内存对齐的需要和更高效的映射方式。一个 8-bytes 的内存块的状态，可以用一字节表示。所以在计算 Shadow Memory 的地址，ASan 采用了简单的方式（意味着高效）。舍弃 Addr 的最后3位（Addr >> 3 ），就可得到一个新的地址，把这个新地址当作与之对应的 Shadow Memory 的内存地址。
但是这样还会有一个问题，就是进程的内存空间和 Shadow Memory 交错在一起了，甚至是无法区分。为了解决这个问题，需要再引入一个固定的值：偏移量(Offset)。在32-bit系统中，ASan 选取了 0x20000000(2^29)，在64-bit系统中，则是 0x0000100000000000(2^44)。
## arrch64下asan有效性验证
需要验证gaia_sanxian工程中，asan的有效性。
增加编译选项：
```
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -fsanitize=address -fno-omit-frame-pointer")
```
注意需要开启debug模式，即增加 -g -o0 选项。
制造如下的错误场景：
1.栈溢出

2.堆溢出

3.释放后使用

4.全局缓存溢出

5.double free

6.内存泄漏
对于内存泄漏，目前来看aarch64 asan不支持。
https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer 中描述到，只有在 x86_64 Linux 和 OS X下才支持LeakSanitizer。
## 模块单元测试
### 测试方案选择

方案一：测试需要重定向标准输出和标准出错：
```
$BIN_PATH/gaia_diagnostic > /tmp/gaia_diagnostic_asan.log 2>&1 &
```

方案二：为不修改启动脚本，我们将 sanitizer output同步输出到syslog中，通过查看/opt/log/syslog来定位是否有AddressSanitizer报错。
AddressSanitizer flags:https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
Run-time flags设置方式：https://github.com/google/sanitizers/wiki/AddressSanitizerFlags
1、环境变量 ASAN_OPTIONS
2、函数 __asan_default_options
设置Run-time flags：ASAN_OPTIONS环境变量添加log_to_syslog

```
export ASAN_OPTIONS=log_to_syslog=true
```