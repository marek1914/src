《Managing Projects with GNU make, 3rd Edition》
变量后的空格会保留

Pattern Rules

predefined variables:
CFLAGS
CXXFLAGS
LDFLAGS
LDLIBS(make文档中未提及，但实测是)

make -s 更安静

递归调用
$(MAKE) -C $(KERNELSRC) O=$(KERNELOUTPUT) 

%.o:%.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

foo.o : foo.c defs.h
	cc -c -g foo.c

A blank line that begins with a tab is not blank: it's an empty command
5元素：explicit rules, implicit rules, variable definitions, directives, comments

默认规则
all: foo
执行显示：
cc foo.c -o foo 

基本形式：
变量=xxx
target: 依赖
	command

sinclude/-include：文件不存在时不提示错误

1 cmake (opencv/x265), GYP(Generate Your Projects)
2 makefile强调自动化
3 make foo="hello world" #若引起shell歧义就加""
4  = recursively expanded  := simply expanded  ?=
5 $x单字母变量

4.10 Multiple Targets in a Rule 
4.11 Multiple Rules for One Target

phony target
2种方式  
1) 没有Commands和Prerequisites的Rules，习惯 FORCE:
2) Special Built-in Target .PHONY (vi黄色)


DATE:=$(shell date)
DATE:=`date` # 可用但make文档中没找到官方说明


模板目标，属于隐式规则

20160423:
我怎么定义一个函数，能类似${info } 有打印确不返回值？

6.3.1 Substitution References
aa=HelloWorld
b=${aa:Hello%:gao}  
b为gao

在命令区域定义变量，不是make变量是shell变量，A=B是shell命令
$(eval) 将内容展开为makefile源码
怎么理解：
eval函数返回空字符串，因此他放在任何地方都不会引起语法错误

It's important to realize that the eval argument is expanded twice; first by the eval function, 
then the results of that expansion are expanded again when they are parsed as makefile syntax.

eval 的关键在于可以二次展开：
jie:=test:all
$(jie)
all:
	echo hello
如上，make test 没有问题，第一次展开后，多了test这个目标，但这样写就不能二次展开，而eval先展开一次，生成新的makefile，再展开一次。

6.10 Target-specific Variable Values
target: AA := BB //定义私有变量

函数什么的，还有各种规则，都是用来“扩展”的
之前考虑函数能否在target的command部分，其实随便，扩展出来的东西是命令就能放，扩展出来不是命令，执行就错误（上了一个台阶）
如：
g=g
all:
	$(subst gao,ec,@$gaoho Hello World!)
等价于echo Hello World!

又如：
$(info text...) 返回空（The result of the expansion of this function is the empty string），所以可以写在任何地方都没影响

别纠结函数顶格写还是tab，都是用来“展开”的，展开后变为2种基本形式：1 定义变量； 2 rule
all:
	@echo Hello
	$(info World!)
输出：
World!
Hello
因为先展开再执行，展开阶段就把函数$(info ...)执行了，它在控制台输出了World!，返回了null，这行是empty command

makefile嵌套调用(也叫递归)3方法：make -C dir(或--directory=dir) 或-f file(或--file=file,--makefile=FILE) 或 include 如:

注：根据shell.txt中，make fork 进程执行echo命令，执行echo前，$A已经展开
改为：
A=12345
all:
	@sleep 10;echo $A
make fork shell进程，在这个进程里执行 sleep 10;echo $A，按照shell.txt中的分析，make中的A变量没有export，shell进程看不到A，echo应该打印不出A的值
而实际上，在创建shell执行sleep 10;echo $A之前，$A已经被make展开了。如果把sleep 10;echo $A写道.sh再调用，就是shell.txt中的情形了。

函数调用 resembles a variable reference:
$(function arguments) 或 ${function arguments}

cc -MM test.cpp可生成cpp的.o的依赖关系

8.12 控制Make的函数
$(info text...)

Functions for Conditionals
$(if  ...)  //kernel的if_change函数用了
$(or  ...)
$(and ...)

$(foreach var, list, text)
$(filter pattern...,text)






System types:
  --build=BUILD     configure for building on BUILD [guessed]
  --host=HOST       cross-compile to build programs to run on HOST [BUILD]

1) If you meant to cross compile, use `--host'.

工具：
libtool
autoconf 变configure.ac 为 configure
libtoolize  ???
aclocal
autoheader
autoreconf

lo: a libtool object file
la: a libtool library file，记录同名动态库和静态库的相关信息 

configure是个shell脚本
那么 ./configure 
工具是perl或shell脚本
automake    由Makefile.am生成Makefile.in
autoconf    产生configuration
autoreconf  Update generated configuration files，相当于先后调用其他各脚本程序
autoscan    perl 生成 configure.scan，修改并重命名.ac(.in是旧格式)
aclocal     扫描 configure.ac 或 configure.in 生成aclocal.m4

AM_ :automake
AC_ .ac :autoconf

 
1 运行autoscan生成 configure.scan :

AC_CONFIG_SRCDIR 靠
print $file "AC_CONFIG_SRCDIR([$cfiles[0]])\n"; 打印，修改为@cfiles 打印全部，顺序与ls -U一致
push @cfiles, $File::Find::name;

AM_INIT_AUTOMAKE 添加这个引入automake，不用加任何参数
AC_OUTPUT 改为 AC_OUTPUT(Makefile) //参数与提供的*.am一致，一般写Makefile
如果autoscan之前已建立Makefile.am，会生成：
AC_CONFIG_FILES([Makefile])，这样AC_OUTPUT就不用修改了
AM_MAINTAINER_MODE ：让configure提供--enable-maintainer-mode选项，默认关闭。maintainer模式使得修改makefile.am(如增加.c)，make会触发重新配置，更新makefile。
故：加了这个模式，删除makefile.am，仍然可以make，否则不行。

AM_SILENT_RULES([yes])
默认--enable-silent-rules 隐藏编译参数详情，若要显示可：--disable-silent-rules 或 "make V=1".

2 autoreconf -i
-i  copy missing auxiliary files 用于拷贝
compile install-sh missing 3个脚本，否则提示找不到这3个文件

不用autoreconf而用autoconf，不认AM_INIT_AUTOMAKE 在configure里面 这个宏没有展开：ac_config_headers="$ac_config_headers config.h"

AM_INIT_AUTOMAKE   #看这里，原文存在
# Checks for programs.
ac_ext=c
ac_cpp='$CPP $CPPFLAGS'

并提示：
configure.ac:8: error: possibly undefined macro: AM_INIT_AUTOMAKE
解决：aclocal 生成aclocal.m4 再执行autoconf

3 ./configure
4 make


宏AC_DEFUN autoconf：
AC_DEFUN (name, [body ])
[Macro]
Autoconf macros are defined using the AC_DEFUN macro, which is similar to the M4 builtin m4_define macro; this creates a macro named name and with body as its expansion. In addition to defining a macro, AC_DEFUN adds to it some code that is used to constrain the order in which macros are called, while avoiding redundant output 


6.4.1 Public Macros
	If your configure.ac has:

	AC_INIT([src/foo.c])
	AM_INIT_AUTOMAKE([mumble], [1.5])

	you should modernize it as follows:

	AC_INIT([mumble], [1.5])
	AC_CONFIG_SRCDIR([src/foo.c])
	AM_INIT_AUTOMAKE

m4 宏处理语言，类似于C预处理器，但更强大通用，用于但不只：
GNU Autoconf: 产生兼容不同类Unix平台的 “configure” 脚本

输入：
m4 
define(AUTHOR, William Shakespeare)   // 这是最基本形式
AUTHOR 
输出：
William Shakespeare


m4_define()  直接用的话，m4程序不解析，可能需要添加头文件

Makefile.am  （被gedit识别为Automake类型文件）输入：
bin_PROGRAMS=demoabc             (2处demoabc 名字要相同)
demoabc_SOURCES= test.c gao.c


运行 autoreconf后 automake把.am 转换成.in， ./configure 把.in 转换为 Makefile 
由makefile.in 转换为 makefile 就是把前者 @xx@ 包含的内容展开，如@CC@ 展开为gcc  @CFLAGS@ 展开为 -g -O2

其中PROGRAMS和SOURCES是关键词，会变色

处理分级目录：


libjpeg:
android版里的用makefile.cfg (貌似不是生成的)，新版用的是makefile.am  makefile.in

编译gstreamer时发现autotool也能只显示 CC xxx.o的形式

编译 doubango 发现，修改configure后，make会发现configure的改动，不用单独执行一遍configure

ifndef SHOW
.SILENT:
endif

foobar.txt: foo.txt
	cat foo.txt bar.txt >foobar.txt
	
define first-makefiles-under
$(shell findleaves.py --mindepth=2 $(1) Android.mk)
endef

all:
	gcc ff-test.c -o ff-test -lfftw3 -lm 


#4 Writing Rules
#4.7 Rules without Commands or(and更恰当吧) Prerequisites
#If a rule has no prerequisites or commands, and the target of the rule is a nonexistent file, 
#then make imagines this target to have been updated whenever its rule is run. 
#This implies that all targets depending on this one will always have their commands run. 
#总觉得有些不严密
#如果存在FORCE和all文件，且all新于FORCE，照样不执行
#没有FORCE文件或旧于all,可以执行
foo: FORCE
	@echo Hello

FORCE:

bar:
	@echo World

#特殊逻辑，只有强记
.PHONY: bar

#android里的一个用法
.PHONY: FORCE
%.pem: $(DSAPARAM) FORCE


#$(wordlist s,e,text)
#Returns the list of words in text starting with word s and ending with word e (inclusive).

test:=$(wordlist 3,5, 1 2 3 4 5 6 7 8 9)

all:
	echo $(test)


$(info $(FLAG) + $(1))
1=sss
all:

all:
	@echo Hello ${foo}  #可展开，执行target是在全部makefile解析完后，
$(info info ${foo}) #而$(info ...)是即时执行
foo=world

#二次展开
xx:
	echo "$(ss)"
