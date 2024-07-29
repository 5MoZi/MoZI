#include "mopch.h"
#include "Moui.h"
#include "MoIcon.h"
#include "FileManage.h"


namespace Moui {

//-----------------------------------------------------------------------------
//                                  全局参数
//-----------------------------------------------------------------------------

    // 字体参数
    static float current_scale = 0.f;           // 当前规模，用于处理不同dip时不同的字体大小
    static ImFont* big_font = NULL;             // 大号字体与图标，用于标题或其他需要大号字体的地方



    // 主题参数
    static ThemeColor theme_color = Moui::ThemeColor_Light;                 // 页面默认色
    const static int theme_color_num = 3;                             // 主题颜色的数量
    static bool* theme_color_register = new bool[theme_color_num]();  // 主题颜色变换器：当启用某个颜色主题时，该颜色下标置为1其余为0;

//-----------------------------------------------------------------------------
//                                  APP初始化
//-----------------------------------------------------------------------------

    // 格式初始化
    static void StyleInit()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 12.0f;
        style.WindowRounding = 12.0f;
        style.ChildRounding = 12.0f;
        style.PopupRounding = 12.0f;
        style.ScrollbarRounding = 12.0f;
        style.TabRounding = 12.0f;
        style.GrabRounding = 12.0f;

        //style.CellPadding = ImVec2(30, 100);
        style.WindowMenuButtonPosition = ImGuiDir_None;
    }

    // 主题颜色初始化
    static void ThemeColorInit(const ThemeColor& theme_color)
    {
        switch (theme_color)
        {
        case ThemeColor_Light:
            theme_color_register[ThemeColor_Light] = true;
            ImGui::StyleColorsLight();
            break;
        case ThemeColor_Dark:
            theme_color_register[ThemeColor_Dark] = true;
            ImGui::StyleColorsDark();
            break;
        case ThemeColor_Classic:
            theme_color_register[ThemeColor_Classic] = true;
            ImGui::StyleColorsClassic();
            break;
        }
    }

    // MoziApp初始化程序
    void MouiInit()
    {
        // UI初始化
        Moui::ThemeColorInit(theme_color);
        Moui::StyleInit();
        //Moui::MzuiPopupStyleInit();
        //// 数据库连接
        //MzMysql::ConnectDatabase(mysql);
    }



    // 动态DPI
    void DynamicDPI(GLFWwindow* window, ImGuiIO& io)
    {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (xscale != current_scale) {
            io.Fonts->Clear();                      // 清楚所有字体
            //std::cout << xscale << std::endl;
            current_scale = xscale;
            /*************************** 初始字体 *****************************/
            float base_pixels = xscale * 16.0f;
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


            /*************************** 第一种字体-字体放大 *****************************/
            float other_pixels = xscale * 35.0f;
            big_font = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, other_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
            config.GlyphMinAdvanceX = other_pixels * 2.0f / 2.3f;
            io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, other_pixels * 2.0f / 2.3f, &config, icons_ranges);
            io.Fonts->Build();              // 建立字体


            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }

    // 连接图标和汉字
    const std::string IconAndChinese(const std::string str1, const std::string str2, const int type)
    {
        std::string temp_str;
        if (type == 0)
        {
            temp_str = "     " + str1 + " " + str2 + "       ";
        }
        else
        {
            temp_str = str1 + " " + str2 + "    ";
        }
        return temp_str;
    }

    // 改变主题颜色
    void ChangeThemeColor()
    {
        if (ImGui::MenuItem(HONEPAGENAME_SUBSETTINGS_LIGHT, NULL, &theme_color_register[ThemeColor_Light]))
        {
            memset(theme_color_register, 0, theme_color_num);
            ImGui::StyleColorsLight();
            theme_color_register[ThemeColor_Light] = true;
        }
        if (ImGui::MenuItem(HONEPAGENAME_SUBSETTINGS_DARK, NULL, &theme_color_register[ThemeColor_Dark]))
        {
            memset(theme_color_register, 0, theme_color_num);
            ImGui::StyleColorsDark();
            theme_color_register[ThemeColor_Dark] = true;
        }
        if (ImGui::MenuItem(HONEPAGENAME_SUBSETTINGS_CLASSIC, NULL, &theme_color_register[ThemeColor_Classic]))
        {
            memset(theme_color_register, 0, theme_color_num);
            ImGui::StyleColorsClassic();
            theme_color_register[ThemeColor_Classic] = true;
        }
    }


 
}