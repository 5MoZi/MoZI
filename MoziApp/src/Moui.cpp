#include "mopch.h"
#include "MoIcon.h"
#include "Log.h"
#include "FileOperate.h"
#include "MoObject.h"
#include "MysqlOperate.h"
#include "Moui.h"
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

    // moui��ʽ����
    static MoObject::MouiPopupStyle popup_base_style;

    
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
    void RemindPopup(const std::string& text, const float& current_scale)
    {
        // ��������һ�α�־λ
        static bool ones_open_popup = true;

        /*-------------------------������Ϣ����----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
        // ������Ϣ
        static float window_hight = current_popup_style.window_hight;
        static float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        static ImVec2 element_pos = current_popup_style.element_pos;
        static ImVec2 element_space = current_popup_style.element_space;
        static ImVec2 bar_size = current_popup_style.bar_size;
        static ImVec2 button_size = current_popup_style.button_size;
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        if (next_scale != current_scale && current_scale != current_popup_style.scale)
        {

            next_scale = current_scale;
            current_popup_style.ChangeStlyeSize(current_scale);

            window_hight = current_popup_style.window_hight;
            window_width = current_popup_style.window_width;
            element_pos = current_popup_style.element_pos;         // Ԫ��λ��
            element_space = current_popup_style.element_space;     // Ԫ�ؼ��
            bar_size = current_popup_style.bar_size;               // �������ߴ�
            button_size = current_popup_style.button_size;         // ��ť�ߴ�
        }

        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            ImGui::OpenPopup(u8"MoZIС��ʾ"); // ��������
            ones_open_popup = false;
        }

        /*-------------------------����λ�����С����----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // ��С������С����
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width * 0.8, window_hight * 0.5));
            flag_ones = true;
        }

        /*-------------------------�������ݲ���----------------------------*/
        if (ImGui::BeginPopupModal(u8"MoZIС��ʾ"), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse)  // �򿪵���
        {
            //// ������С�Ĵ�СΪ1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С

            // ��ʾ��������
            ImGui::TextWrapped(text.c_str());

            // �༭ȷ�ϰ���
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 4, win_h - element_pos.y * 2.5));
            if (ImGui::Button(u8"ȷ��", button_size)) 
            {

                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }

    // ���󴰿�
    bool ErrorPopup(const std::string& text, bool& confirm_flag, const float& current_scale)
    {
        // ��������һ�α�־λ
        static bool ones_open_popup = true;

        /*-------------------------������Ϣ����----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
        // ������Ϣ
        static float window_hight = current_popup_style.window_hight;
        static float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        static ImVec2 element_pos = current_popup_style.element_pos;
        static ImVec2 element_space = current_popup_style.element_space;
        static ImVec2 bar_size = current_popup_style.bar_size;
        static ImVec2 button_size = current_popup_style.button_size;
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        if (next_scale != current_scale && current_scale != current_popup_style.scale)
        {

            next_scale = current_scale;
            current_popup_style.ChangeStlyeSize(current_scale);

            window_hight = current_popup_style.window_hight;
            window_width = current_popup_style.window_width;
            element_pos = current_popup_style.element_pos;         // Ԫ��λ��
            element_space = current_popup_style.element_space;     // Ԫ�ؼ��
            bar_size = current_popup_style.bar_size;               // �������ߴ�
            button_size = current_popup_style.button_size;         // ��ť�ߴ�
        }

        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            ImGui::OpenPopup(u8"MoZIС��ʾ"); // ��������
            ones_open_popup = false;
        }

        /*-------------------------����λ�����С����----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // ��С������С����
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width * 0.8, window_hight * 0.5));
            flag_ones = true;
        }
        /*-------------------------�������ݲ���----------------------------*/
        if (ImGui::BeginPopupModal(u8"MoZIС��ʾ", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))  // �򿪵���
        {
            // ������С�Ĵ�СΪ1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С

            // ��ʾ��������
            ImGui::TextWrapped(text.c_str());

            // �༭ȷ�ϰ���
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 4, win_h - element_pos.y * 2.5));
            if (ImGui::Button(u8"ȷ��", button_size)) 
            {
                ones_open_popup = true;
                confirm_flag =true;
                ImGui::CloseCurrentPopup();
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
                return true;
            }
            // �༭ȡ������
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 8, win_h - element_pos.y * 2.5));
            if (ImGui::Button(u8"ȡ��", button_size)) 
            {
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
                confirm_flag = false;
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
                return false;
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
            return false;
        }
    }
//-----------------------------------------------------------------------------
//                                ��������
//-----------------------------------------------------------------------------
    // �½��ļ����ļ��е���
    void AddNewFileAndFolderPopup(bool& open_popup, const char* popup_name, const FileOperate::FileFormat& file_format,const std::filesystem::path& current_path,
        const float& current_scale)
    {
        // ��������һ�α�־λ
        static bool ones_open_popup = true;

        // �༭ȷ�ϰ���
        static bool rename_flag = false;
        // ���󵯴�һ�α�־λ
        static bool error_flag = false;


        /*-------------------------������Ϣ����----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
        // ������Ϣ
        static float window_hight = current_popup_style.window_hight;
        static float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        static ImVec2 element_pos = current_popup_style.element_pos;
        static ImVec2 element_space = current_popup_style.element_space;
        static ImVec2 bar_size = current_popup_style.bar_size;
        static ImVec2 button_size = current_popup_style.button_size;
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        if (next_scale != current_scale && current_scale != current_popup_style.scale)
        {
            next_scale = current_scale;
            current_popup_style.ChangeStlyeSize(current_scale);
            window_hight = current_popup_style.window_hight;
            window_width = current_popup_style.window_width;
            element_pos = current_popup_style.element_pos;         // Ԫ��λ��
            element_space = current_popup_style.element_space;     // Ԫ�ؼ��
            bar_size = current_popup_style.bar_size;               // �������ߴ�
            button_size = current_popup_style.button_size;         // ��ť�ߴ�
        }

        /*-------------------------�洢��Ϣ����----------------------------*/
        static char name_buff[256] = { 0 };

        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            if(file_format == FileOperate::FileFormat_Directory)        strcpy_s(name_buff, u8"�½��ļ���");
            else if(file_format == FileOperate::FileFormat_TextFile)    strcpy_s(name_buff, u8"�½��ı��ļ�");
            else if (file_format == FileOperate::FileFormat_WordFile)   strcpy_s(name_buff, u8"�½�Word�ĵ�");
            else if (file_format == FileOperate::FileFormat_PptFile)    strcpy_s(name_buff, u8"�½�PPT�ļ�");
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
            if (ImGui::GetWindowWidth() < window_hight - 200 || ImGui::GetWindowHeight() < window_width - 200)flag_ones = false;

            if (file_format == FileOperate::FileFormat_Directory)
            {
                DoubleElementSimpleWrite(FILETREE_ICON_FOLDER, name_buff, IM_ARRAYSIZE(name_buff),
                    element_pos.x, element_space.x, win_w - element_pos.x * 10, "##�½��ļ�������", element_pos.y, 1);
            }
            else //if (file_format == FileOperate::FileFormat_TextFile)
            {
                DoubleElementSimpleWrite(FILETREE_ICON_TEXTFILE, name_buff, IM_ARRAYSIZE(name_buff),
                    element_pos.x, element_space.x, win_w - element_pos.x * 10, "##�½��ļ�����", element_pos.y, 1);
            }

            ImGui::Dummy(ImVec2(0, element_pos.y));        // ������ľ���
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);


            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 5, win_h - element_pos.y * 5));
            if (ImGui::Button(u8"ȷ��", button_size))
            {
                // �����½��ļ�����
                if (!FileManage::NewBuildFolderAndFile(FileOperate::UTF8_To_string(name_buff), file_format, current_path))
                {
                    // ������������������־λ
                    rename_flag = true;
                    error_flag = true;
                }
                else 
                {
                    memset(name_buff, 0, 256);      // ����ļ�����
                    rename_flag = false;
                    error_flag = false;
                    open_popup = false;
                    ones_open_popup = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            if (error_flag && ErrorPopup(u8"�ļ����������Ƿ���Ҫǿ�ƴ���", error_flag, current_scale))
            {
                // ǿ�ƽ����ļ�
                FileManage::NewBuildFolderAndFile(FileOperate::UTF8_To_string(name_buff), file_format, current_path, true);
                // ������־λ�ָ����Լ��������
                rename_flag = false;
                error_flag = false;
                memset(name_buff, 0, 256);      // ����ļ�����
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
                return;
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
            error_flag = false;
        }
    }
    // ����ճ���ļ�����
    void CopyPasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale)
    {
        // ÿһ�β���ʹCopyPasteFile����ֻ����һ�μ��ɣ����͵�������һ��һ��
        // Ϊ�˱��⵱����������ʱ��ÿ�ζ���Ҫ���һ�Σ��˷���Դ
        // ��������һ�α�־λ
        static bool ones_open_popup = true;
        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            if (FileManage::CopyPasteFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_OperateSuccess)
            {
                open_popup = false;
                return;
            }
            else if (FileManage::CopyPasteFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_ExistRename)
            {
                ImGui::OpenPopup(popup_name); // ��������
                ones_open_popup = false;
            }
        }


        // �༭ȷ�ϰ���
        static bool rename_flag = false;
        // ���󵯴�һ�α�־λ
        static bool error_flag = false;


        /*-------------------------������Ϣ����----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
        // ������Ϣ
        static float window_hight = current_popup_style.window_hight;
        static float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        static ImVec2 element_pos = current_popup_style.element_pos;
        static ImVec2 element_space = current_popup_style.element_space;
        static ImVec2 bar_size = current_popup_style.bar_size;
        static ImVec2 button_size = current_popup_style.button_size;
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        if (next_scale != current_scale && current_scale != current_popup_style.scale)
        {
            next_scale = current_scale;
            current_popup_style.ChangeStlyeSize(current_scale);
            window_hight = current_popup_style.window_hight;
            window_width = current_popup_style.window_width;
            element_pos = current_popup_style.element_pos;         // Ԫ��λ��
            element_space = current_popup_style.element_space;     // Ԫ�ؼ��
            bar_size = current_popup_style.bar_size;               // �������ߴ�
            button_size = current_popup_style.button_size;         // ��ť�ߴ�
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
            if (ImGui::GetWindowWidth() < window_hight - 200 || ImGui::GetWindowHeight() < window_width - 200)flag_ones = false;

            //ImGui::Dummy(ImVec2(0, element_pos.y));        // ������ľ���
            //ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);

            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 15, win_h - element_pos.y * 15));
            if (ImGui::Button(u8"�滻�Ѿ����ڵ�ͬ���ļ�", button_size))
            {
                LOG_INFO("CopyPasteFilePopup:�滻�Ѿ����ڵ�ͬ���ļ���...");
                // ��ɾ��Ŀ���ļ����ڽ�ԭ�ļ����и���
                FileManage::CompleteDeleteFile(to_path / from_path.filename());
                FileManage::CopyPasteFile(from_path, to_path);
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            //ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 15, win_h - element_pos.y * 10));
            //if (ImGui::Button(u8"ǿ�ƽ����ļ�", button_size))
            //{
            //    LOG_INFO("CopyPasteFilePopup:��ǿ�ƽ����ļ���...");
            //     ������ļ�����ֻȡ�ļ���
            //    std::filesystem::path new_file_path = from_path;
            //    std::filesystem::path new_file_name = from_path.stem();
            //     ������ļ���չ��
            //    std::filesystem::path new_file_extension = from_path.extension();

            //    while (FileOperate::RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), to_path))
            //    {
            //        new_file_name += "-����";
            //    }
            //    FileOperate::SpecialCopyFolderAndFile(new_file_name.generic_string() + new_file_extension.generic_string(), to_path);
            //    open_popup = false;
            //    ones_open_popup = true;
            //    ImGui::CloseCurrentPopup();
            //}
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 15, win_h - element_pos.y * 5));
            if (ImGui::Button(u8"ȡ�����β���", button_size))
            {
                LOG_INFO("CopyPasteFilePopup:ȡ�����β���");
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

    // ����ճ���ļ�����
    void CutPasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale)
    {
        // ÿһ�β���ʹCopyPasteFile����ֻ����һ�μ��ɣ����͵�������һ��һ��
        // Ϊ�˱��⵱����������ʱ��ÿ�ζ���Ҫ���һ�Σ��˷���Դ
        // ��������һ�α�־λ
        static bool ones_open_popup = true;
        // ��ֻ֤����һ�ε���
        if (ones_open_popup)
        {
            if (FileManage::CutPasteFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_OperateSuccess)
            {
                open_popup = false;
                return;
            }
            else if (FileManage::CutPasteFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_ExistRename)
            {
                ImGui::OpenPopup(popup_name); // ��������
                ones_open_popup = false;
            }
        }


        // �༭ȷ�ϰ���
        static bool rename_flag = false;
        // ���󵯴�һ�α�־λ
        static bool error_flag = false;


        /*-------------------------������Ϣ����----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
        // ������Ϣ
        static float window_hight = current_popup_style.window_hight;
        static float window_width = current_popup_style.window_width;
        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        static ImVec2 element_pos = current_popup_style.element_pos;
        static ImVec2 element_space = current_popup_style.element_space;
        static ImVec2 bar_size = current_popup_style.bar_size;
        static ImVec2 button_size = current_popup_style.button_size;
        // ��ͬ��ģ�仯��ͬ�ĳߴ�
        static float next_scale = 0.f;              // ��֤�ߴ�ֻ����һ�α仯
        if (next_scale != current_scale && current_scale != current_popup_style.scale)
        {
            next_scale = current_scale;
            current_popup_style.ChangeStlyeSize(current_scale);
            window_hight = current_popup_style.window_hight;
            window_width = current_popup_style.window_width;
            element_pos = current_popup_style.element_pos;         // Ԫ��λ��
            element_space = current_popup_style.element_space;     // Ԫ�ؼ��
            bar_size = current_popup_style.bar_size;               // �������ߴ�
            button_size = current_popup_style.button_size;         // ��ť�ߴ�
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
            if (ImGui::GetWindowWidth() < window_hight - 200 || ImGui::GetWindowHeight() < window_width - 200)flag_ones = false;

            //ImGui::Dummy(ImVec2(0, element_pos.y));        // ������ľ���
            //ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);

            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 15, win_h - element_pos.y * 15));
            if (ImGui::Button(u8"�滻�Ѿ����ڵ�ͬ���ļ�", button_size))
            {
                LOG_INFO("CutPasteFilePopup:�滻�Ѿ����ڵ�ͬ���ļ���...");
                // ��ɾ��Ŀ���ļ����ڽ�ԭ�ļ����и���
                FileManage::CompleteDeleteFile(to_path / from_path.filename());
                FileManage::CutPasteFile(from_path, to_path);
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            //ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 15, win_h - element_pos.y * 10));
            //// �ù���δ����������Ҫ������ת�����Ƚ��ļ����Ƶ�һ���ط�����ת�أ���Ȼ���ٽ���ת�ص��ļ�������
            //// Ȼ���ٽ���ת�ص��ļ����е�Ŀ��Ŀ¼�С�
            //// ͬʱΪ�˱�������ת���з�����������Ҫʱ�̱�����ת��ֻ����һ����ת�ļ�
            //if (ImGui::Button(u8"ǿ�ƽ����ļ�", button_size))
            //{
            //    LOG_INFO("CutPasteFilePopup:��ǿ�ƽ����ļ���...");
            //    // ������ļ�����ֻȡ�ļ���
            //    std::filesystem::path new_file_name = from_path.stem();
            //    // ������ļ���չ��
            //    std::filesystem::path new_file_extension = from_path.extension();
            //  
            //    while (FileOperate::RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), to_path))
            //    {
            //        new_file_name += "-����";
            //    }
            //    FileOperate::SpecialCopyFolderAndFile(new_file_name.generic_string() + new_file_extension.generic_string(), to_path);
            //    open_popup = false;
            //    ones_open_popup = true;
            //    ImGui::CloseCurrentPopup();
            //}
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 15, win_h - element_pos.y * 5));
            if (ImGui::Button(u8"ȡ�����β���", button_size))
            {
                LOG_INFO("CutPasteFilePopup:ȡ�����β���");
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

    // MouiPopup��ʽ��ʼ��
    static void MouiPopupStyleInit()
    {
        popup_base_style.scale = 1.5f;        // 2K��ʾ���ߴ�
        popup_base_style.window_hight = 800;
        popup_base_style.window_width = 1000;
        popup_base_style.element_pos = ImVec2(50, 30);
        popup_base_style.element_space = ImVec2(20, 200);
        popup_base_style.bar_size = ImVec2(800.0f, 30.0f);
        popup_base_style.button_size = ImVec2(150, 0);
    }




    // MouiApp��ʼ������
    void MouiInit()
    {
        // UI��ʼ��
        Moui::StyleInit();
        LOG_INFO("Moui��ʽ��ʼ�����");
        Moui::ThemeColorInit(theme_color);
        LOG_INFO("Moui�����ʼ�����");
        Moui::MouiPopupStyleInit();
        LOG_INFO("Moui������ʼ�����");
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