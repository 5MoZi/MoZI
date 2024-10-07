#include "mopch.h"
#include "MoIcon.h"
#include "Log.h"
#include "FileOperate.h"
#include "MoObject.h"
#include "MysqlOperate.h"
#include "Moui.h"
#include "FileManage.h"

#include "Markdown.h"

#include "fonts.h"

namespace Moui {

//-----------------------------------------------------------------------------
//                                  ȫ�ֲ���
//-----------------------------------------------------------------------------

    // �������
    static float current_scale = 0.f;           // ��ǰ��ģ�����ڴ���ͬdipʱ��ͬ�������С
    static const char* text_editor_fonts_name[] = { FontNameZhengHei, FontNameHeiTi ,FontNameXiaoXing };
    static const char* markdown_fonts_heading_name[] = { FontNameHeiTi };
    static const char* markdown_fonts_content_name[] = { FontNameZhengHei,FontNameHeiTi ,FontNameXiaoXing };

    // �������
    static ThemeColor theme_color = Moui::ThemeColor_Light;                 // ҳ��Ĭ��ɫ
    const static int theme_color_num = 3;                             // ������ɫ������
    static bool* theme_color_register = new bool[theme_color_num]();  // ������ɫ�任����������ĳ����ɫ����ʱ������ɫ�±���Ϊ1����Ϊ0;

    // moui��ʽ����
    //static MoObject::MouiPopupStyle popup_base_style;

    static std::string set_fonts_name[2] = { u8"�ı��༭��",u8"Markdown" };
    static std::string sub_set_fonts_name_markdown[2] = { u8"����",u8"����" };


    // ��������ҳ��
    void SetFontsPopup(bool* open_popup)
    {
        static std::string current_choice = { 0 };
        static bool ones_open_popup = true;
        ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
        if (ones_open_popup)
        {
            ImGui::OpenPopup(u8"��������"); // ��������
            ones_open_popup = false;
        }

        if (ImGui::BeginPopupModal(u8"��������", open_popup, ImGuiWindowFlags_NoCollapse))   // �򿪵���
        {

            // ѡ���
            ImGui::BeginChild(u8"ѡ��", ImVec2(200, 0), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX);
            for (int first_uid = 0; first_uid < 2; first_uid++)
            {
                bool node_open = ImGui::TreeNode(set_fonts_name[first_uid].c_str());
                if (ImGui::IsItemClicked(ImGuiMouseButton_Left))current_choice = set_fonts_name[first_uid];
                if (node_open)
                {

                    switch (first_uid)
                    {
                    case 0:
                        break;
                    case 1:
                        for (int second_uid = 0; second_uid < 2; second_uid++)
                        {
                            ImGui::TreeNodeEx(sub_set_fonts_name_markdown[second_uid].c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
                            if (ImGui::IsItemClicked(ImGuiMouseButton_Left))current_choice = sub_set_fonts_name_markdown[second_uid];
                        }
                        break;
                    }
                    ImGui::TreePop();
                }
            }

            ImGui::EndChild();
            ImGui::SameLine();

            // ���ݲ���
            ImGui::BeginChild(u8"����",ImVec2(-FLT_MIN, -FLT_MIN), ImGuiChildFlags_Border);

            if (current_choice == set_fonts_name[0])
            {
                static int item_current = 0;
                ImGui::TextWrapped(u8"����");
                ImGui::Combo("##�ı��༭������", &item_current, text_editor_fonts_name, IM_ARRAYSIZE(text_editor_fonts_name));

                if(item_current==0)
                    Fonts::SetTextEditorFont(Fonts::AllFonts_BaseChinese);
                else if (item_current == 1)
                    Fonts::SetTextEditorFont(Fonts::AllFonts_SimHei);
                else if (item_current == 2)
                    Fonts::SetTextEditorFont(Fonts::AllFonts_XiaoXing);
            }
            else if (current_choice == sub_set_fonts_name_markdown[0])
            {
                static int item_current = 0;
                ImGui::TextWrapped(u8"��������");
                ImGui::Combo("##Markdown��������", &item_current, markdown_fonts_heading_name, IM_ARRAYSIZE(markdown_fonts_heading_name));

                if (item_current == 0)
                    Fonts::SetMarkdownHeadingFonts(Fonts::AllFonts_SimHei);
                //else if (item_current == 1)
                //    Fonts::SetMarkdownHeadingFonts(Fonts::AllFonts_XiaoXing);
            }
            else if (current_choice == sub_set_fonts_name_markdown[1])
            {
                static int item_current = 0;
                ImGui::TextWrapped(u8"��������");
                ImGui::Combo("##Markdown��������", &item_current, markdown_fonts_content_name, IM_ARRAYSIZE(markdown_fonts_content_name));

                if (item_current == 0)
                    Fonts::SetMarkdownContentFonts(Fonts::AllFonts_BaseChinese);
                else if (item_current == 1)
                    Fonts::SetMarkdownContentFonts(Fonts::AllFonts_SimHei);
                else if (item_current == 2)
                    Fonts::SetMarkdownContentFonts(Fonts::AllFonts_XiaoXing);
            }

            ImGui::EndChild();

            ImGui::EndPopup();
        }

        // ֱ�ӹرյ�����ǵ��������ͻ�ԭ��־λ
        if (*open_popup == false)
        {
            ones_open_popup = true;
            current_choice = "";
        }
    }

    static void SubSetFonts_TextEditorFonts_Content()
    {

    }
    static void SubSetFonts_MarkdownFonts_Content()
    {


    }
    static void ShowPlaceholderObject(const char* prefix, int uid)
    {
        // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
        ImGui::PushID(uid);

        // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("my sailor is rich");

        if (node_open)
        {
            static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
            for (int i = 0; i < 8; i++)
            {
                ImGui::PushID(i); // Use field index as identifier.
                if (i < 2)
                {
                    ShowPlaceholderObject("Child", 424242);
                }
                else
                {
                    // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                    ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (i >= 5)
                        ImGui::InputFloat("##value", &placeholder_members[i], 1.0f);
                    else
                        ImGui::DragFloat("##value", &placeholder_members[i], 0.01f);
                    ImGui::NextColumn();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
    
//-----------------------------------------------------------------------------
//                                  ���뺯��
//-----------------------------------------------------------------------------
    // һ����������һ���̶�һ������  
    static float DoubleElementSimpleWrite(const std::string& text_name, char* buff, const size_t buff_size,
        const float& pos_x, const float& space, const float& input_width, const char* label, const float& space_high = 0, const bool& enable_high = 0)
    {
        // һ��������������ǰ��Ԫ�صļ��
        if (enable_high)
            ImGui::Dummy(ImVec2(0, space_high));

        ImGui::SetCursorPosX(pos_x);
        ImGui::Text(text_name.c_str());
        ImGui::SameLine();
        float text_name1_width = ImGui::CalcTextSize(text_name.c_str()).x;
        ImGui::SetCursorPosX(pos_x + text_name1_width + space);
        ImGui::SetNextItemWidth(input_width);
        ImGui::InputText(label, buff, buff_size);
        return pos_x + text_name1_width + space;
    }
//-----------------------------------------------------------------------------
//                                  ��ʾ��������
//-----------------------------------------------------------------------------    
    // ��ʾ����
    void RemindPopup(const char* popup_name, const std::string& text, bool& confirm_flag, const float& current_scale)
    {
        /*-------------------------������Ϣ����----------------------------*/
        //static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        //scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 500, ImVec2(50, 30), ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(120, 0));
        // ������Ϣ
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;
        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }
        /*-------------------------����λ�����С����----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // ��С������С����
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_hight*0.8));
            flag_ones = true;
        }

        /*-------------------------�������ݲ���----------------------------*/
        //, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
        if (ImGui::BeginPopupModal(popup_name, NULL))  // �򿪵���
        {
            //// ������С�Ĵ�СΪ1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С
            if (win_h < window_hight*0.8 || win_w < window_width*0.8)flag_ones = false;
            // ��ʾ��������
            ImGui::TextWrapped(text.c_str());

            // �༭ȷ�ϰ���
            ImGui::SetCursorPos(ImVec2(win_w - button_size.x - element_pos.x, win_h - element_pos.y * 2));
            if (ImGui::Button(u8"ȷ��", button_size))
            {
                confirm_flag = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetCursorPos(ImVec2(button_size.x - element_pos.x*1.5, win_h - element_pos.y * 2));
            if (ImGui::Button(u8"ȡ��", button_size) || ImGui::IsKeyPressed((ImGuiKey_Delete, ImGuiKey_Escape), u8"RemindPopup-ȡ��"))
            {
                confirm_flag = false;;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }

    //// ���󴰿�
    //bool ErrorPopup(const std::string& text, bool& confirm_flag, const float& current_scale)
    //{
    //    // ��������һ�α�־λ
    //    static bool ones_open_popup = true;

    //    /*-------------------------������Ϣ����----------------------------*/
    //    static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
    //    // ������Ϣ
    //    static float window_hight = current_popup_style.window_hight;
    //    static float window_width = current_popup_style.window_width;
    //    // ���õ����ĳߴ��ȷ����ť�Ĵ�С
    //    static ImVec2 element_pos = current_popup_style.element_pos;
    //    static ImVec2 element_space = current_popup_style.element_space;
    //    static ImVec2 bar_size = current_popup_style.bar_size;
    //    static ImVec2 button_size = current_popup_style.button_size;
    //    // ��ͬ��ģ�仯��ͬ�ĳߴ�
    //    static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
    //    if (next_scale != current_scale && current_scale != current_popup_style.scale)
    //    {

    //        next_scale = current_scale;
    //        current_popup_style.ChangeStlyeSize(current_scale);

    //        window_hight = current_popup_style.window_hight;
    //        window_width = current_popup_style.window_width;
    //        element_pos = current_popup_style.element_pos;         // Ԫ��λ��
    //        element_space = current_popup_style.element_space;     // Ԫ�ؼ��
    //        bar_size = current_popup_style.bar_size;               // �������ߴ�
    //        button_size = current_popup_style.button_size;         // ��ť�ߴ�
    //    }

    //    // ��ֻ֤����һ�ε���
    //    if (ones_open_popup)
    //    {
    //        ImGui::OpenPopup(u8"MoZIС��ʾ"); // ��������
    //        ones_open_popup = false;
    //    }

    //    /*-------------------------����λ�����С����----------------------------*/
    //    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    //    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    //    // ��С������С����
    //    static bool flag_ones = false;
    //    if (flag_ones == false)
    //    {
    //        ImGui::SetNextWindowSize(ImVec2(window_width * 0.8, window_hight * 0.5));
    //        flag_ones = true;
    //    }
    //    /*-------------------------�������ݲ���----------------------------*/
    //    if (ImGui::BeginPopupModal(u8"MoZIС��ʾ", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))  // �򿪵���
    //    {
    //        // ������С�Ĵ�СΪ1000*1000
    //        float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С

    //        // ��ʾ��������
    //        ImGui::TextWrapped(text.c_str());

    //        // �༭ȷ�ϰ���
    //        ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 4, win_h - element_pos.y * 2.5));
    //        if (ImGui::Button(u8"ȷ��", button_size)) 
    //        {
    //            ones_open_popup = true;
    //            confirm_flag =true;
    //            ImGui::CloseCurrentPopup();
    //            ImGui::SetItemDefaultFocus();
    //            ImGui::EndPopup();
    //            return true;
    //        }
    //        // �༭ȡ������
    //        ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 8, win_h - element_pos.y * 2.5));
    //        if (ImGui::Button(u8"ȡ��", button_size)) 
    //        {
    //            ones_open_popup = true;
    //            ImGui::CloseCurrentPopup();
    //            confirm_flag = false;
    //            ImGui::SetItemDefaultFocus();
    //            ImGui::EndPopup();
    //            return false;
    //        }
    //        ImGui::SetItemDefaultFocus();
    //        ImGui::EndPopup();
    //        return false;
    //    }
    //}
//-----------------------------------------------------------------------------
//                                ��������
//-----------------------------------------------------------------------------
    
      // �½��ļ����ļ��е���
    void AddNewFilePopup(bool& open_popup, const char* popup_name, const FileOperate::FileFormat& file_format, const std::filesystem::path& current_path,
        const float& current_scale)
    {
        // ��������һ�α�־λ
        static bool ones_open_popup = true;
        // �༭ȷ�ϰ���
        static bool rename_flag = false;
        /*-------------------------������Ϣ����----------------------------*/
        //static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        //scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 500, ImVec2(50, 30), ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(150, 0));
        // ������Ϣ
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;

        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }

        /*-------------------------�洢��Ϣ����----------------------------*/
        static char name_buff[256] = { 0 };

        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            // ����½��ļ���ʽ��������д�ϣ������ط�����Ҫ��
            if (file_format == FileOperate::FileFormat_Directory)           strcpy_s(name_buff, u8"�½��ļ���");
            else if (file_format == FileOperate::FileFormat_TextFile)       strcpy_s(name_buff, u8"�½��ı��ļ�");
            else if (file_format == FileOperate::FileFormat_WordFile)       strcpy_s(name_buff, u8"�½�Word�ĵ�");
            else if (file_format == FileOperate::FileFormat_PptFile)        strcpy_s(name_buff, u8"�½�PPT�ļ�");
            else if (file_format == FileOperate::FileFormat_MarkdownFile)   strcpy_s(name_buff, u8"�½�Markdown�ļ�");
            ImGui::OpenPopup(popup_name); // ��������
            ones_open_popup = false;
        }

        /*-------------------------����λ�����С����----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // ��С������С����
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width, window_hight));
            flag_ones = true;
        }


        /*-------------------------�������ݲ���----------------------------*/
        if (ImGui::BeginPopupModal(popup_name, &open_popup, ImGuiWindowFlags_NoCollapse))   // �򿪵���
        {
            // ������С�Ĵ�СΪ1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С
            if (win_h < window_hight || win_w < window_width)flag_ones = false;

            if (file_format == FileOperate::FileFormat_Directory)
            {
                DoubleElementSimpleWrite(FileOperate::GetFileIcon(file_format), name_buff, IM_ARRAYSIZE(name_buff),
                    element_pos.x, element_space.x, win_w - element_pos.x, "##�½��ļ�������", element_pos.y, 1);
            }
            else
            {
                DoubleElementSimpleWrite(FileOperate::GetFileIcon(file_format), name_buff, IM_ARRAYSIZE(name_buff),
                    element_pos.x, element_space.x, win_w - element_pos.x, "##�½��ļ�����", element_pos.y, 1);
            }

            ImGui::Dummy(ImVec2(0, element_pos.y));        // ������ľ���
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);


            ImGui::SetCursorPos(ImVec2(win_w - button_size.x - element_pos.x, win_h - button_size.y - element_pos.y*2));

            if (ImGui::Button(u8"ȷ��", button_size) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
            {
                // �����½��ļ�����
                if (!FileManage::NewBuildFile(FileOperate::UTF8_To_string(name_buff), file_format, current_path))
                {
                    // ������ʾ����
                    ImGui::OpenPopup(u8"Mozi��ʾ");
                }
                else
                {
                    memset(name_buff, 0, 256);      // ����ļ�����
                    rename_flag = false;
                    open_popup = false;
                    ones_open_popup = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            RemindPopup(u8"Mozi��ʾ", u8"�ļ����������Ƿ���Ҫǿ�ƴ���", rename_flag, current_scale);
            if (rename_flag == true)
            {
                // ǿ�ƽ����ļ�
                FileManage::NewBuildFile(FileOperate::UTF8_To_string(name_buff), file_format, current_path, true);
                // ������־λ�ָ����Լ��������
                rename_flag = false;
                memset(name_buff, 0, 256);      // ����ļ�����
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        // ֱ�ӹرյ�����ǵ��������ͻ�ԭ��־λ
        if (open_popup == false)
        {
            memset(name_buff, 0, 256);      // ����ļ�����
            ones_open_popup = true;
            rename_flag = false;
        }
    }

    // ����ճ���ļ�����
    void PasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale, const bool& copy_cut_flag)
    {
        // ÿһ�β���ʹCopyPasteFile����ֻ����һ�μ��ɣ����͵�������һ��һ��
        // Ϊ�˱��⵱����������ʱ��ÿ�ζ���Ҫ���һ�Σ��˷���Դ
        // ��������һ�α�־λ
        static bool ones_open_popup = true;
        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            bool rename_flag = FileManage::FileManagePasteFile(from_path, to_path, copy_cut_flag);
            // û�з���ճ�����������
            if (rename_flag == true)
            {
                open_popup = false;
                return;
            }
            // ����ճ�����������
            else if (rename_flag == false)
            {
                ImGui::OpenPopup(popup_name); // ��������
                ones_open_popup = false;
            }
        }

        /*-------------------------������Ϣ����----------------------------*/
        //scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 250, ImVec2(50, 30), 
            ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(200, 0));
        // ������Ϣ
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;
        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }

        /*-------------------------����λ�����С����----------------------------*/
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // ��С������С����
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width, window_hight));
            flag_ones = true;
        }

        /*-------------------------�������ݲ���----------------------------*/
        if (ImGui::BeginPopupModal(popup_name, &open_popup, window_flags))   // �򿪵���
        {
            // ������С�Ĵ�СΪ1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С
            if (win_h < window_hight || win_w < window_width)flag_ones = false;

            ImGui::SetCursorPos(ImVec2(win_w*0.5- button_size.x*0.5, win_h*0.5 - element_pos.y*2));
            if (ImGui::Button(u8"�滻�Ѿ����ڵ�ͬ���ļ�", button_size))
            {
                LOG_INFO("PasteFilePopup:�滻�Ѿ����ڵ�ͬ���ļ���...");
                // ��ɾ����������Ŀ���ļ����ڽ�ԭ�ļ����и���
                FileManage::CompleteDeleteFile(to_path / from_path.filename());
                FileManage::FileManagePasteFile(from_path, to_path, copy_cut_flag);
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetCursorPos(ImVec2(win_w * 0.5 - button_size.x * 0.5, win_h*0.5));
            if (ImGui::Button(u8"ǿ�ƽ����ļ�", button_size))
            {
                LOG_INFO("PasteFilePopup:��ǿ�ƽ����ļ���...");
                FileManage::FileManagePasteFile(from_path, to_path, copy_cut_flag, true);
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetCursorPos(ImVec2(win_w * 0.5 - button_size.x * 0.5, win_h * 0.5 + element_pos.y * 2));
            if (ImGui::Button(u8"ȡ�����β���", button_size))
            {
                LOG_INFO("PasteFilePopup:ȡ�����β���");
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        // ֱ�ӹرյ�����ǵ��������ͻ�ԭ��־λ
        if (open_popup == false)
        {
            ones_open_popup = true;
        }
    }

    // ����������
    void RenameFilePopup(bool& open_popup, const char* popup_name,
        const std::filesystem::path& current_path, const float& current_scale)
    {
        // ��������һ�α�־λ
        static bool ones_open_popup = true;

        // �༭ȷ�ϰ���
        static bool rename_flag = false;

        /*-------------------------������Ϣ����----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 500, ImVec2(30, 30), ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(150, 0));
        // ������Ϣ
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;
        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }

        /*-------------------------�洢��Ϣ����----------------------------*/
        static char name_buff[256] = { 0 };

        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            // ���ļ��������������
            // ֻ���ļ����������׺
            strcpy_s(name_buff, current_path.stem().generic_u8string().c_str());
            ImGui::OpenPopup(popup_name); // ��������
            ones_open_popup = false;
        }

        /*-------------------------����λ�����С����----------------------------*/
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // ��С������С����
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width, window_hight));
            flag_ones = true;
        }


        /*-------------------------�������ݲ���----------------------------*/
        if (ImGui::BeginPopupModal(popup_name, &open_popup, window_flags))   // �򿪵���
        {
            // ������С�Ĵ�СΪ1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С
            if (win_h < window_hight || win_w < window_width)flag_ones = false;

            //std::string File_Icon = FileOperate::TreeFileIconConnect(current_path);


            DoubleElementSimpleWrite(FileOperate::GetFileIcon(current_path), name_buff, IM_ARRAYSIZE(name_buff),
                element_pos.x, element_space.x, win_w - element_pos.x, "##����������", element_pos.y, 1);
            ImGui::Dummy(ImVec2(0, element_pos.y));        // ������ľ���
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);

            ImGui::SetCursorPos(ImVec2(win_w - button_size.x - element_pos.x, win_h - button_size.y - element_pos.y * 2));
            if (ImGui::Button(u8"ȷ��", button_size) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
            {
                // �����½��ļ�����
                if (!FileManage::FileManageRenameFile(FileOperate::UTF8_To_string(name_buff), current_path))
                {
                    // ������ʾ����
                    ImGui::OpenPopup(u8"Mozi��ʾ");
                    //// ������������������־λ
                    //rename_flag = true;
                    //error_flag = true;
                }
                else
                {
                    memset(name_buff, 0, 256);      // ����ļ�����
                    rename_flag = false;
                    open_popup = false;
                    ones_open_popup = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            RemindPopup(u8"Mozi��ʾ", u8"�ļ����������Ƿ���Ҫǿ�ƴ���", rename_flag, current_scale);

            if (rename_flag == true)
            {
                // ǿ�ƽ����ļ�
                FileManage::FileManageRenameFile(FileOperate::UTF8_To_string(name_buff), current_path, true);
                // ������־λ�ָ����Լ��������
                rename_flag = false;
                memset(name_buff, 0, 256);      // ����ļ�����
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        // ֱ�ӹرյ�����ǵ��������ͻ�ԭ��־λ
        if (open_popup == false)
        {
            memset(name_buff, 0, 256);      // ����ļ�����
            ones_open_popup = true;
            rename_flag = false;
        }
    }

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

    //// MouiPopup��ʽ��ʼ��
    //static void MouiPopupStyleInit()
    //{
    //    // scale=1 1.5 2�ֱ��ʾ1k��2k��4k��ʾ��
    //    popup_base_style.scale = 1.5f;        // 2K��ʾ���ߴ�
    //    popup_base_style.window_hight = 800;
    //    popup_base_style.window_width = 1000;
    //    popup_base_style.element_pos = ImVec2(50, 30);
    //    popup_base_style.element_space = ImVec2(20, 200);
    //    popup_base_style.bar_size = ImVec2(800.0f, 30.0f);
    //    popup_base_style.button_size = ImVec2(150, 0);
    //}

    // MouiApp��ʼ������
    void MouiInit()
    {
        // UI��ʼ��
        Moui::StyleInit();
        LOG_INFO("Moui��ʽ��ʼ�����");
        Moui::ThemeColorInit(theme_color);
        LOG_INFO("Moui�����ʼ�����");
        //Moui::MouiPopupStyleInit();
        //LOG_INFO("Moui������ʼ�����");
    }

    // ��̬DPI
    //void DynamicDPI(GLFWwindow* window, ImGuiIO& io)
    //{
    //    float xscale, yscale;
    //    glfwGetWindowContentScale(window, &xscale, &yscale);
    //    if (xscale != current_scale) 
    //    {
    //        io.Fonts->Clear();                      // �����������
    //        current_scale = xscale;
    //        /*************************** ��ʼ���� *****************************/
    //        float base_pixels = xscale * 15.0f;
    //        // �ϳ������������
    //        ImFontConfig config;
    //        config.MergeMode = true;    // ��������ϳ�
    //        config.GlyphExtraSpacing.y = 50.0f;
    //        // ͼ����ز�������
    //        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    //        config.PixelSnapH = true;
    //        config.GlyphMinAdvanceX = base_pixels * 2.0f / 2.3f;

    //        // ��ʼ����
    //        io.Fonts->AddFontFromFileTTF(FONT_ENGLISH_BASE_PATH, base_pixels, 0, io.Fonts->GetGlyphRangesDefault());
    //        // ��������
    //        io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, base_pixels, &config, io.Fonts->GetGlyphRangesChineseFull());
    //        // ͼ������
    //        io.Fonts->AddFontFromFileTTF(ICON_BASE_FILE_PATH, base_pixels * 2.0f / 2.3f, &config, icons_ranges);


    //        /*************************** Markdown-����Ŵ� *****************************/
    //        // ���xscale���Ե�ϵ������̫��Ŀǰ���Գ���30�ͻ�ʱapp����
    //        // ͬʱ��Ҫע�⣬һ������ѹ���ڴ�̫������壬���Գ���9M�ͻῨ��
    //        float H1_pixels = xscale * 25.0f;
    //        markdown_H1 = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, H1_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
    //        float H2_pixels = xscale * 20.0f;
    //        markdown_H2 = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, H2_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
    //        float H3_pixels = xscale * 15.0f;
    //        markdown_H3= io.Fonts->AddFontFromFileTTF(FONT_CHINESE_SIMHEI_PATH, H3_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
    //        io.Fonts->Build();              // ��������


    //        Markdown::LoadFonts(markdown_H1, markdown_H2, markdown_H3); // �����������markdown��

    //        ImGui_ImplOpenGL3_DestroyFontsTexture();
    //        ImGui_ImplOpenGL3_CreateFontsTexture();
    //    }
    //}

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