#pragma once

#define FontsNumbers				4
#define TextEditorFontsNumbers		2
#define MarkdownFontsNumbers		1


#define FontNameHeiTi		u8"黑体"
#define FontNameZhengHei	u8"宋体"
#define FontNameXiaoXing	u8"小新字体"

#define FontDPI				20.0f

#include "EnumSet.h"

namespace Fonts {

	struct FontSet
	{
		ImFont* TextEditorFont;
		ImFont* MarkdownHeadFont;
		ImFont* MarkdownContentFont;
	};

	//class FontSet
	//{
	//public:
	//	inline void SetTextEditorFont(ImFont* c_TextEditorFont) { TextEditorFont = c_TextEditorFont; }
	//	inline void SetMarkdownHeadFont(ImFont* c_MarkdownHeadFont) { MarkdownHeadFont = c_MarkdownHeadFont; }
	//	inline void SetMarkdownContentFont(ImFont* c_MarkdownContentFont) { MarkdownContentFont = c_MarkdownContentFont; }


	//	inline ImFont* GetTextEditorFont() { return TextEditorFont; }
	//	inline ImFont* GetMarkdownHeadFont() { return MarkdownHeadFont; }
	//	inline ImFont* GetMarkdownContentFont() { return MarkdownContentFont; }

	//private:
	//	ImFont* WindowsFont;

	//	ImFont* TextEditorFont;
	//	ImFont* MarkdownHeadFont;
	//	ImFont* MarkdownContentFont;
	//};


	void DynamicFontsSize(GLFWwindow* window, ImGuiIO& io);
	float* ReturnCurrentScale();


	ImFont* GetTextEditorFonts();
	ImFont* GetMarkdownHeadFonts();
	ImFont* GetMarkdownContentFonts();

	void SetMarkdownHeadFonts(const EnumSet::AllFonts& choice_font);
	void SetMarkdownContentFonts(const EnumSet::AllFonts& choice_font);
	void SetTextEditorFont(const EnumSet::AllFonts& choice_font);

	void FontsInit();
	FontSet* GetFontSetIO();

}
