#include "BasePlant.h"



BasePlant::BasePlant()
{
	renderer = GetComponentByClass<SpriteRenderer>();
	box = ConstructComponent<BoxCollider>();
	box->SetAttachTo(root);
	box->SetSize({50,75});
	ani = ConstructComponent<Animator>();
	
}

void BasePlant::BeginPlay()
{
	;
}
