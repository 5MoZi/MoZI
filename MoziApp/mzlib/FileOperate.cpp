#include "mopch.h"
#include "FileOperate.h"
#include "Log.h"
#include "MoIcon.h"
#include "MoObject.h"

namespace FileOperate {
    //-----------------------------------------------------------------------------
    //                               file基础操作
    //-----------------------------------------------------------------------------
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

    // 检查文件类型
    FileOperate::FileFormat CheckFileFormat(const std::filesystem::path& file_path)
    {
        std::filesystem::path file_extension = file_path.extension();
        if (file_format.count(file_extension))
            return file_format.at(file_extension);
        return FileFormat::FileFormat_UnknownFileFormat;
    }

    // 检查文件类型
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
    // 设置对应文件夹前面的图标
    const std::string TreeFileIconConnect(const std::filesystem::path& file_path)
    {
        // 文件使用图标
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

    //UTF8字符串转string
    std::string UTF8_To_string(const std::string& str)
    {
        int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

        wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
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

        wchar_t* pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
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
//                               FolderMap相关操作
//-----------------------------------------------------------------------------
    // 初始化FolderMap
    std::map<std::filesystem::path, bool> FolderMap::folder_map;

    void FolderMap::AddFolderPath(const std::filesystem::path& current_path, const bool& tree_node_open)
    {
        if (folder_map.count(current_path) == 0)          // 判断map中是否有该节点,如果没有则添加进去
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("AddFolderPath：添加新文件至FolderMap中：{}", current_path.generic_string());
            return;
        }
        LOG_WARN("AddFolderPath：文件已存在，无需添加");
        return;
    }

    void FolderMap::DeleteFolderPath(const std::filesystem::path& current_path)
    {
        if (folder_map.count(current_path) != 0)          // 判断map中是否有该节点
        {
            folder_map.erase(current_path);
            LOG_INFO("从FolderMap删除文件地址：{}", current_path.generic_string());
            return;
        }
        LOG_WARN("在FolderMap中无文件夹可删除：{}", current_path.generic_string());
        return;
    }


    void FolderMap::ChangeFolderPath(const std::filesystem::path& old_path, const std::filesystem::path& new_path)
    {
        if (folder_map.count(old_path) == 0)          // 判断map中是否有该节点,如果没有则添加进去
        {
            LOG_ERROR("在FolderMap中不存在该路径，修改失败，path：{}", old_path.generic_string());
            return;
        }
        else
        {
            const bool tree_node_open = folder_map.at(old_path);
            folder_map.erase(old_path);
            folder_map.insert({ new_path ,tree_node_open });
            LOG_INFO("在FolderMap中修改文件路径成功path：{0}->{1}", old_path.generic_string(), new_path.generic_string());
            return;
        }
        LOG_ERROR("在FolderMap中修改文件路径失败");
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
                else if (it->second == true) LOG_INFO("文件开关状态：打开，文件路径：{}", it->first.generic_string());
            }
            return;
        }
        return;
    }

    void FolderMap::BuildFolderTree(const std::filesystem::path& current_path, const bool& tree_node_open, std::filesystem::path& double_click_get_path)
    {
        // 初始化上一个路径
        static std::filesystem::path last_path = STORAGE_PATH;
        // 判断map中是否有该节点,如果没有则添加进去
        if (folder_map.count(current_path) == 0)
        {
            folder_map.insert({ current_path ,tree_node_open });
            LOG_INFO("BuildFolderTree：文件树建立，新添加文件夹：{}", current_path.generic_string());
            double_click_get_path = last_path;
            return;
        }
        // 如果该节点存在，则判断该节点上一个tree_node_open状态与当前的是否一样
        else
        {
            // 一样则说明没有进行操作，返回上一个操作的路径
            if (folder_map.at(current_path) == tree_node_open)
            {
                double_click_get_path = last_path;
                return;
            }
            // 其实可以用一个异或来进行判断的，但是为了区别打开和关闭的操作用了两个if
            // 状态由0->1即文件打开
            else if (folder_map.at(current_path) == 0 && tree_node_open == 1)
            {
                last_path = current_path;                       // 刷新上一个文件夹路径
                folder_map.at(current_path) = tree_node_open;   // 更新打开关闭节点
                LOG_INFO("BuildFolderTree：文件夹打开，抓取到当前文件夹路径为：{}", current_path.generic_string());
                double_click_get_path = current_path;
                // 双击打开文件
                if (!std::filesystem::is_directory(current_path))
                {
                    ShellExecute(NULL, L"open", current_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
                }
                return;
            }
            // 状态由1->0即文件打开
            else if (folder_map.at(current_path) == 1 && tree_node_open == 0)
            {
                last_path = current_path;                       // 刷新上一个文件夹路径
                folder_map.at(current_path) = tree_node_open;   // 更新打开关闭节点
                LOG_INFO("BuildFolderTree：文件夹关闭，抓取到当前文件夹路径为：{}", current_path.generic_string());
                double_click_get_path = current_path;
                // 双击打开文件
                if (!std::filesystem::is_directory(current_path))
                {
                    ShellExecute(NULL, L"open", current_path.generic_wstring().c_str(), NULL, NULL, SW_SHOW);
                }
                return;
            }
        }

        LOG_CRITICAL("BuildFolderTree：文件数建立失败");
        double_click_get_path = last_path;
        return;
    }


//-----------------------------------------------------------------------------
//                   文件复制(粘贴)、剪切、删除、新建立等相关操作
//-----------------------------------------------------------------------------

    // 路径是否存在检测
    // 输入：路径容器，可以检测一群路径
    // 返回：false：只要有一个路径不存在
    //       true: 所有路径均存在
    bool PathCheck(const std::vector<std::filesystem::path>& file_path)
    {
        bool PathExists = true;
        for (auto it = file_path.begin(); it != file_path.end(); it++)
        {
            if (!std::filesystem::exists(*it))
            {
                LOG_WARN("PathCheck：路径不存在,path:{}", (*it).generic_string());
                PathExists = false;
            }
        }
        if (PathExists)
        {
            LOG_INFO("PathCheck：检测路径存在");
            return true;
        }
        else return false;
    }

    // 重命名检测存在两种情况
    // 1：将file_path的名字放到target_path目录下进行检测
    // 2：将file_path的名字放到file_path的父目录下进行检测
    //    但是要注意file_path的名字不能和原来的一样，一样要
    //    注意，因为也会发生重命名
    // 输入： file_path：完整的路径或文件名（带扩展名）
    //        target_path：需要被检测的目录，一定要是完整的路径，如果是情况2则可以有file_path=target_path
    //        parent_check_flag：1为情况2，0为情况1
    // 返回值：
    //      true：   检测到重命名
    //      false：  未检测到重命名
    bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path, const bool& parent_check_flag)
    {
        std::filesystem::path file_to_target_path = target_path;
        // 判断是在哪种情况下的检测parent_check_flag=1则在第二种情况下检测
        // 默认是在第一种情况下检测
        if (parent_check_flag)
            file_to_target_path = target_path.parent_path() / file_path.filename();
        else
            file_to_target_path /= file_path.filename();

        if (std::filesystem::exists(file_to_target_path))
        {
            if (parent_check_flag)
                LOG_INFO("文件名：{0}，在路径：{1}中存在同名文件", file_path.filename().generic_string(), target_path.parent_path().generic_string());
            else
                LOG_INFO("文件名：{0}，在路径：{1}中存在同名文件", file_path.filename().generic_string(), target_path.generic_string());
            return true;
        }
        else
        {
            LOG_INFO("RenameCheck：未发现重命名");
            return false;
        }
    }

    // 新建文件夹
    // 输入： file_name：完整的文件名（带扩展名），如果是文件则可以不带
    //        target_path：要放新建文件的文件夹路径，一定要是文件夹
    //        forced_flag：发生重命名后，1强制建立文件，名字会有系统进行更改（加“副本”）
    //                     0 则不强制建立，但是一定要确保输入的file_name在target_path
    //                     文件夹下不存在重命名
    // 返回值：
    //      返回新建文件夹的完整路径
    std::filesystem::path AddFile(const std::filesystem::path& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
    {
        std::filesystem::path new_file_name = file_name.stem();
        std::filesystem::path new_file_extension = file_name.extension();
        if (forced_flag)
        {
            if (new_file_extension == FILE_EXTENSION_FOLDER)        LOG_INFO("AddFile：强制新建文件夹");
            else if (new_file_extension == FILE_EXTENSION_TXT)      LOG_INFO("AddFile：强制新建文本文件");
            else if (new_file_extension == FILE_EXTENSION_WORD)     LOG_INFO("AddFile：强制新建Word文档");
            else if (new_file_extension == FILE_EXTENSION_PPT)      LOG_INFO("AddFile：强制新建PPT文件");
            while (RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), target_path))
            {
                new_file_name += "-副本";
            }
        }
        std::filesystem::path new_path = target_path / (new_file_name.generic_string() + new_file_extension.generic_string());
        if (new_file_extension == FILE_EXTENSION_FOLDER)
        {
            std::filesystem::create_directory(new_path);
            LOG_INFO("AddFile：新建文件夹成功");
        }
        else
        {
            std::ofstream in(new_path, std::ios::out);
            if (new_file_extension == FILE_EXTENSION_TXT)           LOG_INFO("AddFile：新建文本文件成功");
            else if (new_file_extension == FILE_EXTENSION_WORD)     LOG_INFO("AddFile：新建Word文档成功");
            else if (new_file_extension == FILE_EXTENSION_PPT)      LOG_INFO("AddFile：新建PPT文件成功");
        }
        return new_path;
    }

    // 粘贴文件
    // 输入： from_path：完整的路径，即要粘贴的文件
    //        to_path：要放文件的文件夹路径，一定要是文件夹
    // 该函数不做文件路径以及文件重命名检测，因此一定要确保输
    // 入的from_path在target_path文件夹下不存在重命名
    // 返回值：
    //      true：   粘贴成功
    //      false：  粘贴失败
    std::filesystem::path PasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path, const bool& forced_flag)
    {
        LOG_INFO("PasteFile：粘贴文件中......");
        std::filesystem::path new_file_name = from_path.stem();
        std::filesystem::path new_file_extension = from_path.extension();
        if (forced_flag == true)
        {
            LOG_INFO("PasteFile：强制重命名中......");
            while (RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), to_path))
            {
                new_file_name += "-副本";
            }
        }
        std::filesystem::path new_path = from_path.parent_path() / (new_file_name.generic_string() + new_file_extension.generic_string());
        if (std::filesystem::is_directory(from_path))
        {
            // 执行文件夹复制操作
            std::filesystem::copy(from_path, to_path / new_path.filename(), std::filesystem::copy_options::recursive);
            
            LOG_INFO("PasteFile：成功粘贴文件夹 from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
        }
        else if (std::filesystem::is_regular_file(from_path))
        {
            // 执行文件复制操作
            std::filesystem::copy(from_path, to_path / new_path.filename());
            LOG_INFO("PasteFile：成功粘贴文件 from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
        }
        return new_path;
    }

    // 完全删除文件
    // 输入： filepath：要删除的文件路径
    // 该函数不做文件路径是否存在检测，因此一定要确保输
    // 入的filepath路径存在
    // 返回值：
    //      true：   完全删除成功
    //      false：  完全删除失败
    bool CompleteDeleteFile(const std::filesystem::path& filepath)
    {
        if (std::filesystem::is_directory(filepath))
        {
            std::filesystem::remove_all(filepath);
            LOG_INFO("CompleteDeleteFile：成功删除文件夹 path:{}", filepath.generic_string());
            return true;
        }
        else if (std::filesystem::is_regular_file(filepath))
        {
            std::filesystem::remove(filepath);
            LOG_INFO("CompleteDeleteFile：成功删除文件 path:{}", filepath.generic_string());
            return true;
        }

        LOG_ERROR("CompleteDeleteFile：删除失败！");
        return false;
    }
    void DeleteFolderOrFile(const std::filesystem::path& filepath)
    {
        if (!PathCheck({ filepath }))
        {
            LOG_ERROR("存在路径不存在，导致删除操作失败！");
            return;
        }
        if (std::filesystem::is_directory(filepath))
        {
            std::filesystem::remove_all(filepath);
            LOG_INFO("成功删除文件夹 path:{}", filepath.generic_string());
            return;
        }
        else if (std::filesystem::is_regular_file(filepath))
        {
            std::filesystem::remove(filepath);
            LOG_INFO("成功删除文件 path:{}", filepath.generic_string());
            return;
        }

        LOG_ERROR("删除操作失败！");
        return;
    }

    // 重命名文件
    // 注意replace_filename函数会对名字和扩展名进行全部替换
    // 该函数执行强制重命名，并不对重命名进行检测，检测部分需要自己调用
    // 
    // 该函数会自动检测是否重命名如果，则会进行系统重命名
    // 
    // 输入： rename：完整的文件名（带扩展名），同时需要注意输入的新的rename不能与旧的old_path的文件名一样，这样就没有意义
    //        old_path：就的文件路径
    // 返回值：
    //      返回重命名后的文件完整路径
    std::filesystem::path RenameFile(const std::filesystem::path& rename, const std::filesystem::path& old_path)
    {
        LOG_INFO("RenameFile：重命名操作中......");
        std::filesystem::path new_file_path = old_path;
        std::filesystem::path new_file_name = rename.stem();
        std::filesystem::path new_file_extension = rename.extension();

        std::filesystem::path file_to_target_path = old_path.parent_path() / rename;

        if (std::filesystem::exists(file_to_target_path))
        {
            LOG_INFO("RenameFile:文件存在重命名文件");

            while (std::filesystem::exists(file_to_target_path.parent_path() / (new_file_name.generic_string() + new_file_extension.generic_string())))
            {
                new_file_name += "-副本";
            }
        }
        new_file_path.replace_filename(new_file_name.generic_string() + new_file_extension.generic_string());
        std::filesystem::rename(old_path, new_file_path);
        LOG_INFO("RenameFile：重命名操作成功");
        return new_file_path;
    }

}
