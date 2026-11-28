#include "Animator.h"
#include"ResourceManager.h"
#include"SpriteRenderer.h"


void Animation::Load(std::string name, Vector2D delta)
{
	ImageResource aniRes = mainWorld.resourcePool->FetchAnimation(name);
	num = aniRes.num;
	images = aniRes.images;
}

void Animation::Tick()
{
	index = (index + 1) % num;
	if (notifications.find(index) != notifications.end())notifications[index].Execute();
}

void Animator::Update()
{
	if (!rendererAttached) {
		rendererAttached = pOwner->GetComponentByClass<SpriteRenderer>();
	}
	else if(aniNode){
		rendererAttached->sprite = aniNode->images[aniNode->index];
	}

	
}

void Animator::Insert(std::string name, Animation& ani)
{
	if(ani.images)animations.insert({name,ani});
}

void Animator::Activate()
{
	Component::Activate();
	if (aniNode)aniNode->clock.Continue();
}

void Animator::Deactivate()
{
	Component::Deactivate();
	if (aniNode)aniNode->clock.Stop();
}

void Animator::SetNode(std::string nodeName)
{
	if (aniNode)aniNode->clock.Stop();
	aniNode = &(animations.find(nodeName)->second);
	aniNode->clock.Continue();

}



