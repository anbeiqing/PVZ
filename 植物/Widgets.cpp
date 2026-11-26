#include "Widgets.h"
#include<easyx.h>
#include"Math.h"
#include"InputComponent.h"
#include"VisualInterface.h"
#include"Collider.h"
#include"ResourceManager.h"
#include"GameStatic.h"
#include"Controller.h"
#include"SpriteRenderer.h"
#include"UserInterface.h"


std::unordered_map<std::string, COLORREF>Characters::TextcolorMap =
{
    //白色，烟白
    { "$0",WHITE },{ "$1",RGB(245,245,245) },
    //浅灰色，深灰，黑
    { "$2",LIGHTGRAY },{ "$3",DARKGRAY },{ "$4",BLACK },
    //深绿色，绿色，黄色
    { "$5",GREEN },{ "$6",LIGHTGREEN },{ "$7",YELLOW },
    //橙色，浅红色，深红
    { "$8",RGB(255,165,0) },{ "$9",LIGHTRED },{ "$a",RED },
    //粉红，品红，紫色
    { "$b",LIGHTMAGENTA },{ "$c",MAGENTA },{ "$d",RGB(148,0,211) },
    //深蓝色，浅蓝色，天蓝
    { "$e",BLUE },{ "$f",LIGHTBLUE },{ "$g",LIGHTCYAN },
    //青色，褐色
    { "$h",CYAN },{ "$i",BROWN }

};

void Characters::SetCharacters(std::string text, int size, LPCTSTR type)
{
    row = 1;
    column = 0;
    int temp = 0;
    for (int i = 0;i < text.length();++i)
    {
        if (text[i] == '\n')  // 
        {
            row++;
            column = temp > column ? temp : column;
            temp = 0;
        }
        else if (text[i] == '$' && i + 1 < text.length())
        {
            std::string buf = "$";
            buf += text[i + 1];
            if (TextcolorMap.find(buf) != TextcolorMap.end())
            {
                ++i; // 跳过标记字符，不计入文字长度
            }
            else
            {
                temp++; // $不是有效标记，这个$算作普通字符
            }
        }
        else
        {
            temp++;
        }
    }
    column = temp > column ? temp : column;
    this->texts = text;
    this->size = size;
    this->type = type;
}

void Characters::PrintCharacters(Vector2D pos, CharactersPattern pattern)
{
    settextstyle(6 * size, 0, type);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK); // 初始默认颜色
    int pat = 1;
    switch (pattern) {
    case CharactersPattern::left:
        pat = 0;
        break;
    case CharactersPattern::Middle:
        pat = 1;
        break;
    case CharactersPattern::Right:
        pat = 2;
        break;
    default:pat = 1;break;

    }
    std::string temp;
    int r = 0;
    for (int i = 0;i < texts.length();++i)
    {
        if (texts[i] == '\n')
        {
            outtextxy((int)pos.x + (GetWidth() - temp.size() * 3 * size) * pat / 2, pos.y + 6 * r * size, temp.c_str());
            temp.clear();
            ++r;
        }
        else if (texts[i] == '$')
        {

            if (i + 1 >= texts.length())
            {
                temp.push_back('$');
                continue;
            }
            std::string buf = "$";
            buf += texts[i + 1];
            if (TextcolorMap.find(buf) != TextcolorMap.end())
            {
                settextcolor(TextcolorMap[buf]);
                ++i;
            }
            else
            {
                temp.push_back('$');
            }
        }
        else
        {
            temp.push_back(texts[i]);
        }
    }
    outtextxy((int)pos.x + (GetWidth() - temp.size() * 3 * size) * pat / 2, pos.y + r * 6 * size, temp.c_str());
    settextcolor(BLACK);
}

Vector2D Widget::GetLayoutOffset() const
{
    //部件的位置一律以"中心点"为准（Image/Text 渲染时都减去半个尺寸），
    //所以相对父级的位置偏移也应该以父级中心为原点：左上角 = (-w/2, -h/2)
    Vector2D ps = parent ? parent->Getsize() : Vector2D(0, 0);
    switch (layoutPattern)
    {
        case LayoutPattern::LeftTop:return Vector2D(-ps.x * 0.5f, -ps.y * 0.5f);
        case LayoutPattern::MiddleTop:return Vector2D(0, -ps.y * 0.5f);
        case LayoutPattern::RightTop:return Vector2D(ps.x * 0.5f, -ps.y * 0.5f);
        case LayoutPattern::LeftMiddle:return Vector2D(-ps.x * 0.5f, 0);
        case LayoutPattern::Center:return Vector2D(0, 0);
        case LayoutPattern::RightMiddle:return Vector2D(ps.x * 0.5f, 0);
        case LayoutPattern::LeftBottom:return Vector2D(-ps.x * 0.5f, ps.y * 0.5f);
        case LayoutPattern::MiddleBottom:return Vector2D(0, ps.y * 0.5f);
        case LayoutPattern::RightBottom:return Vector2D(ps.x * 0.5f, ps.y * 0.5f);
        default:return Vector2D(0, 0);
    }
    return Vector2D(0, 0);
}

bool Widget::IsUnderCursor() const
{
    float x = InputComponent::GetMousePosition().x;
    float y = InputComponent::GetMousePosition().y;

    //原来写成 empty() 才遍历：格子为空时循环一次都不进，永远命中不了；
    //而且判定的是"这一格里有没有任意一个部件被命中"，不是"我自己被命中"，
    //于是鼠标停在第一张卡片上时 8 张卡片的信息框会全部叠在鼠标处（最后画的赢）。
    //UIdetectZones 的分格维护本身也有毛病（只登记到左上角那一格），这里直接做矩形判定最稳。
    Vector2D loc = GetScreenPosition();
    Vector2D sz = Getsize();
    return x<sz.x * 0.5f + loc.x && x>loc.x - sz.x * 0.5f
        && y<sz.y * 0.5f + loc.y && y>loc.y - sz.y * 0.5f;
}

Widget::~Widget()
{
    for (int i = point.x;i < point_1.x;++i)for (int j = point.y;j < point_1.y;++j)mainWorld.UIdetectZones[i][j].erase(this);
}

void Widget::Update()
{
    if (uiPattern != UIPattern::VisibleAndInteractive)return;
    Vector2D pos = GetScreenPosition() - Getsize()/2;
    Pair newPoint(Math::Clamp(int(pos.x) / 100, 0, 7), Math::Clamp(int(pos.y) / 100, 0, 5));
    pos += size * 2;
    Pair newPoint_1(Math::Clamp(int(pos.x) / 100, 0, 7), Math::Clamp(int(pos.y) / 100, 0, 5));
    if (newPoint == point && newPoint_1 == point_1)return;
    if (point != Pair(-1, -1))for (int i = point.x;i < point_1.x;++i)for (int j = point.y;j < point_1.y;++j)mainWorld.UIdetectZones[i][j].erase(this);
    point = newPoint;point_1 = newPoint_1;
    for (int i = point.x;i < point_1.x;++i)for (int j = point.y;j < point_1.y;++j)mainWorld.UIdetectZones[i][j].insert(this);
}

void Widget::ShowInfoBox()
{
    if (bInfoBox && uiPattern == UIPattern::VisibleAndInteractive && IsUnderCursor())
    {
        setlinecolor(BLACK);
        setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
        setfillcolor(RGB(255, 247, 213));

        Vector2D pos = InputComponent::GetMousePosition() + Vector2D(-15, 15);
        fillrectangle((int)pos.x, (int)pos.y, (int)pos.x + InfoText.GetWidth(), (int)pos.y + InfoText.GetHigh());
        InfoText.PrintCharacters(pos);
    }

}
void Widget::DrawDebugRect()
{
    setlinecolor(BLUE);
    Vector2D pos = GetScreenPosition() - Getsize() / 2;
    int left = int(pos.x), top = int(pos.y);
    pos += Getsize();
    int right = int(pos.x), bottom = int(pos.y);
    rectangle(left, top, right, bottom);
}

Vector2D Widget::Getsize() const
{
    //原来这里固定返回 (0,0)：Image 的渲染宽高 = 尺寸 * 源图区域 / 源图宽高 → 0，什么都不画
    return size;
}

void Widget::AttachTo(Widget* par)
{
    if (par) {
        par->children.insert(this);
        parent = par;
    }
}

void Widget::DetachFrom(Widget* par)
{
    if (par) {
        par->children.erase(this);
        parent = nullptr;
    }
}

Vector2D Widget::GetScreenPosition() const
{
    if (parent)return parent->GetScreenPosition() + GetRelativePosition() + GetLayoutOffset();
    else return GetRelativePosition();
}

float Widget::GetScreenRotation() const
{
    if (parent)return parent->GetScreenRotation() + GetRelativeRoation();
    else return GetRelativeRoation();
}

Vector2D Widget::GetScreenScale() const
{
    if (parent)return parent->GetScreenScale() + GetRelativeScale();
    else return GetRelativeScale();
}
Panel::~Panel()
{
    if (members_ui.empty())return;
    for (auto& ui : members_ui)
    {
        ui->RemoveFromViewport();
    }
}

void Panel::Update()
{
    Widget::Update();
    if (members.empty())return;
    int index = 0;
    for (auto& member : members)
    {
        AdjustMemberPosition(member, index++);
    }
}

void Panel::SetUnitSize(Vector2D size)
{
    unitSize = size;
    Panel::Update();
}

void Panel::AddMember(Widget* member, int index)
{
    member->AttachTo(this);
    if (index >= 0)members.emplace(members.begin() + index, member);
    else members.push_back(member);
    member->attachedPanel = this;
    AdjustMemberSizeToUnit(member);
    AdjustMemberPosition(member, index >= 0 ? index : members.size() - 1);
}
void Panel::RemoveMember(Widget* member)
{
    member->DetachFrom(this);
    members.erase(std::remove(members.begin(), members.end(), member), members.end());
    member->attachedPanel = nullptr;
}
void Panel::AddMember(UserInterface* member, int index)
{
    //根画布挂在面板里时用中心布局，否则会再叠加一次左上角偏移
    member->rootCanvas->SetLayoutPattern(LayoutPattern::Center);
    member->rootCanvas->AttachTo(this);
    index = Math::Clamp(index, -1, int(members.size()));
    if (index >= 0)members.emplace(members.begin() + index, member->rootCanvas);
    else members.push_back(member->rootCanvas);
    members_ui.push_back(member);
    member->rootCanvas->attachedPanel = this;
    //被当成面板成员使用的 UserInterface（比如种子卡片 SeedUI）不是通过 AddToViewport 创建的，
    //它内部的部件默认 uiPattern=None，而 UserInterface::Update 只 Update 非 None 的部件，
    //于是按钮永远不 Update → 贴图和裁剪区域都是空的，卡片什么也画不出来
    member->AddToViewport();
    AdjustMemberSizeToUnit(member->rootCanvas);
    AdjustMemberPosition(member->rootCanvas, index >= 0 ? index : members.size() - 1);
}
void Panel::RemoveMember(UserInterface* member)
{
    member->rootCanvas->DetachFrom(this);
    members.erase(std::remove(members.begin(), members.end(), member->rootCanvas), members.end());
    members_ui.erase(std::remove(members_ui.begin(), members_ui.end(), member), members_ui.end());
    member->rootCanvas->attachedPanel = nullptr;
}

void Panel::AdjustMemberSizeToUnit(Widget* member)
{
    int x = member->Getsize().x;
    int y = member->Getsize().y;
    if (x <= 0 || y <= 0)return;//尺寸还没设置，别算出 inf/nan 缩放
    member->SetrelativeScale(unitSize / sqrtf (x*x+y*y));
}

void HorizontalPanel::AdjustMemberPosition(Widget* member, int index)
{
    if (index < 0)return;
    //部件位置是中心点：整排以面板中心为轴左右对称，而不是从面板左上角开始摆
    float start = -Getsize().x * 0.5f + unitSize.x * 0.5f;
    Vector2D pos = Vector2D(start + index * (unitSize.x + spacing), 0);
    member->SetRelativePosition(pos);
}

Vector2D HorizontalPanel::Getsize() const
{
    return members.empty() ? Vector2D() : Vector2D(members.size() * (unitSize.x + spacing) - spacing, unitSize.y);
}

void VerticalPanel::AdjustMemberPosition(Widget* member, int index)
{
    if (index < 0)return;
    float start = -Getsize().y * 0.5f + unitSize.y * 0.5f;
    Vector2D pos = Vector2D(0, start + index * (unitSize.y + spacing));
    member->SetRelativePosition(pos);
}

Vector2D VerticalPanel::Getsize() const
{
    return members.empty() ? Vector2D() : Vector2D(unitSize.x, members.size() * (unitSize.y + spacing) - spacing);
}

void GridPanel::AdjustMemberPosition(Widget* member, int index)
{
    if (index < 0)return;
    float startX = -Getsize().x * 0.5f + unitSize.x * 0.5f;
    float startY = -Getsize().y * 0.5f + unitSize.y * 0.5f;
    Vector2D pos = Vector2D(startX + (index % column) * (unitSize.x + spacingX),
        startY + (index / column) * (unitSize.y + spacingY));
    member->SetRelativePosition(pos);
}

Vector2D GridPanel::Getsize() const
{
    return members.empty() ? Vector2D() : Vector2D(column * (unitSize.x + spacingX) - spacingX, row * (unitSize.y + spacingY) - spacingY);
}






void Text::Update()
{
    Widget::Update();
    if (bindedText)texts.SetCharacters(*bindedText);
    size = Vector2D(float(texts.GetWidth()), float(texts.GetHigh()));
}

void Text::Render()
{
    texts.PrintCharacters(GetScreenPosition() - size * 0.5f, textPattern);
}








void Image::Update()
{
    Widget::Update();
    if (!sprite)return;

    if (copy)spriteInfo.size = Pair(copy->getwidth(), copy->getheight());
    else spriteInfo.size = Pair(sprite->getwidth(), sprite->getheight());

    //源图的裁剪区域默认取整张图（Button 在 Load*Picture 里、SeedUI 在冷却动画里会自己覆盖）
    if (spriteInfo.startLoc.x == 0 && spriteInfo.startLoc.y == 0
        && spriteInfo.endLoc.x == 0 && spriteInfo.endLoc.y == 0)
    {
        spriteInfo.endLoc = spriteInfo.size;
    }

    if (GetScreenRotation() != angle)
    {
        angle = GetScreenRotation();
        //RotateImage(PI * angle / 180);
        //if (filterLayers.size() > 0)FilterImage();
    }
}

void Image::Render()
{
    if (!sprite)return;

    Vector2D pos = GetScreenPosition();
    HDC dstDC = GetImageHDC();

    IMAGE* img = copy ? copy : sprite;
    HDC srcDC = GetImageHDC(img);
    int w = spriteInfo.endLoc.x - spriteInfo.startLoc.x;
    int h = spriteInfo.endLoc.y - spriteInfo.startLoc.y;

    int dw = int(Getsize().x * w / img->getwidth());
    int dh = int(Getsize().y * h / img->getheight());

    BLENDFUNCTION bf = { AC_SRC_OVER,0,alpha,AC_SRC_ALPHA };
   // if (filterLayers.size() > 0 && filter)srcDC = GetImageHDC(filter);
    AlphaBlend(dstDC, (int)pos.x - Getsize().x / 2, (int)pos.y - Getsize().y / 2, dw, dh,
        srcDC, (int)spriteInfo.startLoc.x, (int)spriteInfo.startLoc.y, w, h, bf);
}

bool Image::IsMouseOn()
{
    return IsUnderCursor();
}







void Button::Update()
{
    Image::Update();

    if (!isHover && IsMouseOn()) { OnMouseHoverBegin.BroadCast(); isHover = true; }
    else if (isHover && !IsMouseOn()) { OnMouseHoverEnd.BroadCast(); isHover = false; }

    if (!isPressed && IsMousePressed()) { OnMousePressedBegin.BroadCast(); isPressed = true; }
    else if (isPressed && !IsMousePressed()) { OnMousePressedEnd.BroadCast(); isPressed = false; }


    //没加载悬停/按下贴图时回退到普通贴图，避免解引用空指针
    IMAGE* target = normal;
    if (isHover && !isPressed && hover)target = hover;
    else if (isPressed && pressed)target = pressed;
    sprite = target;
    if (sprite)spriteInfo.endLoc = { sprite->getwidth(), sprite->getheight() };
}

void Button::LoadNormalPicture(std::string path)
{
    normal = mainWorld.resourcePool->Fetch(path);
    if (!normal)return;
    spriteInfo.endLoc = { normal->getwidth(), normal->getheight() };
}

void Button::LoadHoverPicture(std::string path)
{
    hover = mainWorld.resourcePool->Fetch(path);
    if (!hover)return;
    spriteInfo.endLoc = { hover->getwidth(), hover->getheight() };
}

void Button::LoadClickPicture(std::string path)
{
    pressed = mainWorld.resourcePool->Fetch(path);
    if (!pressed)return;
    spriteInfo.endLoc = { pressed->getwidth(), pressed->getheight() };
}

bool Button::IsMousePressed()
{
    return IsMouseOn() && GameStatics::GetController()->IsMouseClicked();
    return false;
}

void Bar::Update()
{
    Widget::Update();
}

void Bar::Render()
{
    Vector2D pos = GetScreenPosition();
    HDC dstDC = GetImageHDC();
    BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };

    if (barBack)
    {
        HDC srcDC = GetImageHDC(barBack);
        int w = barBack->getwidth();
        int h = barBack->getheight();
        AlphaBlend(dstDC, (int)pos.x - sizeBack.x / 2, (int)pos.y - sizeBack.y / 2, sizeBack.x, sizeBack.y, srcDC, 0, 0, w, h, bf);
    }

    if (barFront)
    {
        HDC srcDC = GetImageHDC(barFront);

        Pair startPosition, endPosition;

        switch (direction)
        {
        case BarDirection::RightToLeft: startPosition = { 0,0 }; endPosition = { (int)(sizeFront.x * percentage),sizeFront.y }; break;
        case BarDirection::LeftToRight: startPosition = { (int)(sizeFront.x * (1 - percentage)),0 }; endPosition = { sizeFront.x,sizeFront.y }; break;
        case BarDirection::TopToBottom: startPosition = { 0,(int)(sizeFront.y * (1 - percentage)) }; endPosition = { sizeFront.x,sizeFront.y }; break;
        case BarDirection::BottomToTop: startPosition = { 0,0 }; endPosition = { sizeFront.x,(int)(sizeFront.y * percentage), }; break;
        default: startPosition = { 0,0 }; endPosition = { 0,0 }; break;
        }

        int w = endPosition.x - startPosition.x;
        int h = endPosition.y - startPosition.y;
        AlphaBlend(dstDC, (int)pos.x - sizeFront.x / 2, (int)pos.y - sizeFront.y / 2, sizeFront.x, sizeFront.y, srcDC, 0, 0, w, h, bf);

        if (barButton)
        {
            AlphaBlend(dstDC, endPosition.x - sizeButton.x / 2, endPosition.y - sizeButton.y / 2, sizeButton.x, sizeButton.y,
                GetImageHDC(barButton), 0, 0, barButton->getwidth(), barButton->getheight(), bf);
        }
    }

}

void Bar::LoadBarFrontPicture(std::string path)
{
    barFront = mainWorld.resourcePool->Fetch(path);
    SetFrontSize(Pair(barFront->getwidth(), barFront->getheight()));
}

void Bar::LoadBarBackPicture(std::string path)
{
    barBack = mainWorld.resourcePool->Fetch(path);
    SetBackSize(Pair(barBack->getwidth(), barBack->getheight()));
}

void Bar::LoadBarButtonPicture(std::string path)
{
    barButton = mainWorld.resourcePool->Fetch(path);
    SetButtonSize(Pair(barButton->getwidth(), barButton->getheight()));
}








