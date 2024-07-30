#include "mopch.h"
#include "FileManage.h"
#include "MoIcon.h"
#include "Log.h"



namespace FileManage {

    // 文件格式的映射
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

    // 初始化FolderMap
    std::map<std::filesystem::path, bool> FolderMap::folder_map;


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

    // 检查文件类型
    FileFormat CheckFileType(const std::filesystem::path& file_path)
    {
        std::filesystem::path file_extension = file_path.extension();
        if (file_format.count(file_extension))
            return file_format.at(file_extension);
        return FileFormat_UnknownFile;
    }

    // 设置对应文件夹前面的图标
    const std::string TreeFileIconConnect(const std::filesystem::path& file_path)
    {
        //// 计划使用的图标
        //MoZi::ObjectType object_type = MzMysql::CheckObjectType(file_path);
        //if (object_type == MoZi::ObjectType_IsPlan)
        //    return IconAndChinese(FILETREE_ICON_PLAN, file_path.filename().generic_u8string(), 1);

        // 其他文件使用图标
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

	// 扫描文件夹函数，返回其下第一层的文件夹路径和文件路径
	void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path, const std::filesystem::path& current_path)
	{
		// 清空存储
		folder_path.clear(); file_path.clear();

		if (!std::filesystem::exists(current_path))return;

		if (std::distance(std::filesystem::directory_iterator(current_path), std::filesystem::directory_iterator{}) == 0)return;

		std::filesystem::directory_iterator path_list(current_path);	        //文件入口容器
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
        if (folder_map.count(current_path) == 0)          // 判断map中是否有该节点,如果没有则添加进去
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("添加新文件夹至FolderMap中：{}", current_path.generic_string());
            return;
        }
        LOG_WARN("无文件夹可添加");
        return;
    }

    void FolderMap::DeleteFolder(const std::filesystem::path& current_path)
    {
        if (folder_map.count(current_path) == 0)          // 判断map中是否有该节点,如果没有则添加进去
        {
            folder_map.erase(current_path);
            LOG_INFO("从FolderMap删除文件夹：{}", current_path.generic_string());
            return;
        }
        LOG_WARN("无文件夹可删除");
        return;
    }

    void FolderMap::LookFolderMap()
    {
        if (folder_map.empty())
        {
            LOG_INFO("FolderMap为空，无存储");
            return;
        }
        else
        {
            LOG_INFO("查看FolderMap存储内容...");
            for (std::map<std::filesystem::path, bool>::iterator it = folder_map.begin(); it != folder_map.end(); ++it) 
            {
                if (it->second == false) LOG_INFO("文件开关状态：关闭，文件路径：{}", it->first.generic_string());
                else if(it->second == true) LOG_INFO("文件开关状态：打开，文件路径：{}", it->first.generic_string());
            }
            return;
        }
        return;
    }

    const std::filesystem::path& FolderMap::BuildFolderTree(const std::filesystem::path& current_path, const bool& tree_node_open)
    {
        // 初始化上一个路径
        static std::filesystem::path last_path = STORAGE_PATH;
        // 判断map中是否有该节点,如果没有则添加进去
        if (folder_map.count(current_path) == 0)          
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("文件树建立__新添加文件夹：{}", current_path.generic_string());
            return last_path;
        }
        // 如果该节点存在，则判断该节点上一个tree_node_open状态与当前的是否一样
        else           
        {
            // 一样则说明没有进行操作，返回上一个操作的路径
            if (folder_map.at(current_path) == tree_node_open)
            {
                return last_path;
            }
            // 其实可以用一个异或来进行判断的，但是为了区别打开和关闭的操作用了两个if
            // 状态由0->1即文件打开
            else if (folder_map.at(current_path) == 0 && tree_node_open == 1)   
            {
                last_path = current_path;                       // 刷新上一个文件夹路径
                folder_map.at(current_path) = tree_node_open;   // 更新打开关闭节点
                LOG_INFO("文件夹打开，抓取到当前文件夹路径为：{}", current_path.generic_string());
                return current_path;
            }
            // 状态由1->0即文件打开
            else if (folder_map.at(current_path) == 1 && tree_node_open == 0)
            {
                last_path = current_path;                       // 刷新上一个文件夹路径
                folder_map.at(current_path) = tree_node_open;   // 更新打开关闭节点
                LOG_INFO("文件夹关闭，抓取到当前文件夹路径为：{}", current_path.generic_string());
                return current_path;
            }
        }

        LOG_CRITICAL("文件数建立失败");
        return last_path;
    }
}