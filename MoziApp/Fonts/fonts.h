#pragma once

#define FontsNumbers				4
#define TextEditorFontsNumbers		2
#define MarkdownFontsNumbers		1


#define FontNameHeiTi		u8"黑体"
#define FontNameZhengHei	u8"宋体"
#define FontNameXiaoXing	u8"小新字体"

#include "EnumSet.h"

namespace Fonts {


	//enum  TextEditorFonts
	//{
	//	TextEditorFonts_ZhoneHei = 0,	//基本字体
	//	TextEditorFonts_XiaoXing = 1			//楷体
	//};

	//enum  MarkdownFonts
	//{
	//	MarkdownFonts_SimHei = 0,
	//	MarkdownFonts_XiaoXing = 1,
	//};

	class SetFonts
	{
	public:
		inline void SetTextEditorFont(ImFont* c_TextEditorFont) { TextEditorFont = c_TextEditorFont; }
		inline void SetMarkdownHeadFont(ImFont* c_MarkdownHeadFont) { MarkdownHeadFont = c_MarkdownHeadFont; }
		inline void SetMarkdownContentFont(ImFont* c_MarkdownContentFont) { MarkdownContentFont = c_MarkdownContentFont; }


		inline ImFont* GetTextEditorFont() { return TextEditorFont; }
		inline ImFont* GetMarkdownHeadFont() { return MarkdownHeadFont; }
		inline ImFont* GetMarkdownContentFont() { return MarkdownContentFont; }

	private:
		ImFont* WindowsFont;
		ImFont* TextEditorFont;

		ImFont* MarkdownHeadFont;
		ImFont* MarkdownContentFont;
	};



	void DynamicDPI(GLFWwindow* window, ImGuiIO& io);
	float* ReturnCurrentScale();


	ImFont* GetTextEditorFonts();
	ImFont* GetMarkdownHeadingFonts();
	ImFont* GetMarkdownContentFonts();

	void SetMarkdownHeadingFonts(const EnumSet::AllFonts& choice_font);
	void SetMarkdownContentFonts(const EnumSet::AllFonts& choice_font);
	void SetTextEditorFont(const EnumSet::AllFonts& choice_font);

}
