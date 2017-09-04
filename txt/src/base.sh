#!/bin/bash #(Bourne Again Shell) Shebang line
# ctrl+d #terminal
info # 所有命令
less	
clear
lsof
reset	
fdisk
parallel 
pkg-config
beep
parted / gparted
ascii #命令
zip/unzip
gzip/gunzip (gnu zip)
bc/dc # calculator 
gzip # -9 --fast -9 --best
# adduser,addgroup 是底层工具useradd, groupadd的包装
adduser  user group # 将已存在用户user加入到已存在组group中
deluser foo # pi上提示返回8，man发现是perl什么模块
#并且提示systemd 进程占用，kill这个进程，在deluser ok
usermod #  modify a user account
usermod -l # 修改用户名
usermod -d # 修改home目录
groupmod -n #修改组名
hostname foobar #修改hostname
rsync #remote /local file-copying
readelf -A
#迅速生成几百M
cat /dev/zero >test.bin
split foo.bin -b 1024M
uname -r
cpio
tail
head
blkid #显示分区 UUID
declare
who # su 也显示最开始登录的用户名
w
#查看运行时间：
who -b
last reboot
top
uptime
dos2unix
insmod
lsmod
modprobe
iptables
netfilter
column -t
ntpdate  # set the date and time via NTP
tzselect # view timezones
su - #(切换到root，并切换环境变量）
sgdisk/gdisk #GPT manipulator
cp  -n (--no-clobber) #不覆盖同名文件
locate
uudecode/uuencode (sharutils)
taskset

hdparm/sdparm
hdparm -tT /dev/sdb1  测试硬盘性能
netperf - a network performance benchmark
iperf - perform network throughput tests

cat /etc/issue
time cp test1 test2  测试拷贝速度
fuser -m -v 查看谁在用某个文件描述符
shutdown -h 10:42
netstat -a 查看开启的网络服务
zip -Z store bootanimation.zip part*/* desc.txt  #store 不压缩 or -0
/lib/modules/`uname -r`/build  #得到kernel版本号
curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -  #下载脚本并执行

ls |xargs -I{} signapk.jar ... {} sign/{}
ls ori_apk | xargs -I{} java -jar .... ori_apk/{} sing_apk/{}
ls *.apk|xargs -n 1 aapt dump badging
ls |xargs -n1 du -sk |sort -n

find -name "*.png" | xargs cp -t ./png
find -name config.h|xargs git add  -f
find -type d -empty|grep -v .git|xargs -I{} touch {}/.tmp //除.git空目录添加.tmp

ip route add 172.16.0.0/24 dev eth0
arp-scan –I eth0 -l
service mysql restart

# 3337 -> 0x37
# 以每行每2字节为基本单位，遇到不可转换字符(0-9a-z)跳到下一行
xxd -r -p a.txt 
hexdump

/sbin/service WowzaMediaServer start

#get file size
stat -f "%z" foo.bin
file # /usr/share/misc/magic.mgc

#version
cat /proc/version 
cat /etc/issue 
lsb_release -a

dmesg # print or control the kernel ring buffer
cat /proc/kmsg

cut -f1 -d ' '
cut -f1 -d- #arm-elf-gun -> arm

# bs == ibs + obs  bs配合count 方便计算
dd if=xx of=xx -skip n  #cut file head
cat uldr.bin /dev/zero | dd bs=1 count=64k > uldr_padded_64k.bin

# here document  EOF 分隔符可任意字符串
<< EOF
foo
bar
EOF
#等价于
echo foo
echo bar
#或
echo -e "foo\nbar"


# 撤销变量： unset VAR_NAME
# "!"执行 history 列表中的命令(history expansion)
# single quotes(3.1.2.2)不展开变量，双引号扩展，如 echo '$PATH' #输出$PATH
# Double Quotes(3.1.2.3)不展开，除 ‘$’, ‘`’, ‘\’, ‘!’
# 函数内部$1接收函数参数，函数外部$1接收命令行参数（.sh文件可理解为最外层函数）
# [] [[]] 不完全等价
# 区分内建命令和关键词 (4 Shell Builtin Commands)
# 命令行编辑 ctl+left 跳一个词
# main(int argc, char *argv[ ], char **env) env即为全局环境变量
# 子进程修改同名变量不影响父进程，是复制
# . foo.sh ,foo.sh是参数而非命令，无需x权限和./
# PATH=arm-linux-xx/bin:$PATH #PATH本就是全局无需export

./configure --enable-shared=yes 
${1} = --enable-shared=yes 内部解释含义

set 显示全局+局部变量


#----------------
I/O Redirection

stdin (keyboard), 
stdout(screen)
stderr(error messages output to the screen)
# >> append
# |& standfor 2>&1|

M>N  # M is fd(default 1) N is file
M>&N # M is fd(default 1) N is fd
 >&N # Redirects fd 1 to N

1>log  # Redirect stdout to file
2>log  # Redirect stderr to file
&>log  # Redirect both stdout and stderr to file
echo foo 1>log
echo bar >log # default 1    
2>&1 # Redirects stderr to stdout.

>log.txt 2>&1 # Appends stdout and stderr to the file
2>&1 >log.txt # stderr do not write to file!!!
cmd >/dev/null 2>&1 #all output is shoved into a black hole

cmd < input-file > output-file

0<filename , <filename  #input from a file

# Open file for reading and writing and assign fd "j" to it
# If file does not exist, create it
# If fd "j" is not specified, default to fd 0
# An application of this is writing at a specified place in a file
[j]<>filename

exec 3<> filename   # open file and assign fd 3 to it
read -n 4 <&3   # read only 4 characters.
echo -n . >&3   # write a decimal point there
exec 3>&-       # close fd 3

| # Pipe, use for chaining commands, scripts together.

# closing fd
# Child processes inherit opened fd. This is why pipes work. To prevent an fd from being inherited, close it.

n<&- # close input fd n
0<&-, <&- # close stdin
n>&- # close output fd n
1>&-, >&- # close stdout

# Redirecting only stderr to a pipe.
exec 3>&1  # Save current "value" of stdout.
ls -l 2>&1 >&3 3>&- | grep bad 3>&-  # Close fd 3 for 'grep' (but not 'ls').
exec 3>&-  # close it for the remainder of the script.


foo=1 bar=2 #当前进程定义2变量
foo=1 bar=2 sleep 1 #传递到子进程，当前进程看不到
ARCH=arm64 echo $ARCH   #打不出来 ^_^
ARCH=arm64 ;echo $ARCH  #能打出来 ^_^
make ARCH=arm   #ARCH被定义在全局区域，make里面再起make还能检测到此变量
ARCH=arm make   #make里能检测到， 相当于3，跟例2不同
ARCH=arm64 echo $ARCH   #打不出
ARCH=arm64 ;echo $ARCH  #打出
make ARCH=arm   #全局，相当于make内部export
ARCH=arm make   #为子进程定义全局环境变量

echo $(echo 00.00.00.03 |sed s/\\.//g)
echo `echo 00.00.00.03 |sed s/\\\.//g`
#用` 就需要三个反斜才能转义，为何？

b=`pwd|sed 's/Note/gao/'`
a="pwd|sed 's/Note/gao/'" #适当使用“”，否则a=pwd为一条指令

# tr 不支持utf-8 multibyte char
tr '[A-Z]' '[a-z]'
tr ' ' '\n' < list | sort -u > list-uniq


mount -o nolock 192.168.0.150:/home/nfsroot /mnt
mount -o loop # 加强
mount system.img /mnt  # ext4 镜像，什么参数都不用加？

echo `echo 00.00.00.03 |sed 's/\\.//g'` #.需要转义,\本身还有特殊含义，又需要转义，所以需要\\

find -name "*.c" -print0 | xargs -0 

vim -b 以二进制形式打开 :%!xxd -g 1  切换到十六进制模式显示
xargs # build and execute command lines from standard input


-n 1  一个
-L 1  一行  一行里可能有多个



cat  A20-galaxy.tar.bz2.a*  | tar xj 
cat xx*  > xx

tar --exclude=.git   
-f, --file ARCHIVE (czvf,f在最后的原因)
vim -bd base.ko base2.ko 二进制文件比较


CC="ccache arm-linux-androideabi-gcc"  or
ln -s /usr/bin/ccache /usr/local/bin/gcc

ln -sf /foo/bar/ ~/foo/foobar # 连接目录目标已存在，出现嵌套，好像不是backup，并未指定参数，可以先删除

busybox devmem 读写 /dev/mem(1-4byte)
cat /dev/mem

id #显示real和effective的用户和组IDs，如：
uid=1000(user1) gid=1000(user1) groups=1000(user1),27(sudo),1001(wireshark)
表示当前用户是user1，当前组是user1，user1用户在组user1，sudo，wireshark中

logcat -v threadtime log &
pid=$!

#把5060端口加入 iptables
iptables -A OUTPUT -p tcp --sport 5060 -j ACCEPT
iptables -A INPUT -p  udp --dport 5060 -j ACCEPT

sudo netstat -lnp | grep 5060

modprobe usbmon
/proc/cpuinfo
/dev/random
/dev/urandom

arm_new_gcc.sh :
#!/bin/sh
SETGNUEABIHF_PATH=$(echo $PATH | grep arm-linux-gnueabihf)
if [  "$SETGNUEABIHF_PATH" = "" ]; then
ARMHF_PREFIX=/opt/gcc-linaro-arm-linux-gnueabihf
PATH=$PATH:${ARMHF_PREFIX}/bin
fi
置于/etc/profile.d/

# man udev: Linux dynamic device management
# 管理/dev，处理设备添加删除事件
# mdev
#节点用mknod手动创建，或mdev(udev)自动创建

udevadm control --reload #reload udev rule
#or
service udev restart
#等价于
/etc/init.d/udev restart

service用了start stop exec 这些命令

udev脚本里的start字段的打印，在service udev start中不显示

链接目录，  cd命令 可以处理 ../..   但 ls vi 等不行，会退到文件夹的真实位置

printf "PERM LINKS OWNER GROUP SIZE MONTH DAY HH:MM/YEAR NAME\n" ;ls -l | sed 1d | column -t

