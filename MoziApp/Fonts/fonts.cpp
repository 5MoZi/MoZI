#include "mopch.h"
#include "fonts.h"







namespace Fonts {
    static std::vector<ImFont*> all_fonts;
    static std::vector<ImFont*> text_editor_fonts;
    static std::vector<ImFont*> markdown_fonts;
    static float current_scale = 0;

    void InitFonts(const float& scale, ImGuiIO& io)
    {
        float fonts_size = scale * 20.0f;
        for (int i = 0; i < FontsNumbers; i++)
        {
            switch (i)
            {
            case AllFonts_Arial:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case AllFonts_BaseChinese:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case AllFonts_SimHei:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case AllFonts_XiaoXing:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_XIAOXING_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            }
        }
    }

    //void InitTextEditorFonts(const float& scale, ImGuiIO& io)
    //{
    //    float fonts_size = scale * 30.0f;
    //    for (int i = 0; i < TextEditorFontsNumbers; i++)
    //    {
    //        switch (i)
    //        {
    //        //case 0: 
    //        //    text_editor_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
    //        //    break;
    //        case 1:
    //            text_editor_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SONGTI_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
    //            break;
    //        }
    //    }
    //}

    //void InitMarkdownFonts(const float& scale, ImGuiIO& io)
    //{
    //    float fonts_size = scale * 20.0f;
    //    for (int i = 0; i < MarkdownFontsNumbers; i++)
    //    {
    //        switch (i)
    //        {
    //        case 0:
    //            markdown_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, fonts_size * 1.25, 0, io.Fonts->GetGlyphRangesChineseFull()));
    //            break;
    //        case 1:
    //            markdown_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
    //            break;
    //        case 2:
    //            markdown_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, fonts_size / 1.25, 0, io.Fonts->GetGlyphRangesChineseFull()));
    //            break;
    //        }
    //    }
    //}

    float* ReturnCurrentScale() { return &current_scale; }

    std::vector<ImFont*>* GetTextEditorFonts()
    {
        for (int i = 0; i < TextEditorFontsNumbers; i++)
        {
            switch (i)
            {
            case TextEditorFonts_ZhoneHei:
                text_editor_fonts.push_back(all_fonts[AllFonts_BaseChinese]);
                break;
            case TextEditorFonts_XiaoXing:
                text_editor_fonts.push_back(all_fonts[AllFonts_XiaoXing]);
                break;
            }
        }
        return &text_editor_fonts;
    }
    std::vector<ImFont*>* GetMarkdownFonts()
    {
        for (int i = 0; i < MarkdownFontsNumbers; i++)
        {
            switch (i)	
            {
            case MarkdownFonts_SimHei:
                markdown_fonts.push_back(all_fonts[AllFonts_SimHei]);
                break;
            case MarkdownFonts_XiaoXing:
                markdown_fonts.push_back(all_fonts[AllFonts_XiaoXing]);
                break;
            }
        }
        return &markdown_fonts;
    }


    // ��̬DPI
    void DynamicDPI(GLFWwindow* window, ImGuiIO& io)
    {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (xscale != current_scale)
        {
            // ************************imp************************* //
            // ���xscale���Ե�ϵ������̫��Ŀǰ���Գ���30�ͻ�ʱapp����
            // ͬʱ��Ҫע�⣬һ������ѹ���ڴ�̫������壬���Գ���9M�ͻῨ��

            io.Fonts->Clear();                      // �����������
            current_scale = xscale;
            /*************************** ��ʼ���� *****************************/
            float base_pixels = xscale * 15.0f;
            // �ϳ������������
            ImFontConfig config;
            config.MergeMode = true;    // ��������ϳ�
            config.GlyphExtraSpacing.y = 50.0f;
            // ͼ����ز�������
            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            config.PixelSnapH = true;
            config.GlyphMinAdvanceX = base_pixels * 2.0f / 2.3f;

            // ��ʼ����
            io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesDefault());
            // ��������
            io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, base_pixels, &config, io.Fonts->GetGlyphRangesChineseFull());
            // ͼ������
            io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, base_pixels * 2.0f / 2.3f, &config, icons_ranges);

            io.Fonts->Build();              // ��������

            //InitMarkdownFonts(xscale, io);          // ����markdown����
            //InitTextEditorFonts(xscale, io);        // �����ı��༭������
            InitFonts(xscale, io);

            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }







}