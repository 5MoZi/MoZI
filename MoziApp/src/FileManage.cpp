#include "mopch.h"
#include "FileManage.h"
#include "MoIcon.h"
#include "Log.h"



namespace FileManage {

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

    // ��ʼ��FolderMap
    std::map<std::filesystem::path, bool> FolderMap::folder_map;


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
        FileManage::FileFormat file_format = FileManage::CheckFileType(file_path);
        switch (file_format)
        {
        case FileManage::FileFormat_UnknownFile:
            return IconAndChinese(FILETREE_ICON_NOFILE, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_Directory:
            return IconAndChinese(FILETREE_ICON_FOLDER, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_TextFile:
            return IconAndChinese(FILETREE_ICON_TEXTFILE, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_WordFile:
            return IconAndChinese(FILETREE_ICON_WORD, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_PdfFile:
            return IconAndChinese(FILETREE_ICON_PDF, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_PngFile:
            return IconAndChinese(FILETREE_ICON_PNG, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_JpgFile:
            return IconAndChinese(FILETREE_ICON_JPG, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_XlsxFile:
            return IconAndChinese(FILETREE_ICON_XLSX, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_PptFile:
            return IconAndChinese(FILETREE_ICON_PPT, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_Mp3File:
            return IconAndChinese(FILETREE_ICON_VOICE, file_path.filename().generic_u8string(), 1);
        case FileManage::FileFormat_Mp4File:
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



    void FolderMap::AddFolder(const std::filesystem::path& current_path, const bool& tree_node_open)
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

    void FolderMap::DeleteFolder(const std::filesystem::path& current_path)
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
                else if(it->second == true) LOG_INFO("�ļ�����״̬���򿪣��ļ�·����{}", it->first.generic_string());
            }
            return;
        }
        return;
    }

    const std::filesystem::path& FolderMap::BuildFolderTree(const std::filesystem::path& current_path, const bool& tree_node_open)
    {
        // ��ʼ����һ��·��
        static std::filesystem::path last_path = STORAGE_PATH;
        // �ж�map���Ƿ��иýڵ�,���û������ӽ�ȥ
        if (folder_map.count(current_path) == 0)          
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("�ļ�������__������ļ��У�{}", current_path.generic_string());
            return last_path;
        }
        // ����ýڵ���ڣ����жϸýڵ���һ��tree_node_open״̬�뵱ǰ���Ƿ�һ��
        else           
        {
            // һ����˵��û�н��в�����������һ��������·��
            if (folder_map.at(current_path) == tree_node_open)
            {
                return last_path;
            }
            // ��ʵ������һ������������жϵģ�����Ϊ������򿪺͹رյĲ�����������if
            // ״̬��0->1���ļ���
            else if (folder_map.at(current_path) == 0 && tree_node_open == 1)   
            {
                last_path = current_path;                       // ˢ����һ���ļ���·��
                folder_map.at(current_path) = tree_node_open;   // ���´򿪹رսڵ�
                LOG_INFO("�ļ��д򿪣�ץȡ����ǰ�ļ���·��Ϊ��{}", current_path.generic_string());
                return current_path;
            }
            // ״̬��1->0���ļ���
            else if (folder_map.at(current_path) == 1 && tree_node_open == 0)
            {
                last_path = current_path;                       // ˢ����һ���ļ���·��
                folder_map.at(current_path) = tree_node_open;   // ���´򿪹رսڵ�
                LOG_INFO("�ļ��йرգ�ץȡ����ǰ�ļ���·��Ϊ��{}", current_path.generic_string());
                return current_path;
            }
        }

        LOG_CRITICAL("�ļ�������ʧ��");
        return last_path;
    }
}