// PL
#include "mopch.h"
#include "Log/Log.h"
#include "PathSet.h"


#include "InitFile.h"



static InitFile* mozi_init = NULL;

// ���ؽӿ�
InitFile* GetInitFileIO()
{
	return mozi_init;
}

InitFile::InitFile()
{
	if (std::filesystem::exists(MzInitFilePath))
	{
		LOG_INFO("mozi��ʼ���ļ�����");
		// ��mzfile�ļ��ж�ȡ��Ӧ���������ڳ�ʼ��
		ReadInitFile();		
	}
	else
	{
		LOG_INFO("mozi��ʼ���ļ������ڣ�������ʼ���ļ�");
		std::ofstream mzfile(MzInitFilePath, std::ios::out);
		// ��ʼ����������
		InitParameterSet();
	}
	mozi_init = this;
}

InitFile::~InitFile()
{
	// Ӧ�ý����󽫳�ʼ������д���ļ���
	WriteInitFile();
}

void InitFile::InitParameterSet()
{
	LOG_INFO("��ʼ������������......");
	setting_map.insert({ MoziInitAppTheme ,EnumSet::ThemeColor_Light });
	setting_map.insert({ MoziInitSetTextEditorFonts ,EnumSet::AllFonts_XiaoXing });
	setting_map.insert({ MoziInitSetMarkdownContentFonts ,EnumSet::AllFonts_XiaoXing });
	setting_map.insert({ MoziInitSetMarkdownHeadFonts ,EnumSet::AllFonts_SimHei });
	LOG_INFO("��ʼ���������óɹ�");
}

void InitFile::ReadInitFile()
{
	bool value_flag = false;
	std::string current_level;

	current_first_level = "";
	std::ifstream mzfile(MzInitFilePath);
	std::string str;
	LOG_INFO("��ȡmozi��ʼ���ļ�������......");
	while (getline(mzfile, str))
	{
		if (str == MoziInitSettingLevel)
		{
			current_first_level = MoziInitSettingLevel;
			continue;
		}
		else if (str == MoziInitEditorLevel)
		{
			current_first_level = MoziInitEditorLevel;
			continue;
		}

		if (current_first_level == MoziInitSettingLevel)
		{
			// ����Ƿ�Ҫ����
			if (value_flag == true)
			{
				setting_map.insert({ current_level,stoi(str) });
				value_flag = false;
				continue;
			}

			if (str.substr(3, 1) == "V")
			{
				current_level = str;
				value_flag = true;
				continue;
			}
			else
			{
				setting_map.insert({ str,0 });
				continue;
			}
		}
	}
	mzfile.close();

	LOG_INFO("��ȡmozi��ʼ���ļ����ݳɹ�");
}

void InitFile::WriteInitFile()
{
	LOG_INFO("д���ʼ���ļ���......");
	std::ofstream mzfile(MzInitFilePath, std::ios::out);

	// ��������д��
	LOG_INFO("���ó�ʼ������д����......");
	mzfile << MoziInitSettingLevel << "\n";
	for (auto iter = setting_map.begin(); iter != setting_map.end(); ++iter)
	{
		mzfile << iter->first << "\n" << iter->second << "\n";
	}
	LOG_INFO("���ó�ʼ������д�����");

	LOG_INFO("��ʼ������д�����");

}


void InitFile::SetInitFileAppTheme(const int& app_theme)
{
	setting_map[MoziInitAppTheme] = app_theme;
}

void InitFile::SetInitFileTextEditorFonts(const int& editor_fonts)
{
	setting_map[MoziInitSetTextEditorFonts] = editor_fonts;
}

void InitFile::SetInitFileMarkdownContentFonts(const int& markdown_content_fonts)
{
	setting_map[MoziInitSetMarkdownContentFonts] = markdown_content_fonts;
}

void InitFile::SetInitFileMarkdownHeadFonts(const int& markdown_head_fonts)
{
	setting_map[MoziInitSetMarkdownHeadFonts] = markdown_head_fonts;
}

EnumSet::ThemeColor InitFile::GetInitFileAppTheme()
{
	return EnumSet::ThemeColor(setting_map[MoziInitAppTheme]);
}

EnumSet::AllFonts InitFile::GetInitFileTextEditorFonts()
{
	return EnumSet::AllFonts(setting_map[MoziInitSetTextEditorFonts]);
}	

EnumSet::AllFonts InitFile::GetInitFileMarkdownContentFonts()
{
	return EnumSet::AllFonts(setting_map[MoziInitSetMarkdownContentFonts]);
}

EnumSet::AllFonts InitFile::GetInitFileMarkdownHeadFonts()
{
	return EnumSet::AllFonts(setting_map[MoziInitSetMarkdownHeadFonts]);
}
