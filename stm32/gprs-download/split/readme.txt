1 升级文件命名为 update.bin
2 如果crc8执行有问题（如32bit/64bit机器问题），可以在运行机器上重新编译：
  gcc crc8.c -o crc8
3 将crc8 page.bin split.sh 放到device/updateFile（tomcat可访问到的）目录下
4 执行 ./split.sh 在当前目录生成切片下载文件
