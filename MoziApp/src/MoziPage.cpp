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
//                                  ȫ�ֲ���
//-----------------------------------------------------------------------------
    // ����򿪱�־λ
    static bool sourse_open = true;             // ��Դ������ҳ�濪����־λ
    static bool debug_display_open = false;      // debug��ʾҳ�濪����־λ
    static bool text_editor_open = false;        // �ı��༭ҳ��
    static bool markdown_display_open = false;   // markdown��ʾ����

    // ·������
    static std::filesystem::path double_click_get_path = STORAGE_PATH;       // ��ȡ˫���ļ��к󣬸��ļ��е�·��
    static std::filesystem::path right_click_get_path  = STORAGE_PATH;       // ��ȡ�һ��ļ��к󣬸��ļ��е�·��
    static std::filesystem::path last_click_get_path   = STORAGE_PATH;       // ������һ�ε����·�������ļ��е�·��


    // �ļ���ز�����־λ
    // �������½������ơ�ճ�������С�����������ȫɾ����ɾ��������վ
    static FileOperate::FileFormat add_new_file_fileformat;     // �½��ļ�����
    static bool add_new_file_flag = false;                      // �½��ļ���־λ
    static bool open_file_flag = false;                         // ���ļ���־λ

    static bool complete_delete_file_flag = false;              // ��ȫɾ���ļ���־λ
    static bool delete_file_to_bin_flag = false;                // ɾ���ļ�������վ��־λ
    static bool copy_file_flag = false;                         // �����ļ���־λ
    static bool cut_file_flag = false;                          // �����ļ���־λ
    static bool paste_file_flag = false;                        // ճ���ļ���־λ
    static bool rename_file_flag = false;                       // �������ļ���־λ

    static bool add_now_file_flag = false;                      // ��������ļ���־λ
    static bool download_file_flag = false;                     // �����ļ���־λ


    static TextEditor text_editor(STORAGE_PATH);          // �ı��༭��

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
        // ����ļ��ǿյ��򲻽�����ʾ�������ļ���Ҫ��md��׺���ļ�����ʹ��
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
    // MoZIApp��ʼ��
    void MoziAppInit()
    {


        // ���ݿ�����
        //LOG_INFO("Mysql���ݿ��ʼ������...");
        //MysqlOperate::ConnectDatabase();
        //MysqlOperate::CreateFileDataTable({ DATABASE_FILETABLE_NAME,DATABASE_TEMP_FILETABLE_NAME });
        //MysqlOperate::MysqlData mysql_data_base(DATABASE_USER_NAME, DATABASE_PASSWORD, DATABASE_NAME);
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
        static bool show_set_fonts_popup = false;
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
            // ����
            if (ImGui::MenuItem(HONEPAGENAME_SUBSETTINGS_FONTS, NULL, &show_set_fonts_popup));
            ImGui::EndMenu();
        }


        if (show_set_fonts_popup) Moui::SetFontsPopup(&show_set_fonts_popup);
    }

    // ������
    static void HomePageMainMenuBarWindows()
    {
        if (ImGui::BeginMenu(HONEPAGENAME_WINDOWS))
        {
            // ·������
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_SOURSEPAGE, NULL, &sourse_open);
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_DEBUG_DISPLAY, "F1", &debug_display_open);
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_TEXT_EDITOR, "F2", &text_editor_open);
            ImGui::MenuItem(HONEPAGENAME_SUBWINDOWS_MARKDOWN_DISPLAY, "F3", &markdown_display_open);
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

    // ��ҳ��ݼ�
    static void HomePageHotKeys()
    {
        if (ImGui::IsKeyPressed(ImGuiKey_F1))debug_display_open = !debug_display_open;

    }

    // ��ҳ
	void HomePage(GLFWwindow* glfw_window)
	{
        // ��̬DIP��������Ļ��DIP���趨�����С
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

        // ��ҳ��������
        if (ImGui::BeginMainMenuBar())
        {
            HomePageMainMenuBarSettings();
            HomePageMainMenuBarWindows();
            HomePageMainMenuBarHelp();

            ImGui::EndMainMenuBar();
        }

        // ��ҳ���������
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

        // ��ҳ��ݼ�
        HomePageHotKeys();

        // ����ҳ�����
        if(sourse_open)             SoursePage();               // ��Դ����������
        if(debug_display_open)      DebugDisplayPage();         // debug��ʾҳ��
        if(text_editor_open)        TextEditorPage();           // �ı��༭ҳ��
        if(markdown_display_open)   MarkdownDisplayPage();      // Markdown��ʾҳ��
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
            //if (ImGui::MenuItem(SOURSEPAGE_ICON_REFLASH_FOLDER)) { /*reflash_top_file = true;*/ }

            //if (ImGui::MenuItem(SOURSEPAGE_ICON_EXPAND_FOLDERS)) { /*open_all_file_action = 1;*/ }

            //if (ImGui::MenuItem(SOURSEPAGE_ICON_COLLAPSE_FOLDERS)) { /*open_all_file_action = 0;*/ }

            ImGui::EndMenuBar();
        }


        OpenFolder(STORAGE_PATH, true);
        // ���������
        if (add_now_file_flag)
        {
            FileManage::SelectFile(right_click_get_path);
            add_now_file_flag = false;
        }
        // ���½��ļ��е���
        if (add_new_file_flag)
        {
            // ����Ҫ����µ������ļ�ʱ��ֻ��Ҫ�����switch��д�ϣ�����RightFolderPopup�����н�ͼ����ϣ�
            // ͬʱ��Moui::AddNewFilePopup�����м���һ�����У������ط�����Ҫ�ģ�ǰ�����Լ���FileOperate.h����ӵ����ļ����͵ĺ�׺��
            switch (add_new_file_fileformat)
            {
            case FileOperate::FileFormat_Directory:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"�½��ļ���", FileOperate::FileFormat_Directory, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_TextFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"�½��ı��ļ�", FileOperate::FileFormat_TextFile, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_WordFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"�½�Word�ļ�", FileOperate::FileFormat_WordFile, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_PptFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"�½�PPT�ļ�", FileOperate::FileFormat_PptFile, right_click_get_path, 0.f);
                break;
            case FileOperate::FileFormat_MarkdownFile:
                Moui::AddNewFilePopup(add_new_file_flag,
                    u8"�½�Markdown�ļ�", FileOperate::FileFormat_MarkdownFile, right_click_get_path, 0.f);
                break;
            default:break;
            }
        }
        // ���½��ļ��е���
        if (rename_file_flag)
        {
            Moui::RenameFilePopup(rename_file_flag, u8"�������ļ���", right_click_get_path, 0.f);
        }
        
        // ����ճ��������һ�θ��ƿ�����n��ճ��
        if (copy_file_flag && paste_file_flag)
        {
            Moui::PasteFilePopup(paste_file_flag, u8"����ճ���ļ�", last_click_get_path, right_click_get_path, 0.f, false);
        }
        // ����ճ��������һ�μ���ֻ����һ��ճ��
        if (cut_file_flag && paste_file_flag)
        {
            Moui::PasteFilePopup(paste_file_flag, u8"����ճ���ļ�", last_click_get_path, right_click_get_path, 0.f, true);
            // ��ִ����ճ����ʹ���б�־λ�ָ�����һ�μ���ֻ����һ��ճ��
            if (!paste_file_flag)cut_file_flag = false;
        }

        if (open_file_flag)
        {
            ShellExecute(NULL, L"open", right_click_get_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
            open_file_flag = false;
        }

        // �����ļ�
        if (download_file_flag)
        {
            FileManage::DownloadFile(right_click_get_path);
            download_file_flag = false;
        }

        // ɾ���ļ�
        // ɾ���ļ�������վ
        if (delete_file_to_bin_flag)
        {
            FileManage::DeleteFileToBin(right_click_get_path);
            delete_file_to_bin_flag = false;
        }
        // ����ɾ���ļ�
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

    // �һ��ļ��д��Ӵ�
    static void RightFolderPopup(const char* str_id, ImGuiPopupFlags popup_flags, const std::filesystem::path& current_path, const bool& bin_file_flag)
    {
        // ����ʹ���º���ֱ��һ����λ
        // ImGui::OpenPopupOnItemClick(str_idd.c_str(), popup_flags);   // �����Ӵ�
        // ImGui::BeginPopup(str_idd.c_str());                          // ���Ӵ�
        // �ж��һ����ļ�����Ŀ¼��1ΪĿ¼0Ϊ�ļ�
        bool foler_flag = std::filesystem::is_directory(current_path);

        if (ImGui::BeginPopupContextItem(str_id, popup_flags))
        {
            // ����վ���в���
            if (current_path.generic_u8string() == MOZI_RECYCLE_BIN_PATH)
            {
                // һ�����ɾ��
                if (ImGui::MenuItem(SOURSEPAGE_RECYCLE_BIN_POPUP_DELETE)) 
                {
                    right_click_get_path = MOZI_RECYCLE_BIN_PATH;
                    // ����վ�ڵ��ļ�����ɾ�������ǻ���վ���ļ�ʹ��ɾ���ƶ������վ��
                    complete_delete_file_flag = true;
                    LOG_INFO("һ����ջ���վ���");
                }
            }
            else
            {
                // ֻ�в��ǻ���վ�ڵ��ļ��в��еĲ���
                if (foler_flag && !bin_file_flag)
                {
                    // ��Ӳ���
                    if (ImGui::BeginMenu(SOURSEPAGE_FOLDER_POPUP_ADD))
                    {
                        // ������
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_ADDFILE)) {
                            right_click_get_path = current_path;
                            add_now_file_flag = true;
                        }
                        // �½��ļ���
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEWFOLDER)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_Directory;
                        }
                        // �½��ı��ļ�
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_TEXTFILE)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_TextFile;
                        }
                        // �½�Word�ĵ�
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_WORDFILE)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_WordFile;
                        }
                        // �½�PPT�ļ�
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_PPTFILE)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_PptFile;
                        }
                        // �½�Markdown�ļ�
                        if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_SUBADD_NEW_Markdown)) {
                            right_click_get_path = current_path;
                            add_new_file_flag = true;
                            add_new_file_fileformat = FileOperate::FileFormat_MarkdownFile;
                        }
                        ImGui::EndMenu();
                    }
                }

                // �ļ����ز���
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_DOWNLOAD)) {
                    right_click_get_path = current_path;
                    download_file_flag = true;
                }

                // ֻ���ļ����еĲ���
                if (!foler_flag)
                {
                    // ���ļ�
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_OPEN)) {
                        right_click_get_path = current_path;
                        open_file_flag = true;
                    }
                }

                // ���к͸��Ʋ���ͬһʱ��ֻ�ܴ���һ�������ܹ���
                // ���в���
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_CUT)) {
                    last_click_get_path = current_path;
                    cut_file_flag = true;
                    copy_file_flag = false;
                }
                // ���Ʋ���
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_COPY)) {
                    last_click_get_path = current_path;
                    copy_file_flag = true;
                    cut_file_flag = false;
                }
                // ֻ�и��ƻ������ճ������Ч,�����жϽ����ļ����ļ�����Ŀ¼��������ļ��򲻽���ֻ��Ŀ¼���ܽ���
                // ֻ���ļ����еĲ���
                if (foler_flag)
                {
                    if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_PASTE, 0, false, copy_file_flag || cut_file_flag))
                    {
                        right_click_get_path = current_path;
                        paste_file_flag = true;
                    }
                }

                // ɾ������
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_DELETE)) {
                    right_click_get_path = current_path;
                    // ����վ�ڵ��ļ�����ɾ�������ǻ���վ���ļ�ʹ��ɾ���ƶ������վ��
                    if (bin_file_flag)complete_delete_file_flag = true;
                    else delete_file_to_bin_flag = true;
                }
                // ����������
                if (ImGui::MenuItem(SOURSEPAGE_FOLDER_POPUP_RENAME)) {
                    right_click_get_path = current_path;
                    rename_file_flag = true;
                }
            }    
            ImGui::EndPopup();
        }
    }

    // �ݹ������ļ�
    static void OpenFolder(const std::filesystem::path& parent_path, bool top_flag, const bool& bin_file_flag)
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
  
            RightFolderPopup(folder_only_flag.c_str(), ImGuiPopupFlags_MouseButtonRight, folder_path[i], bin_file_flag);
            // ˫����ȡ��ǰ�ļ��е�·��
            DoubleClickedFile(folder_path[i], double_click_get_path);
            
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
            // �������ڵ�
            ImGui::TreeNodeEx(file_path[i].c_str(), ImGuiTreeNodeFlags_Leaf| ImGuiTreeNodeFlags_Bullet, FileOperate::TreeFileIconConnect(file_path[i]).c_str());
            // ˫���ļ�����
            DoubleClickedFile(file_path[i], double_click_get_path);
            ImGui::TreePop();
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
            RightFolderPopup(recycle_bin_folder.generic_string().c_str(), ImGuiPopupFlags_MouseButtonRight, recycle_bin_folder, 0); // �һ��ļ���
            if (recycle_node_open)
            {
                OpenFolder(recycle_bin_folder, 0, 1);
                ImGui::TreePop();
            }
        }
    }
    
    // ˫���ļ�����
    static void DoubleClickedFile(const std::filesystem::path& current_path, std::filesystem::path& double_click_get_path)
    {
        //static std::filesystem::path last_path = STORAGE_PATH;
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            // �ļ���˫������
            if (std::filesystem::is_directory(current_path))
            {
                
            }
            // �ļ�˫������
            else if (std::filesystem::is_regular_file(current_path))
            {
                double_click_get_path = current_path;
                if ((FileOperate::CheckFileFormat(current_path) == FileOperate::FileFormat_MarkdownFile) ||
                    (FileOperate::CheckFileFormat(current_path) == FileOperate::FileFormat_TextFile))
                {
                    text_editor.SetEditorFilePath(current_path);
                    markdown_display_open = true;       // ˫��markdown�ļ������ȴ���ʾ����
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
//                                  debug��ʾҳ��
//-----------------------------------------------------------------------------

    static void DebugDisplayPage()
    {
        static bool is_display = true;
        static bool no_move = false;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar;

        if (no_move)window_flags |= ImGuiWindowFlags_NoMove;

        // sourse_open Ϊ1ʱ����Դ������ҳ�棬Ϊ0ʱ�ر���Դ������ҳ��
        // ��С������С����
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
            // ��ԭ���б�־λ
            no_move = false;
            return;
        }
        // ������С�Ĵ�СΪ1000*1000
        float win_h = ImGui::GetWindowHeight(), win_w = ImGui::GetWindowWidth();        // ���ڵ�ʵʱ��С
        if (win_h < 400 || win_w < 700)flag_ones = false;

        // ǰ����
        ImGui::TextWrapped(u8"��ӭʹ��MoZI�е�debug��ʾ��������");
        ImGui::Separator();
        //ImGui::SameLine();
        // ������ʹ��
        mde::ddp.Filter.Draw("Filter##DebugDisplayPage", win_w - 70);
        ImGui::Separator();


        // ������ʾ��
        const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        if (ImGui::BeginChild("##DebugDisplayPage_DisplayRegion", ImVec2(0, -footer_height_to_reserve), ImGuiChildFlags_None, ImGuiChildFlags_Border | ImGuiWindowFlags_HorizontalScrollbar))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            for (const char* item : mde::ddp.Items)
            {
                // ������ʹ��
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

        // ��ť��
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

