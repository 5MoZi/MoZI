#pragma once

#define MOZI_VERSION "0.1"

#define SOURSE_PAGE_NAME	    u8"文件资源管理器"
#define PROPERTY_PAGE_NAME      u8"详情页面"
#define CONTENT_PAGE_NAME       u8"显示页面"

namespace MoziPage {


	void HomePage();
	// 内置函数
	static void SoursePage();


	// 主页菜单栏函数
	static void HomePageMainMenuBarFile();
	static void HomePageMainMenuBarSettings();
	static void HomePageMainMenuBarHelp();

	// 文件树建立函数
	static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag);
}
