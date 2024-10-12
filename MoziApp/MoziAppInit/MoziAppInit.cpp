#include "mopch.h"
#include "MoziAppInit.h"

#include "InitFile/InitFile.h"
#include "Fonts/fonts.h"
#include "Log/Log.h"


static MoziAppInit* mozi_app_init = NULL;

MoziAppInit* GetMoziAppInitIO()
{
	return mozi_app_init;
}


MoziAppInit::MoziAppInit()
{
	// 需要注意初始化的顺序，先对日志系统进行初始化，
	// 然后接着在建立初始化对象执行，初始化文件读取操作
	MoLog::Log::LogInit();						// 日志初始化
	InitFile* mozi_file_init = new InitFile;	// 建立初始化文件对象，获取初始化文件内的内容，为了保证InitFile一直存在使用new进行空间分配
	StyleInit();								// 应用格式初始化
	ThemeColorInit();							// 主题色彩初始化
	Fonts::FontsInit();							// 字体初始化

	mozi_app_init = this;
	LOG_INFO("MoziApp初始化完成");
}



// 格式初始化
void MoziAppInit::StyleInit()
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
	LOG_INFO("应用格式初始化设置完成");
}

// 主题颜色初始化
void MoziAppInit::ThemeColorInit()
{
	InitFile* init_file = GetInitFileIO();
	EnumSet::ThemeColor theme_color = init_file->GetInitFileAppTheme();
	switch (theme_color)
	{
	case EnumSet::ThemeColor_Light:
		theme_color_changer[EnumSet::ThemeColor_Light] = true;
		ImGui::StyleColorsLight();
		break;
	case EnumSet::ThemeColor_Dark:
		theme_color_changer[EnumSet::ThemeColor_Dark] = true;
		ImGui::StyleColorsDark();
		break;
	case EnumSet::ThemeColor_Classic:
		theme_color_changer[EnumSet::ThemeColor_Classic] = true;
		ImGui::StyleColorsClassic();
		break;
	}
	LOG_INFO("App主题初始化设置完成");
}
// 获取主题颜色寄存器指针
bool* MoziAppInit::GetThemeColorChanger()
{
	return theme_color_changer;
}

