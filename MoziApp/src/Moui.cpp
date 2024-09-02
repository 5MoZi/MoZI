#include "mopch.h"
#include "MoIcon.h"
#include "Log.h"
#include "FileOperate.h"
#include "MoObject.h"
#include "MysqlOperate.h"
#include "Moui.h"
#include "FileManage.h"

#include "Markdown.h"
namespace Moui {

//-----------------------------------------------------------------------------
//                                  全局参数
//-----------------------------------------------------------------------------

    // 字体参数
    static float current_scale = 0.f;           // 当前规模，用于处理不同dip时不同的字体大小
    static ImFont* big_font = NULL;             // 大号字体与图标，用于标题或其他需要大号字体的地方
    static ImFont* markdown_H1 = NULL;
    static ImFont* markdown_H2 = NULL;
    static ImFont* markdown_H3 = NULL;
    // 主题参数
    static ThemeColor theme_color = Moui::ThemeColor_Light;                 // 页面默认色
    const static int theme_color_num = 3;                             // 主题颜色的数量
    static bool* theme_color_register = new bool[theme_color_num]();  // 主题颜色变换器：当启用某个颜色主题时，该颜色下标置为1其余为0;

    // moui格式参数
    static MoObject::MouiPopupStyle popup_base_style;

    
//-----------------------------------------------------------------------------
//                                  输入函数
//-----------------------------------------------------------------------------
    // 一行两个即：一个固定一个输入  
    static float DoubleElementSimpleWrite(const std::string& text_name, char* buff, const size_t buff_size,
        const float& pos_x, const float& space, const float& input_width, const char* label, const float& space_high = 0, const bool& enable_high = 0)
    {
        // 一行两个，带有与前面元素的间距
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
//                                  显示弹窗函数
//-----------------------------------------------------------------------------    
    // 提示窗口
    void RemindPopup(const char* popup_name, const std::string& text, bool& confirm_flag, const float& current_scale)
    {
        /*-------------------------窗口信息设置----------------------------*/
        //static float next_scale = 0.f;              // 保证尺寸只进行一次变化
        //scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 500, ImVec2(50, 30), ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(120, 0));
        // 窗口信息
        // 不同规模变化不同的尺寸
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // 设置弹窗的尺寸和确定按钮的大小
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;
        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }
        /*-------------------------弹窗位置与大小设置----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // 最小弹窗大小设置
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width*0.8, window_hight*0.8));
            flag_ones = true;
        }

        /*-------------------------弹窗内容部分----------------------------*/
        //, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
        if (ImGui::BeginPopupModal(popup_name, NULL))  // 打开弹窗
        {
            //// 窗口最小的大小为1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // 窗口的实时大小
            if (win_h < window_hight*0.8 || win_w < window_width*0.8)flag_ones = false;
            // 显示错误内容
            ImGui::TextWrapped(text.c_str());

            // 编辑确认按键
            ImGui::SetCursorPos(ImVec2(win_w - button_size.x - element_pos.x, win_h - element_pos.y * 2));
            if (ImGui::Button(u8"确定", button_size))
            {
                confirm_flag = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetCursorPos(ImVec2(button_size.x - element_pos.x*1.5, win_h - element_pos.y * 2));
            if (ImGui::Button(u8"取消", button_size) || ImGui::IsKeyPressed((ImGuiKey_Delete, ImGuiKey_Escape), u8"RemindPopup-取消"))
            {
                confirm_flag = false;;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }
    }

    // 错误窗口
    bool ErrorPopup(const std::string& text, bool& confirm_flag, const float& current_scale)
    {
        // 弹窗建立一次标志位
        static bool ones_open_popup = true;

        /*-------------------------窗口信息设置----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style = popup_base_style;
        // 窗口信息
        static float window_hight = current_popup_style.window_hight;
        static float window_width = current_popup_style.window_width;
        // 设置弹窗的尺寸和确定按钮的大小
        static ImVec2 element_pos = current_popup_style.element_pos;
        static ImVec2 element_space = current_popup_style.element_space;
        static ImVec2 bar_size = current_popup_style.bar_size;
        static ImVec2 button_size = current_popup_style.button_size;
        // 不同规模变化不同的尺寸
        static float next_scale = 0.f;              // 保证尺寸只进行一次变化
        if (next_scale != current_scale && current_scale != current_popup_style.scale)
        {

            next_scale = current_scale;
            current_popup_style.ChangeStlyeSize(current_scale);

            window_hight = current_popup_style.window_hight;
            window_width = current_popup_style.window_width;
            element_pos = current_popup_style.element_pos;         // 元素位置
            element_space = current_popup_style.element_space;     // 元素间距
            bar_size = current_popup_style.bar_size;               // 进度条尺寸
            button_size = current_popup_style.button_size;         // 按钮尺寸
        }

        // 保证只建立一次弹窗
        if (ones_open_popup)
        {
            ImGui::OpenPopup(u8"MoZI小提示"); // 建立弹窗
            ones_open_popup = false;
        }

        /*-------------------------弹窗位置与大小设置----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // 最小弹窗大小设置
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width * 0.8, window_hight * 0.5));
            flag_ones = true;
        }
        /*-------------------------弹窗内容部分----------------------------*/
        if (ImGui::BeginPopupModal(u8"MoZI小提示", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))  // 打开弹窗
        {
            // 窗口最小的大小为1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // 窗口的实时大小

            // 显示错误内容
            ImGui::TextWrapped(text.c_str());

            // 编辑确认按键
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 4, win_h - element_pos.y * 2.5));
            if (ImGui::Button(u8"确定", button_size)) 
            {
                ones_open_popup = true;
                confirm_flag =true;
                ImGui::CloseCurrentPopup();
                ImGui::SetItemDefaultFocus();
                ImGui::EndPopup();
                return true;
            }
            // 编辑取消按键
            ImGui::SetCursorPos(ImVec2(win_w - element_pos.x * 8, win_h - element_pos.y * 2.5));
            if (ImGui::Button(u8"取消", button_size)) 
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
//                                弹窗函数
//-----------------------------------------------------------------------------
    
      // 新建文件和文件夹弹窗
    void AddNewFilePopup(bool& open_popup, const char* popup_name, const FileOperate::FileFormat& file_format, const std::filesystem::path& current_path,
        const float& current_scale)
    {
        // 弹窗建立一次标志位
        static bool ones_open_popup = true;
        // 编辑确认按键
        static bool rename_flag = false;
        /*-------------------------窗口信息设置----------------------------*/
        //static float next_scale = 0.f;              // 保证尺寸只进行一次变化
        //scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 500, ImVec2(50, 30), ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(150, 0));
        // 窗口信息
        // 不同规模变化不同的尺寸
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // 设置弹窗的尺寸和确定按钮的大小
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;

        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }

        /*-------------------------存储信息设置----------------------------*/
        static char name_buff[256] = { 0 };

        // 保证只建立一次弹窗
        if (ones_open_popup)
        {
            if (file_format == FileOperate::FileFormat_Directory)        strcpy_s(name_buff, u8"新建文件夹");
            else if (file_format == FileOperate::FileFormat_TextFile)    strcpy_s(name_buff, u8"新建文本文件");
            else if (file_format == FileOperate::FileFormat_WordFile)   strcpy_s(name_buff, u8"新建Word文档");
            else if (file_format == FileOperate::FileFormat_PptFile)    strcpy_s(name_buff, u8"新建PPT文件");
            ImGui::OpenPopup(popup_name); // 建立弹窗
            ones_open_popup = false;
        }

        /*-------------------------弹窗位置与大小设置----------------------------*/
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // 最小弹窗大小设置
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width, window_hight));
            flag_ones = true;
        }


        /*-------------------------弹窗内容部分----------------------------*/
        if (ImGui::BeginPopupModal(popup_name, &open_popup, ImGuiWindowFlags_NoCollapse))   // 打开弹窗
        {
            // 窗口最小的大小为1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // 窗口的实时大小
            if (win_h < window_hight || win_w < window_width)flag_ones = false;

            if (file_format == FileOperate::FileFormat_Directory)
            {
                DoubleElementSimpleWrite(FileOperate::GetFileIcon(file_format), name_buff, IM_ARRAYSIZE(name_buff),
                    element_pos.x, element_space.x, win_w - element_pos.x, "##新建文件夹名称", element_pos.y, 1);
            }
            else
            {
                DoubleElementSimpleWrite(FileOperate::GetFileIcon(file_format), name_buff, IM_ARRAYSIZE(name_buff),
                    element_pos.x, element_space.x, win_w - element_pos.x, "##新建文件名称", element_pos.y, 1);
            }

            ImGui::Dummy(ImVec2(0, element_pos.y));        // 与上面的距离
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);


            ImGui::SetCursorPos(ImVec2(win_w - button_size.x - element_pos.x, win_h - button_size.y - element_pos.y*2));

            if (ImGui::Button(u8"确定", button_size) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
            {
                // 调用新建文件函数
                if (!FileManage::NewBuildFile(FileOperate::UTF8_To_string(name_buff), file_format, current_path))
                {
                    // 开启提示弹窗
                    ImGui::OpenPopup(u8"Mozi提示");
                }
                else
                {
                    memset(name_buff, 0, 256);      // 清空文件名称
                    rename_flag = false;
                    open_popup = false;
                    ones_open_popup = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            RemindPopup(u8"Mozi提示", u8"文件重命名，是否需要强制创建", rename_flag, current_scale);
            if (rename_flag == true)
            {
                // 强制建立文件
                FileManage::NewBuildFile(FileOperate::UTF8_To_string(name_buff), file_format, current_path, true);
                // 各个标志位恢复，以及清空数组
                rename_flag = false;
                memset(name_buff, 0, 256);      // 清空文件名称
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        // 直接关闭弹窗后记得清空数组和还原标志位
        if (open_popup == false)
        {
            memset(name_buff, 0, 256);      // 清空文件名称
            ones_open_popup = true;
            rename_flag = false;
        }
    }

    // 复制粘贴文件弹窗
    void PasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale, const bool& copy_cut_flag)
    {
        // 每一次操作使CopyPasteFile函数只运行一次即可，即和弹窗建立一次一样
        // 为了避免当出现重命名时，每次都需要检测一次，浪费资源
        // 弹窗建立一次标志位
        static bool ones_open_popup = true;
        // 保证只建立一次弹窗
        if (ones_open_popup)
        {
            bool rename_flag = FileManage::FileManagePasteFile(from_path, to_path, copy_cut_flag);
            // 没有发生粘贴后的重命名
            if (rename_flag == true)
            {
                open_popup = false;
                return;
            }
            // 发生粘贴后的重命名
            else if (rename_flag == false)
            {
                ImGui::OpenPopup(popup_name); // 建立弹窗
                ones_open_popup = false;
            }
        }

        /*-------------------------窗口信息设置----------------------------*/
        //scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 250, ImVec2(50, 30), 
            ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(200, 0));
        // 窗口信息
        // 不同规模变化不同的尺寸
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // 设置弹窗的尺寸和确定按钮的大小
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;
        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }

        /*-------------------------弹窗位置与大小设置----------------------------*/
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // 最小弹窗大小设置
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width, window_hight));
            flag_ones = true;
        }

        /*-------------------------弹窗内容部分----------------------------*/
        if (ImGui::BeginPopupModal(popup_name, &open_popup, window_flags))   // 打开弹窗
        {
            // 窗口最小的大小为1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // 窗口的实时大小
            if (win_h < window_hight || win_w < window_width)flag_ones = false;

            ImGui::SetCursorPos(ImVec2(win_w*0.5- button_size.x*0.5, win_h*0.5 - element_pos.y*2));
            if (ImGui::Button(u8"替换已经存在的同名文件", button_size))
            {
                LOG_INFO("PasteFilePopup:替换已经存在的同名文件中...");
                // 先删除重命名的目标文件，在将原文件进行复制
                FileManage::CompleteDeleteFile(to_path / from_path.filename());
                FileManage::FileManagePasteFile(from_path, to_path, copy_cut_flag);
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetCursorPos(ImVec2(win_w * 0.5 - button_size.x * 0.5, win_h*0.5));
            if (ImGui::Button(u8"强制建立文件", button_size))
            {
                LOG_INFO("PasteFilePopup:替强制建立文件中...");
                FileManage::FileManagePasteFile(from_path, to_path, copy_cut_flag, true);
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetCursorPos(ImVec2(win_w * 0.5 - button_size.x * 0.5, win_h * 0.5 + element_pos.y * 2));
            if (ImGui::Button(u8"取消本次操作", button_size))
            {
                LOG_INFO("PasteFilePopup:取消本次操作");
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        // 直接关闭弹窗后记得清空数组和还原标志位
        if (open_popup == false)
        {
            ones_open_popup = true;
        }
    }

    // 重命名弹窗
    void RenameFilePopup(bool& open_popup, const char* popup_name,
        const std::filesystem::path& current_path, const float& current_scale)
    {
        // 弹窗建立一次标志位
        static bool ones_open_popup = true;

        // 编辑确认按键
        static bool rename_flag = false;

        /*-------------------------窗口信息设置----------------------------*/
        static MoObject::MouiPopupStyle current_popup_style(0.f, 300, 500, ImVec2(30, 30), ImVec2(20, 200), ImVec2(800.0f, 30.0f), ImVec2(150, 0));
        // 窗口信息
        // 不同规模变化不同的尺寸
        float window_hight = current_popup_style.window_hight;
        float window_width = current_popup_style.window_width;
        // 设置弹窗的尺寸和确定按钮的大小
        ImVec2 element_pos = current_popup_style.element_pos;
        ImVec2 element_space = current_popup_style.element_space;
        ImVec2 button_size = current_popup_style.button_size;
        if (current_scale != current_popup_style.scale)
        {
            current_popup_style.ChangeStlyeSize(current_scale);
        }

        /*-------------------------存储信息设置----------------------------*/
        static char name_buff[256] = { 0 };

        // 保证只建立一次弹窗
        if (ones_open_popup)
        {
            // 将文件名放入输入框中
            // 只放文件名不放入后缀
            strcpy_s(name_buff, current_path.stem().generic_u8string().c_str());
            ImGui::OpenPopup(popup_name); // 建立弹窗
            ones_open_popup = false;
        }

        /*-------------------------弹窗位置与大小设置----------------------------*/
        static ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        // 最小弹窗大小设置
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(window_width, window_hight));
            flag_ones = true;
        }


        /*-------------------------弹窗内容部分----------------------------*/
        if (ImGui::BeginPopupModal(popup_name, &open_popup, window_flags))   // 打开弹窗
        {
            // 窗口最小的大小为1000*1000
            float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // 窗口的实时大小
            if (win_h < window_hight || win_w < window_width)flag_ones = false;

            //std::string File_Icon = FileOperate::TreeFileIconConnect(current_path);


            DoubleElementSimpleWrite(FileOperate::GetFileIcon(current_path), name_buff, IM_ARRAYSIZE(name_buff),
                element_pos.x, element_space.x, win_w - element_pos.x, "##重命名名称", element_pos.y, 1);
            ImGui::Dummy(ImVec2(0, element_pos.y));        // 与上面的距离
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);

            ImGui::SetCursorPos(ImVec2(win_w - button_size.x - element_pos.x, win_h - button_size.y - element_pos.y * 2));
            if (ImGui::Button(u8"确定", button_size) || ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
            {
                // 调用新建文件函数
                if (!FileManage::FileManageRenameFile(FileOperate::UTF8_To_string(name_buff), current_path))
                {
                    // 开启提示弹窗
                    ImGui::OpenPopup(u8"Mozi提示");
                    //// 出现重命名，开启标志位
                    //rename_flag = true;
                    //error_flag = true;
                }
                else
                {
                    memset(name_buff, 0, 256);      // 清空文件名称
                    rename_flag = false;
                    open_popup = false;
                    ones_open_popup = true;
                    ImGui::CloseCurrentPopup();
                }
            }

            RemindPopup(u8"Mozi提示", u8"文件重命名，是否需要强制创建", rename_flag, current_scale);

            if (rename_flag == true)
            {
                // 强制建立文件
                FileManage::FileManageRenameFile(FileOperate::UTF8_To_string(name_buff), current_path, true);
                // 各个标志位恢复，以及清空数组
                rename_flag = false;
                memset(name_buff, 0, 256);      // 清空文件名称
                open_popup = false;
                ones_open_popup = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();
            ImGui::EndPopup();
        }

        // 直接关闭弹窗后记得清空数组和还原标志位
        if (open_popup == false)
        {
            memset(name_buff, 0, 256);      // 清空文件名称
            ones_open_popup = true;
            rename_flag = false;
        }
    }

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

    // MouiPopup格式初始化
    static void MouiPopupStyleInit()
    {
        popup_base_style.scale = 1.5f;        // 2K显示器尺寸
        popup_base_style.window_hight = 800;
        popup_base_style.window_width = 1000;
        popup_base_style.element_pos = ImVec2(50, 30);
        popup_base_style.element_space = ImVec2(20, 200);
        popup_base_style.bar_size = ImVec2(800.0f, 30.0f);
        popup_base_style.button_size = ImVec2(150, 0);
    }

    // MouiApp初始化程序
    void MouiInit()
    {
        // UI初始化
        Moui::StyleInit();
        LOG_INFO("Moui格式初始化完成");
        Moui::ThemeColorInit(theme_color);
        LOG_INFO("Moui主题初始化完成");
        Moui::MouiPopupStyleInit();
        LOG_INFO("Moui弹窗初始化完成");
    }

    // 动态DPI
    void DynamicDPI(GLFWwindow* window, ImGuiIO& io)
    {
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (xscale != current_scale) 
        {
            io.Fonts->Clear();                      // 清楚所有字体
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


            /*************************** Markdown-字体放大 *****************************/
            float H1_pixels = xscale * 30.0f;
            markdown_H1 = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, H1_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
            float H2_pixels = xscale * 25.0f;
            markdown_H2 = io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, H2_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
            float H3_pixels = xscale * 20.0f;
            markdown_H3= io.Fonts->AddFontFromFileTTF(FONT_CHINESE_BASE_PATH, H3_pixels, 0, io.Fonts->GetGlyphRangesChineseFull());
            io.Fonts->Build();              // 建立字体


            Markdown::LoadFonts(markdown_H1, markdown_H2, markdown_H3); // 将字体加载如markdown中

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