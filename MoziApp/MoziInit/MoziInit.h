#pragma once


#define MzInitFileName		"mozi.mzinit"			// 初始化文件名字	
#define MzInitFilePath		(PROJECT_PATH/MzInitFileName).generic_u8string().c_str()	// 初始化文件路径


// 层级标志
#define MoziInitSettingLevel			"*[A****]Setting"
#define MoziInitEditorLevel				"*[A****]Editor"

// 关键字定义
#define MoziInitAppTheme					"*[BV***]AppTheme"
#define MoziInitSetFonts					"*[B****]Fonts"
#define MoziInitSetTextEditorFonts			"*[CV***]TextEditorFonts"
#define MoziInitSetMarkdownContentFonts		"*[CV***]MarkdownContentFonts"
#define MoziInitSetMarkdownHeadFonts		"*[CV***]MarkdownHeadFonts"

//// UI主题颜色
//enum ThemeColor
//{
//	ThemeColor_Light = 0,		// 白色
//	ThemeColor_Dark = 1,		// 黑色
//	ThemeColor_Classic = 2	    // 经典紫色
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
	std::string current_first_level;							// 当前的第一层级
	std::map<std::string, int> setting_map;			// 设置存储器内容

};

MoziInit* GetMoziInitIo();
