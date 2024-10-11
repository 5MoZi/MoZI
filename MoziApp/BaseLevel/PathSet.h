#pragma once


// 路径设置
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// 解决方案路径
#define PROJECT_PATH			std::filesystem::current_path()							// 项目路径

/*****字体路径*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()							// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()				// 中文字体
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"lib\\fonts\\simhei.ttf").generic_u8string().c_str()							// 黑体字体
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"lib\\fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// 联想小新字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif

#ifdef PUBLIC_VERSION
#define SOLUTION_PATH           std::filesystem::current_path()		// 解决方案路径

/*****字体路径*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"fonts\\arial.ttf").generic_u8string().c_str()				// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// 中文字体
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"fonts\\simhei.ttf").generic_u8string().c_str()							// 黑体字体
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// 联想小新字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif