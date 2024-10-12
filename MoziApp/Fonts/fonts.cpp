// PL
#include "mopch.h"
#include "PathSet.h"
#include "EnumSet.h"
#include "Log/Log.h"

// LL
#include "InitFile/InitFile.h"


#include "fonts.h"
namespace Fonts {


    // ������������
    static std::vector<ImFont*> all_fonts_memory;

    static FontSet all_fonts_set;

    static float current_scale = 0;

    FontSet* GetFontSetIO() { return &all_fonts_set; }
    float* ReturnCurrentScale() { return &current_scale; }

    ImFont* GetTextEditorFonts() { return all_fonts_set.TextEditorFont; }
    ImFont* GetMarkdownHeadFonts() { return all_fonts_set.MarkdownHeadFont; }
    ImFont* GetMarkdownContentFonts() { return all_fonts_set.MarkdownContentFont; }

    void SetTextEditorFont(const EnumSet::AllFonts& choice_font) { all_fonts_set.TextEditorFont = all_fonts_memory[choice_font]; }
    void SetMarkdownHeadFonts(const EnumSet::AllFonts& choice_font) { all_fonts_set.MarkdownHeadFont = all_fonts_memory[choice_font]; }
    void SetMarkdownContentFonts(const EnumSet::AllFonts& choice_font) { all_fonts_set.MarkdownContentFont = all_fonts_memory[choice_font]; }

    static void FontSetInit()
    {
        InitFile* init_file = GetInitFileIO();
        EnumSet::AllFonts text_editor_fonts = init_file->GetInitFileTextEditorFonts();
        EnumSet::AllFonts markdown_head_fonts = init_file->GetInitFileMarkdownHeadFonts();
        EnumSet::AllFonts markdown_content_fonts = init_file->GetInitFileMarkdownContentFonts();

        all_fonts_set.TextEditorFont = all_fonts_memory[text_editor_fonts];
        all_fonts_set.MarkdownHeadFont = all_fonts_memory[markdown_head_fonts];
        all_fonts_set.MarkdownContentFont = all_fonts_memory[markdown_content_fonts];
    }
    void FontsInit()
    {
        ImGuiIO& io = ImGui::GetIO();
        /*************************** ��ʼ���� *****************************/
        float base_pixels = FontDPI;
        // �ϳ������������
        ImFontConfig config;
        config.MergeMode = true;                // ��������ϳ�
        config.GlyphExtraSpacing.y = 50.0f;
        // ͼ����ز�������
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        config.PixelSnapH = true;
        config.GlyphMinAdvanceX = base_pixels * 2.0f / 2.3f;

        // ��ʼ����
        ImFont* a1 = io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesDefault());
        // ��������
        ImFont* a2 = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, base_pixels, &config, io.Fonts->GetGlyphRangesChineseFull());
        // ͼ������
        ImFont* a3 = io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, base_pixels * 2.0f / 2.3f, &config, icons_ranges);

        io.Fonts->Build();              // ��������

        // ��Ҫ��base_pixels����������Ĵ�С��base_pixels�����������ľ�ϸ�̶ȣ�dpi����
        // ��io.FontGlobalScale����������Ĵ�С
        //io.FontGlobalScale = xscale * 0.75f;

        // Ϊ�˱�֤���ظ���������
        all_fonts_memory.clear();
        for (int i = 0; i < FontsNumbers; i++)
        {
            switch (i)
            {
            case EnumSet::AllFonts_Arial:
                all_fonts_memory.push_back(a1);
                break;
            case EnumSet::AllFonts_Song:
                all_fonts_memory.push_back(a2);
                break;
            case EnumSet::AllFonts_SimHei:
                all_fonts_memory.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case EnumSet::AllFonts_XiaoXing:
                all_fonts_memory.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_XIAOXING_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            }
        }

        FontSetInit();                           // ��������
        LOG_INFO("�����ʼ���������");
    }

    void DynamicFontsSize(GLFWwindow* window, ImGuiIO& io)
    {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (current_scale != xscale)
        {
            io.FontGlobalScale = xscale * 0.75f;
            current_scale = xscale;
        }
    }


  


}