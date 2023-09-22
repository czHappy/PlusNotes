
1. 分析coredump可知deserilize有问题， run
2. 重复播放一个bag,N次之后才会报错，排除特定message的问题
3. 试验一个完整bag和裁减的Bag，都会报错，排除bag裁剪的问题
4. 试验不同的bag，都会发生coredump，排除特定bag的问题
5. 过滤抛出异常的topic,仍然在其他topic报错，排除特定topic的问题
6. 统计不同bag的抛出异常时间，不尽相同，差别很大，不是运行到特定一段时间就报错的问题
7. 运行一段时间才coredump，分析代码中有无内存泄漏问题，没有发现内存不释放的问题，
观察top内存增长，到6%就长时间不再增长，排除内存泄漏问题。
1. 试验不同x86和adu的组合
beijing4 + 118A ===> OK
电信云 + 118 ====> coredump
beijing4 + 
