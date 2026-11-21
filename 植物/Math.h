#pragma once
#include<cmath>
class Math
{
	
public:
	//生成一个指定范围的随机整数
	static int RandInt(int lower, int upper);
	//生成一个指定范围的随机实数
	static double Randreal(double lower, double upper);
	//生成一个随机百分比
	static double Randerc();
	//控制在指定范围内
	static double Clamp(double value, double lower, double upper);
};


