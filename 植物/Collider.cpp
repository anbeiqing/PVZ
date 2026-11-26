#include "Collider.h"
#include<easyx.h>
#include"Camera.h"
#include"Controller.h"
#include"CollisionManager.h"
#include"SpriteRenderer.h"


bool (*Collider::collisionJudgeMap[3])(Collider*, Collider*) =
{ &Collider::collisionJudgeCircleToCircle,Collider::collisionJudgeCircleToBox,Collider::collisionJudgeBoxToBox };

HitResult(*Collider::collisionHitMap[3])(Collider*, Collider*) =
{ &Collider::collisionHitCircleToCircle,Collider::collisionHitCircleToBox,Collider::collisionHitBoxToBox };




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

Vector2D BoxCollider::GetOverlapRect(const Rect& r1, const Rect& r2)
{
    return { min(r1.right, r2.right) - max(r1.left, r2.left), min(r1.top, r2.top) - max(r1.bottom, r2.bottom) };
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

bool Collider::CollisionJudge(Collider* another)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionJudgeMap[shape1 * shape1 + shape2 * shape2](this, another);
}

bool Collider::collisionJudgeCircleToCircle(Collider* c1, Collider* c2)
{
    CircleCollider* circle1 = Cast<CircleCollider>(c1);
    CircleCollider* circle2 = Cast<CircleCollider>(c2);
    return Vector2D::Distance(circle1->GetWorldPosition(), circle2->GetWorldPosition()) <= circle1->GetRadius() + circle2->GetRadius();
}

bool Collider::collisionJudgeCircleToBox(Collider* c1, Collider* c2)
{
    CircleCollider* circle;BoxCollider* box;
    if (c1->GetShape() == ColliderShape::Circle)
    {
        circle = Cast<CircleCollider>(c1), box = Cast<BoxCollider>(c2);
    }
    else
    {
        circle = Cast<CircleCollider>(c2), box = Cast<BoxCollider>(c1);
    }
    float radius = circle->GetRadius();Vector2D pos = circle->GetWorldPosition();
    BoxCollider::Rect rect = box->GetRect();

    if (pos.x <= rect.right && pos.x >= rect.left && pos.y <= rect.top && pos.y >= rect.bottom)
        return true;
    else
    {
        if (pos.x < rect.left)
        {
            if (pos.y > rect.top)return Vector2D::Distance(pos, { rect.left,rect.top }) <= radius;
            else if (pos.y < rect.bottom)return Vector2D::Distance(pos, { rect.left,rect.bottom }) <= radius;
            else return rect.left - pos.x <= radius;
        }
        else if (pos.x > rect.right)
        {
            if (pos.y > rect.top)return Vector2D::Distance(pos, { rect.right,rect.top }) <= radius;
            else if (pos.y < rect.bottom)return Vector2D::Distance(pos, { rect.right,rect.bottom }) <= radius;
            else return pos.x - rect.right <= radius;
        }
        else
        {
            if (pos.y > rect.top)return pos.y - rect.top <= radius;
            else return rect.bottom - pos.y <= radius;
        }
    }
}

bool Collider::collisionJudgeBoxToBox(Collider* c1, Collider* c2)
{
    BoxCollider* box1 = Cast<BoxCollider>(c1);
    BoxCollider* box2 = Cast<BoxCollider>(c2);
    Vector2D pos1 = box1->GetWorldPosition() - box1->GetSize() / 2;
    Vector2D pos2 = box2->GetWorldPosition() - box2->GetSize() / 2;
    return (pos1.x < pos2.x + box2->GetSize().x && pos1.x + box1->GetSize().x > pos2.x &&
        pos1.y < pos2.y + box2->GetSize().y && pos1.y + box1->GetSize().y > pos2.y);
}

HitResult Collider::CollisionHit(Collider* another)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionHitMap[shape1 * shape1 + shape2 * shape2](this, another);
}

HitResult Collider::collisionHitCircleToCircle(Collider* c1, Collider* c2)
{
    CircleCollider* circle1 = Cast<CircleCollider>(c1);
    CircleCollider* circle2 = Cast<CircleCollider>(c2);
    Vector2D impactNormal = (circle2->GetWorldPosition() - circle1->GetWorldPosition()).Normalize();
    Vector2D impactPoint = circle1->GetWorldPosition() + impactNormal * circle1->GetRadius();
    return HitResult(impactPoint, impactNormal, circle2->pOwner, circle2);
}

HitResult Collider::collisionHitCircleToBox(Collider* c1, Collider* c2)
{
    CircleCollider* circle; BoxCollider* box;
    if (c1->GetShape() == ColliderShape::Circle)
    {
        circle = Cast<CircleCollider>(c1), box = Cast<BoxCollider>(c2);
    }
    else
    {
        circle = Cast<CircleCollider>(c2), box = Cast<BoxCollider>(c1);
    }
    float radius = circle->GetRadius(); Vector2D pos = circle->GetWorldPosition();
    BoxCollider::Rect rect = box->GetRect();

    Vector2D impactNormal;
    Vector2D impactPoint;

    if (pos.x <= rect.right && pos.x >= rect.left && pos.y <= rect.top && pos.y >= rect.bottom)
    {
        impactPoint = pos;
        impactNormal = (c2->GetWorldPosition() - c1->GetWorldPosition()).Normalize();
    }
    else
    {
        if (pos.x < rect.left)
        {
            if (pos.y > rect.top) { impactPoint = { rect.left,rect.top };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize(); }
            else if (pos.y < rect.bottom) { impactPoint = { rect.left,rect.bottom };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize(); }
            else { impactPoint = { rect.left,pos.y };  impactNormal = { 1,0 }; }
        }
        else if (pos.x > rect.right)
        {
            if (pos.y > rect.top) { impactPoint = { rect.right,rect.top };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize(); }
            else if (pos.y < rect.bottom) { impactPoint = { rect.right,rect.bottom };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize(); }
            else { impactPoint = { rect.right,pos.y }; impactNormal = { -1,0 }; }
        }
        else
        {
            if (pos.y > rect.top) { impactPoint = { pos.x,rect.top }; impactNormal = { 0,-1 }; }
            else { impactPoint = { pos.x,rect.bottom }; impactNormal = { 0,1 }; }
        }
    }
    return HitResult(impactPoint, impactNormal * (c1 == circle ? 1.f : -1.f), c2->pOwner, c2);
}

HitResult Collider::collisionHitBoxToBox(Collider* c1, Collider* c2)
{
    BoxCollider* box1 = Cast<BoxCollider>(c1);
    BoxCollider* box2 = Cast<BoxCollider>(c2);
    BoxCollider::Rect r1 = box1->GetRect(), r2 = box2->GetRect();
    Vector2D overlapRect = BoxCollider::GetOverlapRect(r1, r2);
    Vector2D impactNormal;
    if (overlapRect.x >= overlapRect.y)
    {
        box1->GetWorldPosition().y - box2->GetWorldPosition().y > 0 ? impactNormal = { 0,-1 } : impactNormal = { 0,1 };
    }
    else
    {
        box1->GetWorldPosition().x - box2->GetWorldPosition().x > 0 ? impactNormal = { -1,0 } : impactNormal = { 1,0 };
    }

    //计算重叠区域中心点
    float centerX = max(r1.left, r2.left) + overlapRect.x / 2;
    float centerY = max(r1.bottom, r2.bottom) + overlapRect.y / 2;

    return HitResult({ centerX, centerY }, impactNormal, box2->pOwner, box2);
}
