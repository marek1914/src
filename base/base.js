javascript中的函数是“复合数据类型”，又称为“引用类型”。
函数语句后面可 ； 和不；
function max_num(num_arr)  
{  
    //保存结果，初始化为数组的第一个元素  
    var  max=num_arr[0];  
    var i=0; //循环变量初始化  
   //遍历数组，找出最大值放在max中  
    for(i;i<num_arr.length;i++)  
    {  
        if(max<num_arr[i])  
        max=num_arr[i];  
    }  
    return max;  
}  

function result(function_name)  
{  
    var number_arr=new Array();  
    //将参数中除去第一个参数的其余参数赋值给数组number_arr  
    var i =1;  
    for(i;i<arguments.length;i++)  
    {  
        number_arr[i-1] = arguments[i];  
    }  
    return function_name(number_arr);  
} 

var a=result(max_num);

使用getJSON的例子如下：
$.getJSON("view_detail.json",function(rsp) {  });
function 是关键词，代表定义一个无名函数并且传递给了函数参数
所以，jquery的 getJSON函数定义为：
jQuery.getJSON(url,data,success(data,status,xhr))，函数里面可以调用success函数。
并且这个函数可以现定义。

一个极简单html引用一个js，在chrome里面就可以调试了
