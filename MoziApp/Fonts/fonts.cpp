#include "mopch.h"
#include "fonts.h"







namespace Fonts {


    static std::vector<ImFont*> all_fonts;

    static SetFonts reset_fonts;
    static SetFonts all_fonts_set;

    static ImFont* text_editor_fonts;
    static ImFont* markdown_heading_fonts;
    static ImFont* markdown_content_fonts;

    static float current_scale = 0;
    void InitLoadFonts(const float& scale, ImGuiIO& io)
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
    void InitSetFonts()
    {
        all_fonts_set.SetTextEditorFont(all_fonts[AllFonts_XiaoXing]);
        all_fonts_set.SetMarkdownHeadFont(all_fonts[AllFonts_SimHei]);
        all_fonts_set.SetMarkdownContentFont(all_fonts[AllFonts_BaseChinese]);
    }

    float* ReturnCurrentScale() { return &current_scale; }

    ImFont* GetTextEditorFonts() { return all_fonts_set.GetTextEditorFont(); }
    ImFont* GetMarkdownHeadingFonts() { return all_fonts_set.GetMarkdownHeadFont(); }
    ImFont* GetMarkdownContentFonts() { return all_fonts_set.GetMarkdownContentFont(); }

    void SetTextEditorFont(const AllFonts& choice_font) { all_fonts_set.SetTextEditorFont(all_fonts[choice_font]); }
    void SetMarkdownHeadingFonts(const AllFonts& choice_font) { all_fonts_set.SetMarkdownHeadFont(all_fonts[choice_font]); }
    void SetMarkdownContentFonts(const AllFonts& choice_font) { all_fonts_set.SetMarkdownContentFont(all_fonts[choice_font]); }

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
            InitLoadFonts(xscale, io);
            InitSetFonts();
            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }







}