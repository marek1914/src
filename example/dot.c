例如1.txt：
digraph G{
	size = "4, 4"
	a->b->c;
	b->d;
	
	a[shape = polygon, sides = 5, peripheries=3, color = lightblue, style = filled];
	//多边形，有五条边，3条边框， 颜色的淡蓝色， 样式为填充
	c[shape = polygon, sides = 4, skew= 0.4, lable = "hello world"];
	//4变形， 角的弯曲度0.4, 里面的内容为"hello world"
	d[shape = invtriange];
	//三角形
	e[shape = polygon, side = 4, distortion = .7];
	//梯形
}

执行
dot -Tpng 1.txt -o 1.png
可以生成流程图
