#pragma once



namespace EnumSet
{
	// UI主题颜色
	enum ThemeColor
	{
		ThemeColor_Light = 0,		// 白色
		ThemeColor_Dark = 1,		// 黑色
		ThemeColor_Classic = 2	    // 经典紫色
	};

	// 文本编辑字体
	enum  TextEditorFonts
	{
		TextEditorFonts_ZhoneHei = 0,	// 基本字体
		TextEditorFonts_SimHei = 1,		// 黑体
		TextEditorFonts_XiaoXing = 2	// 小新字体
	};

	// Markdown显示字体
	enum  MarkdownFonts
	{
		MarkdownFonts_ZhoneHei = 0,		// 基本字体
		MarkdownFonts_SimHei = 1,		// 黑体
		MarkdownFonts_XiaoXing = 2,		// 小新字体
	};

	enum AllFonts
	{
		AllFonts_Arial = 0,				// 英文
		AllFonts_Song = 1,				// 宋体
		AllFonts_SimHei = 2,			// 黑体
		AllFonts_XiaoXing = 3			// 小新字体
	};
}
