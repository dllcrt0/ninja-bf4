#pragma once

class GFxDrawText {
public:
	TextRendererData* CreateText(DWORD* pManager, const char* pText, UIElementFontStyle* pParams = nullptr);
	void SetText(TextRendererData* pText, const char* _pText, int unk);
	void Display(TextRendererData* pText, GRect<float>* pRect, float scale);
	void SetTextAlign(TextRendererData* pText, int a, int va);
	UIElementFontStyle* CreateStyle(const char* pFontName, const char* pFontSizeStr);
	
	DWORD dwRendererManager;
	DWORD dwRendererBase;
};
extern GFxDrawText pGFxDrawText;

class Renderer {
public:
	struct Colors {
		GColor menuBackground;
		GColor border;
		GColor descriptionText;
		GColor scrollbar;
		GColor selectedText;
		GColor text;
		GColor titleText;
		GColor topBottom;
		GColor slider;
	} UIColors;

	string TitleRenderText;

	string WordWrap(string pText, float fontScale, int maxX, int* lines);
	void DrawLine(float x1, float y1, float x2, float y2, GColor color, float thickness);
	void DrawRect(float x, float y, float w, float h, Justification justify, GColor color, unsigned long long unk = 0xAD00000009);
	void DrawString(const char* text, float x, float y, float scale, GColor color);
	void DrawOutlinedRect(float x, float y, float w, float h, float thickness, GColor boxColor, GColor borderColor, bool sides = false);
	void DrawCircle(const Vec3 &Center, float Radius, GColor color, unsigned short Sides);
	void DrawTitle(const char* title);

	void Init() {
		DbgPrint("Initializing renderer");

		fOptionHeight = 22.f;

		UIColors.menuBackground = GColor(0, 0, 0, 226, false);
		UIColors.border = GColor(255, 255, 255, 255, false);
		UIColors.descriptionText = GColor(255, 255, 255, 255, false);
		UIColors.scrollbar = GColor(102, 0, 255, 102, false);
		UIColors.selectedText = GColor(255, 255, 255, 255, false);
		UIColors.text = GColor(255, 255, 255, 255, false);
		UIColors.titleText = GColor(255, 255, 255, 255, false);
		UIColors.topBottom = GColor(102, 0, 255, 226, false);
		UIColors.slider = GColor(255, 255, 255, 255, false);
	}

	void Render();
	void RenderHeader();
	void RenderBackground();
	void RenderOptionHighlight();
	void RenderFooter();

	void SetTooltip(string name) {
		Tooltip = name;
	}

	int Clamp(int val, int min, int max);

	string Tooltip;
	int iRenderOptions;
	float fOptionHeight;
	float fBgPosition;
	float fFooterYCoord;
	float fHeaderYCoord;
	float fPositionAnimFix;
	float fBgYCoord;
	float fScrollPosition;
	float fScrollYCoordTemp;
	float fScrollBarHeight;
	float fScrollBarYCoord;
};
extern Renderer pRenderer;

class Text {
public:
	Text(const char* pText);
	~Text() { delete pParams; }

	Text& SetPosition(float x, float y);
	Text& SetColor(GColor color);
	Text& SetGlowColor(GColor color);
	Text& SetRightAlignParams();

	Text& SetFont(const char* pFont);
	Text& SetHorizontalAlignment(Alignment a);
	Text& SetVerticalAlignment(VAlignment a);
	Text& SetFontSize(float fSize);
	Text& SetFontSizeStr(const char* pStr);
	Text& SetLetterSpacing(float fSpacing);
	Text& SetRowSpacing(float fSpacing);

	void Render();
private:
	const char* pText;
	float fX, fY;
	UIElementFontStyle* pParams;
	bool bHasAlignment;
	Alignment HAlignment;
	VAlignment _VAlignment;
	bool bRight;
	bool bCenter;
};

class DrawText {
public:
	void DrawTooltipWrap(const char* text_, float x, float y, float boxY);
	void DrawOptionText(const char* text_, float x, float y, float scale, GColor color);
	void DrawOptionTextShadow(const char* text_, float x, float y, float scale, GColor color, GColor shadow);
	void DrawOptionTextBold(const char* text_, float x, float y, float scale, GColor color);
	void DrawOptionTextCenterAlign(const char* text_, float x, float y, float scale, GColor color);
	void DrawOptionTextBoldCenterAlign(const char* text_, float x, float y, float scale, GColor color);
	void DrawOptionTextBoldRightAlign(const char* text_, float x, float y, float scale, GColor color);
	void DrawOptionTextWithBox(const char* text_, float x, float y, float scale, GColor textColor, GColor boxColor, bool rightAlign);
	void DrawOptionTextBoldCenterAlignWithBox(const char* text_, float x, float boxX, float y, float scale, GColor color, GColor boxColor);
};
extern DrawText pDrawText;