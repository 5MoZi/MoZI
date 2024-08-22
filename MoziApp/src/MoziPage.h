#pragma once

#define MOZI_VERSION "0.1"

#define SOURSE_PAGE_NAME			u8"�ļ���Դ������"
#define DEBUG_DISPLAY_PAGE_NAME	    u8"debug��ʾҳ��"
#define PROPERTY_PAGE_NAME			u8"����ҳ��"
#define CONTENT_PAGE_NAME			u8"��ʾҳ��"

namespace MoziPage {

	void MoziAppInit();
	void HomePage();
	// ���ú���
	static void SoursePage();
	static void DebugDisplayPage();

	// ��ҳ�˵�������
	static void HomePageMainMenuBarFile();
	static void HomePageMainMenuBarSettings();
	static void HomePageMainMenuBarHelp();
	static void HomePageMainMenuBarWindows();
	static void HomePageHotKeys();
	// �ļ�����������
	static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag, const bool& bin_file_flag = 0);
	static void RightFolderPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path, const bool& bin_file_flag);
}
