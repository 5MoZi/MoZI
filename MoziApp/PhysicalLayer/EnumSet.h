#pragma once



namespace EnumSet
{
	// UI������ɫ
	enum ThemeColor
	{
		ThemeColor_Light = 0,		// ��ɫ
		ThemeColor_Dark = 1,		// ��ɫ
		ThemeColor_Classic = 2	    // ������ɫ
	};

	// �ı��༭����
	enum  TextEditorFonts
	{
		TextEditorFonts_ZhoneHei = 0,	// ��������
		TextEditorFonts_SimHei = 1,		// ����
		TextEditorFonts_XiaoXing = 2	// С������
	};

	// Markdown��ʾ����
	enum  MarkdownFonts
	{
		MarkdownFonts_ZhoneHei = 0,		// ��������
		MarkdownFonts_SimHei = 1,		// ����
		MarkdownFonts_XiaoXing = 2,		// С������
	};

	enum AllFonts
	{
		AllFonts_Arial = 0,				// Ӣ��
		AllFonts_Song = 1,				// ����
		AllFonts_SimHei = 2,			// ����
		AllFonts_XiaoXing = 3			// С������
	};
}
