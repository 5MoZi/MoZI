#include "mopch.h"
#include "FileOperate.h"
#include "Log.h"
#include "MoIcon.h"
#include "MoObject.h"

namespace FileOperate {
    //-----------------------------------------------------------------------------
    //                               file��������
    //-----------------------------------------------------------------------------
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
    // �ļ���ʽ��ӳ��
    const static std::map<std::filesystem::path, FileFormat> file_format
    {
        {"",FileFormat_Directory},
        {".txt",FileFormat_TextFile},
        {".docx",FileFormat_WordFile},
        {".pdf",FileFormat_PdfFile},
        {".png",FileFormat_PngFile},
        {".jpg",FileFormat_JpgFile},
        {".xlsx",FileFormat_XlsxFile},
        {".pptx",FileFormat_PptFile},
        {".mp3",FileFormat_Mp3File},
        {".mp4",FileFormat_Mp4File},
    };

    // ����ļ�����
    FileOperate::FileFormat CheckFileFormat(const std::filesystem::path& file_path)
    {
        std::filesystem::path file_extension = file_path.extension();
        if (file_format.count(file_extension))
            return file_format.at(file_extension);
        return FileFormat::FileFormat_UnknownFileFormat;
    }

    // ����ļ�����
    const std::string CheckFileExtension(const FileOperate::FileFormat& file_format)
    {
        switch (file_format)
        {
        case FileOperate::FileFormat_Directory:	return FILE_EXTENSION_FOLDER;
        case FileOperate::FileFormat_TextFile:	return FILE_EXTENSION_TXT; 
        case FileOperate::FileFormat_WordFile:	return FILE_EXTENSION_WORD;
        case FileOperate::FileFormat_PptFile:	return FILE_EXTENSION_PPT;
        default: break;
        }
    }
    // ���ö�Ӧ�ļ���ǰ���ͼ��
    const std::string TreeFileIconConnect(const std::filesystem::path& file_path)
    {
        // �ļ�ʹ��ͼ��
        FileOperate::FileFormat file_format = FileOperate::CheckFileFormat(file_path);
        switch (file_format)
        {
        case FileOperate::FileFormat_UnknownFileFormat:
            return IconAndChinese(FILETREE_ICON_NOFILE, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_Directory:
            return IconAndChinese(FILETREE_ICON_FOLDER, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_TextFile:
            return IconAndChinese(FILETREE_ICON_TEXTFILE, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_WordFile:
            return IconAndChinese(FILETREE_ICON_WORD, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_PdfFile:
            return IconAndChinese(FILETREE_ICON_PDF, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_PngFile:
            return IconAndChinese(FILETREE_ICON_PNG, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_JpgFile:
            return IconAndChinese(FILETREE_ICON_JPG, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_XlsxFile:
            return IconAndChinese(FILETREE_ICON_XLSX, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_PptFile:
            return IconAndChinese(FILETREE_ICON_PPT, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_Mp3File:
            return IconAndChinese(FILETREE_ICON_VOICE, file_path.filename().generic_u8string(), 1);
        case FileOperate::FileFormat_Mp4File:
            return IconAndChinese(FILETREE_ICON_VIDEO, file_path.filename().generic_u8string(), 1);
        }
        return "";
    }

    // ɨ���ļ��к������������µ�һ����ļ���·�����ļ�·��
    void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path, const std::filesystem::path& current_path)
    {
        // ��մ洢
        folder_path.clear(); file_path.clear();

        if (!std::filesystem::exists(current_path))return;

        if (std::distance(std::filesystem::directory_iterator(current_path), std::filesystem::directory_iterator{}) == 0)return;

        std::filesystem::directory_iterator path_list(current_path);	        //�ļ��������
        for (auto const& dir_entry : path_list)
        {
            if (std::filesystem::is_directory(dir_entry.path()))
            {
                folder_path.push_back(dir_entry.path());
            }
            else if (std::filesystem::is_regular_file(dir_entry.path()))
            {
                file_path.push_back(dir_entry.path());
            }
        }
    }

    //UTF8�ַ���תstring
    std::string UTF8_To_string(const std::string& str)
    {
        int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

        wchar_t* pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
        memset(pwBuf, 0, nwLen * 2 + 2);

        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, (size_t)nwLen);

        int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

        char* pBuf = new char[nLen + 1];
        memset(pBuf, 0, nLen + 1);

        WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

        std::string retStr = pBuf;

        delete[]pBuf;
        delete[]pwBuf;

        pBuf = NULL;
        pwBuf = NULL;

        return retStr;
    }

    std::string string_To_UTF8(const std::string& str)
    {
        int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

        wchar_t* pwBuf = new wchar_t[nwLen + 1];//һ��Ҫ��1����Ȼ�����β��
        ZeroMemory(pwBuf, nwLen * 2 + 2);

        ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

        int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

        char* pBuf = new char[nLen + 1];
        ZeroMemory(pBuf, nLen + 1);

        ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

        std::string retStr(pBuf);

        delete[]pwBuf;
        delete[]pBuf;

        pwBuf = NULL;
        pBuf = NULL;

        return retStr;
    }

//-----------------------------------------------------------------------------
//                               FolderMap��ز���
//-----------------------------------------------------------------------------
    // ��ʼ��FolderMap
    std::map<std::filesystem::path, bool> FolderMap::folder_map;

    void FolderMap::AddFolderPath(const std::filesystem::path& current_path, const bool& tree_node_open)
    {
        if (folder_map.count(current_path) == 0)          // �ж�map���Ƿ��иýڵ�,���û������ӽ�ȥ
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("AddFolderPath��������ļ���FolderMap�У�{}", current_path.generic_string());
            return;
        }
        LOG_WARN("AddFolderPath���ļ��Ѵ��ڣ��������");
        return;
    }

    void FolderMap::DeleteFolderPath(const std::filesystem::path& current_path)
    {
        if (folder_map.count(current_path) != 0)          // �ж�map���Ƿ��иýڵ�
        {
            folder_map.erase(current_path);
            LOG_INFO("��FolderMapɾ���ļ���ַ��{}", current_path.generic_string());
            return;
        }
        LOG_WARN("��FolderMap�����ļ��п�ɾ����{}", current_path.generic_string());
        return;
    }


    void FolderMap::ChangeFolderPath(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (folder_map.count(old_path) == 0)          // �ж�map���Ƿ��иýڵ�,���û������ӽ�ȥ
        {
            LOG_ERROR("��FolderMap�в����ڸ�·�����޸�ʧ�ܣ�path��{}", old_path.generic_string());
            return;
        }
        else
        {
            const bool tree_node_open = folder_map.at(old_path);
            folder_map.erase(old_path);
            folder_map.insert({ new_path ,tree_node_open });
            LOG_INFO("��FolderMap���޸��ļ�·���ɹ�path��{0}->{1}", old_path.generic_string(), new_path.generic_string());
            return;
        }
        LOG_ERROR("��FolderMap���޸��ļ�·��ʧ��");
        return;
    }

    void FolderMap::LookFolderMap()
    {
        if (folder_map.empty())
        {
            LOG_INFO("FolderMapΪ�գ��޴洢");
            return;
        }
        else
        {
            LOG_INFO("�鿴FolderMap�洢����...");
            for (std::map<std::filesystem::path, bool>::iterator it = folder_map.begin(); it != folder_map.end(); ++it)
            {
                if (it->second == false) LOG_INFO("�ļ�����״̬���رգ��ļ�·����{}", it->first.generic_string());
                else if (it->second == true) LOG_INFO("�ļ�����״̬���򿪣��ļ�·����{}", it->first.generic_string());
            }
            return;
        }
        return;
    }

    void FolderMap::BuildFolderTree(const std::filesystem::path& current_path, const bool& tree_node_open, std::filesystem::path& double_click_get_path)
    {
        // ��ʼ����һ��·��
        static std::filesystem::path last_path = STORAGE_PATH;
        // �ж�map���Ƿ��иýڵ�,���û������ӽ�ȥ
        if (folder_map.count(current_path) == 0)
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("BuildFolderTree���ļ���������������ļ��У�{}", current_path.generic_string());
            double_click_get_path = last_path;
            return;
        }
        // ����ýڵ���ڣ����жϸýڵ���һ��tree_node_open״̬�뵱ǰ���Ƿ�һ��
        else
        {
            // һ����˵��û�н��в�����������һ��������·��
            if (folder_map.at(current_path) == tree_node_open)
            {
                double_click_get_path = last_path;
                return;
            }
            // ��ʵ������һ������������жϵģ�����Ϊ������򿪺͹رյĲ�����������if
            // ״̬��0->1���ļ���
            else if (folder_map.at(current_path) == 0 && tree_node_open == 1)
            {
                last_path = current_path;                       // ˢ����һ���ļ���·��
                folder_map.at(current_path) = tree_node_open;   // ���´򿪹رսڵ�
                LOG_INFO("BuildFolderTree���ļ��д򿪣�ץȡ����ǰ�ļ���·��Ϊ��{}", current_path.generic_string());
                double_click_get_path = current_path;
                // ˫�����ļ�
                if (!std::filesystem::is_directory(current_path))
                {
                    ShellExecute(NULL, L"open", current_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
                }
                return;
            }
            // ״̬��1->0���ļ���
            else if (folder_map.at(current_path) == 1 && tree_node_open == 0)
            {
                last_path = current_path;                       // ˢ����һ���ļ���·��
                folder_map.at(current_path) = tree_node_open;   // ���´򿪹رսڵ�
                LOG_INFO("BuildFolderTree���ļ��йرգ�ץȡ����ǰ�ļ���·��Ϊ��{}", current_path.generic_string());
                double_click_get_path = current_path;
                // ˫�����ļ�
                if (!std::filesystem::is_directory(current_path))
                {
                    ShellExecute(NULL, L"open", current_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
                }
                return;
            }
        }

        LOG_CRITICAL("BuildFolderTree���ļ�������ʧ��");
        double_click_get_path = last_path;
        return;
    }


//-----------------------------------------------------------------------------
//                   �ļ�����(ճ��)�����С�ɾ�����½�������ز���
//-----------------------------------------------------------------------------

    // ·���Ƿ���ڼ��
    // ���룺·�����������Լ��һȺ·��
    // ���أ�false��ֻҪ��һ��·��������
    //       true: ����·��������
    bool PathCheck(const std::vector<std::filesystem::path>& file_path)
    {
        bool PathExists = true;
        for (auto it = file_path.begin(); it != file_path.end(); it++)
        {
            if (!std::filesystem::exists(*it))
            {
                LOG_WARN("PathCheck��·��������,path:{}", (*it).generic_string());
                PathExists = false;
            }
        }
        if (PathExists)
        {
            LOG_INFO("PathCheck�����·������");
            return true;
        }
        else return false;
    }

    // �������������������
    // 1����file_path�����ַŵ�target_pathĿ¼�½��м��
    // 2����file_path�����ַŵ�file_path�ĸ�Ŀ¼�½��м��
    //    ����Ҫע��file_path�����ֲ��ܺ�ԭ����һ����һ��Ҫ
    //    ע�⣬��ΪҲ�ᷢ��������
    // ���룺 file_path��������·�����ļ���������չ����
    //        target_path����Ҫ������Ŀ¼��һ��Ҫ��������·������������2�������file_path=target_path
    //        parent_check_flag��1Ϊ���2��0Ϊ���1
    // ����ֵ��
    //      true��   ��⵽������
    //      false��  δ��⵽������
    bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path, const bool& parent_check_flag)
    {
        std::filesystem::path file_to_target_path = target_path;
        // �ж�������������µļ��parent_check_flag=1���ڵڶ�������¼��
        // Ĭ�����ڵ�һ������¼��
        if (parent_check_flag)
            file_to_target_path = target_path.parent_path() / file_path.filename();
        else
            file_to_target_path /= file_path.filename();

        if (std::filesystem::exists(file_to_target_path))
        {
            if (parent_check_flag)
                LOG_INFO("�ļ�����{0}����·����{1}�д���ͬ���ļ�", file_path.filename().generic_string(), target_path.parent_path().generic_string());
            else
                LOG_INFO("�ļ�����{0}����·����{1}�д���ͬ���ļ�", file_path.filename().generic_string(), target_path.generic_string());
            return true;
        }
        else
        {
            LOG_INFO("RenameCheck��δ����������");
            return false;
        }
    }

    // �½��ļ���
    // ���룺 file_name���������ļ���������չ������������ļ�����Բ���
    //        target_path��Ҫ���½��ļ����ļ���·����һ��Ҫ���ļ���
    //        forced_flag��������������1ǿ�ƽ����ļ������ֻ���ϵͳ���и��ģ��ӡ���������
    //                     0 ��ǿ�ƽ���������һ��Ҫȷ�������file_name��target_path
    //                     �ļ����²�����������
    // ����ֵ��
    //      �����½��ļ��е�����·��
    std::filesystem::path AddFile(const std::filesystem::path& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
    {
        std::filesystem::path new_file_name = file_name.stem();
        std::filesystem::path new_file_extension = file_name.extension();
        if (forced_flag)
        {
            if (new_file_extension == FILE_EXTENSION_FOLDER)        LOG_INFO("AddFile��ǿ���½��ļ���");
            else if (new_file_extension == FILE_EXTENSION_TXT)      LOG_INFO("AddFile��ǿ���½��ı��ļ�");
            else if (new_file_extension == FILE_EXTENSION_WORD)     LOG_INFO("AddFile��ǿ���½�Word�ĵ�");
            else if (new_file_extension == FILE_EXTENSION_PPT)      LOG_INFO("AddFile��ǿ���½�PPT�ļ�");
            while (RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), target_path))
            {
                new_file_name += "-����";
            }
        }
        std::filesystem::path new_path = target_path / (new_file_name.generic_string() + new_file_extension.generic_string());
        if (new_file_extension == FILE_EXTENSION_FOLDER)
        {
            std::filesystem::create_directory(new_path);
            LOG_INFO("AddFile���½��ļ��гɹ�");
        }
        else
        {
            std::ofstream in(new_path, std::ios::out);
            if (new_file_extension == FILE_EXTENSION_TXT)           LOG_INFO("AddFile���½��ı��ļ��ɹ�");
            else if (new_file_extension == FILE_EXTENSION_WORD)     LOG_INFO("AddFile���½�Word�ĵ��ɹ�");
            else if (new_file_extension == FILE_EXTENSION_PPT)      LOG_INFO("AddFile���½�PPT�ļ��ɹ�");
        }
        return new_path;
    }

    // ճ���ļ�
    // ���룺 from_path��������·������Ҫճ�����ļ�
    //        to_path��Ҫ���ļ����ļ���·����һ��Ҫ���ļ���
    // �ú��������ļ�·���Լ��ļ���������⣬���һ��Ҫȷ����
    // ���from_path��target_path�ļ����²�����������
    // ����ֵ��
    //      true��   ճ���ɹ�
    //      false��  ճ��ʧ��
    std::filesystem::path PasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path, const bool& forced_flag)
    {
        LOG_INFO("PasteFile��ճ���ļ���......");
        std::filesystem::path new_file_name = from_path.stem();
        std::filesystem::path new_file_extension = from_path.extension();
        if (forced_flag == true)
        {
            LOG_INFO("PasteFile��ǿ����������......");
            while (RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), to_path))
            {
                new_file_name += "-����";
            }
        }
        std::filesystem::path new_path = from_path.parent_path() / (new_file_name.generic_string() + new_file_extension.generic_string());
        if (std::filesystem::is_directory(from_path))
        {
            // ִ���ļ��и��Ʋ���
            std::filesystem::copy(from_path, to_path / new_path.filename(), std::filesystem::copy_options::recursive);
            
            LOG_INFO("PasteFile���ɹ�ճ���ļ��� from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
        }
        else if (std::filesystem::is_regular_file(from_path))
        {
            // ִ���ļ����Ʋ���
            std::filesystem::copy(from_path, to_path / new_path.filename());
            LOG_INFO("PasteFile���ɹ�ճ���ļ� from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
        }
        return new_path;
    }

    // ��ȫɾ���ļ�
    // ���룺 filepath��Ҫɾ�����ļ�·��
    // �ú��������ļ�·���Ƿ���ڼ�⣬���һ��Ҫȷ����
    // ���filepath·������
    // ����ֵ��
    //      true��   ��ȫɾ���ɹ�
    //      false��  ��ȫɾ��ʧ��
    bool CompleteDeleteFile(const std::filesystem::path& filepath)
    {
        if (std::filesystem::is_directory(filepath))
        {
            std::filesystem::remove_all(filepath);
            LOG_INFO("CompleteDeleteFile���ɹ�ɾ���ļ��� path:{}", filepath.generic_string());
            return true;
        }
        else if (std::filesystem::is_regular_file(filepath))
        {
            std::filesystem::remove(filepath);
            LOG_INFO("CompleteDeleteFile���ɹ�ɾ���ļ� path:{}", filepath.generic_string());
            return true;
        }

        LOG_ERROR("CompleteDeleteFile��ɾ��ʧ�ܣ�");
        return false;
    }
    void DeleteFolderOrFile(const std::filesystem::path& filepath)
    {
        if (!PathCheck({ filepath }))
        {
            LOG_ERROR("����·�������ڣ�����ɾ������ʧ�ܣ�");
            return;
        }
        if (std::filesystem::is_directory(filepath))
        {
            std::filesystem::remove_all(filepath);
            LOG_INFO("�ɹ�ɾ���ļ��� path:{}", filepath.generic_string());
            return;
        }
        else if (std::filesystem::is_regular_file(filepath))
        {
            std::filesystem::remove(filepath);
            LOG_INFO("�ɹ�ɾ���ļ� path:{}", filepath.generic_string());
            return;
        }

        LOG_ERROR("ɾ������ʧ�ܣ�");
        return;
    }

    // �������ļ�
    // ע��replace_filename����������ֺ���չ������ȫ���滻
    // �ú���ִ��ǿ�������������������������м�⣬��ⲿ����Ҫ�Լ�����
    // 
    // �ú������Զ�����Ƿ������������������ϵͳ������
    // 
    // ���룺 rename���������ļ���������չ������ͬʱ��Ҫע��������µ�rename������ɵ�old_path���ļ���һ����������û������
    //        old_path���͵��ļ�·��
    // ����ֵ��
    //      ��������������ļ�����·��
    std::filesystem::path RenameFile(const std::filesystem::path& rename, const std::filesystem::path& old_path)
    {
        LOG_INFO("RenameFile��������������......");
        std::filesystem::path new_file_path = old_path;
        std::filesystem::path new_file_name = rename.stem();
        std::filesystem::path new_file_extension = rename.extension();

        std::filesystem::path file_to_target_path = old_path.parent_path() / rename;

        if (std::filesystem::exists(file_to_target_path))
        {
            LOG_INFO("RenameFile:�ļ������������ļ�");

            while (std::filesystem::exists(file_to_target_path.parent_path() / (new_file_name.generic_string() + new_file_extension.generic_string())))
            {
                new_file_name += "-����";
            }
        }
        new_file_path.replace_filename(new_file_name.generic_string() + new_file_extension.generic_string());
        std::filesystem::rename(old_path, new_file_path);
        LOG_INFO("RenameFile�������������ɹ�");
        return new_file_path;
    }

}
