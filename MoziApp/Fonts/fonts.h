#pragma once


/*****�����������Ŀ·��*****/
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// �������·��

/*****����·��*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()							// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()				// ��������
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"lib\\fonts\\simhei.ttf").generic_u8string().c_str()							// ��������
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"lib\\fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// ����С������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()
#endif


#ifdef PUBLIC_VERSION
//#define SOLUTION_PATH           std::filesystem::current_path().parent_path().parent_path()			// �������·��
#define SOLUTION_PATH				std::filesystem::current_path()

#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"fonts\\arial.ttf").generic_u8string().c_str()				// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// ��������
#define FONT_CHINESE_SIMHEI_PATH	(SOLUTION_PATH/"fonts\\simhei.ttf").generic_u8string().c_str()							// ��������
#define FONT_CHINESE_XIAOXING_PATH	(SOLUTION_PATH/"fonts\\TextEditorFonts\\xiaoxing.ttf").generic_u8string().c_str()		// ����С������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif

#define FontsNumbers				4
#define TextEditorFontsNumbers		2
#define MarkdownFontsNumbers		1


#define FontNameHeiTi		u8"����"
#define FontNameZhengHei	u8"����"
#define FontNameXiaoXing	u8"С������"

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
		TextEditorFonts_ZhoneHei = 0,	//��������
		TextEditorFonts_XiaoXing = 1			//����
	};

	enum  MarkdownFonts
	{
		MarkdownFonts_SimHei = 0,
		MarkdownFonts_XiaoXing = 1,
	};

	class SetFonts
	{
	public:
		inline void SetTextEditorFont(ImFont* c_TextEditorFont) { TextEditorFont = c_TextEditorFont; }
		inline void SetMarkdownHeadFont(ImFont* c_MarkdownHeadFont) { MarkdownHeadFont = c_MarkdownHeadFont; }
		inline void SetMarkdownContentFont(ImFont* c_MarkdownContentFont) { MarkdownContentFont = c_MarkdownContentFont; }


		inline ImFont* GetTextEditorFont() { return TextEditorFont; }
		inline ImFont* GetMarkdownHeadFont() { return MarkdownHeadFont; }
		inline ImFont* GetMarkdownContentFont() { return MarkdownContentFont; }

	private:
		ImFont* WindowsFont;
		ImFont* TextEditorFont;

		ImFont* MarkdownHeadFont;
		ImFont* MarkdownContentFont;
	};



	void DynamicDPI(GLFWwindow* window, ImGuiIO& io);
	float* ReturnCurrentScale();


	ImFont* GetTextEditorFonts();
	ImFont* GetMarkdownHeadingFonts();
	ImFont* GetMarkdownContentFonts();

	void SetMarkdownHeadingFonts(const AllFonts& choice_font);
	void SetMarkdownContentFonts(const AllFonts& choice_font);
	void SetTextEditorFont(const AllFonts& choice_font);

}
