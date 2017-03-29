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

boost (libboost-dev)

www.open-std.org
http://libcxx.llvm.org/
libc++ is a new implementation of the C++ standard library, targeting C++11.


C++11: ISO/IEC 14882:2011 Information technology-Programming languages C++

In addition to the facilities provided by C, C ++ provides additional data types, classes, templates, exceptions, namespaces,
operator overloading, function name overloading, references, free store management operators, and additional library facilities.

1 类中类，结构体中结构体
2 含纯虚函数的类是抽象基类
3 gcc/g++识别c++: .C/.cc(C with classes)/.cpp(C plus plus)/.CPP/.c++/.cp/.cxx
4 non-const references can't bind temp objects.  Foo &obj = Foo(); //vs ok,gcc fail
"Basically, you shouldn't try to modify temporaries for the very reason that they are 
temporary objects and will die any moment now. The reason you are allowed to call non-const 
methods is that, well, you are welcome to do some "stupid" things as long as you know what 
you are doing and you are explicit about it (like, using reinterpret_cast). But if you bind 
a temporary to a non-const reference, you can keep passing it around "forever" just to have 
your manipulation of the object disappear, because somewhere along the way you completely 
forgot this was a temporary. "

create 变为 _ZN7android15createEv

class foo : public virtual bar {//虚继承
}

定义struct 跟定义class 有同样的效果

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

----
RefBase.h中定义智能指针
template <typename T>
class sp
{ ...
private:
  ...    
    T*  m_ptr; 这个是真正类地址，由于是private 外部看不到
};

get是sp模板类的方法，用来得到自己。

skia：
类定义 virtual void onDraw(SkCanvas* canvas) 
定义非虚函数 draw调用 onDraw

类外执行gm->draw (draw可在子类中重写，他不是虚函数所以这里调用基类draw) 
若gm指向其子类，那么draw的this就是子类的，所以draw里面调用的onDraw
(写成this->onDraw 看起来更明显) 是子类的onDraw。

如果draw里只是简单调用了onDraw，大可不必包装一层draw，gm->onDraw 达到相同效果。
这样做的意义可能是，我可以在draw里面加入一些必要代码。而这部分又不需要在子类中重新实现。


C/C++/Java

static const

static final Singleton<IActivityManager> gDefault = 
static const int COUNT=5;

static inline int foo(void)
{
	...
}

类static公有成员独立于class对象之外，不必产生对象也可以使用它们。

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

静态成员和静态成员函数

声明为static的类成员或者成员函数便能在类的范围内共同享，我们把这样的成员称做静态成员和静态成员函数。
类的静态成员是属于类的而不是属于哪一个对象的 所以静态成员的使用应该是类名称加域区分符加成员名称的，在上面的代码中就是Internet::count，
虽然我们仍然可以使用对象名加点操作符号加成员名称的方式使用，但是不推荐的，静态态类成员的特性就是属于类而不专属于某一个对象。

静态成员函数由于与对象无关系，所以在其中是不能对类的普通成员进行直接操作的。

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


#include <iostream>
using namespace std;

namespace android {
}

struct person {
	char *name;
	char age;
}

int main(void)
{
	cout << "Hello World" <<endl;
}

//operator +
#include <iostream>
using namespace std;

//Box operator+(const Box&, const Box&);

class Box
{
	public:

	double getVolume(void)
	{
		return length * breadth * height;
	}
	void setLength( double len )
	{
		length = len;
	}
	void setBreadth( double bre )
	{
		breadth = bre;
	}
	void setHeight( double hei )
	{
		height = hei;
	}

	// Overload + operator to add two Box objects.
	Box operator+(const Box& b)
	{
		Box box;
		box.length = this->length + b.length;
		box.breadth = this->breadth + b.breadth;
		box.height = this->height + b.height;
		return box;
	}
	private:
	double length;
	double breadth;
	double height;
};


int main( )
{
	Box Box1;
	Box Box2;
	Box Box3;
	double volume = 0.0;

	Box1.setLength(6.0); 
	Box1.setBreadth(7.0); 
	Box1.setHeight(5.0);

	Box2.setLength(12.0); 
	Box2.setBreadth(13.0); 
	Box2.setHeight(10.0);

	volume = Box1.getVolume();
	cout << "Volume of Box1 : " << volume <<endl;

	volume = Box2.getVolume();
	cout << "Volume of Box2 : " << volume <<endl;

	Box3 = Box1 + Box2;

	volume = Box3.getVolume();
	cout << "Volume of Box3 : " << volume <<endl;

	return 0;
}

//operator ++
#include <iostream>
using namespace std;
 
class Time
{
   private:
      int hours;             // 0 to 23
      int minutes;           // 0 to 59
   public:
      Time(){
         hours = 0;
         minutes = 0;
      }
      Time(int h, int m){
         hours = h;
         minutes = m;
      }
      // method to display time
      void displayTime()
      {
         cout << "H: " << hours << " M:" << minutes <<endl;
      }
      // overloaded prefix ++ operator
      /*
      Time operator++ ()  
      {
         ++minutes;          // increment this object
         if(minutes >= 60)  
         {
            ++hours;
            minutes -= 60;
         }
         return Time(hours, minutes);
      }
      */
      // overloaded postfix ++ operator
      Time operator++()         
      {
         // save the orignal value
         Time T(hours, minutes);
         // increment this object
         ++minutes;                    
         if(minutes >= 60)
         {
            ++hours;
            minutes -= 60;
         }
         return T; 
      }
};
int main()
{
   Time T1(11, 59), T2(10,40);
 
   ++T1;
   T1.displayTime();        // display T
  // ++T1;
  // T1.displayTime();
 
   T2++;
   T2.displayTime();
  // T2++;
  // T2.displayTime();
   return 0;
}
