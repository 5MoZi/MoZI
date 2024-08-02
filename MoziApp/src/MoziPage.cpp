#include "mopch.h"

#include "Log.h"
#include "FileOperate.h"
#include "MysqlOperate.h"
#include "MoObject.h"

#include "Moui.h"
#include "MoIcon.h"


#include "FileManage.h"
#include "MoziPage.h"

namespace MoziPage{


//-----------------------------------------------------------------------------
//                                  全局参数
//-----------------------------------------------------------------------------
    // 界面打开标志位
    static bool sourse_open = true;             // 资源管理器页面
    static bool property_open = false;          // 详细页面
    static bool content_open = true;            // 显示页面


    // 路径参数
    static std::filesystem::path double_click_get_path = STORAGE_PATH;       // 获取双击文件夹后，该文件夹的路径
    static std::filesystem::path right_click_get_path = STORAGE_PATH;       // 获取双击文件夹后，该文件夹的路径
    static FileOperate::FolderMap folder_maper;


    // 弹窗标志位
    static bool add_new_folder = false;                     // 开启新建文件夹弹窗标志位
    static bool add_new_text_file = false;                  // 开启新建文件夹弹窗标志位
    static bool add_new_plan_popup = false;                    // 新建计划标志位
    static bool open_rename_popup = false;                     // 开启新建文件夹弹窗标志位



    // MoZIApp初始化
    void MoziAppInit()
    {
        // 日志初始化
        MoLog::Log::LogInit();
        LOG_INFO("日志初始化设置完成");
        // 初始化设置
        LOG_INFO("MOUI初始化设置中...");
        Moui::MouiInit();              // UI初始化
        LOG_INFO("MOUI初始化完成");
        // 数据库连接
        LOG_INFO("Mysql数据库初始设置中...");
        MysqlOperate::MysqlData mysql_data_base(DATABASE_USER_NAME, DATABASE_PASSWORD, DATABASE_NAME);
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
        static bool show_soursepage = true;
        static bool show_filepage = true;
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

    // 主页
	void HomePage()
	{
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
            HomePageMainMenuBarFile();
            HomePageMainMenuBarSettings();
            HomePageMainMenuBarHelp();

            ImGui::EndMainMenuBar();
        }

        // 主页快捷栏设置
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_ARROW_LEFT)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_ARROW_RIGHT)) {}


            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_FOLDER_PLUS)) {
                //prior_file_path = STORAGE_PATH;
                //add_new_folder_popup = true;
            }

            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_FOLDER_OPEN)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_FLOPPY_DISK)) {}

            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_PLAY)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_DOWNLOAD)) {}
            if (ImGui::MenuItem(HOMEPAGE_IOCN_FA_UPLOAD)) {
                //MzFile::SelectDirectoryAndNew(STORAGE_PATH);
            }
            ImGui::EndMenuBar();
        }


        // 其他页面调用
        SoursePage();   // 资源管理器界面
        //ContentPage(double_click_current_path);  // 显示界面

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
            if (ImGui::MenuItem(SOURSEPAGE_ICON_REFLASH_FOLDER)) { /*reflash_top_file = true;*/ }

            if (ImGui::MenuItem(SOURSEPAGE_ICON_EXPAND_FOLDERS)) { /*open_all_file_action = 1;*/ }

            if (ImGui::MenuItem(SOURSEPAGE_ICON_COLLAPSE_FOLDERS)) { /*open_all_file_action = 0;*/ }

            ImGui::EndMenuBar();
        }


        OpenFolder(STORAGE_PATH, true);

        ////// 打开新建文件夹弹窗
        if (add_new_folder)Moui::AddNewFileAndFolderPopup(add_new_folder, 
            u8"新建文件夹", FileOperate::FileFormat_Directory, right_click_get_path, 0.f);
        //if (add_new_plan_popup) MZUI::AddNewPlanPopup(add_new_plan_popup, prior_file_path, current_scale, big_font);
        ////// 重命名
        ////if (open_rename_popup && (prior_file_path != STORAGE_PATH))RenamePopup(open_rename_popup, prior_file_path);


        ////if (property_open)PropertyPage(double_click_current_path);    // 展示文件属性页面
        ////if (content_open)ContentPage(double_click_current_path);    // 展示显示页面

        //open_all_file_action = -1;      // 恢复展开标志位

        ImGui::End();
    }

    // 右击回收站
    static void RightRecycleBinPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path)
    {

        if (ImGui::BeginPopupContextItem(str_id, popup_flags))
        {
            // 对回收站下的文件进行操作
            if (current_path.filename() != MOZI_RECYCLE_BIN)
            {
                // 剪切操作，可指定还原回的位置
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_CUT)) {
                    //prior_file_path = current_path;
                    //paste_file = true;
                    //cut_file = true;
                }

                // 还原回主目录
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_RETURN)) {
                 /*   if (std::filesystem::is_directory(current_path))MzFile::CopyFolderToFolder(current_path, STORAGE_PATH);
                    else if (std::filesystem::is_regular_file(current_path))MzFile::CopyFileToFolder(current_path, STORAGE_PATH);*/

                    //MzFile::DeleteFolderOrFile(current_path);                   // 对还原的文件进行删除
                }

                // 彻底删除
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_DELETE)) {
                    //MzFile::DeleteFolderOrFile(current_path, 1);                  // 对还原的文件进行删除
                }
            }

            // 只有回收站才有的操作
            // 清空回收站操作
            if (current_path.filename() == MOZI_RECYCLE_BIN)
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_EMPTY)) {
                    //MzFile::ClearBin();
                }

            ImGui::EndPopup();
        }
    }

    // 右击文件夹打开视窗
    static void RightFolderPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path)
    {
        //MoZi::ObjectType path_type = MzMysql::CheckObjectType(current_path);


        // 这里使用新函数直接一步到位
        // ImGui::OpenPopupOnItemClick(str_idd.c_str(), popup_flags);   // 创建视窗
        // ImGui::BeginPopup(str_idd.c_str());                          // 打开视窗
        // 判断右击是文件还是目录，1为文件0为目录
        //bool file_flag = std::filesystem::is_regular_file(current_path);

        if (ImGui::BeginPopupContextItem(str_id, popup_flags))
        {
            //if (path_type == MoZi::ObjectType_IsFolder)        // 只有文件才有的操作
            if (1)        // 只有文件才有的操作
            {
                if (ImGui::BeginMenu(SOURSEPAGE_FOLDER_POPUP_ADD))
                {
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEWFOLDER)) {
                        //prior_file_path = current_path;
                        right_click_get_path = current_path;
                        add_new_folder = true;
                    }
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEWTEXTFILE)) {
                        //MzFile::SelectDirectoryAndNew(STORAGE_PATH);
                    }
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDPLAN)) {
                        //prior_file_path = current_path;
                        //add_new_plan_popup = true;
                    }
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDFILE)) {
                        //MzFile::SelectDirectoryAndNew(current_path);
                    }
                    ImGui::EndMenu();
                }
            }

            if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_COPY)) {
                //prior_file_path = current_path;
                //paste_file = true;
            }
            // 只有复制或剪切了粘贴才有效,并且判断接受文件是文件还是目录，如果是文件则不接收只有目录才能接收
            //if (path_type == MoZi::ObjectType_IsFolder)          // 只有文件才有的操作
            //{
            //    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_PASTE, 0, false, paste_file)) {
            //        if (std::filesystem::is_directory(prior_file_path))MzFile::CopyFolderToFolder(prior_file_path, current_path, "", !cut_file);
            //        else if (std::filesystem::is_regular_file(prior_file_path))MzFile::CopyFileToFolder(prior_file_path, current_path);
            //        if (cut_file)       // 剪切文件时要对复制的文件进行删除
            //        {
            //            MzFile::DeleteFolderOrFile(prior_file_path);
            //            cut_file = false;
            //        }
            //        paste_file = false;
            //    }
            //}
            if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_CUT)) {
                //prior_file_path = current_path;
                //paste_file = true;
                //cut_file = true;
            }

            if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_DELETE)) {
                //MzFile::DeleteFolderOrFileToBin(current_path);
            }
            if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_RENAME)) {
                //prior_file_path = current_path;
                //open_rename_popup = true;
            }
            //if (path_type == MoZi::ObjectType_IsFolder)          // 只有文件才有的操作
            //{
            //    // 属性弹窗
            //    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_PROPERTY)) {
            //        property_open = true;
            //    }
            //}
            ImGui::EndPopup();
        }
    }


    // 递归打开逐层文件
    static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag)
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
            //if (bin_flag == 1)RightRecycleBinPopup(tree_flag_name.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);
            //else 
            RightFolderPopup(folder_only_flag.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);

            // 双击获取当前文件夹的路径
            //double_click_current_path = folder_maper.BuildFolderTree(folder_path[i], node_open);
            folder_maper.BuildFolderTree(folder_path[i], node_open, double_click_get_path);
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
            //ImGui::BulletText(FileManage::TreeFileIconConnect(file_path[i]).c_str());
            //RightFolderPopup(file_path[i].filename().generic_string().c_str(), ImGuiPopupFlags_MouseButtonRight, file_path[i]);
            std::string file_only_flag = file_path[i].generic_string();
            bool file_node_open = ImGui::TreeNodeEx(file_only_flag.c_str(), tree_flag, FileOperate::TreeFileIconConnect(file_path[i]).c_str());
            //if (bin_flag == 1)RightRecycleBinPopup(tree_flag_name.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);
            //else 
            RightFolderPopup(file_only_flag.c_str(), ImGuiPopupFlags_MouseButtonRight, file_path[i]);

            // 双击获取当前文件夹的路径
            folder_maper.BuildFolderTree(file_path[i], file_node_open, double_click_get_path);
            // 关闭树节点
            if (file_node_open) ImGui::TreePop();

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
            RightRecycleBinPopup(recycle_bin_folder.generic_string().c_str(), ImGuiPopupFlags_MouseButtonRight, recycle_bin_folder); // 右击文件夹
            if (recycle_node_open)
            {
                OpenFolder(recycle_bin_folder, 0);
                ImGui::TreePop();
            }
        }
    }


}

