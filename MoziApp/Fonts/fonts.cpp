#include "mopch.h"

#include "PathSet.h"
#include "EnumSet.h"
#include "fonts.h"


#include "MoziInit.h"
namespace Fonts {


    // ������������
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
            //float base_pixels = xscale * 10.0f;
            float base_pixels = 20.0f;
            // �ϳ������������
            ImFontConfig config;
            config.MergeMode = true;    // ��������ϳ�
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
            io.FontGlobalScale = xscale*0.75f;

            // Ϊ�˱�֤���ظ���������
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

            //InitLoadFonts(xscale, io);                // ����������������
            InitSetFonts();                           // ��������

            // �ڼ���ڴ����ʱ��������������һ��Ҫ�ģ����ȥ��������л���ͬ�ֱ�����Ļʱ��ֱ�Ӻ���
            ImGui_ImplOpenGL3_DestroyFontsTexture();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }







}