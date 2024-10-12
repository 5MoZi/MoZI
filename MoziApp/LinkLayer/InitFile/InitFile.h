#pragma once


#define MzInitFileName		"mozi.mzinit"			// 初始化文件名字	
#define MzInitFilePath		(PROJECT_PATH/MzInitFileName).generic_u8string().c_str()	// 初始化文件路径


// 主题相关
#define ThemeColorNum 3					// 主题颜色的数量

// 层级标志
#define MoziInitSettingLevel				"*[A****]Setting"
#define MoziInitEditorLevel					"*[A****]Editor"

// 关键字定义
#define MoziInitAppTheme					"*[BV***]AppTheme"
#define MoziInitSetTextEditorFonts			"*[CV***]TextEditorFonts"
#define MoziInitSetMarkdownContentFonts		"*[CV***]MarkdownContentFonts"
#define MoziInitSetMarkdownHeadFonts		"*[CV***]MarkdownHeadFonts"


#include "EnumSet.h"

class InitFile
{
public:

	InitFile();
	~InitFile();

	void InitParameterSet();	// 初始化参数设置，在mzfile文件不存在时设置
	void WriteInitFile();		// 向Mzfile写入内容
	void ReadInitFile();		// 从Mzfile读取内容

	void SetInitFileTextEditorFonts(const int& editor_fonts);					// 设置TextEditor字体类型
	void SetInitFileMarkdownContentFonts(const int& markdown_content_fonts);	// 设置MarkdownContent字体类型
	void SetInitFileMarkdownHeadFonts(const int& markdown_head_fonts);			// 设置MarkdownHead字体类型
	void SetInitFileAppTheme(const int& app_theme);								// 设置主题类型

	EnumSet::AllFonts GetInitFileTextEditorFonts();								// 获得TextEditor字体类型
	EnumSet::AllFonts GetInitFileMarkdownContentFonts();						// 获得MarkdownContent字体类型
	EnumSet::AllFonts GetInitFileMarkdownHeadFonts();							// 获得MarkdownHead字体类型
	EnumSet::ThemeColor GetInitFileAppTheme();									// 获得主题类型



private:
	std::string current_first_level;											// 当前的第一层级
	std::map<std::string, int> setting_map;										// 设置存储器存储格式
						
};


// 获取初始化接口函数
InitFile* GetInitFileIO();


