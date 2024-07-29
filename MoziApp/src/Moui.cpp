#include "mopch.h"
#include "Moui.h"
#include "MoIcon.h"
#include "FileManage.h"


namespace Moui {

//-----------------------------------------------------------------------------
//                                  ȫ�ֲ���
//-----------------------------------------------------------------------------

    // �������
    static float current_scale = 0.f;           // ��ǰ��ģ�����ڴ���ͬdipʱ��ͬ�������С
    static ImFont* big_font = NULL;             // ���������ͼ�꣬���ڱ����������Ҫ�������ĵط�



    // �������
    static ThemeColor theme_color = Moui::ThemeColor_Light;                 // ҳ��Ĭ��ɫ
    const static int theme_color_num = 3;                             // ������ɫ������
    static bool* theme_color_register = new bool[theme_color_num]();  // ������ɫ�任����������ĳ����ɫ����ʱ������ɫ�±���Ϊ1����Ϊ0;

//-----------------------------------------------------------------------------
//                                  APP��ʼ��
//-----------------------------------------------------------------------------

    // ��ʽ��ʼ��
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

    // ������ɫ��ʼ��
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

    // MoziApp��ʼ������
    void MouiInit()
    {
        // UI��ʼ��
        Moui::ThemeColorInit(theme_color);
        Moui::StyleInit();
        //Moui::MzuiPopupStyleInit();
        //// ���ݿ�����
        //MzMysql::ConnectDatabase(mysql);
    }



    // ��̬DPI
    void DynamicDPI(GLFWwindow* window, ImGuiIO& io)
    {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (xscale != current_scale) {
            io.Fonts->Clear();                      // �����������
            //std::cout << xscale << std::endl;
            current_scale = xscale;
            /*************************** ��ʼ���� *****************************/
            float base_pixels = xscale * 16.0f;
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


            /*************************** ��һ������-����Ŵ� *****************************/
            float other_pixels = xscale * 35.0f;
            big_font = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, other_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
            config.GlyphMinAdvanceX = other_pixels * 2.0f / 2.3f;
            io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, other_pixels * 2.0f / 2.3f, &config, icons_ranges);
            io.Fonts->Build();              // ��������


            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }

    // ����ͼ��ͺ���
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

    // �ı�������ɫ
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