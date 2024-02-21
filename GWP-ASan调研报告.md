## asan
### 原理及使用参考
### 官方介绍
[查内存泄漏试试AScan](https://www.jianshu.com/p/9e85345e500b)

https://gcc.gnu.org/onlinedocs/gcc-13.1.0/gcc/Instrumentation-Options.html

https://github.com/google/sanitizers/wiki/AddressSanitizer

https://github.com/google/sanitizers/wiki/AddressSanitizerLeakSanitizer

https://clang.llvm.org/docs/AddressSanitizer.html

https://learn.microsoft.com/zh-cn/cpp/sanitizers/error-alloc-dealloc-mismatch?view=msvc-170

flag说明：https://github.com/google/sanitizers/wiki/AddressSanitizerFlags

https://github.com/google/sanitizers/wiki/SanitizerCommonFlags

### 原理介绍
[Address Sanitizer（Asan）原理及实战定位_asan原理-CSDN博客](https://blog.csdn.net/wads23456/article/details/105141997?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-1.nonecase)

[Linux下内存检测工具：asan_quarantine_size-CSDN博客](https://blog.csdn.net/hanlizhong85/article/details/78076668?utm_source=blogxgwz6)

[Linux 下的 AddressSanitizer-CSDN博客](https://blog.csdn.net/tq08g2z/article/details/90347700)

[AddressSanitizer原理浅析](https://www.jianshu.com/p/8f5830431266)

https://xie.infoq.cn/article/db70d91cf1355627d0a370821

https://wwm0609.github.io/2020/04/17/hwasan/

https://github.com/CheggEng/electric-fence

## gwp-asan
### 原理及使用参考
gwp-asan原理说明：
[PC GWP-ASan方案原理 | 堆破坏问题排查实践](https://mp.weixin.qq.com/s/xipHtjHPVlyFQ6W-1HfUQQ)

GItHub：
tcmalloc编译 https://github.com/google/tcmalloc/blob/master/docs/quickstart.md

gwp-asan使用说明 https://google.github.io/tcmalloc/gwp-asan.html#what-should-i-set-the-sampling-rate-to

https://github.com/google/tcmalloc/blob/master/docs/gwp-asan.md
