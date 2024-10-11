#pragma once

#include "EnumSet.h"
#include <unordered_map>
#define MzInitFileName		"mozi.mzinit"			// ��ʼ���ļ�����	
#define MzInitFilePath		(PROJECT_PATH/MzInitFileName).generic_u8string().c_str()	// ��ʼ���ļ�·��


// �㼶��־
#define MoziInitSettingLevel			"*[A****]Setting"
#define MoziInitEditorLevel				"*[A****]Editor"

// �ؼ��ֶ���
#define MoziInitAppTheme					"*[BV***]AppTheme"
#define MoziInitSetFonts					"*[B****]Fonts"
#define MoziInitSetMarkdownEditorFonts		"*[CV***]MarkdownEditorFonts"
#define MoziInitSetMarkdownContentFonts		"*[CV***]MarkdownContentFonts"


//// UI������ɫ
//enum ThemeColor
//{
//	ThemeColor_Light = 0,		// ��ɫ
//	ThemeColor_Dark = 1,		// ��ɫ
//	ThemeColor_Classic = 2	    // ������ɫ
//};

class MoziInit
{
public:

	MoziInit();
	~MoziInit();

	void WriteMzInitFile();


	void SetMoziInitMarkdownEditorFonts(const int& editor_fonts);
	void SetMoziInitMarkdownContentFonts();
	void SetMoziInitAppTheme(const int& app_theme);


	TextEditorFonts GetMoziInitMarkdownEditorFonts();
	int GetMoziInitMarkdownContentFonts();
	EnumSet::ThemeColor GetMoziInitAppTheme();

private:
	std::string current_first_level;							// ��ǰ�ĵ�һ�㼶
	std::unordered_map<std::string, int> setting_map;			// ���ô洢������

};

MoziInit* GetMoziInitIo();
