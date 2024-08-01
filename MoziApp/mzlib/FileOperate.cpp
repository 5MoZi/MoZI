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
    FileFormat CheckFileType(const std::filesystem::path& file_path)
    {
        std::filesystem::path file_extension = file_path.extension();
        if (file_format.count(file_extension))
            return file_format.at(file_extension);
        return FileFormat_UnknownFile;
    }

    // ���ö�Ӧ�ļ���ǰ���ͼ��
    const std::string TreeFileIconConnect(const std::filesystem::path& file_path)
    {
        //// �ƻ�ʹ�õ�ͼ��
        //MoZi::ObjectType object_type = MzMysql::CheckObjectType(file_path);
        //if (object_type == MoZi::ObjectType_IsPlan)
        //    return IconAndChinese(FILETREE_ICON_PLAN, file_path.filename().generic_u8string(), 1);

        // �����ļ�ʹ��ͼ��
        FileOperate::FileFormat file_format = FileOperate::CheckFileType(file_path);
        switch (file_format)
        {
        case FileOperate::FileFormat_UnknownFile:
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
            LOG_INFO("������ļ�����FolderMap�У�{}", current_path.generic_string());
            return;
        }
        LOG_WARN("���ļ��п����");
        return;
    }

    void FolderMap::DeleteFolderPath(const std::filesystem::path& current_path)
    {
        if (folder_map.count(current_path) == 0)          // �ж�map���Ƿ��иýڵ�,���û������ӽ�ȥ
        {
            folder_map.erase(current_path);
            LOG_INFO("��FolderMapɾ���ļ��У�{}", current_path.generic_string());
            return;
        }
        LOG_WARN("���ļ��п�ɾ��");
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
            LOG_INFO("�ļ�������__������ļ��У�{}", current_path.generic_string());
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
                LOG_INFO("�ļ��д򿪣�ץȡ����ǰ�ļ���·��Ϊ��{}", current_path.generic_string());
                double_click_get_path = current_path;
                return;
            }
            // ״̬��1->0���ļ���
            else if (folder_map.at(current_path) == 1 && tree_node_open == 0)
            {
                last_path = current_path;                       // ˢ����һ���ļ���·��
                folder_map.at(current_path) = tree_node_open;   // ���´򿪹رսڵ�
                LOG_INFO("�ļ��йرգ�ץȡ����ǰ�ļ���·��Ϊ��{}", current_path.generic_string());
                double_click_get_path = current_path;
                return;
            }
        }

        LOG_CRITICAL("�ļ�������ʧ��");
        double_click_get_path = last_path;
        return;
    }

//-----------------------------------------------------------------------------
//                   �ļ�����(ճ��)�����С�ɾ�����½�������ز���
//-----------------------------------------------------------------------------

    // ·���Ƿ���ڼ��
    bool PathCheck(const std::vector<std::filesystem::path>& file_path)
    {
        bool PathExists = true;
        for (auto it = file_path.begin(); it != file_path.end(); it++)
        {
            if (!std::filesystem::exists(*it))
            {
                LOG_WARN("·��������,path:{}", (*it).generic_string());
                PathExists = false;
            }
        }
        if (PathExists)
        {
            LOG_INFO("�������·��������");
            return true;
        }
        else return false;
    }
    // �½��ļ���
    void AddFolder(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
    {
        std::string new_file_name = file_name;
        if (forced_flag)
        {
            LOG_INFO("AddFolder:ǿ���½��ļ���");
            while (RenameCheck(new_file_name, target_path))
            {
                new_file_name += "-����";
            }
        }
        std::filesystem::path new_path = target_path / new_file_name;
        std::filesystem::create_directory(new_path);
        LOG_INFO("�½��ļ��гɹ�");
        return;
    }
    
    // �½��ı��ļ�
    void AddTextFile(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
    {
        std::string new_file_name = file_name;
        if (forced_flag)
        {
            LOG_INFO("ǿ���½��ı��ļ�");
            while (RenameCheck(new_file_name, target_path))
            {
                new_file_name += "-����";
            }
        }
        std::filesystem::path new_path = target_path / (file_name + ".txt");
        std::ofstream in(new_path, std::ios::out);
        LOG_INFO("�½��ı��ļ��ɹ�");
        return;
    }

    // ���������
    // ����false������������Ҳ���������������ʧ�ܣ�����true˵������������
    bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path)
    {
        if (!PathCheck({ file_path,target_path }))
        {
            LOG_ERROR("����·�������ڣ��������������ʧ�ܣ�");
            return false;
        }

        std::filesystem::path file_to_target_path = target_path;
        std::filesystem::rename(file_to_target_path, file_path);
        if (std::filesystem::exists(file_to_target_path))
        {
            LOG_INFO("�ļ�����{0}����·����{1}�д���ͬ���ļ�", file_path.filename().generic_string(), target_path.generic_string());
            return true;
        }
        else
        {
            LOG_INFO("δ����������");
            return false;
        }
        LOG_ERROR("���������ʧ�ܣ�");
        return false;
    }
    // �������������
    bool RenameCheck(const std::string& rename, const std::filesystem::path& target_path)
    {
        if (!PathCheck({ target_path }))
        {
            LOG_ERROR("����·�������ڣ��������������ʧ�ܣ�");
            return false;
        }
        std::cout << "2" << std::endl;
        //std::cout << target_path / rename << std::endl;
        std::filesystem::path file_to_target_path = target_path;
        std::cout << file_to_target_path << std::endl;
        std::cout << rename << std::endl;
        file_to_target_path /= rename;
        std::cout << "1" << std::endl;
        //std::cout << file_to_target_path << std::endl;
        //file_to_target_path /= rename;
        std::cout << file_to_target_path << std::endl;

        //std::filesystem::path file_to_target_path = target_path.parent_path();
        //std::cout << "2" << std::endl;
        //std::cout << file_to_target_path << std::endl;
        //file_to_target_path /= rename;
        //std::cout << file_to_target_path << std::endl;
        if (std::filesystem::exists(file_to_target_path))
        {
            LOG_INFO("�ļ�����{0}����·����{1}�д���ͬ���ļ�", rename, target_path.generic_string());
            return true;
        }
        else
        {
            LOG_INFO("δ����������");
            return false;
        }
        LOG_ERROR("���������ʧ�ܣ�");
        return false;
    }
    // ����ճ���ļ����ļ���
    void CopyFolderAndFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path)
    {
        if (!PathCheck({ from_path,to_path }))
        {
            LOG_ERROR("����·�������ڣ����¸��Ʋ���ʧ�ܣ�");
            return;
        }
        if (std::filesystem::is_directory(to_path))
        {
            // �������������
            LOG_INFO("����ճ�����������������...");
            if (!RenameCheck(from_path, to_path))
            {
                if (std::filesystem::is_directory(from_path))
                {
                    // ִ���ļ��и��Ʋ���
                    std::filesystem::copy(from_path, to_path, std::filesystem::copy_options::recursive
                        | std::filesystem::copy_options::update_existing);
                    LOG_INFO("�ɹ������ļ��� from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
                    return;
                }
                else if (std::filesystem::is_regular_file(from_path))
                {
                    // ִ���ļ����Ʋ���
                    std::filesystem::copy(from_path, to_path);
                    LOG_INFO("�ɹ������ļ� from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
                    return;
                }
            } 
            else
            {
                LOG_INFO("�����������ļ�");
                return;
            }
        }
        else
        {
            LOG_WARN("���Ƶ���·�������ļ��� path:{}", to_path.generic_string());
            return;
        }
        LOG_ERROR("���Ʋ���ʧ�ܣ�");
        return;
    }
    // ��ȫɾ���ļ�
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
    // �������ļ��л��ļ�
    // ע������Ƕ��ļ����и��������renameһ��Ҫ����չ����
    // ��Ϊreplace_filename����������ֺ���չ������ȫ���滻
    // �ú���ִ��ǿ�������������������������м�⣬��ⲿ����Ҫ�Լ�����
    void RenameFile(const std::string& rename, std::filesystem::path& old_path)
    {
        if (!PathCheck({ old_path }))
        {
            LOG_ERROR("����·�������ڣ�����������ʧ�ܣ�");
            return;
        }
        old_path.replace_filename(rename);
        LOG_INFO("�����������ɹ���");
        return;
    }

}
