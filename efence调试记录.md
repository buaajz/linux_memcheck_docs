## efence介绍
https://linux.die.net/man/3/efence
## 编译记录
在linux-aarch64环境下：
源码库：https://github.com/CheggEng/electric-fence
*libefence.so*
## 使用示例
1. ulimit -a 查看ulimit -c 设置core file size
2. export LIBRARY_PATH=/tmp/gwt_asan_test/gpt_asan_test/:$LIBRARY_PATH
3. gcc -g main.c -o main -lefence -lpthread
4. ./main
## 使用结论
目前看存在运行时堆溢出等问题，暂时分析源代码需要做修改和适配。能够检测简单示例程序。