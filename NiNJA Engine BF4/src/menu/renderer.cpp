#include "stdafx.h"

GFxDrawText pGFxDrawText;
Renderer pRenderer;
DrawText pDrawText;

void Renderer::DrawTitle(const char* title) {
	pDrawText.DrawOptionTextBoldCenterAlign(title, 0, pMenu.iYPositonEditable + 360 - (fBgPosition / 2) + fPositionAnimFix - 38 + 6, 22.f, UIColors.titleText); // current sub
}

void Renderer::Render() {
	iRenderOptions = (pSubmenuHandler.GetTotalOptions() > pMenu.iMaxOptions ? pMenu.iMaxOptions : pSubmenuHandler.GetTotalOptions());
	
	float bgPositionAnimTrans = (iRenderOptions * fOptionHeight) + 20;
	pUtils.Ease(fBgPosition, bgPositionAnimTrans, 1.5f);

	if (pSubmenuHandler.GetTotalOptions() < pMenu.iMaxOptions) {
		if (pSubmenuHandler.GetTotalOptions() % 2 == 0) {
			// if it's even
			fPositionAnimFix = fOptionHeight;
		}
	}

	int Y_coord = Clamp(pMenu.iCurrentOption - pMenu.iScrollOffset > pMenu.iMaxOptions ? pMenu.iMaxOptions : pMenu.iCurrentOption - pMenu.iScrollOffset, 0, pMenu.iMaxOptions);
	float scrollPositionAnimTrans = pMenu.iYPositonEditable + (360 - (fBgPosition / 2) + fPositionAnimFix) + (Y_coord * fOptionHeight) + 11;
	pUtils.Ease(fScrollPosition, scrollPositionAnimTrans, 1.5f);

	RenderHeader();
	RenderBackground();
	RenderOptionHighlight();
	RenderFooter();
}

void Renderer::RenderFooter() {
	auto countNewLines = [](string s) -> int {
		int count = 0;

		for (int i = 0; i < s.size(); i++)
			if (s[i] == '\n') count++;

		return count;
	};

	float yPos = pMenu.iYPositonEditable + (360 - (fBgPosition / 2) + fPositionAnimFix) + fBgPosition;
	pUtils.Ease(fFooterYCoord, yPos, 1.5f);

	float width = 420;
	if (Tooltip.length() == 0) {
		DrawOutlinedRect(640 - (width / 2), fFooterYCoord, width, 37, 1, UIColors.topBottom, UIColors.border, true); // footer
	} else {
		int count = countNewLines(Tooltip);
		DrawOutlinedRect(640 - (width / 2), fFooterYCoord, width, 37 + (22.f * count), 1, UIColors.topBottom, UIColors.border, true); // footer
		pDrawText.DrawOptionTextBold(Tooltip.c_str(), 437, fFooterYCoord + 3, 20.f, UIColors.descriptionText);
	}
}

void Renderer::RenderOptionHighlight() {
	pMenu.iCurrentRenderingScroll = Clamp(pMenu.iCurrentOption - pMenu.iScrollOffset > pMenu.iMaxOptions ? pMenu.iMaxOptions : (pMenu.iCurrentOption - pMenu.iScrollOffset) + 1, 0, pMenu.iMaxOptions);

	float width = 410;
	DrawRect(640 - (width / 2), fScrollPosition, 410, 22, JustifyLeft, UIColors.scrollbar); // scroller
}

void Renderer::RenderBackground() {
	float trans = pMenu.iYPositonEditable + 360 - (fBgPosition / 2) + fPositionAnimFix;
	pUtils.Ease(fBgYCoord, trans, 1.5f);

	float width = 410;
	DrawRect(640 - (width / 2), fBgYCoord, width, fBgPosition, JustifyLeft, UIColors.menuBackground);
}

void Renderer::RenderHeader() {
	float trans = pMenu.iYPositonEditable + 360 - (fBgPosition / 2) + fPositionAnimFix - 38;
	pUtils.Ease(fHeaderYCoord, trans, 1.5f);

	float width = 420;
	DrawOutlinedRect(640 - (width / 2), fHeaderYCoord, width, 37, 1, UIColors.topBottom, UIColors.border, true);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, GColor color, float thickness) {
	GRect<float> from(x1, y1, 0.f, 0.f);
	GRect<float> to(x2, y2, 0.f, 0.f);

	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwDrawLine), *(DWORD*)(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRectPtr)), &from, &to, &color, thickness);
}

void Renderer::DrawRect(float x, float y, float w, float h, Justification justify, GColor color, unsigned long long unk) {
	if (!*(DWORD*)(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRectPtr))) {
		DbgPrint("not rendering");
		return;
	}

	GRect<float> box;

	switch (justify) {
	case JustifyLeft:
		box.Left = x;
		box.Top = y;
		box.Right = x + w;
		box.Bottom = y + h;
		break;
	case JustifyCenter:
		box.Left = x - (w / 2);
		box.Top = y;
		box.Right = box.Left + w;
		box.Bottom = y + h;
		break;
	case JustifyRight:
		box.Left = x - w;
		box.Top = y;
		box.Right = box.Left + w;
		box.Bottom = y + h;
		break;
	}

	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwDrawRect), *(DWORD*)(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRectPtr)), &box, &color, 0xAD00000009);
}

void Renderer::DrawString(const char* text, float x, float y, float scale, GColor color) {
	pDrawText.DrawOptionText(text, x, y, scale, color);
}

void Renderer::DrawOutlinedRect(float x, float y, float w, float h, float thickness, GColor boxColor, GColor borderColor, bool sides) {
	DrawRect(x, y, w, h, JustifyLeft, boxColor);
	DrawRect(x, y - thickness, w, thickness, JustifyLeft, borderColor);
	DrawRect(x, y + h, w, thickness, JustifyLeft, borderColor);

	if (sides) {
		DrawRect(x - thickness, y - thickness, thickness, h + (thickness * 2), JustifyLeft, borderColor);
		DrawRect(x + w, y - thickness, thickness, h + (thickness * 2), JustifyLeft, borderColor);
	}
}

void Renderer::DrawCircle(const Vec3 &Center, float Radius, GColor color, unsigned short Sides) {
	float Angle = ((360.0f / Sides) * 3.14159265359) / 180;
	float Cos = cos(Angle);
	float Sin = sin(Angle);

	Vec3 vec(Radius, 0.f, 0.f);

	for (unsigned short i = 0; i < Sides; ++i) {
		Vec3 rot(Cos*vec.x - Sin * vec.y, Sin*vec.x + Cos * vec.y, 0.f);

		rot += Center;
		vec += Center;

		DrawLine(vec.x, vec.y, rot.x, rot.y, color, 1);

		vec = rot - Center;
	}
}

int Renderer::Clamp(int val, int min, int max) {
	return val < min ? min : val > max ? max : val;
}

#pragma region helpers
TextRendererData* GFxDrawText::CreateText(DWORD* pManager, const char* pText, UIElementFontStyle* pParams) {
	return Invoke::Call<TextRendererData*>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRenderingCreateText), pManager, pText, pParams);
}

void GFxDrawText::SetText(TextRendererData* pText, const char* _pText, int unk) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSetText), pText, _pText, unk);
}

void GFxDrawText::Display(TextRendererData* pText, GRect<float>* pRect, float scale) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwDisplay), *(DWORD*)(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRectPtr)), pText, pRect, scale);
}

void GFxDrawText::SetTextAlign(TextRendererData* pText, int a, int va) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSetTextAlign), pText, a, va);
}

UIElementFontStyle* GFxDrawText::CreateStyle(const char* pFontName, const char* pFontSizeStr) {
	/*DWORD dwRendererAlloc = (DWORD)malloc(0xC0);

	UIElementFontStyle *fontStyle = (UIElementFontStyle*)(dwRendererAlloc);
	UIElementFontDefinition *fontDef = (UIElementFontDefinition*)(dwRendererAlloc + 0x20);
	UIElementTextFilterGlow *textGlow = (UIElementTextFilterGlow*)(dwRendererAlloc + 0x80);

	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwConstructTextGlow), textGlow);
	textGlow->pSelfPtr = textGlow;
	textGlow->fGlowStrength = 13.37f;

	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwConstructFontDefinition), fontDef);
	fontDef->pGlow = (UIElementTextFilterGlow*)&textGlow->pSelfPtr;

	fontStyle->pVtable = (void*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwFontStyleVtable);
	fontStyle->pFont = pFontName;
	fontStyle->pFontDefinition = fontDef;*/

	UIElementFontStyle* style = (UIElementFontStyle*)*(DWORD*)(*(DWORD*)(pGFxDrawText.dwRendererBase + 0x14) + 0x38);

	if (style) {
		if (style->pFontDefinition) {
			if (style->pFontDefinition->pGlow) {
				style->pFontDefinition->pGlow->fGlowStrength = 13.37f;
			}
		}

		style->pFont = pFontName;
	}

	return style;
}

Text::Text(const char* pText_) {
	/*
		UI/elements/FontStyles/GenericPuristaEAMedium18
		UI/elements/FontStyles/Default
		UI/elements/ElementSettings/Battledash/BattledashWidgetFontStyle
		UI/elements/FontStyles/GenericPuristaEAMedium16
		UI/elements/FontStyles/GenericPuristaEASemibold14
	*/

	bRight = false;
	HAlignment = HorizontalLeftAlign;
	_VAlignment = VerticalNoAlign;
	pText = pText_;
	pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
}

Text& Text::SetFontSizeStr(const char* pStr) {
	if (pParams)
		if (pParams->pFontDefinition)
			pParams->pFontDefinition->pFontSizeStr = pStr;
	return *this;
}

Text& Text::SetPosition(float x, float y) {
	fX = x;
	fY = y;
	return *this;
}

Text& Text::SetColor(GColor color) {
	if (pParams)
		if (pParams->pFontDefinition)
			pParams->pFontDefinition->Color = color;
	return *this;
}

Text& Text::SetGlowColor(GColor color) {
	if (pParams) {
		if (pParams->pFontDefinition) {
			UIElementTextFilterGlow *glowFilter = (UIElementTextFilterGlow*)(*(DWORD*)(pParams->pFontDefinition->pGlow));
			if (glowFilter) {
				glowFilter->bUnkGlow = 0x17;
				glowFilter->Color = color;
				glowFilter->fGlowStrength = 100.f;
			}
		}
	}

	return *this;
}

Text& Text::SetRightAlignParams() {
	bRight = true;
	return *this;
}

Text& Text::SetFont(const char* pFont_) {
	if (pParams)
		pParams->pFont = pFont_;
	return *this;
}

Text& Text::SetHorizontalAlignment(Alignment a) {
	if (a == HorizontalCenterAlign)
		bCenter = true;
	HAlignment = a;
	return *this;
}

Text& Text::SetVerticalAlignment(VAlignment a) {
	_VAlignment = a;
	return *this;
}

Text& Text::SetFontSize(float fSize) {
	if (pParams)
		if (pParams->pFontDefinition)
			pParams->pFontDefinition->fScale = fSize;
	return *this;
}

Text& Text::SetLetterSpacing(float fSpacing) {
	if (pParams)
		if (pParams->pFontDefinition)
			pParams->pFontDefinition->fLetterSpacing = fSpacing;
	return *this;
}

Text& Text::SetRowSpacing(float fSpacing) {
	if (pParams)
		if (pParams->pFontDefinition)
			pParams->pFontDefinition->fRowSpacing = fSpacing;
	return *this;
}

void Text::Render() {
	if (!ValidPtr((DWORD*)pGFxDrawText.dwRendererManager)
		|| !ValidPtr((DWORD*)(pGFxDrawText.dwRendererManager + 4))
		|| !pGFxDrawText.dwRendererBase
		|| !ValidPtr((DWORD*)pGFxDrawText.dwRendererBase)) {
		return;
	}

	auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, pText, pParams);
	if (ptr) {
		GRect<float> pos(fX, fY, 1280, 720);

		if (bRight) {
			pos.Right = fX + 400;
		}

		pGFxDrawText.SetTextAlign(ptr, HAlignment, _VAlignment);
		pGFxDrawText.SetText(ptr, pText, 1);
		pGFxDrawText.Display(ptr, &pos, 1.0f);
	}
}
#pragma endregion

#pragma region draw text setups
bool CanRenderText() {
	return ValidPtr((DWORD*)pGFxDrawText.dwRendererManager)
		&& ValidPtr((DWORD*)(pGFxDrawText.dwRendererManager + 4))
		&& pGFxDrawText.dwRendererBase
		&& ValidPtr((DWORD*)pGFxDrawText.dwRendererBase)
		&& ValidPtr((DWORD*)*(DWORD*)((pGFxDrawText.dwRendererManager + 4) + 4))
		&& ValidPtr((DWORD*)*(DWORD*)(pGFxDrawText.dwRendererBase + 0x14))
		&& ValidPtr((DWORD*)*(DWORD*)(*(DWORD*)(pGFxDrawText.dwRendererBase + 0x14) + 0x38));
}

void DrawText::DrawTooltipWrap(const char* text_, float x, float y, float boxY) {
	if (!CanRenderText()) {
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
	if (pParams) {
		pParams->pFontDefinition->Color = pRenderer.UIColors.descriptionText;
		pParams->pFontDefinition->fScale = 20.f;
		pParams->pFontDefinition->pFontSizeStr = "$MENU_bold";

		auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
		if (ptr) {
			GRect<float> pos(x, y, 1280, 720);

			pGFxDrawText.SetTextAlign(ptr, HorizontalLeftAlign, VerticalNoAlign);
			pGFxDrawText.SetText(ptr, text_, 1);

			int lines = 0;
			string text(text_);

			float textWidth = ptr->GetWidth();
			if (textWidth >= 400) {
				for (size_t i = 0; i < text.length(); i++) {
					string newstr = text.substr(0, i);
					pGFxDrawText.SetText(ptr, newstr.c_str(), 1);
					float twidth = ptr->GetWidth();
					if (twidth >= 400) {
						int pos = newstr.find_last_of(" ");
						if (pos != newstr.npos) {
							text[pos] = '\n';
							lines++;
						}
					}
				}
			}

			pGFxDrawText.SetText(ptr, text.c_str(), 1);

			pRenderer.DrawOutlinedRect(640 - (420 / 2), boxY, 420, 37 + (22.f * lines), 1, pRenderer.UIColors.topBottom, pRenderer.UIColors.border, true); // footer

			pGFxDrawText.Display(ptr, &pos, 1.0f);
		}
	}
}

void DrawText::DrawOptionTextShadow(const char* text_, float x, float y, float scale, GColor color, GColor shadow) {
	DrawOptionText(text_, x, y, scale, color);
}

void DrawText::DrawOptionText(const char* text_, float x, float y, float scale, GColor color) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_medium");
	if (pParams) {
		if (pParams->pFontDefinition) {
			pParams->pFontDefinition->Color = color;
			pParams->pFontDefinition->fScale = scale;
			pParams->pFontDefinition->pFontSizeStr = "$MENU_medium";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);

				pGFxDrawText.SetTextAlign(ptr, HorizontalLeftAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);
				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}

void DrawText::DrawOptionTextBold(const char* text_, float x, float y, float scale, GColor color) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
	if (pParams) {
		if (pParams->pFontDefinition) {
			pParams->pFontDefinition->Color = color;
			pParams->pFontDefinition->fScale = scale;
			pParams->pFontDefinition->pFontSizeStr = "$MENU_bold";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);

				pGFxDrawText.SetTextAlign(ptr, HorizontalLeftAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);
				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}

void DrawText::DrawOptionTextBoldCenterAlign(const char* text_, float x, float y, float scale, GColor color) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
	if (pParams) {
		if (pParams->pFontDefinition) {
			pParams->pFontDefinition->Color = color;
			pParams->pFontDefinition->fScale = scale;
			pParams->pFontDefinition->pFontSizeStr = "$MENU_bold";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);

				pGFxDrawText.SetTextAlign(ptr, HorizontalCenterAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);
				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}

void DrawText::DrawOptionTextCenterAlign(const char* text_, float x, float y, float scale, GColor color) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_medium");
	if (pParams) {
		if (pParams->pFontDefinition) {
			pParams->pFontDefinition->Color = color;
			pParams->pFontDefinition->fScale = scale;
			pParams->pFontDefinition->pFontSizeStr = "$MENU_medium";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);

				pGFxDrawText.SetTextAlign(ptr, HorizontalCenterAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);
				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}

void DrawText::DrawOptionTextBoldRightAlign(const char* text_, float x, float y, float scale, GColor color) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
	if (pParams) {
		if (pParams->pFontDefinition) {
			pParams->pFontDefinition->Color = color;
			pParams->pFontDefinition->fScale = scale;
			pParams->pFontDefinition->pFontSizeStr = "$MENU_bold";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);
				pos.Right = x + 400;

				pGFxDrawText.SetTextAlign(ptr, HorizontalRightAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);
				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}

void DrawText::DrawOptionTextWithBox(const char* text_, float x, float y, float scale, GColor textColor, GColor boxColor, bool rightAlign) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto style = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
	if (style) {
		if (style->pFontDefinition) {
			style->pFontDefinition->Color = textColor;
			style->pFontDefinition->fScale = scale;
			style->pFontDefinition->pFontSizeStr = "$MENU_medium";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, style);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);

				if (rightAlign) {
					pos.Right = x + 400;
				}

				pGFxDrawText.SetTextAlign(ptr, HorizontalLeftAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);

				pRenderer.DrawRect(pos.Left - 2.f, pos.Top, ptr->GetWidth() + 4.f, ptr->GetHeight(), rightAlign ? JustifyRight : JustifyLeft, boxColor);

				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}

void DrawText::DrawOptionTextBoldCenterAlignWithBox(const char* text_, float x, float boxX, float y, float scale, GColor color, GColor boxColor) {
	if (!CanRenderText()) {
		DbgPrint("Prevented rendering crash, oi");
		return;
	}

	auto pParams = pGFxDrawText.CreateStyle("UI/elements/FontStyles/GenericPuristaEASemibold14", "$MENU_bold");
	if (pParams) {
		if (pParams->pFontDefinition) {
			pParams->pFontDefinition->Color = color;
			pParams->pFontDefinition->fScale = scale;
			pParams->pFontDefinition->pFontSizeStr = "$MENU_bold";

			auto ptr = pGFxDrawText.CreateText((DWORD*)pGFxDrawText.dwRendererManager, text_, pParams);
			if (ptr) {
				GRect<float> pos(x, y, 1280, 720);

				pGFxDrawText.SetTextAlign(ptr, HorizontalCenterAlign, VerticalNoAlign);
				pGFxDrawText.SetText(ptr, text_, 1);

				pRenderer.DrawRect(boxX - 2.f, pos.Top, ptr->GetWidth() + 4.f, ptr->GetHeight(), JustifyCenter, boxColor);

				pGFxDrawText.Display(ptr, &pos, 1.0f);
			}
		}
	}
}
#pragma endregion
