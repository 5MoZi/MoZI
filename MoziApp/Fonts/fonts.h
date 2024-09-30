#pragma once


/*****解决方案与项目路径*****/
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// 解决方案路径

/*****字体路径*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()							// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()				// 中文字体
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"lib\\fonts\\simhei.ttf").generic_u8string().c_str()							// 黑体字体
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"lib\\fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// 联想小新字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()
#endif


#ifdef PUBLIC_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path().parent_path()			// 解决方案路径

#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// 中文字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif

#define FontsNumbers				4
#define TextEditorFontsNumbers		2
#define MarkdownFontsNumbers		1
namespace Fonts {


	enum AllFonts
	{
		AllFonts_Arial = 0,
		AllFonts_BaseChinese = 1,
		AllFonts_SimHei = 2,
		AllFonts_XiaoXing = 3
	};

	enum  TextEditorFonts
	{
		TextEditorFonts_ZhoneHei = 0,	//基本字体
		TextEditorFonts_XiaoXing = 1			//楷体
	};

	enum  MarkdownFonts
	{
		MarkdownFonts_SimHei = 0,
		MarkdownFonts_XiaoXing = 1,
	};

	void DynamicDPI(GLFWwindow* window, ImGuiIO& io);
	float* ReturnCurrentScale();

	std::vector<ImFont*>* GetTextEditorFonts();
	std::vector<ImFont*>* GetMarkdownFonts();

}
