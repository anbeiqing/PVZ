#include "Collider.h"
#include<easyx.h>
#include"Camera.h"
#include"Controller.h"
#include"CollisionManager.h"
#include"SpriteRenderer.h"






const std::vector<Object*>& Collider::GetCollisions(CollisionType type)
{
    aims.clear();
    for (auto& it : collisions)
    {
        if (it->type == type)
        {
            aims.push_back(it->pOwner);
        }
    }
    return aims;
}

void Collider::Clear()
{
    for (auto& ano : collisions) {
        ano->collisions.erase(this);
        OnComponentEndOverlap.BroadCast(this, ano, ano->pOwner);
        ano->OnComponentEndOverlap.BroadCast(ano, this, pOwner);
    }
    collisions.clear();
    
}

void Collider::Erase()
{
    collisions_to_rease.clear();
    for (auto& ano : collisions) {
        if (!CollisionJudge(ano)) {
            ano->collisions.erase(this);
            collisions_to_rease.push_back(ano);
            OnComponentEndOverlap.BroadCast(this, ano, ano->pOwner);
            ano->OnComponentEndOverlap.BroadCast(ano, this, pOwner);
            
        }
    }
    for (auto& ano : collisions_to_rease)collisions.erase(ano);
}

void Collider::Insert(Collider* another)
{
    if (mainWorld.collisionManager->FindMapping(this->type,another->type) 
        && collisions.find(another) == collisions.end() && CollisionJudge(another)) {
        collisions.insert(another);
        another->collisions.insert(this);
        OnComponentBeginOverlap.BroadCast(this, another, another->pOwner);
        another->OnComponentBeginOverlap.BroadCast(another, this, pOwner);
        
    }
}
bool CircleCollider::CollisionJudge(Collider* another)
{
    if (another->GetShape() == Circle)
    {
        CircleCollider* circle = Cast<CircleCollider>(another);
        return Vector2D::Distance(GetWorldPosition()+Vector2D(radius,radius) ,
            circle->GetWorldPosition()+Vector2D(circle->radius,circle->radius)) <= radius + circle->radius;
    }
    else {
        BoxCollider* box = Cast<BoxCollider>(another);
        Vector2D pos = GetWorldPosition()+Vector2D(radius, radius), 
            pos_a = box->GetWorldPosition();
        Vector2D size = box->GetSize();
        float top = pos_a.y + size.y , bottom = pos_a.y ,
            left = pos_a.x , right = pos_a.x + size.x ;
        if (pos.x <= right && pos.x >= left && pos.y <= top && pos.y > bottom) {
            return true;
        }
        else {
            if (pos.x < left) {
                if (pos.y > top)return Vector2D::Distance(pos, { left,top }) <= radius;
                else if (pos.y < bottom)return Vector2D::Distance(pos, { left,bottom }) <= radius;
                else return left - pos.x <= radius;

            }
            else if (pos.x > right) {
                if (pos.y > top)return Vector2D::Distance(pos, { right,top }) <= radius;
                else if (pos.y < bottom)return Vector2D::Distance(pos, { right,bottom }) <= radius;
                else return right - pos.x <= radius;
            }
            else {
                if (pos.y > top)return pos.y - top <= radius;
                else return bottom - pos.y <= radius;
            }
        }
        return false;

    }
}

void CircleCollider::Update()
{
    radius = radius_ini * sqrtf(GetWorldScale().x * GetWorldScale().y);

}

void CircleCollider::DrawDebugLine()
{
    Vector2D pos = GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition();
    circle((int)pos.x+radius, (int)pos.y+radius, int(radius));
}

bool CircleCollider::IsMouseOver()
{
    return Vector2D::Distance(GetWorldPosition()+Vector2D(radius, radius), mainWorld.mainController->GetCursorPosition()) <= radius;
}

bool BoxCollider::CollisionJudge(Collider* another)
{
    if (another->GetShape() == Circle) {
        CircleCollider* circle = Cast<CircleCollider>(another);
        Vector2D pos_a = circle->GetWorldPosition()+ Vector2D(circle->GetRadius(), circle->GetRadius()), 
            pos = GetWorldPosition();
        float radius = circle->GetRadius();
        float top= pos.y + size.y , bottom = pos.y ,
            left = pos.x , right = pos.x + size.x;
        if (pos_a.x <= right && pos_a.x >= left && pos_a.y <= top && pos_a.y > bottom)return true;
        else {
            if (pos_a.x < left) {
                if (pos_a.y > top)return Vector2D::Distance(pos_a, { left,top }) <= radius;
                else if (pos_a.y < bottom)return Vector2D::Distance(pos_a, { left,bottom }) <= radius;
                else return left - pos_a.x <= radius;

            }
            else if (pos_a.x > right) {
                if (pos_a.y > top)return Vector2D::Distance(pos_a, { right,top }) <= radius;
                else if (pos_a.y < bottom)return Vector2D::Distance(pos_a, { right,bottom }) <= radius;
                else return right - pos_a.x <= radius;
            }
            else {
                if (pos_a.y > top)return pos_a.y - top <= radius;
                else return bottom - pos_a.y <= radius;
            }
        }
    }
    else {
        BoxCollider* box = Cast<BoxCollider>(another);
        //盒-盒统一用「左上角 + 尺寸」的 AABB 判定，与 DrawDebugLine/IsMouseOver/圆-盒一致
        //（原实现把 pos 减了半尺寸当中心，且 y 轴写成了 pos.y<pos.y+... 恒为真，等于完全不判 y）
        Vector2D pos = GetWorldPosition(), size_a = box->GetSize(), pos_a = box->GetWorldPosition();
        return (pos.x < pos_a.x + size_a.x && pos.x + size.x > pos_a.x
            && pos.y < pos_a.y + size_a.y && pos.y + size.y > pos_a.y);
    }
    
    return false;
}

void BoxCollider::Update()
{
    size = size_ini * sqrtf(GetWorldScale().x * GetWorldScale().y);
}

void BoxCollider::DrawDebugLine()
{
    Vector2D pos= GetWorldPosition() - mainWorld.mainCamera->GetCameraPosition();
    Vector2D si = this->size;
    float left = pos.x , right = pos.x + si.x ,
        top = pos.y + si.y , bottom = pos.y ;
    rectangle((int)left, (int)top, (int)right, (int)bottom);
}

bool BoxCollider::IsMouseOver()
{
    Vector2D pos = GetWorldPosition(), pos_m = mainWorld.mainController->GetCursorPosition();
    return pos_m.x <= pos.x + size.x  && pos_m.x >= pos.x 
        && pos_m.y <= pos.y + size.y && pos_m.y >= pos.y ;
}

void CollisionDelegate::BroadCast(Collider* OverlapCollider, Collider* OtherCollider, Object* OtherActor)
{
    for (auto& callback : callbacks)callback(OverlapCollider, OtherCollider, OtherActor);
}
