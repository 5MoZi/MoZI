#pragma once


#define MzInitFileName		"mozi.mzinit"			// ��ʼ���ļ�����	
#define MzInitFilePath		(PROJECT_PATH/MzInitFileName).generic_u8string().c_str()	// ��ʼ���ļ�·��


// �������
#define ThemeColorNum 3					// ������ɫ������

// �㼶��־
#define MoziInitSettingLevel				"*[A****]Setting"
#define MoziInitEditorLevel					"*[A****]Editor"

// �ؼ��ֶ���
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

	void InitParameterSet();	// ��ʼ���������ã���mzfile�ļ�������ʱ����
	void WriteInitFile();		// ��Mzfileд������
	void ReadInitFile();		// ��Mzfile��ȡ����

	void SetInitFileTextEditorFonts(const int& editor_fonts);					// ����TextEditor��������
	void SetInitFileMarkdownContentFonts(const int& markdown_content_fonts);	// ����MarkdownContent��������
	void SetInitFileMarkdownHeadFonts(const int& markdown_head_fonts);			// ����MarkdownHead��������
	void SetInitFileAppTheme(const int& app_theme);								// ������������

	EnumSet::AllFonts GetInitFileTextEditorFonts();								// ���TextEditor��������
	EnumSet::AllFonts GetInitFileMarkdownContentFonts();						// ���MarkdownContent��������
	EnumSet::AllFonts GetInitFileMarkdownHeadFonts();							// ���MarkdownHead��������
	EnumSet::ThemeColor GetInitFileAppTheme();									// �����������



private:
	std::string current_first_level;											// ��ǰ�ĵ�һ�㼶
	std::map<std::string, int> setting_map;										// ���ô洢���洢��ʽ
						
};


// ��ȡ��ʼ���ӿں���
InitFile* GetInitFileIO();


