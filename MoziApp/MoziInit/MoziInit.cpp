#include "mopch.h"
#include "Log.h"
#include "PathSet.h"



#include "MoziInit.h"



static MoziInit* mozi_init = NULL;

// 返回接口
MoziInit* GetMoziInitIo()
{
	return mozi_init;
}

// 格式初始化
static void StyleInit()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 12.0f;
	style.WindowRounding = 12.0f;
	style.ChildRounding = 12.0f;
	style.PopupRounding = 12.0f;
	style.ScrollbarRounding = 12.0f;
	style.TabRounding = 12.0f;
	style.GrabRounding = 12.0f;
	style.WindowMenuButtonPosition = ImGuiDir_None;
}


MoziInit::MoziInit()
{
	if (std::filesystem::exists(MzInitFilePath))
	{
		LOG_INFO("mozi初始化文件存在");

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
			else if(str == MoziInitEditorLevel)
			{
				current_first_level = MoziInitEditorLevel;
				continue;
			}

			if (current_first_level == MoziInitSettingLevel)
			{	
				// 检测是否要插入
				if (value_flag == true)
				{
					setting_map.insert({ current_level,stoi(str)});
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
	else
	{
		LOG_INFO("mozi初始化文件不存在，创建初始化文件");
		std::ofstream mzfile(MzInitFilePath, std::ios::out);

		LOG_INFO("初始化设置中......");
		setting_map.insert({ MoziInitAppTheme ,EnumSet::ThemeColor_Light });
		setting_map.insert({ MoziInitSetMarkdownEditorFonts ,TextEditorFonts_XiaoXing });
		setting_map.insert({ MoziInitSetMarkdownContentFonts ,MarkdownFonts_XiaoXing });
		LOG_INFO("初始化设置成功");
	}

	//for (auto iter = setting_map.begin(); iter != setting_map.end(); ++iter)
	//{
	//	std::cout << iter->first << "  " << iter->second << std::endl;
	//}

	mozi_init = this;
}

void MoziInit::WriteMzInitFile()
{
	LOG_INFO("写入初始化文件中......");
	std::ofstream mzfile(MzInitFilePath, std::ios::out);

	// 设置内容写入
	LOG_INFO("设置初始化内容写入中......");
	mzfile << MoziInitSettingLevel << "\n";
	for (auto iter = setting_map.begin(); iter != setting_map.end(); ++iter)
	{
		if (iter->second == 0)
			mzfile << iter->first << "\n";
		else
		{
			mzfile << iter->first << "\n" << iter->second << "\n";
		}
	}
	LOG_INFO("设置初始化内容写入完成");

	LOG_INFO("初始化内容写入完成");
}


MoziInit::~MoziInit()
{
	WriteMzInitFile();
}

void MoziInit::SetMoziInitAppTheme(const int& app_theme)
{
	setting_map[MoziInitAppTheme] = app_theme;
}

EnumSet::ThemeColor MoziInit::GetMoziInitAppTheme()
{
	int i = setting_map[MoziInitAppTheme];
	switch (i)
	{
	case 0:
		return EnumSet::ThemeColor_Light;
	case 1:
		return EnumSet::ThemeColor_Dark;
	case 2:
		return EnumSet::ThemeColor_Classic;
	}
}


void MoziInit::SetMoziInitMarkdownEditorFonts(const int& editor_fonts)
{
	setting_map[MoziInitSetMarkdownEditorFonts] = editor_fonts;
}

TextEditorFonts MoziInit::GetMoziInitMarkdownEditorFonts()
{
	int i = setting_map[MoziInitSetMarkdownEditorFonts];
	switch (i)
	{
	case 0:
		return TextEditorFonts_ZhoneHei;
	case 1:
		return TextEditorFonts_SimHei;
	case 2:
		return TextEditorFonts_XiaoXing;
	}
}	