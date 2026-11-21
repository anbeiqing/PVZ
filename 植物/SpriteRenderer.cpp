#include "SpriteRenderer.h"
#pragma comment(lib,"Msimg32.lib")
#include"Camera.h"
#include"Math.h"

void SpriteRenderer::Render()
{
	if (sprite) {
		Vector2D pos = GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition();
			

		HDC dstDC = GetImageHDC();

		IMAGE* img = copy ? copy : sprite;
		HDC srcDC = GetImageHDC(img);
		int w = sprite->getwidth();
		int h = sprite->getheight();

		BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };
		if (bFilterEnabled && filterInfo.filter) {
			HDC srcDC_filter = GetImageHDC(filterInfo.filter);
			AlphaBlend(dstDC, (int)pos.x ,(int)pos.y,w,h,srcDC_filter, 0, 0, w, h, bf);
		}
		else AlphaBlend(dstDC, (int)pos.x, (int)pos.y,w,h, srcDC, 0, 0, w, h, bf);
	}

}


StaticMesh::StaticMesh()
{
	renderer = ConstructComponent<SpriteRenderer>();
	renderer->SetAttachTo(root);
	
}


