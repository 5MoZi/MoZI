#pragma once

enum  TextEditorFonts
{
	TextEditorFonts_ZhoneHei = 0,	//基本字体
	TextEditorFonts_SimHei = 1, 
	TextEditorFonts_XiaoXing = 2	//楷体
};

enum  MarkdownFonts
{
	MarkdownFonts_SimHei = 0,
	MarkdownFonts_XiaoXing = 1,
};

namespace EnumSet
{
	// UI主题颜色
	enum ThemeColor
	{
		ThemeColor_Light = 0,		// 白色
		ThemeColor_Dark = 1,		// 黑色
		ThemeColor_Classic = 2	    // 经典紫色
	};
}
