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
//                                  ȫ�ֲ���
//-----------------------------------------------------------------------------
    // ����򿪱�־λ
    static bool sourse_open = true;             // ��Դ������ҳ��
    static bool property_open = false;          // ��ϸҳ��
    static bool content_open = true;            // ��ʾҳ��


    // ·������
    static std::filesystem::path double_click_get_path = STORAGE_PATH;       // ��ȡ˫���ļ��к󣬸��ļ��е�·��
    static std::filesystem::path right_click_get_path = STORAGE_PATH;       // ��ȡ˫���ļ��к󣬸��ļ��е�·��
    static FileOperate::FolderMap folder_maper;


    // ������־λ
    static bool add_new_folder = false;                     // �����½��ļ��е�����־λ
    static bool add_new_text_file = false;                  // �����½��ļ��е�����־λ
    static bool add_new_plan_popup = false;                    // �½��ƻ���־λ
    static bool open_rename_popup = false;                     // �����½��ļ��е�����־λ



    // MoZIApp��ʼ��
    void MoziAppInit()
    {
        // ��־��ʼ��
        MoLog::Log::LogInit();
        LOG_INFO("��־��ʼ���������");
        // ��ʼ������
        LOG_INFO("MOUI��ʼ��������...");
        Moui::MouiInit();              // UI��ʼ��
        LOG_INFO("MOUI��ʼ�����");
        // ���ݿ�����
        LOG_INFO("Mysql���ݿ��ʼ������...");
        MysqlOperate::MysqlData mysql_data_base(DATABASE_USER_NAME, DATABASE_PASSWORD, DATABASE_NAME);
    }

//-----------------------------------------------------------------------------
//                                  ��ҳ
//-----------------------------------------------------------------------------
    // �ļ���
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

    // ������
    static void HomePageMainMenuBarSettings()
    {
        static bool show_soursepage = true;
        static bool show_filepage = true;
        if (ImGui::BeginMenu(HONEPAGENAME_SETTINGS))
        {

            // ����
            if (ImGui::BeginMenu(HONEPAGENAME_SUBSETTINGS_THEME))
            {
                Moui::ChangeThemeColor();
                ImGui::EndMenu();
            }
            // ��ͼ
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

    // ������
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
                ImGui::Text(Moui::IconAndChinese(u8"MoZI��ǰ�汾��", MOZI_VERSION).c_str());
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }
    }

    // ��ҳ
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

        // ��ҳ��������
        if (ImGui::BeginMainMenuBar())
        {
            HomePageMainMenuBarFile();
            HomePageMainMenuBarSettings();
            HomePageMainMenuBarHelp();

            ImGui::EndMainMenuBar();
        }

        // ��ҳ���������
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


        // ����ҳ�����
        SoursePage();   // ��Դ����������
        //ContentPage(double_click_current_path);  // ��ʾ����

        ImGui::End();
	}


//-----------------------------------------------------------------------------
//                                  ��Դ����ҳ��
//-----------------------------------------------------------------------------
    // ��Դ������ҳ��
    static void SoursePage()
    {
        static bool no_move = false;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;

        if (no_move)window_flags |= ImGuiWindowFlags_NoMove;

        ///***imp***/
        //// ��ֹ���ļ���Դ��������û��ʱ���㿪�½��ļ��У��ڴ��ļ���Դ������ʱ���������½��ļ��е���
        //if (!sourse_open && add_new_folder_popup) add_new_folder_popup = false;

        // sourse_open Ϊ1ʱ����Դ������ҳ�棬Ϊ0ʱ�ر���Դ������ҳ��
        if (sourse_open)
        {
            ImGui::Begin(SOURSE_PAGE_NAME, &sourse_open, window_flags);
        }
        else
        {
            // ��ԭ���б�־λ
            no_move = false;
            return;
        }

        // ҳ������
        if (ImGui::BeginMenuBar())
        {
            if (no_move == false) {
                if (ImGui::MenuItem(SOURSEPAGE_IOCN_WINDOWS_MOVE)) { no_move = true; }
            }
            else {
                if (ImGui::MenuItem(SOURSEPAGE_ICON_WINDOWS_NO_MOVE)) { no_move = false; }
            }
            // �ļ���ˢ��
            if (ImGui::MenuItem(SOURSEPAGE_ICON_REFLASH_FOLDER)) { /*reflash_top_file = true;*/ }

            if (ImGui::MenuItem(SOURSEPAGE_ICON_EXPAND_FOLDERS)) { /*open_all_file_action = 1;*/ }

            if (ImGui::MenuItem(SOURSEPAGE_ICON_COLLAPSE_FOLDERS)) { /*open_all_file_action = 0;*/ }

            ImGui::EndMenuBar();
        }


        OpenFolder(STORAGE_PATH, true);

        ////// ���½��ļ��е���
        if (add_new_folder)Moui::AddNewFileAndFolderPopup(add_new_folder, 
            u8"�½��ļ���", FileOperate::FileFormat_Directory, right_click_get_path, 0.f);
        //if (add_new_plan_popup) MZUI::AddNewPlanPopup(add_new_plan_popup, prior_file_path, current_scale, big_font);
        ////// ������
        ////if (open_rename_popup && (prior_file_path != STORAGE_PATH))RenamePopup(open_rename_popup, prior_file_path);


        ////if (property_open)PropertyPage(double_click_current_path);    // չʾ�ļ�����ҳ��
        ////if (content_open)ContentPage(double_click_current_path);    // չʾ��ʾҳ��

        //open_all_file_action = -1;      // �ָ�չ����־λ

        ImGui::End();
    }

    // �һ�����վ
    static void RightRecycleBinPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path)
    {

        if (ImGui::BeginPopupContextItem(str_id, popup_flags))
        {
            // �Ի���վ�µ��ļ����в���
            if (current_path.filename() != MOZI_RECYCLE_BIN)
            {
                // ���в�������ָ����ԭ�ص�λ��
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_CUT)) {
                    //prior_file_path = current_path;
                    //paste_file = true;
                    //cut_file = true;
                }

                // ��ԭ����Ŀ¼
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_RETURN)) {
                 /*   if (std::filesystem::is_directory(current_path))MzFile::CopyFolderToFolder(current_path, STORAGE_PATH);
                    else if (std::filesystem::is_regular_file(current_path))MzFile::CopyFileToFolder(current_path, STORAGE_PATH);*/

                    //MzFile::DeleteFolderOrFile(current_path);                   // �Ի�ԭ���ļ�����ɾ��
                }

                // ����ɾ��
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_DELETE)) {
                    //MzFile::DeleteFolderOrFile(current_path, 1);                  // �Ի�ԭ���ļ�����ɾ��
                }
            }

            // ֻ�л���վ���еĲ���
            // ��ջ���վ����
            if (current_path.filename() == MOZI_RECYCLE_BIN)
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_EMPTY)) {
                    //MzFile::ClearBin();
                }

            ImGui::EndPopup();
        }
    }

    // �һ��ļ��д��Ӵ�
    static void RightFolderPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path)
    {
        //MoZi::ObjectType path_type = MzMysql::CheckObjectType(current_path);


        // ����ʹ���º���ֱ��һ����λ
        // ImGui::OpenPopupOnItemClick(str_idd.c_str(), popup_flags);   // �����Ӵ�
        // ImGui::BeginPopup(str_idd.c_str());                          // ���Ӵ�
        // �ж��һ����ļ�����Ŀ¼��1Ϊ�ļ�0ΪĿ¼
        //bool file_flag = std::filesystem::is_regular_file(current_path);

        if (ImGui::BeginPopupContextItem(str_id, popup_flags))
        {
            //if (path_type == MoZi::ObjectType_IsFolder)        // ֻ���ļ����еĲ���
            if (1)        // ֻ���ļ����еĲ���
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
            // ֻ�и��ƻ������ճ������Ч,�����жϽ����ļ����ļ�����Ŀ¼��������ļ��򲻽���ֻ��Ŀ¼���ܽ���
            //if (path_type == MoZi::ObjectType_IsFolder)          // ֻ���ļ����еĲ���
            //{
            //    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_PASTE, 0, false, paste_file)) {
            //        if (std::filesystem::is_directory(prior_file_path))MzFile::CopyFolderToFolder(prior_file_path, current_path, "", !cut_file);
            //        else if (std::filesystem::is_regular_file(prior_file_path))MzFile::CopyFileToFolder(prior_file_path, current_path);
            //        if (cut_file)       // �����ļ�ʱҪ�Ը��Ƶ��ļ�����ɾ��
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
            //if (path_type == MoZi::ObjectType_IsFolder)          // ֻ���ļ����еĲ���
            //{
            //    // ���Ե���
            //    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_PROPERTY)) {
            //        property_open = true;
            //    }
            //}
            ImGui::EndPopup();
        }
    }


    // �ݹ������ļ�
    static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag)
    {
        // �ļ���ز���
        static ImGuiTreeNodeFlags tree_flag = ImGuiTreeNodeFlags_OpenOnArrow             // ����ļ�������־λ
            | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        // �洢�ļ���
        std::vector<std::filesystem::path> folder_path;
        // �洢�ļ�
        std::vector<std::filesystem::path> file_path;

        // ʵʱɨ�赱ǰ�ļ����µ��ļ�����
        FileOperate::ScanDirectory(folder_path, file_path, parent_path);

        // �����ļ�����
        for (int i = 0; i < folder_path.size(); i++)
        {
            // ȫ��չ���ļ�
            //if (open_all_file_action != -1)
            //    ImGui::SetNextItemOpen(open_all_file_action != 0);

            //if (ignore_folder.count(folder_path[i].filename().generic_string()))continue;// ������Ӧ�ļ���


            // �����ļ���·����Ψһ�ģ����ʹ��·����Ϊ�ļ����ļ���Ψһ��ʶ��������folder_path[i].generic_string()
            std::string folder_only_flag = folder_path[i].generic_string();
            bool node_open = ImGui::TreeNodeEx(folder_only_flag.c_str(), tree_flag, FileOperate::TreeFileIconConnect(folder_path[i]).c_str());
            //if (bin_flag == 1)RightRecycleBinPopup(tree_flag_name.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);
            //else 
            RightFolderPopup(folder_only_flag.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);

            // ˫����ȡ��ǰ�ļ��е�·��
            //double_click_current_path = folder_maper.BuildFolderTree(folder_path[i], node_open);
            folder_maper.BuildFolderTree(folder_path[i], node_open, double_click_get_path);
            if (node_open)
            {
                OpenFolder(folder_path[i],0);
                ImGui::TreePop();
            }
        }
        // �����ļ���
        for (int i = 0; i < file_path.size(); i++)
        {
            //if (ignore_extension.count(file_path[i].extension().generic_string()))continue;// ����mozi��׺��Ϣ�ļ�
            //ImGui::BulletText(FileManage::TreeFileIconConnect(file_path[i]).c_str());
            //RightFolderPopup(file_path[i].filename().generic_string().c_str(), ImGuiPopupFlags_MouseButtonRight, file_path[i]);
            std::string file_only_flag = file_path[i].generic_string();
            bool file_node_open = ImGui::TreeNodeEx(file_only_flag.c_str(), tree_flag, FileOperate::TreeFileIconConnect(file_path[i]).c_str());
            //if (bin_flag == 1)RightRecycleBinPopup(tree_flag_name.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i]);
            //else 
            RightFolderPopup(file_only_flag.c_str(), ImGuiPopupFlags_MouseButtonRight, file_path[i]);

            // ˫����ȡ��ǰ�ļ��е�·��
            folder_maper.BuildFolderTree(file_path[i], file_node_open, double_click_get_path);
            // �ر����ڵ�
            if (file_node_open) ImGui::TreePop();

        }

        // ֻ�е�0���л���վ��������Ŀ¼
        if (top_flag == true)
        {
            // ���û���վ�ĸ߶ȣ�ʵʱ�䶯��
            float d_w_h = ImGui::GetWindowHeight();                 // ���ڶ�̬�߶�
            float d_file_y = ImGui::GetCursorPosY();                // �����ļ���ռ�ݵĸ߶�ֵ
            if (d_w_h * 2 / 3 < d_file_y + 50)
                ImGui::Dummy(ImVec2(0, 50));
            else
                ImGui::SetCursorPos(ImVec2(0, d_w_h * 2 / 3));

            // ��������վ����Ҫ�ѻ���վ���������
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, 5.0);
            // ����վ���·��
            static std::filesystem::path recycle_bin_folder = MOZI_RECYCLE_BIN_PATH;

            // ����վ�������������
            if (!std::filesystem::exists(recycle_bin_folder))        
            {
                std::filesystem::create_directory(recycle_bin_folder);
            }

            bool recycle_node_open = ImGui::TreeNodeEx(u8"����վ", tree_flag, FILETREE_ICON_BIN);
            RightRecycleBinPopup(recycle_bin_folder.generic_string().c_str(), ImGuiPopupFlags_MouseButtonRight, recycle_bin_folder); // �һ��ļ���
            if (recycle_node_open)
            {
                OpenFolder(recycle_bin_folder, 0);
                ImGui::TreePop();
            }
        }
    }


}

