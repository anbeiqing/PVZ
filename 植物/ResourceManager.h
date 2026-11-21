#pragma once



#include<unordered_map>
#include<easyx.h>
#include<string>



struct ImageResource {
	IMAGE** images;
	int num;
};
class ResourceManager {
public:
	std::unordered_map<std::string, IMAGE*> imagepool;
	std::unordered_map<std::string, ImageResource>animationPool;
	

	void Load(std::string name, std::string path, int we, int hi);//加载单张图片
	void Load(std::string name, std::string path, int we, int hi, int num);//加载多张动画图片
	void Load(std::string name, std::string path, int we, int hi, int num, int row, int col);//加载动画图片

	void Initialize();
	ResourceManager();
	~ResourceManager();

	IMAGE* Fetch(std::string name);
	ImageResource FetchAnimation(std::string name);


};
