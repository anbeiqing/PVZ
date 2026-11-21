#pragma once
#include"coreMinimal.h"
#include"VisualInterface.h"
#include<unordered_map>
#include"Delegate.h"

enum class CharactersPattern
{
	left,
	Middle,
	Right

};

class Characters
{
	int row = 1;
	int column = 0;
	std::string texts = " ";
	int size = 24;
	LPCTSTR type = _T("楷体");
	int lineH = size + size / 2;


	//颜色映射库
	static std::unordered_map<std::string, COLORREF>TextcolorMap;


public:

	int GetWidth()const { return column * size * 3; }
	int GetHigh()const { return row * size*6; }
	void SetCharacters(std::string text, int size = 3, LPCTSTR type = _T("楷体"));
	void PrintCharacters(Vector2D pos, CharactersPattern pattern = CharactersPattern::Middle);

};
enum class LayoutPattern
{
	LeftTop,
	MiddleTop,
	RightTop,
	LeftMiddle,
	Center,
	RightMiddle,
	LeftBottom,
	MiddleBottom,
	RightBottom
};
enum class UIPattern
{
	None,
	VisibleOnly,
	VisibleAndInteractive
};


//基础部件
class Widget :public Base, public LayerInterface
{
	friend class Panel;
	Pair point{-1,-1};
	Pair point_1{ -1,-1 };
	Vector2D GetLayoutOffset()const;
protected:
	Transform transform;
	Widget* parent = nullptr;
	std::unordered_set<Widget*>children;
	Panel* attachedPanel = nullptr;

	Vector2D size;
	LayoutPattern layoutPattern;
	UIPattern uiPattern;

	Characters InfoText;
	bool bInfoBox=false;
	bool IsUnderCursor()const;
public:
	Widget():layoutPattern(LayoutPattern::LeftTop),uiPattern(UIPattern::None){}
	virtual ~Widget();
	virtual void Update();
	virtual void BeginPlay(){}
	virtual void Render(){}
	void ShowInfoBox();
	void DrawDebugRect();

	void SetLayoutPattern(LayoutPattern pattern) { layoutPattern = pattern; }
	void SetUIPattern(UIPattern pattern) { uiPattern = pattern; }
	UIPattern GetUIPattern()const { return uiPattern; }
	void SetInfoBox(bool info) { bInfoBox = info; }
	void SetInfoText(std::string text) { InfoText.SetCharacters(text); }

	virtual Vector2D Getsize()const;
	void SetSize(Vector2D si) { size = si; }

	void AttachTo(Widget* par);
	void DetachFrom(Widget* par);

	Vector2D GetScreenPosition()const;
	float GetScreenRotation()const;
	Vector2D GetScreenScale()const;

	Vector2D GetRelativePosition()const { return transform.position; }
	float GetRelativeRoation()const { return transform.rotation; }
	Vector2D GetRelativeScale()const { return transform.scale; }

	void SetRelativePosition(Vector2D pos) { transform.position = pos; }
	void SetRelativeRoation(float ro) { transform.rotation = ro; }
	void SetrelativeScale(Vector2D sc) { transform.scale = sc; }



};
//容器面板，容纳小部件
class Panel :public Widget
{
	void SetSize(Vector2D size) { this->size = size; }//不允许手动设置Panel的大小
protected:
	std::vector<class Widget*>members;
	std::vector<class UserInterface*>members_ui;

	Vector2D unitSize;
	virtual void AdjustMemberPosition(Widget* member, int index) = 0;
public:
	virtual ~Panel();

	virtual void Update()override;

	void SetUnitSize(Vector2D size);

	void AddMember(Widget* member, int index = -1);
	void RemoveMember(Widget* member);

	void AddMember(UserInterface* member, int index = -1);
	void RemoveMember(UserInterface* member);

private:
	void AdjustMemberSizeToUnit(Widget* member);
};

/* 水平面板 */
class HorizontalPanel final : public Panel
{
	float spacing = 0;
	void AdjustMemberPosition(Widget* member, int index)override;
public:
	void SetSpacing(float space) { spacing = space; }
	Vector2D Getsize() const override;
};

/* 垂直面板 */
class VerticalPanel final : public Panel
{
	float spacing = 0;
	void AdjustMemberPosition(Widget* member, int index)override;
public:
	void SetSpacing(float space) { spacing = space; }
	Vector2D Getsize() const override;
};

/* 网格面板 */
class GridPanel final : public Panel
{
	int row = 1;
	int column = 1;
	float spacingX = 0;
	float spacingY = 0;
	void AdjustMemberPosition(Widget* member, int index)override;
public:
	void SetRow(int num) { row = num; }
	void SetColumn(int num) { column = num; }
	void SetSpacingX(float space) { spacingX = space; }
	void SetSpacingY(float space) { spacingY = space; }
	Vector2D Getsize() const override;
};






/* 文本部件 */
class Text :public Widget
{
protected:
	Characters texts;
	CharactersPattern textPattern;
	const std::string* bindedText = nullptr;
public:
	Text() :textPattern(CharactersPattern::Middle) {}

	virtual void Update();

	virtual void Render()override;

	void SetPattern(CharactersPattern pattern) { textPattern = pattern; }
	void SetText(std::string te, int si = 3, LPCTSTR ty = "楷体") { texts.SetCharacters(te, si, ty); }
	void BindText(const std::string& origin) { bindedText = &origin; }
};







/* 图像部件 */
class Image : public Widget, public ImageInterface
{
public:
	virtual void Update()override;
	virtual void Render()override;

	bool IsMouseOn();
};






//按钮委托
DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(ButtonDelegate)

/* 按钮部件 */
class Button :public Image
{
	IMAGE* normal;
	IMAGE* hover;
	IMAGE* pressed;

	bool isPressed = false;
	bool isHover = false;
public:
	virtual void Update()override;

	void LoadNormalPicture(std::string path);
	void LoadHoverPicture(std::string path);
	void LoadClickPicture(std::string path);

	bool IsMousePressed();

	ButtonDelegate OnMousePressedBegin;
	ButtonDelegate OnMousePressedEnd;
	ButtonDelegate OnMouseHoverBegin;
	ButtonDelegate OnMouseHoverEnd;
};

//滑动条部件方向
enum class BarDirection 
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};

/* 滑动条部件 */
class Bar : public Widget
{
	float percentage = 0;
	IMAGE* barFront;
	IMAGE* barBack;
	IMAGE* barButton;
	Pair sizeFront = Pair(0, 0);
	Pair sizeBack = Pair(0, 0);
	Pair sizeButton = Pair(0, 0);
	BarDirection direction;
public:
	virtual void Update()override;
	virtual void Render()override;

	void LoadBarFrontPicture(std::string path);
	void LoadBarBackPicture(std::string path);
	void LoadBarButtonPicture(std::string path);

	void SetDirection(BarDirection dir) { direction = dir; }
	void SetFrontSize(Pair size) { sizeFront = size; }
	void SetBackSize(Pair size) { sizeBack = size; }
	void SetButtonSize(Pair size) { sizeButton = size; }

	void SetPercentage(float per) { percentage = per; }
	float GetPercentage() const { return percentage; }
};

