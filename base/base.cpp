/*
C++11 Keywords(73):
alignas		continue		friend		register			true
alignof		decltype		goto		reinterpret_cast	try
asm			default			if			return				typedef
auto		delete			inline		short				typeid
bool		do				int			signed				typename
break		double			long		sizeof				union
case		dynamic_cast	mutable		static				unsigned
catch		else			namespace	static_assert		using
char		enum			new			static_cast			virtual
char16_t	explicit		noexcept	struct				void
char32_t	export			nullptr		switch				volatile
class		extern			operator	template			wchar_t
const		false			private		this				while
constexpr	float			protected	thread_local
const_cast	for				public		throw
*/

<<C++ Coding Standards: 101 Rules, Guidelines, and Best Practices>>

增加 .*  ->*
C++将前++/-- 和后++/-- 分为不同优先级
C++ 前++/-- 定为第一有限级，跟. [] () 同级，C没有这种情况

boost (libboost-dev)

www.open-std.org
libcxx.llvm.org/
libc++ is a new implementation of the C++ standard library, targeting C++11.

C++11: ISO/IEC 14882:2011 Information technology-Programming languages C++

In addition to the facilities provided by C, C ++ provides additional data types, classes, templates, exceptions, namespaces,
operator overloading, function name overloading, references, free store management operators, and additional library facilities.

1 类中类，结构体中结构体
2 含纯虚函数的类是抽象基类
3 gcc/g++识别c++: .C/.cc(C with classes)/.cpp(C plus plus)/.CPP/.c++/.cp/.cxx
4 non-const references can't bind temp objects.  Foo &obj = Foo(); //vs ok,gcc fail


C++11:
7.3
Namespaces

标准头文件位置：
/usr/include/c++/4.8/ostream
文件头注释：
forwarding header (转接头文件)
GNU ISO C++ Library
ISO C++ 14882: 27.8.2  C Library files

const_cast<>
reinterpret_cast<>
dynamic_cast<>
static_cast<> 类型转换
用于类层次结构中基类（父类）和派生类（子类）之间指针或引用的转换。进行上行转换（把派生类的指针或引用转换成基类表示）是安全的；进行下行转换（把基
类指针或引用转换成派生类表示）时，由于没有动态类型检查，所以是不安全的。

stdio.h 有用到
__BEGIN_NAMESPACE_STD
定义在：
osxr.org:8080/glibc/source/misc/sys/cdefs.h#0112

c++调用c的库

Vector is a class template in the C++ Standard Template Library, which functions like a dynamic array. 

C++标准中的定义：
16.8 Predefined macro names
__cplusplus (如果程序按C++编译则被定义)

int a(5)；即 int a=5; 用构造函数构造初始化类，如classA a(b); 

cout /cin 字符输出
endl end line

私有析构函数

private
 ~FramebufferNativeWindow(); // this class cannot be overloaded

//delete a;//错误  也不能delete的！

分配在堆的对象也不能释放了

《C++编程规范》说：如果允许通过指向基类Base的指针执行删除操作，则Base的析构函数必须是公用且虚拟的。否则，就应该是保护且非虚拟的！

C++ 3种继承方式：私有成员都变不可用。公有继承，公有保护保持原样，保护继承公有保护全保护，私有继承公有保护全私有。
不派生时，保护与私有的地位一致，类外都无法访问，但保护成员可以被派生类的成员函数引用。
Java继承不分共有私有保护，简化了继承关系处理，但也能解决实际问题，是C++搞复杂了吗？

static const

static final Singleton<IActivityManager> gDefault = 
static const int COUNT=5;

static inline int foo(void)
{
	...
}

1 常变量必须时赋值
const int i=2;
而
const int i;
i=2 //编译错误

char * const a; // a不能变
const char * a; // a指向内容不能变
char const * a; // 同上

foo(const char * src) //*src 不允许修改
foo(char const * src) //同上
foo(char * const src) //src不允许修改

若修改，编译提示：
error: increment of read-only parameter ‘src’

test( char * const c)
{
*c++;  //fail: increment of read-only parameter ‘c’
(*c)++; //ok，测试了单目优先级和const
}

char c[]="Hello World";
test(c);
printf("%s",c); //-> Iello World

vector
http://www.cplusplus.com/reference/vector/vector/

#include <stdio.h>
#include <iostream>
using namespace std;

#if defined __cplusplus && defined _GLIBCPP_USE_NAMESPACES
int i=1;
#else
int i=2;
#endif

int main(void)
{
	cout << "Hello World!"<<endl;

	printf("i=%d\n", i);
	return 1;
}


