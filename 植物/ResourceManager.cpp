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
	Load("sunflower", "res/animations/plants/sunflower.png", 483, 320, 24, 4, 6);
	Load("wallnut", "res/animations/plants/wallnut.png", 483, 480, 32, 6, 6);
	Load("wallnut_1", "res/animations/plants/wallnut_1.png", 65, 803, 11, 11, 1);
	Load("wallnut_2", "res/animations/plants/wallnut_2.png", 483, 480, 32, 6, 6);

	//子弹
	Load("pea", "res/animations/bullets/pea.png", 28, 28);


	//僵尸资源
	Load("normalwalk", "res/animations/zombies/normal/walk.png", 1000, 695, 46, 5, 10);
	Load("normaldeath", "res/animations/zombies/die.png", 166, 1440, 10, 10, 1);
	Load("normaleat", "res/animations/zombies/normal/eat.png", 1000, 556, 39, 4, 10);

	//奶娃
	Load("naiwa", "res/animations/zombies/naiwa/naiwa.png", 1000, 695, 46, 5, 10);
	Load("naieat", "res/animations/zombies/naiwa/quan.png", 800, 600, 30, 4, 8);


	Load("guang", "res/animations/zombies/naiwa/guang.png", 1000, 120, 8, 1, 8);
	//种子银行
	Load("seedbank", "res/images/bar.png", 530, 91);

	//卡片
	Load("card1", "res/images/cards/card1.png", 54, 77);
	Load("card2", "res/images/cards/card2.png", 54, 77);
	Load("card3", "res/images/cards/card3.png", 54, 77);
	Load("card4", "res/images/cards/card4.png", 54, 77);
	Load("card5", "res/images/cards/card5.png", 54, 77);
	Load("card6", "res/images/cards/card6.png", 54, 77);
	Load("card7", "res/images/cards/card7.png", 54, 77);
	Load("card8", "res/images/cards/card8.png", 54, 77);
	Load("black", "res/images/cards/black.png", 54, 77);
	Load("gray", "res/images/cards/black.png", 54, 77);

	//Menu
	Load("surface", "res/images/Menu/Surface_800.png", 800, 600);
	Load("adventure", "res/images/Menu/Adventure.png", 331, 145);
	Load("adventure_down", "res/images/Menu/Adventure_1.png", 331, 145);
	
	
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
