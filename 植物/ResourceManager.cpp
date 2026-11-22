#include "ResourceManager.h"

ResourceManager::~ResourceManager()
{
}

void ResourceManager::Load(std::string name, std::string path, int we, int hi)
{
	IMAGE* image = new IMAGE;
	loadimage(image, LPCTSTR(path.c_str()), we, hi);


	imagepool.insert({name, image});

}

void ResourceManager::Load(std::string name, std::string path, int we, int hi, int num)
{
	IMAGE**images = new IMAGE*[num];
	for (int i = 0; i < num; i++)
	{
		images[i]=new IMAGE;
		loadimage(images[i], LPCTSTR((path+"/" + std::to_string(i) + ".jpg").c_str()), we, hi);
	}
	animationPool.insert({ name,{ images,num } });
}

void ResourceManager::Load(std::string name, std::string path, int we, int hi, int num, int row, int col)
{
	IMAGE** images = new IMAGE * [num];
	IMAGE image;
	loadimage(&image, LPCTSTR(path.c_str()),we,hi);
	DWORD* src = GetImageBuffer(&image);
	int width = we / col;
	int height = hi / row;
	for (int i = 0;i < row;i++)
		for (int j = 0; j < col; j++)
		{
			int x = j * width;
			int y = i * height;
			int index = i * col + j;
			if (index >= num)break;
			images[index] = new IMAGE(width, height);
			DWORD* dest = GetImageBuffer(images[index]);
			for(int x1=0;x1<width;x1++)
				for(int y1=0;y1<height;y1++)
				{
					dest[x1+y1*width] = src[x1+x+(y1+y)*we];
				}
		}
	animationPool.insert({ name,{ images,num } });
}


void ResourceManager::Initialize()
{
	//加载图片资源
	Load("bg0","res/images/bg3.jpg",1400,600);
	Load("sun", "res/animations/bullets/sun.png", 483, 160, 12, 2, 6);
	


	//植物
	Load("peashooter", "res/animations/plants/peashooter.png", 483, 320, 24, 4, 6);

	//子弹
	Load("pea", "res/animations/bullets/pea.png", 28, 28);
	
}

ResourceManager::ResourceManager()
{
}

IMAGE* ResourceManager::Fetch(std::string name)
{
	return (*imagepool.find(name)).second;
}

ImageResource ResourceManager::FetchAnimation(std::string name)
{
	return animationPool.find(name)->second;
}
