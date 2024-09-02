#pragma once

#define MOZI_VERSION "0.1"

#define SOURSE_PAGE_NAME						u8"�ļ���Դ������"
#define DEBUG_DISPLAY_PAGE_NAME					u8"Debug��ʾҳ��"
#define TEXT_EDITOR_PAGE_NAME					u8"�ı��༭ҳ��"
#define MARKDOWN_DISPLAY_PAGE_NAME				u8"Markdown��ʾҳ��"


//extern int my_image_width;
//extern int my_image_height;
//extern GLuint my_image_texture;

namespace MoziPage {

	void MoziAppInit();
	void HomePage();
	// ���ú���
	static void SoursePage();
	static void DebugDisplayPage();
	static void TextEditorPage();
	static void MarkdownDisplayPage();

	static void DoubleClickedFile(const std::filesystem::path& current_path, std::filesystem::path& double_click_get_path);
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
