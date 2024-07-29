#include "mopch.h"
#include "MoziPage.h"


#include "MoIcon.h"
#include "Moui.h"
#include "FileManage.h"





namespace MoziPage{


//-----------------------------------------------------------------------------
//                                  全局参数
//-----------------------------------------------------------------------------
    // 界面打开标志位
    static bool sourse_open = true;             // 资源管理器页面
    static bool property_open = false;          // 详细页面
    static bool content_open = true;            // 显示页面


    // 路径参数
    static std::filesystem::path double_click_current_path = STORAGE_PATH;       // 获取双击文件夹后，该文件夹的路径


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

        OpenFolder(STORAGE_PATH, 0, true);
        ////// 建立文件树
        ////BuiltTopFileTree();

        ////// 打开新建文件夹弹窗
        //if (add_new_folder_popup) MZUI::AddNewFolderPopup(add_new_folder_popup, prior_file_path, current_scale, big_font);
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


    // 获取当前文件夹路径(核心代码)
    static void ReflashPath(const std::string& file_tree_name, const bool& tree_node_open, std::filesystem::path& current_path)
    {
        static std::map<std::string, bool> name_map;      // 依据文件的名字来判断其是否打开状态
        if (name_map.count(file_tree_name) == 0)          // 判断map中是否有该节点
        {
            name_map.insert({ file_tree_name ,tree_node_open });
        }
        else            // 如果该节点存在，则判断该节点上一个tree_node_open状态与当前的是否一样
        {
            if (name_map.at(file_tree_name) == tree_node_open)
            {
                return;
            }
            else if (name_map.at(file_tree_name) == 0 && tree_node_open == 1)   // 转态有0->1即文件打开
            {
                double_click_current_path = current_path;
                name_map.at(file_tree_name) = tree_node_open;
            }
            else if (name_map.at(file_tree_name) == 1 && tree_node_open == 0)   // 转态有0->1即文件关闭
            {
                double_click_current_path = current_path;
                name_map.at(file_tree_name) = tree_node_open;
            }

        }
    }
    // 递归打开逐层文件
    static void OpenFolder(const std::filesystem::path& parent_path, int file_layer, bool top_flag, bool bin_flag)
    {
        // 文件相关参数
        static ImGuiTreeNodeFlags tree_flag = ImGuiTreeNodeFlags_OpenOnArrow             // 添加文件夹树标志位
            | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        // 存储文件夹
        std::vector<std::filesystem::path> folder_path;
        // 存储文件
        std::vector<std::filesystem::path> file_path;

        // 实时扫描当前文件夹下的文件内容
        FileManage::ScanDirectory(folder_path, file_path, parent_path);

        // 建立文件夹树
        for (int i = 0; i < folder_path.size(); i++)
        {
            // 全部展开文件
            //if (open_all_file_action != -1)
            //    ImGui::SetNextItemOpen(open_all_file_action != 0);

            //if (ignore_folder.count(folder_path[i].filename().generic_string()))continue;// 跳过对应文件夹

            // 为了保证父目录的名字与子目录名字在相同情况下其树的id不同
            // 只有文件夹要此操作文件就不用了因为文件没有下一级
            std::string tree_flag_name = folder_path[i].filename().generic_u8string();
            if (parent_path.filename() == folder_path[i].filename())
            {
                tree_flag_name += std::to_string(file_layer);
                file_layer++;
            }

            bool node_open = ImGui::TreeNodeEx(tree_flag_name.c_str(), tree_flag, FileManage::TreeFileIconConnect(folder_path[i]).c_str());

            //if (bin_flag == 1)RightRecycleBinPopup(tree_flag_name.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);
            //else RightFolderPopup(tree_flag_name.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);

            ReflashPath(folder_path[i].generic_string(), node_open, folder_path[i]);

            if (node_open)
            {

                OpenFolder(folder_path[i], file_layer, 0);
                ImGui::TreePop();
            }
        }
        // 建立文件树
        for (int i = 0; i < file_path.size(); i++)
        {
            //if (ignore_extension.count(file_path[i].extension().generic_string()))continue;// 跳过mozi后缀信息文件
            ImGui::BulletText(FileManage::TreeFileIconConnect(file_path[i]).c_str());
            //RightFolderPopup(file_path[i].filename().generic_u8string().c_str(), ImGuiPopupFlags_MouseButtonRight, file_path[i]);
        }

        // 只有第0层有回收站，即顶层目录
        if (top_flag == 1)
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
            static std::filesystem::path recycle_bin_folder = MOZI_RECYCLE_BIN_PATH;

            if (!std::filesystem::exists(recycle_bin_folder))        // 回收站如果不存在则建立
            {
                std::filesystem::create_directory(recycle_bin_folder);
            }

            //bool recycle_node_open = ImGui::TreeNodeEx(recycle_bin_folder.c_str(), tree_flag, FILETREE_ICON_BIN);
            bool recycle_node_open = ImGui::TreeNodeEx(u8"回收站", tree_flag, FILETREE_ICON_BIN);
            RightRecycleBinPopup(recycle_bin_folder.filename().generic_u8string().c_str(), ImGuiPopupFlags_MouseButtonRight, recycle_bin_folder); // 右击文件夹
            if (recycle_node_open)
            {
                OpenFolder(recycle_bin_folder, 1, 0, 1);
                ImGui::TreePop();
            }
        }
    }

















}

