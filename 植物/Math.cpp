#include "Math.h"
#include<random>

int Math::RandInt(int lower, int upper)
{
	if (lower > upper)
	{
		std::swap(lower, upper);
	}
	std::random_device rd;//种子
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(lower, upper);
	return dis(gen);
}

double Math::Randreal(double lower, double upper)
{
	if (lower > upper)
	{
		std::swap(lower, upper);
	}
	std::random_device rd;//种子
	std::mt19937 gen(rd());
	std::uniform_real_distribution<double> dis(lower, upper);
	return dis(gen);
}

double Math::Randerc()
{
	std::random_device rd;//种子
	std::mt19937 gen(rd());
	return std::generate_canonical<double, 10>(gen);
}

double Math::Clamp(double value, double lower, double upper)
{
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))
	return min(max(value, min(lower, upper)), max(lower, upper));
}
