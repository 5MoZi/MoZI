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
	// ��Ҫע���ʼ����˳���ȶ���־ϵͳ���г�ʼ����
	// Ȼ������ڽ�����ʼ������ִ�У���ʼ���ļ���ȡ����
	MoLog::Log::LogInit();						// ��־��ʼ��
	InitFile* mozi_file_init = new InitFile;	// ������ʼ���ļ����󣬻�ȡ��ʼ���ļ��ڵ����ݣ�Ϊ�˱�֤InitFileһֱ����ʹ��new���пռ����
	StyleInit();								// Ӧ�ø�ʽ��ʼ��
	ThemeColorInit();							// ����ɫ�ʳ�ʼ��
	Fonts::FontsInit();							// �����ʼ��

	mozi_app_init = this;
	LOG_INFO("MoziApp��ʼ�����");
}



// ��ʽ��ʼ��
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
	LOG_INFO("Ӧ�ø�ʽ��ʼ���������");
}

// ������ɫ��ʼ��
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
	LOG_INFO("App�����ʼ���������");
}
// ��ȡ������ɫ�Ĵ���ָ��
bool* MoziAppInit::GetThemeColorChanger()
{
	return theme_color_changer;
}

