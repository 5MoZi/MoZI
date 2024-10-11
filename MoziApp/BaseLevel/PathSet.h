#pragma once


// ·������
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// �������·��
#define PROJECT_PATH			std::filesystem::current_path()							// ��Ŀ·��

/*****����·��*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()							// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()				// ��������
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"lib\\fonts\\simhei.ttf").generic_u8string().c_str()							// ��������
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"lib\\fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// ����С������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif

#ifdef PUBLIC_VERSION
#define SOLUTION_PATH           std::filesystem::current_path()		// �������·��

/*****����·��*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"fonts\\arial.ttf").generic_u8string().c_str()				// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// ��������
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"fonts\\simhei.ttf").generic_u8string().c_str()							// ��������
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// ����С������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif