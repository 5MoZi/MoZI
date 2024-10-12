// PL
#include "mopch.h"
#include "Log/Log.h"
#include "PathSet.h"


#include "InitFile.h"



static InitFile* mozi_init = NULL;

// 返回接口
InitFile* GetInitFileIO()
{
	return mozi_init;
}

InitFile::InitFile()
{
	if (std::filesystem::exists(MzInitFilePath))
	{
		LOG_INFO("mozi初始化文件存在");
		// 从mzfile文件中读取对应的内容用于初始化
		ReadInitFile();		
	}
	else
	{
		LOG_INFO("mozi初始化文件不存在，创建初始化文件");
		std::ofstream mzfile(MzInitFilePath, std::ios::out);
		// 初始化参数设置
		InitParameterSet();
	}
	mozi_init = this;
}

InitFile::~InitFile()
{
	// 应用结束后将初始化内容写入文件中
	WriteInitFile();
}

void InitFile::InitParameterSet()
{
	LOG_INFO("初始化参数设置中......");
	setting_map.insert({ MoziInitAppTheme ,EnumSet::ThemeColor_Light });
	setting_map.insert({ MoziInitSetTextEditorFonts ,EnumSet::AllFonts_XiaoXing });
	setting_map.insert({ MoziInitSetMarkdownContentFonts ,EnumSet::AllFonts_XiaoXing });
	setting_map.insert({ MoziInitSetMarkdownHeadFonts ,EnumSet::AllFonts_SimHei });
	LOG_INFO("初始化参数设置成功");
}

void InitFile::ReadInitFile()
{
	bool value_flag = false;
	std::string current_level;

	current_first_level = "";
	std::ifstream mzfile(MzInitFilePath);
	std::string str;
	LOG_INFO("读取mozi初始化文件内容中......");
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
			// 检测是否要插入
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

	LOG_INFO("读取mozi初始化文件内容成功");
}

void InitFile::WriteInitFile()
{
	LOG_INFO("写入初始化文件中......");
	std::ofstream mzfile(MzInitFilePath, std::ios::out);

	// 设置内容写入
	LOG_INFO("设置初始化内容写入中......");
	mzfile << MoziInitSettingLevel << "\n";
	for (auto iter = setting_map.begin(); iter != setting_map.end(); ++iter)
	{
		mzfile << iter->first << "\n" << iter->second << "\n";
	}
	LOG_INFO("设置初始化内容写入完成");

	LOG_INFO("初始化内容写入完成");

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
