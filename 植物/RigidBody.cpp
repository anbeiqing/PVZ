#include "RigidBody.h"
#include"SpriteRenderer.h"




void RigidBody::Update()
{
	if (bMoveable) {
		if (bGraivtyEnabled) {
			velocity.y += gravity * DELTA_TIME;
			if (pOwner) {
				pOwner->AddLocalPosition(velocity);
				pOwner->AddLocalRotation(angulerVeocity);
			}
		}	
	}

}
