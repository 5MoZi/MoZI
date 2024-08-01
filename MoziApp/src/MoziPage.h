#pragma once

#define MOZI_VERSION "0.1"

#define SOURSE_PAGE_NAME	    u8"�ļ���Դ������"
#define PROPERTY_PAGE_NAME      u8"����ҳ��"
#define CONTENT_PAGE_NAME       u8"��ʾҳ��"

namespace MoziPage {


	void HomePage();
	// ���ú���
	static void SoursePage();


	// ��ҳ�˵�������
	static void HomePageMainMenuBarFile();
	static void HomePageMainMenuBarSettings();
	static void HomePageMainMenuBarHelp();

	// �ļ�����������
	static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag);
}
