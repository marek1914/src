#!/bin/bash #(Bourne Again Shell) Shebang line
# ctrl+d #terminal
info # 所有命令

ls |xargs -n1 du -sk |sort -n
env

ls * #与ls 显示不同,*先展开再执行 
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

less	
clear	
reset	
fdisk
parallel 
pkg-config

zip/unzip
gzip/gunzip (gnu zip)

gzip # -9 --fast -9 --best

zip -Z store bootanimation.zip part*/* desc.txt  #store 不压缩 or -0

# {} 任何不易出现字符, -I 包含 -L 1
ls |xargs -I{} java -jar signapk.jar xx.x509.pem xx.pk8 {} sign/{}
ls ori_apk | xargs -I{} java -jar .... ori_apk/{} sing_apk/{}
ls *.apk|xargs -n 1 aapt dump badging  //apk依次执行aapt

find -name "*.png" | xargs cp -t ./png

# 3337 -> 0x37
# 以每行每2字节为基本单位，遇到不可转换字符(0-9a-z)跳到下一行
xxd -r -p a.txt 
hexdump

locate #find files by name
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
dd if=/dev/fb of=fbfile   #save fb
dd if=/dev/zero of=/dev/sdb count=1
dd if=xx of=xx -skip n  #cut file head
cat uldr.bin /dev/zero | dd bs=1 count=64k > uldr_padded_64k.bin
dd if=x.iso of=/dev/sdb bs=8M conv=fsync

# 后定义的变量先引用值为null
# 撤销环境变量： unset VAR_NAME
# "!"执行 history 列表中的命令(history expansion)

# single quotes(3.1.2.2)不展开变量，双引号扩展，如 echo '$PATH' #输出$PATH
# Double Quotes(3.1.2.3)不展开，除 ‘$’, ‘`’, ‘\’, ‘!’
# 预定义变量 PATH, PWD
# foo.sh V=1  # ${1} = V=1
# make V=1 B  #定义变量V=1，编译目标B
# 函数内部$1接收函数参数，函数外部$1接收命令行参数（.sh文件可理解为最外层函数）

# [] [[]] 不完全等价
# 区分内建命令和关键词 (4 Shell Builtin Commands)
# 命令行编辑 ctl+left 跳一个词
 
#if

if [ -f "$APKLIBPATH/*.*" ] #不识别通配符，真有*.*文件
if [ "$color" = yes ]
if [ -n "$1" ] # 必须是 "$1" 而不是 $1
if [ -n "$force_color_prompt" ]

3.2.3 Lists of Commands
A list is a sequence of one or more pipelines separated by one of the operators ‘;’, ‘&’, ‘&&’, ‘||’, 
and terminated by one of ‘;’, ‘&’, or a newline. 

configure 是shell脚本
./configure --enable-shared=yes CFLAGS=-g
${1} = --enable-shared=yes ${2} =  CFLAGS=-g
内部解释成变量CFLAGS = -g --enable-shared=yes的含义是脚本实现

shell/uboot:
$foo / ${foo}
Command Substitution(3.5.4): $(foo) / `foo`

make：
变量 $(foo) ${foo}
单字母：$f  $foo -> $f+oo

# export
# makefile的export是make关键词，都是操作环境变量区
如:
test.sh:
echo $A

makefile：
export A=123
all:
	@./test.sh
或：
makefile:
export A
all:
	@A=123;./test.sh

export 全局变量会被复制到子进程
注意:
 1 不限定子shell，任何类型进程均可，即main(int argc, char *argv[ ], char **env)的第三个参数
 2 是复制，子进程修改同名变量不影响父进程

set 显示全局+局部变量

export TEST=1
Makefile 目标调用.sh，make后，这个shell可以读到TEST全局变量。

9 makefile在target中运行shell:

. foo.sh #foo.sh是参数而非命令，无需x权限和./

3.5.3 Shell Parameter Expansion

parameter and variable expansion

# 3.5 Shell Expansions (7 types)
#brace expansion
#tilde expansion
#parameter and variable expansion
#command substitution
#arithmetic expansion
#word splitting
#filename expansion 

-s file True if file exists and has a size greater than zero. 

arg1 OP arg2
-eq ==
-ne !=
-lt <
-le <=
-gt >
-ge >=

----------------
I/O Redirection

stdin (keyboard), 
stdout(screen)
stderr(error messages output to the screen)

COMMAND > filename
  Redirect stdout to a file.
  Creates the file if not present, otherwise overwrites it.(truncate to 0 length)

COMMAND >>
  Creates the file if not present, otherwise appends to it.

# Single-line redirection commands (affect only the line they are on):

1>file  # Redirect stdout to file
1>>file # Redirect and append stdout to file
2>file  # Redirect stderr to file
2>>file # Redirect and append stderr to file
&>file  # Redirect both stdout and stderr to file


M>N
 # "M" is a fd, defaults 1, if not set.
 # "N" is a filename.
 # File descriptor "M" is redirect to file "N."
M>&N
# "M" is a fd, defaults 1, if not set.
# "N" is another fd.

echo "hello world" 1>log.txt

# Redirection commands automatically "reset" after each line.

# Redirecting stderr, one line at a time.

bad_command1 2>error.txt       #  Error message sent to error.txt.
bad_command2 2>>error.txt      #  Error message appended to error.txt.
             
2>&1
  # Redirects stderr to stdout.
  # Error messages get sent to same place as standard output.
    >>filename 2>&1
        bad_command >>filename 2>&1
        # Appends both stdout and stderr to the file "filename" ...
    2>&1 | [command(s)]
        bad_command 2>&1 | awk '{print $5}'   # found
        # Sends stderr through a pipe.
        # |& was added to Bash 4 as an abbreviation for 2>&1 |.

i>&j
  # Redirects file descriptor i to j.
  # All output of file pointed to by i gets sent to file pointed to by j.

>&j
  # Redirects, by default, file descriptor 1 (stdout) to j.
  # All stdout gets sent to file pointed to by j.

0< FILENAME
< FILENAME
  # Accept input from a file.
  # Companion command to ">", and often used in combination with it.
  #
  # grep search-word <filename


[j]<>filename
  #  Open file "filename" for eading and writing,
  #+ and assign file descriptor "j" to it.
  #  If "filename" does not exist, create it.
  #  If file descriptor "j" is not specified, default to fd 0, stdin.
  #
  #  An application of this is writing at a specified place in a file. 
  echo 1234567890 > File    # Write string to "File".
  exec 3<> File             # Open "File" and assign fd 3 to it.
  read -n 4 <&3             # Read only 4 characters.
  echo -n . >&3             # Write a decimal point there.
  exec 3>&-                 # Close fd 3.
  cat File                  # ==> 1234.67890
  #  Random access, by golly.

|
  # Pipe.
  # General purpose process and command chaining tool.
  # Similar to ">", but more general in effect.
  # Useful for chaining commands, scripts, files, and programs together.
  cat *.txt | sort | uniq > result-file
  # Sorts the output of all the .txt files and deletes duplicate lines,
  # finally saves results to "result-file".

Multiple instances of input and output redirection and/or pipes can be combined in a single command line.

command < input-file > output-file
# Or the equivalent:
< input-file command > output-file   # Although this is non-standard.

command1 | command2 | command3 > output-file

Multiple output streams may be redirected to one file.

ls -yz >> command.log 2>&1
#  Capture result of illegal options "yz" in file "command.log."
#  Because stderr is redirected to the file,
#+ any error messages will also be there.

xx >/dev/null 2>&1
All output from this command should be shoved into a black hole.

#  Note, however, that the following does *not* give the same result.
ls -yz 2>&1 >> command.log
#  Outputs an error message, but does not write to file.
#  More precisely, the command output (in this case, null)
#+ writes to the file, but the error message goes only to stdout.

#  If redirecting both stdout and stderr,the order of the commands makes a difference.

Closing File Descriptors

n<&-
    Close input file descriptor n.
0<&-, <&-
    Close stdin.
n>&-
    Close output file descriptor n.
1>&-, >&-
    Close stdout.

Child processes inherit open file descriptors. This is why pipes work. To prevent an fd from being inherited, close it.

# Redirecting only stderr to a pipe.

exec 3>&1                              # Save current "value" of stdout.
ls -l 2>&1 >&3 3>&- | grep bad 3>&-    # Close fd 3 for 'grep' (but not 'ls').
#
exec 3>&-                              # Now close it for the remainder of the script.


array=(1 2 3 ....N) 
echo ${array[index]} 
for data in ${array[@]}  # @取全部
do  
    echo ${data}  
done

3.5.3 Shell Parameter Expansion
匹配(android gcc-sdk/gcc里用到了 ^^)：
options=" ${@} " 
suffix_m32=${options##* -m32 }    # suffix after the last -m32
suffix_m64=${options##* -m64 }    # suffix after the last -m64
len_m32=${#suffix_m32}            # length of suffix after the last -m32
len_m64=${#suffix_m64}            # length of suffix after the last -m64

while true
do
	input keyevent KEYCODE_DPAD_UP
	sleep 3
	PLAY=$(($PLAY+1))
done

gitdir=$(git rev-parse --git-dir); scp -p -P 29418 user@ip:hooks/commit-msg ${gitdir}/hooks/
foo= bar # =后面有空格，提示bar不是命令
foo=1 bar=2 #当前进程定义2个变量
foo=1 bar=2 sleep 1 #变量传递到sleep进程，当前进程看不到
sleep 2 3 # 等价于sleep 5
sleep 2h  # sleep 2小时

#命令替换嵌套变量扩展
foo=pwd
bar=`$foo`
echo $b

a="pwd|sed 's/a/b/'"  # 后面如此怪异时，要加""
echo $(echo 00.00.00.03 |sed s/\\.//g)
echo `echo 00.00.00.03 |sed s/\\\.//g`
#用` 就需要三个反斜才能转义，为何？
b=`pwd|sed 's/Note/gao/'`
echo $b 
#b是被替换了字符串的目录

a=pwd|sed 's/Note/gao/'
b=`$a`
echo $b 
#命令仅执行了pwd，结果跟测试1同，后面的sed没有执行， 变量展开后，|管道不灵了

PATH=arm-linux-xx/bin:$PATH #PATH本就是全局无需export

ARCH=arm64 echo $ARCH   #打不出
ARCH=arm64 ;echo $ARCH  #打出
make ARCH=arm   #全局，相当于make内部export
ARCH=arm make   #为子进程定义全局环境变量

# tr只能删除单字符，要删除utf-8 比如 tr -d “\022\034\270”  不能作为一个组合整体来删除
tr '[A-Z]' '[a-z]'
tr ' ' '\n' < list | sort -u > list-uniq


ip route add 172.16.0.0/24 dev eth0

rsync #remote /local file-copying

readelf -A
#迅速生成几百M
cat /dev/zero >test.bin

split foo.bin -b 1024M

uname -r
cpio

tail
head
#显示分区的 UUID
blkid
declare

arp-scan –I eth0 -l
service mysql restart


who #命令 登录后，su切换了用户， who还是现实最开始登录的用户名
w
#查看运行时间：
who -b
who -r
last reboot
top
uptime

dos2unix

insmod

lsmod
modprobe

mount -o nolock 192.168.0.150:/home/nfsroot /mnt
mount -o loop # 加强一下。。。。
mount system.img /mnt  # ext4 镜像，什么参数都不用加？

iptables
netfilter

cat /etc/issue

time cp test1 test2  测试拷贝速度
fuser -m -v 查看谁在用某个文件描述符
shutdown -h 10:42
netstat -a 查看开启的网络服务

### file rename
i=1
for f in *.jpg; do
    mv $f ${i}.jpg  #$i.jpg也可,因为.不可能出现在变量名里
    ((i++))
done

#3.5.5 Arithmetic Expansion:
$((expression))
v1=11 ; v2=11
v3=$(($v1*$v2))
v3=$((v1*v2))
$((v3=$v1*$v2))


#pad 4k 0xff
while ((i<4096)); do printf "\377" >>pad.bin; ((i++)); done


#regular expression:

# . * ^ $ x|y [xyz] [^xyz] [a-z] [^a-z]
# rename is perl
# grep -v '^$' foo.txt > bar.txt //去除空行

-n 用于测试

rename 's/arm-linux/arm-eabi/' *   # arm-linux-gcc -> arm-eabi-gcc
rename 's/$/.zip/' *  #尾部追加.zip
rename 's/^/foo/' *   #首部追加foo
 
tac file|sed 1,3d|tac #delete last 3 lines
sed '1d'
sed '1,3d'
sed '$d'
sed -i '$d' # delete last line
sed -i '/bar/d'  # delete line contain "bar"
sed -i '/bar/,+2d'

#android 取函数名
# /^function / 定位到以此开头的行
# \1 匹配第一个()里的内容
sed -n "/^function /s/function \([a-z_]*\).*/\1/p"

echo `echo 00.00.00.03 |sed 's/\\.//g'` #.需要转义,\本身还有特殊含义，又需要转义，所以需要\\

find -name *.c -print0 | xargs -0 sed -i 's/x_a/x_b/g'

vim -b 以二进制形式打开 :%!xxd -g 1  切换到十六进制模式显示
uudecode/uuencode (sharutils包)
usleep/sleep
taskset
xargs - build and execute command lines from standard input
组建命令1条或多条  -n 1 就会每个前级输出组建一条
不指定也有最大值限制，所以find结果很多时cp命令会自动分次数


-n 1  一个
-L 1  一行  一行里可能有多个

find -name "*.c"
find -name *.c //paths must precede expression, shell展开的第二个.c 被认为是 path

find -name config.h|xargs git add  -f //添加被.gitignore忽略的文件
find -type d -empty|grep -v .git|xargs -i touch {}/.tmp //除.git外的空目录添加.tmp文件 -i等价于-I{}
find -name .svn|xargs rm -r //不用双引号，.不会被shell解析

cat  A20-galaxy.tar.bz2.a*  | tar xj 
cat xx*  > xx

tar --exclude=.git   
 -f, --file ARCHIVE 所以以前总是写 czvf,没明白意思，不明白为啥f必须在最后，因为f指定文件，否则输出到stdout
cp -T //没明白这个参数
tar 遇到链接文件会保留链接，不会把指向的文件拷贝过来
vim -bd base.ko base2.ko 二进制文件比较
sgdisk - Command-line GUID partition table (GPT) manipulator Boardcom支持emmc使用这个。
gdisk - Interactive GUID partition table (GPT) manipulator
cp命令默认是覆盖同名文件的，除非加上 -n (--no-clobber) 选项


CC="ccache arm-linux-androideabi-gcc"  or
ln -s /usr/bin/ccache /usr/local/bin/gcc

ln -sf /foo/bar/  ~/foo/foobar #目标是目录时要以/结束，否则出现目标目录循环嵌套链接，why？


busybox devmem 读写 /dev/mem(1-4byte)
cat /dev/mem
dd if=/dev/mem skip

id #显示real和effective的用户和组IDs，如：
uid=1000(user1) gid=1000(user1) groups=1000(user1),27(sudo),1001(wireshark)
表示当前用户是user1，当前组是user1，user1用户在组user1，sudo，wireshark中

#su - (su切换到root，没有切换root的环境变量，"su -"一同切换环境变量）

hdparm/sdparm
hdparm -tT /dev/sdb1  测试硬盘性能

netperf - a network performance benchmark
iperf - perform network throughput tests

logcat -v threadtime log &
pid=$!
size=`busybox du -k logfile | busybox awk '{print $1}'`

#把5060端口加入 iptables
iptables -A OUTPUT -p tcp --sport 5060 -j ACCEPT
iptables -A INPUT -p  tcp --dport 5060 -j ACCEPT
iptables -A OUTPUT -p udp --sport 5060 -j ACCEPT
iptables -A INPUT -p  udp --dport 5060 -j ACCEPT

sudo netstat -lnp | grep 5060

modprobe usbmon
/proc/cpuinfo
/dev/random
/dev/urandom

lsof
/lib/modules/`uname -r`/build

#这条命令有点意思 下载脚本并执行
curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -


arm_new_gcc.sh :
#!/bin/sh
SETGNUEABIHF_PATH=$(echo $PATH | grep arm-linux-gnueabihf)
if [  "$SETGNUEABIHF_PATH" = "" ]; then
ARMHF_PREFIX=/opt/gcc-linaro-arm-linux-gnueabihf
PATH=$PATH:${ARMHF_PREFIX}/bin
fi
置于/etc/profile.d/


# adduser,addgroup 是底层工具useradd, groupadd的包装
adduser  user group # 将已存在用户user加入到已存在组group中

deluser foo # pi上提示返回8，man发现是perl什么模块
#并且提示systemd 进程占用，kill这个进程，在deluser ok

usermod #  modify a user account
usermod -l # 修改用户名
usermod -d # 修改home目录
groupmod -n #修改组名

#修改hostname
hostname foobar

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


#install beep 用于发声


bc - An arbitrary precision calculator language
ntpdate - set the date and time via NTP
tzselect - view timezones

parted / gparted

ascii #命令

链接目录，  cd命令 可以处理 ../..   但 ls vi 等不行，会退到文件夹的真实位置

printf "PERM LINKS OWNER GROUP SIZE MONTH DAY HH:MM/YEAR NAME\n" ;ls -l | sed 1d | column -t


整理一个 查找替换 的命令


if [ "$(id -u)" != "0" ]; then
   echo "This script must be run as root"
   exit
fi

