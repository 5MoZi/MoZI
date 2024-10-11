#include "mopch.h"

#include "PathSet.h"
#include "EnumSet.h"
#include "fonts.h"


#include "MoziInit.h"
namespace Fonts {


    // 保存所有字体
    static std::vector<ImFont*> all_fonts;

    static SetFonts all_fonts_set;

    static float current_scale = 0;


    void InitLoadFonts(const float& scale, ImGuiIO& io)
    {
        all_fonts.clear();
        float fonts_size = 20.0f;
        for (int i = 0; i < FontsNumbers; i++)
        {
            switch (i)
            {
            case EnumSet::AllFonts_Arial:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case EnumSet::AllFonts_Song:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case EnumSet::AllFonts_SimHei:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            case EnumSet::AllFonts_XiaoXing:
                all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_XIAOXING_PATH, fonts_size, 0, io.Fonts->GetGlyphRangesChineseFull()));
                break;
            }
        }
    }

    void InitSetFonts()
    {
        MoziInit* mozi_init = GetMoziInitIo();
        EnumSet::AllFonts text_editor_fonts = mozi_init->GetMoziInitTextEditorFonts();
        EnumSet::AllFonts markdown_content_fonts = mozi_init->GetMoziInitMarkdownContentFonts();
        EnumSet::AllFonts markdown_head_fonts = mozi_init->GetMoziInitMarkdownHeadFonts();

        all_fonts_set.SetTextEditorFont(all_fonts[text_editor_fonts]);
        all_fonts_set.SetMarkdownContentFont(all_fonts[markdown_content_fonts]);
        all_fonts_set.SetMarkdownHeadFont(all_fonts[markdown_head_fonts]);
    }

    float* ReturnCurrentScale() { return &current_scale; }

    ImFont* GetTextEditorFonts() { return all_fonts_set.GetTextEditorFont(); }
    ImFont* GetMarkdownHeadingFonts() { return all_fonts_set.GetMarkdownHeadFont(); }
    ImFont* GetMarkdownContentFonts() { return all_fonts_set.GetMarkdownContentFont(); }

    void SetTextEditorFont(const EnumSet::AllFonts& choice_font) { all_fonts_set.SetTextEditorFont(all_fonts[choice_font]); }
    void SetMarkdownHeadingFonts(const EnumSet::AllFonts& choice_font) { all_fonts_set.SetMarkdownHeadFont(all_fonts[choice_font]); }
    void SetMarkdownContentFonts(const EnumSet::AllFonts& choice_font) { all_fonts_set.SetMarkdownContentFont(all_fonts[choice_font]); }

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
            //float base_pixels = xscale * 10.0f;
            float base_pixels = 20.0f;
            // 合成字体参数设置
            ImFontConfig config;
            config.MergeMode = true;    // 开启字体合成
            config.GlyphExtraSpacing.y = 50.0f;
            // 图标相关参数设置
            static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
            config.PixelSnapH = true;
            config.GlyphMinAdvanceX = base_pixels * 2.0f / 2.3f;

            // 初始字体
            ImFont* a1 = io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesDefault());
            // 汉字字体
            ImFont* a2 = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, base_pixels, &config, io.Fonts->GetGlyphRangesChineseFull());
            // 图标字体
            ImFont* a3 = io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, base_pixels * 2.0f / 2.3f, &config, icons_ranges);

            io.Fonts->Build();              // 建立字体

            // 不要用base_pixels来调节字体的大小，base_pixels用与调节字体的精细程度（dpi），
            // 用io.FontGlobalScale来调节字体的大小
            io.FontGlobalScale = xscale*0.75f;

            // 为了保证不重复加载字体
            all_fonts.clear();
            for (int i = 0; i < FontsNumbers; i++)
            {
                switch (i)
                {
                case EnumSet::AllFonts_Arial:
                    all_fonts.push_back(a1);
                    break;
                case EnumSet::AllFonts_Song:
                    all_fonts.push_back(a2);
                    break;
                case EnumSet::AllFonts_SimHei:
                    all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesChineseFull()));
                    break;
                case EnumSet::AllFonts_XiaoXing:
                    all_fonts.push_back(io.Fonts->AddFontFromFileTTF(FONT_CHINESE_XIAOXING_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesChineseFull()));
                    break;
                }
            }

            //InitLoadFonts(xscale, io);                // 加载其他类型字体
            InitSetFonts();                           // 保存字体

            // 在检测内存过大时，下面这两行是一定要的，如果去除则会在切换不同分辨率屏幕时，直接黑屏
            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }







}