#pragma once


#define MzInitFileName		"mozi.mzinit"			// ��ʼ���ļ�����	
#define MzInitFilePath		(PROJECT_PATH/MzInitFileName).generic_u8string().c_str()	// ��ʼ���ļ�·��


// �㼶��־
#define MoziInitSettingLevel			"*[A****]Setting"
#define MoziInitEditorLevel				"*[A****]Editor"

// �ؼ��ֶ���
#define MoziInitAppTheme					"*[BV***]AppTheme"
#define MoziInitSetFonts					"*[B****]Fonts"
#define MoziInitSetTextEditorFonts			"*[CV***]TextEditorFonts"
#define MoziInitSetMarkdownContentFonts		"*[CV***]MarkdownContentFonts"
#define MoziInitSetMarkdownHeadFonts		"*[CV***]MarkdownHeadFonts"

//// UI������ɫ
//enum ThemeColor
//{
//	ThemeColor_Light = 0,		// ��ɫ
//	ThemeColor_Dark = 1,		// ��ɫ
//	ThemeColor_Classic = 2	    // ������ɫ
//};

#include "EnumSet.h"

class MoziInit
{
public:

	MoziInit();
	~MoziInit();

	void WriteMzInitFile();

	void SetMoziInitTextEditorFonts(const int& editor_fonts);
	void SetMoziInitMarkdownContentFonts(const int& markdown_content_fonts);
	void SetMoziInitMarkdownHeadFonts(const int& markdown_head_fonts);
	void SetMoziInitAppTheme(const int& app_theme);

	EnumSet::AllFonts GetMoziInitTextEditorFonts();
	EnumSet::AllFonts GetMoziInitMarkdownContentFonts();
	EnumSet::AllFonts GetMoziInitMarkdownHeadFonts();
	EnumSet::ThemeColor GetMoziInitAppTheme();

private:
	std::string current_first_level;							// ��ǰ�ĵ�һ�㼶
	std::map<std::string, int> setting_map;			// ���ô洢������

};

MoziInit* GetMoziInitIo();
