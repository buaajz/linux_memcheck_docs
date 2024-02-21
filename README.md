# 运行时内存检测方案调研
## 方案汇总
Google的sanitizers一共有5种：
* AddressSanitizer (检查寻址问题) ：包含LeakSanitizer (检查内存泄漏问题)
* ThreadSanitizer：检查数据竞争和死锁问题（支持C++和Go）
* MemorySanitizer：检查使用未初始化的内存问题
* HWASAN（Hardware-assisted AddressSanitizer）：AddressSanitizer的变种，相比AddressSanitizer消耗的内存更少。
* UBSan：检查使用语言的UndefinedBehavior问题。
Google sanitizers和其他内存工具的对比


DBI: dynamic binary instrumentation
CTI: compile-time instrumentation
UMR: uninitialized memory reads
UAF: use-after-free (aka dangling pointer)
UAR: use-after-return
OOB: out-of-bounds
Guard Page: a family of memory error detectors (Electric fence or DUMA on Linux, Page Heap on Windows, libgmalloc on OS X)
gperftools: various performance tools/error detectors bundled with TCMalloc. Heap checker (leak detector) is only available on Linux. Debug allocator provides both guard pages and canary values for more precise detection of OOB writes, so it's better than guard page-only detectors.

## 检测结论

Asan可以检测大部分内存问题。

| 内存错误类型          | asan | efence | Heap-checker（gperftools） | GWP-ASan（tcmalloc） |
|----------------------|------|--------|----------------------------|----------------------|
| Use after free       | Y    | Y      | Y                          | N                    |
| Heap buffer overflow| Y    | Y      | Y                          | N                    |
| Stack buffer overflow| Y    | N      | N                          | N                    |
| Global buffer overflow| Y    | N      | N                          | N                    |
| Double free          | Y    | N      | N                          | N                    |
| Memory leaks         | N    | N      | N                          | N                    |
