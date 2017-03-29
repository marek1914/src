http://fedoraproject.org/wiki/Releases/FeatureBuildId


wait queue实现阻塞
init_waitqueue_head(&outq);
wait_event_interruptible

void *kzalloc(size_t size, gfp_t flags)

irq.c 主要是实现一个 struct irq_chip 结构体

针对不同的平台，要移植irq.c 
他干什么呢？ 要处理一级中断。

1-1.3:1.0 ：摄像头使用根集线器编号为1，集线器端口号为1，集线器（摄像头使用）端口号为3，配置为1，接口为0
/Documentation/kbuild/modules.txt
/lib/modules/`uname -r`/build 指向 /usr/src/linux-headers-2.6.32-21
