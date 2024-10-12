#include "mopch.h"

#include "Log/Log.h"
#include "FileOperate.h"
#include "MysqlOperate.h"
#include "MoObject.h"
#include "Moui.h"
#include "MoIcon.h"
#include "FileManage.h"
#include "MoziPage.h"
#include "MoDubug.h"


#include "Markdown.h"
#include "TextEditor.h"
#include "Fonts/fonts.h"
//int my_image_width = 0;
//int my_image_height = 0;
//GLuint my_image_texture = 0;
//ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
namespace MoziPage{


//-----------------------------------------------------------------------------
//                                  全局参数
//-----------------------------------------------------------------------------
    // 界面打开标志位
    static bool sourse_open = true;             // 资源管理器页面开启标志位
    static bool debug_display_open = false;      // debug显示页面开启标志位
    static bool text_editor_open = false;        // 文本编辑页面
    static bool markdown_display_open = false;   // markdown显示界面

    // 路径参数
    static std::filesystem::path double_click_get_path = STORAGE_PATH;       // 获取双击文件夹后，该文件夹的路径
    static std::filesystem::path right_click_get_path  = STORAGE_PATH;       // 获取右击文件夹后，该文件夹的路径
    static std::filesystem::path last_click_get_path   = STORAGE_PATH;       // 保持上一次点击的路径，该文件夹的路径


    // 文件相关操作标志位
    // 包含：新建、复制、粘贴、剪切、重命名、完全删除、删除至回收站
    static FileOperate::FileFormat add_new_file_fileformat;     // 新建文件类型
    static bool add_new_file_flag = false;                      // 新建文件标志位
    static bool open_file_flag = false;                         // 打开文件标志位

    static bool complete_delete_file_flag = false;              // 完全删除文件标志位
    static bool delete_file_to_bin_flag = false;                // 删除文件至回收站标志位
    static bool copy_file_flag = false;                         // 复制文件标志位
    static bool cut_file_flag = false;                          // 剪切文件标志位
    static bool paste_file_flag = false;                        // 粘贴文件标志位
    static bool rename_file_flag = false;                       // 重命名文件标志位

    static bool add_now_file_flag = false;                      // 添加现有文件标志位
    static bool download_file_flag = false;                     // 下载文件标志位


    static TextEditor text_editor(STORAGE_PATH);          // 文本编辑器

    static void TextEditorPage()
    {
        ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
        if (text_editor_open)
            ImGui::Begin(TEXT_EDITOR_PAGE_NAME, &text_editor_open, ImGuiWindowFlags_HorizontalScrollbar);
        else return;

        text_editor.TextEditorBegin();

        ImGui::End();
    }

    static void MarkdownDisplayPage()
    {
        ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
        if (markdown_display_open)
            ImGui::Begin(MARKDOWN_DISPLAY_PAGE_NAME, &markdown_display_open, 
                ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar);
        else return;
        // 如果文件是空的则不进行显示，并且文件需要问md后缀的文件才能使用
        if (!text_editor.GetContent().empty() && (text_editor.file_extension_flag == TextEditor::TextExtensionFlag_Markdown))
            Markdown::MarkdownBegin(text_editor.GetContent(), text_editor.file_path);

        ImGui::End();
    }


    static void DynamicFontsSize(GLFWwindow* window)
    {
        static float current_scale = 0.f;
        float xscale, yscale;
        glfwGetWindowContentScale(window, &xscale, &yscale);
        if (current_scale != xscale)
        {
            ImGuiIO& io = ImGui::GetIO();
            io.FontGlobalScale = xscale * 0.75f;
            current_scale = xscale;
        }
    }
    // MoZIApp初始化
    void MoziAppInit()
    {


        // 数据库连接
        //LOG_INFO("Mysql数据库初始设置中...");
        //MysqlOperate::ConnectDatabase();
        //MysqlOperate::CreateFileDataTable({ DATABASE_FILETABLE_NAME,DATABASE_TEMP_FILETABLE_NAME });
        //MysqlOperate::MysqlData mysql_data_base(DATABASE_USER_NAME, DATABASE_PASSWORD, DATABASE_NAME);
    }

//-----------------------------------------------------------------------------
//                                  主页
//-----------------------------------------------------------------------------
    // 文件栏
    static void HomePageMainMenuBarFile()
    {
        if (ImGui::BeginMenu(HONEPAGENAME_FILE))
        {
            if (ImGui::BeginMenu(HONEPAGENAME_SUBFILE_NEW))
            {
                if (ImGui::MenuItem(HONEPAGENAME_SUBNEW_NEWFILE)) {}
                if (ImGui::MenuItem(HONEPAGENAME_SUBNEW_NEWFOLDER)) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(HONEPAGENAME_SUBFILE_OPEN))
            {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(HONEPAGENAME_SUBFILE_SAVE))
            {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(HONEPAGENAME_SUBFILE_CLOSE))
            {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(HONEPAGENAME_SUBFILE_EXIT))
            {

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }

    // 设置栏
    static void HomePageMainMenuBarSettings()
    {
        static bool show_set_fonts_popup = false;
        if (ImGui::BeginMenu(HONEPAGENAME_SETTINGS))
        {
            // 主题
            if (ImGui::BeginMenu(HONEPAGENAME_SUBSETTINGS_THEME))
            {
                Moui::ChangeThemeColor();
                ImGui::EndMenu();
            }
            // 视图
            if (ImGui::BeginMenu(HONEPAGENAME_SUBSETTINGS_VIEW))
            {
                //ImGui::MenuItem(HONEPAGENAME_SUBVIEWS_SOURSEPAGE, NULL, &sourse_open);

                //ImGui::MenuItem(HONEPAGENAME_SUBVIEWS_PROPERTYPAGE, NULL, &property_open);

                //ImGui::MenuItem(HONEPAGENAME_SUBVIEWS_CONTENTPAGE, NULL, &content_open);

                ImGui::EndMenu();
            }
            // 字体
            if (ImGui::MenuItem(HONEPAGENAME_SUBSETTINGS_FONTS, NULL, &show_set_fonts_popup));
            ImGui::EndMenu();
        }


        if (show_set_fonts_popup) Moui::SetFontsPopup(&show_set_fonts_popup);
    }

    // 窗口栏
    static void HomePageMainMenuBarWindows()
    {
        if (ImGui::BeginMenu(HONEPAGENAME_WINDOWS))
        {
            // 路径参数
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_SOURSEPAGE, NULL, &sourse_open);
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_DEBUG_DISPLAY, "F1", &debug_display_open);
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_TEXT_EDITOR, "F2", &text_editor_open);
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_MARKDOWN_DISPLAY, "F3", &markdown_display_open);
            ImGui::EndMenu();
        }
    }

    // 帮助栏
    static void HomePageMainMenuBarHelp()
    {
        if (ImGui::BeginMenu(HONEPAGENAME_HELP))
        {
            if (ImGui::BeginMenu(HONEPAGENAME_SUBHELP_HELPFILE))
            {

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu(HONEPAGENAME_SUBHELP_VERSION))
            {
                ImGui::Text(Moui::IconAndChinese(u8"MoZI当前版本：", MOZI_VERSION).c_str());
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }

    // 主页快捷键
    static void HomePageHotKeys()
    {
        if (ImGui::IsKeyPressed(ImGuiKey_F1))debug_display_open = !debug_display_open;

    }

    // 主页
	void HomePage(GLFWwindow* glfw_window)
	{
        // 动态DIP，依据屏幕的DIP来设定字体大小
        DynamicFontsSize(glfw_window);
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));


        ImGui::Begin("MoziApp DockSpace", 0, window_flags);

        if (!opt_padding)
            ImGui::PopStyleVar();
        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("HomePageDock");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        // 主页主栏设置
        if (ImGui::BeginMainMenuBar())
        {
            HomePageMainMenuBarSettings();
            HomePageMainMenuBarWindows();
            HomePageMainMenuBarHelp();

            ImGui::EndMainMenuBar();
        }

        // 主页快捷栏设置
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_ARROW_LEFT)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_ARROW_RIGHT)) {}


            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_FOLDER_PLUS)) {
                right_click_get_path = STORAGE_PATH;
                add_new_file_flag = true;
                add_new_file_fileformat = FileOperate::FileFormat_Directory;
            }

            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_FOLDER_OPEN)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_FLOPPY_DISK)) {}

            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_PLAY)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_DOWNLOAD)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_UPLOAD)) {
                FileManage::SelectFile(STORAGE_PATH);
            }
            ImGui::EndMenuBar();
        }

        // 主页快捷键
        HomePageHotKeys();

        // 其他页面调用
        if(sourse_open)             SoursePage();               // 资源管理器界面
        if(debug_display_open)      DebugDisplayPage();         // debug显示页面
        if(text_editor_open)        TextEditorPage();           // 文本编辑页面
        if(markdown_display_open)   MarkdownDisplayPage();      // Markdown显示页面
        ImGui::End();
	}


//-----------------------------------------------------------------------------
//                                  资源管理页面
//-----------------------------------------------------------------------------
    // 资源管理器页面
    static void SoursePage()
    {
        static bool no_move = false;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;

        if (no_move)window_flags |= ImGuiWindowFlags_NoMove;

        ///***imp***/
        //// 防止在文件资源管理器还没打开时，点开新建文件夹，在打开文件资源管理器时立即出现新建文件夹弹窗
        //if (!sourse_open && add_new_folder_popup) add_new_folder_popup = false;

        // sourse_open 为1时打开资源管理器页面，为0时关闭资源管理器页面
        if (sourse_open)
        {
            ImGui::Begin(SOURSE_PAGE_NAME, &sourse_open, window_flags);
        }
        else
        {
            // 还原所有标志位
            no_move = false;
            return;
        }

        // 页面快捷栏
        if (ImGui::BeginMenuBar())
        {
            if (no_move == false) {
                if (ImGui::MenuItem(SOURSEPAGE_IOCN_WINDOWS_MOVE)) { no_move = true; }
            }
            else {
                if (ImGui::MenuItem(SOURSEPAGE_ICON_WINDOWS_NO_MOVE)) { no_move = false; }
            }
            // 文件夹刷新
            //if (ImGui::MenuItem(SOURSEPAGE_ICON_REFLASH_FOLDER)) { /*reflash_top_file = true;*/ }

            //if (ImGui::MenuItem(SOURSEPAGE_ICON_EXPAND_FOLDERS)) { /*open_all_file_action = 1;*/ }

            //if (ImGui::MenuItem(SOURSEPAGE_ICON_COLLAPSE_FOLDERS)) { /*open_all_file_action = 0;*/ }

            ImGui::EndMenuBar();
        }


        OpenFolder(STORAGE_PATH, true);
        // 添加现有项
        if (add_now_file_flag)
        {
            FileManage::SelectFile(right_click_get_path);
            add_now_file_flag = false;
        }
        // 打开新建文件夹弹窗
        if (add_new_file_flag)
        {
            // 当需要添加新的类型文件时，只需要在这个switch中写上，并在RightFolderPopup函数中将图标加上，
            // 同时在Moui::AddNewFilePopup函数中加上一处就行，其他地方不需要改（前提是以及在FileOperate.h中添加的新文件类型的后缀）
            switch (add_new_file_fileformat)
            {
            case FileOperate::FileFormat_Directory:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"新建文件夹", FileOperate::FileFormat_Directory, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_TextFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"新建文本文件", FileOperate::FileFormat_TextFile, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_WordFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"新建Word文件", FileOperate::FileFormat_WordFile, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_PptFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"新建PPT文件", FileOperate::FileFormat_PptFile, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_MarkdownFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"新建Markdown文件", FileOperate::FileFormat_MarkdownFile, right_click_get_path, 0.f);
                break;
            default:break;
            }
        }
        // 打开新建文件夹弹窗
        if (rename_file_flag)
        {
            Moui::RenameFilePopup(rename_file_flag, u8"重命名文件夹", right_click_get_path, 0.f);
        }
        
        // 复制粘贴操作，一次复制可以有n次粘贴
        if (copy_file_flag && paste_file_flag)
        {
            Moui::PasteFilePopup(paste_file_flag, u8"复制粘贴文件", last_click_get_path, right_click_get_path, 0.f, false);
        }
        // 剪切粘贴操作，一次剪切只能有一次粘贴
        if (cut_file_flag && paste_file_flag)
        {
            Moui::PasteFilePopup(paste_file_flag, u8"剪切粘贴文件", last_click_get_path, right_click_get_path, 0.f, true);
            // 当执行完粘贴后，使剪切标志位恢复，即一次剪切只能有一次粘贴
            if (!paste_file_flag)cut_file_flag = false;
        }

        if (open_file_flag)
        {
            ShellExecute(NULL, L"open", right_click_get_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
            open_file_flag = false;
        }

        // 下载文件
        if (download_file_flag)
        {
            FileManage::DownloadFile(right_click_get_path);
            download_file_flag = false;
        }

        // 删除文件
        // 删除文件至回收站
        if (delete_file_to_bin_flag)
        {
            FileManage::DeleteFileToBin(right_click_get_path);
            delete_file_to_bin_flag = false;
        }
        // 彻底删除文件
        else if (complete_delete_file_flag)
        {
            FileManage::CompleteDeleteFile(right_click_get_path);
            complete_delete_file_flag = false;
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Keypad0))
        {
            mde::ddp("hello%s", "asfas");
        }
        ImGui::End();
    }

    // 右击文件夹打开视窗
    static void RightFolderPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path, const bool& bin_file_flag)
    {
        // 这里使用新函数直接一步到位
        // ImGui::OpenPopupOnItemClick(str_idd.c_str(), popup_flags);   // 创建视窗
        // ImGui::BeginPopup(str_idd.c_str());                          // 打开视窗
        // 判断右击是文件还是目录，1为目录0为文件
        bool foler_flag = std::filesystem::is_directory(current_path);

        if (ImGui::BeginPopupContextItem(str_id, popup_flags))
        {
            // 回收站特有操作
            if (current_path.generic_u8string() == MOZI_RECYCLE_BIN_PATH)
            {
                // 一键清空删除
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_DELETE)) 
                {
                    right_click_get_path = MOZI_RECYCLE_BIN_PATH;
                    // 回收站内的文件彻底删除，不是回收站的文件使用删除移动入回收站中
                    complete_delete_file_flag = true;
                    LOG_INFO("一键清空回收站完成");
                }
            }
            else
            {
                // 只有不是回收站内的文件夹才有的操作
                if (foler_flag && !bin_file_flag)
                {
                    // 添加部分
                    if (ImGui::BeginMenu(SOURSEPAGE_FOLDER_POPUP_ADD))
                    {
                        // 现有项
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDFILE)) {
                            right_click_get_path = current_path;
                            add_now_file_flag = true;
                        }
                        // 新建文件夹
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEWFOLDER)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_Directory;
                        }
                        // 新建文本文件
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_TEXTFILE)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_TextFile;
                        }
                        // 新建Word文档
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_WORDFILE)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_WordFile;
                        }
                        // 新建PPT文件
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_PPTFILE)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_PptFile;
                        }
                        // 新建Markdown文件
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_Markdown)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_MarkdownFile;
                        }
                        ImGui::EndMenu();
                    }
                }

                // 文件下载操作
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_DOWNLOAD)) {
                    right_click_get_path = current_path;
                    download_file_flag = true;
                }

                // 只有文件才有的操作
                if (!foler_flag)
                {
                    // 打开文件
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_OPEN)) {
                        right_click_get_path = current_path;
                        open_file_flag = true;
                    }
                }

                // 剪切和复制操作同一时刻只能存在一个，不能共存
                // 剪切操作
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_CUT)) {
                    last_click_get_path = current_path;
                    cut_file_flag = true;
                    copy_file_flag = false;
                }
                // 复制操作
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_COPY)) {
                    last_click_get_path = current_path;
                    copy_file_flag = true;
                    cut_file_flag = false;
                }
                // 只有复制或剪切了粘贴才有效,并且判断接受文件是文件还是目录，如果是文件则不接收只有目录才能接收
                // 只有文件才有的操作
                if (foler_flag)
                {
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_PASTE, 0, false, copy_file_flag || cut_file_flag))
                    {
                        right_click_get_path = current_path;
                        paste_file_flag = true;
                    }
                }

                // 删除操作
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_DELETE)) {
                    right_click_get_path = current_path;
                    // 回收站内的文件彻底删除，不是回收站的文件使用删除移动入回收站中
                    if (bin_file_flag)complete_delete_file_flag = true;
                    else delete_file_to_bin_flag = true;
                }
                // 重命名操作
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_RENAME)) {
                    right_click_get_path = current_path;
                    rename_file_flag = true;
                }
            }    
            ImGui::EndPopup();
        }
    }

    // 递归打开逐层文件
    static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag, const bool& bin_file_flag)
    {
        // 文件相关参数
        static ImGuiTreeNodeFlags tree_flag = ImGuiTreeNodeFlags_OpenOnArrow             // 添加文件夹树标志位
            | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        // 存储文件夹
        std::vector<std::filesystem::path> folder_path;
        // 存储文件
        std::vector<std::filesystem::path> file_path;

        // 实时扫描当前文件夹下的文件内容
        FileOperate::ScanDirectory(folder_path, file_path, parent_path);

        // 建立文件夹树
        for (int i = 0; i < folder_path.size(); i++)
        {
            // 全部展开文件
            //if (open_all_file_action != -1)
            //    ImGui::SetNextItemOpen(open_all_file_action != 0);

            //if (ignore_folder.count(folder_path[i].filename().generic_string()))continue;// 跳过对应文件夹

            // 由于文件的路径是唯一的，因此使用路径作为文件树文件的唯一标识符，即：folder_path[i].generic_string()
            std::string folder_only_flag = folder_path[i].generic_string();
            bool node_open = ImGui::TreeNodeEx(folder_only_flag.c_str(), tree_flag, FileOperate::TreeFileIconConnect(folder_path[i]).c_str());
  
            RightFolderPopup(folder_only_flag.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i], bin_file_flag);
            // 双击获取当前文件夹的路径
            DoubleClickedFile(folder_path[i], double_click_get_path);
            
            if (node_open)
            {
                OpenFolder(folder_path[i],0);
                ImGui::TreePop();
            }
        }
        // 建立文件树
        for (int i = 0; i < file_path.size(); i++)
        {
            //if (ignore_extension.count(file_path[i].extension().generic_string()))continue;// 跳过mozi后缀信息文件
            // 建立树节点
            ImGui::TreeNodeEx(file_path[i].c_str(), ImGuiTreeNodeFlags_Leaf| ImGuiTreeNodeFlags_Bullet, FileOperate::TreeFileIconConnect(file_path[i]).c_str());
            // 双击文件操作
            DoubleClickedFile(file_path[i], double_click_get_path);
            ImGui::TreePop();
        }

        // 只有第0层有回收站，即顶层目录
        if (top_flag == true)
        {
            // 设置回收站的高度（实时变动）
            float d_w_h = ImGui::GetWindowHeight();                 // 窗口动态高度
            float d_file_y = ImGui::GetCursorPosY();                // 所有文件所占据的高度值
            if (d_w_h * 2 / 3 < d_file_y + 50)
                ImGui::Dummy(ImVec2(0, 50));
            else
                ImGui::SetCursorPos(ImVec2(0, d_w_h * 2 / 3));

            // 建立回收站树，要把回收站放在最底下
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);
            // 回收站存放路径
            static std::filesystem::path recycle_bin_folder = MOZI_RECYCLE_BIN_PATH;

            // 回收站如果不存在则建立
            if (!std::filesystem::exists(recycle_bin_folder))        
            {
                std::filesystem::create_directory(recycle_bin_folder);
            }

            bool recycle_node_open = ImGui::TreeNodeEx(u8"回收站", tree_flag, FILETREE_ICON_BIN);
            RightFolderPopup(recycle_bin_folder.generic_string().c_str(), ImGuiPopupFlags_MouseButtonRight, recycle_bin_folder, 0); // 右击文件夹
            if (recycle_node_open)
            {
                OpenFolder(recycle_bin_folder, 0, 1);
                ImGui::TreePop();
            }
        }
    }
    
    // 双击文件操作
    static void DoubleClickedFile(const std::filesystem::path& current_path, std::filesystem::path& double_click_get_path)
    {
        //static std::filesystem::path last_path = STORAGE_PATH;
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            // 文件夹双击操作
            if (std::filesystem::is_directory(current_path))
            {
                
            }
            // 文件双击操作
            else if (std::filesystem::is_regular_file(current_path))
            {
                double_click_get_path = current_path;
                if ((FileOperate::CheckFileFormat(current_path) == FileOperate::FileFormat_MarkdownFile) ||
                    (FileOperate::CheckFileFormat(current_path) == FileOperate::FileFormat_TextFile))
                {
                    text_editor.SetEditorFilePath(current_path);
                    markdown_display_open = true;       // 双击markdown文件后优先打开显示界面
                    text_editor_open = true;
                }
                else
                {
                    ShellExecute(NULL, L"open", current_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
                }
            }
        }
    }
//-----------------------------------------------------------------------------
//                                  debug显示页面
//-----------------------------------------------------------------------------

    static void DebugDisplayPage()
    {
        static bool is_display = true;
        static bool no_move = false;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar;

        if (no_move)window_flags |= ImGuiWindowFlags_NoMove;

        // sourse_open 为1时打开资源管理器页面，为0时关闭资源管理器页面
        // 最小弹窗大小设置
        static bool flag_ones = false;
        if (flag_ones == false)
        {
            ImGui::SetNextWindowSize(ImVec2(700, 500));
            flag_ones = true;
        }
        if (debug_display_open)
        {
            ImGui::Begin(DEBUG_DISPLAY_PAGE_NAME, &debug_display_open, window_flags);
        }
        else
        {
            // 还原所有标志位
            no_move = false;
            return;
        }
        // 窗口最小的大小为1000*1000
        float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // 窗口的实时大小
        if (win_h < 400 || win_w < 700)flag_ones = false;

        // 前言区
        ImGui::TextWrapped(u8"欢迎使用MoZI中的debug显示区！！！");
        ImGui::Separator();
        //ImGui::SameLine();
        // 过滤器使用
        mde::ddp.Filter.Draw("Filter##DebugDisplayPage", win_w - 70);
        ImGui::Separator();


        // 内容显示区
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("##DebugDisplayPage_DisplayRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiChildFlags_Border | ImGuiWindowFlags_HorizontalScrollbar))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            for (const char* item : mde::ddp.Items)
            {
                // 过滤器使用
                if (!mde::ddp.Filter.PassFilter(item))
                    continue;

                ImVec4 color;
                bool has_color = false;
                if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
                else if (strncmp(item, "Mozi->: ", 8) == 0) { color = ImVec4(0.807f, 0.014f, 1.0f, 0.784f); has_color = true; }
                //else if (strncmp(item, "Mozi->: ", 8) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
                if (has_color)
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(item);
                if (has_color)
                    ImGui::PopStyleColor();
            }

            if (mde::ddp.ScrollToBottom || (mde::ddp.AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
                ImGui::SetScrollHereY(1.0f);
            mde::ddp.ScrollToBottom = false;


            ImGui::PopStyleVar();

        }
        ImGui::EndChild();
        ImGui::Separator();
        // Command-line
        bool reclaim_focus = false;
        ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
        if (ImGui::InputText("##DebugDisplayPage_Input", mde::ddp.InputBuf, IM_ARRAYSIZE(mde::ddp.InputBuf), input_text_flags, &mde::ddp.TextEditCallbackStub, &mde::ddp))
        {
            char* s = mde::ddp.InputBuf;
            mde::ddp.Strtrim(s);
            if (s[0])
                mde::ddp.ExecCommand(s);
            strcpy_s(s, 256, "");
            reclaim_focus = true;
        }

        // Auto-focus on window apparition
        ImGui::SetItemDefaultFocus();
        if (reclaim_focus)
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        // 按钮区
        ImGui::SameLine(0, 10);
        ImGui::SetCursorPosX(ImGui::GetWindowWidth()-220);
        if (is_display)
        {
            if (ImGui::Button("Stop##debug_display", ImVec2(100, 0))) 
            { 
                mde::ddp.StopDisplay();
                is_display = false;
            }
        }
        else
        {
            if (ImGui::Button("Begin##debug_display", ImVec2(100, 0)))
            {
                mde::ddp.BeginDisplay();
                is_display = true;
            }
        }
        
        ImGui::SameLine(0, 10);
        if (ImGui::Button("Clear##debug_display", ImVec2(100, 0))) { mde::ddp.ClearLog(); }
        ImGui::End();
    }

}

