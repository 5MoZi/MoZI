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


    // 动态DPI
    void DynamicDPI(GLFWwindow* window, ImGuiIO& io)
    {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (xscale != current_scale)
        {
            // ************************imp************************* //
            // 这个xscale乘以的系数不能太大，目前测试超过30就会时app黑屏
            // 同时需要注意，一定不能压缩内存太大的字体，测试超过9M就会卡死

            io.Fonts->Clear();                      // 清楚所有字体
            current_scale = xscale;
            /*************************** 初始字体 *****************************/
            float base_pixels = xscale * 15.0f;
            // 合成字体参数设置
            ImFontConfig config;
            config.MergeMode = true;    // 开启字体合成
            config.GlyphExtraSpacing.y = 50.0f;
            // 图标相关参数设置
            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            config.PixelSnapH = true;
            config.GlyphMinAdvanceX = base_pixels * 2.0f / 2.3f;

            // 初始字体
            io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesDefault());
            // 汉字字体
            io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, base_pixels, &config, io.Fonts->GetGlyphRangesChineseFull());
            // 图标字体
            io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, base_pixels * 2.0f / 2.3f, &config, icons_ranges);

            io.Fonts->Build();              // 建立字体

            //InitMarkdownFonts(xscale, io);          // 加载markdown字体
            //InitTextEditorFonts(xscale, io);        // 加载文本编辑器字体
            InitFonts(xscale, io);

            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }







}