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
            LOG_INFO("添加新文件夹至FolderMap中：{}", current_path.generic_string());
            return;
        }
        LOG_WARN("无文件夹可添加");
        return;
    }

    void FolderMap::DeleteFolderPath(const std::filesystem::path& current_path)
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
            LOG_INFO("文件树建立__新添加文件夹：{}", current_path.generic_string());
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
                LOG_INFO("文件夹打开，抓取到当前文件夹路径为：{}", current_path.generic_string());
                double_click_get_path = current_path;
                return;
            }
            // 状态由1->0即文件打开
            else if (folder_map.at(current_path) == 1 && tree_node_open == 0)
            {
                last_path = current_path;                       // 刷新上一个文件夹路径
                folder_map.at(current_path) = tree_node_open;   // 更新打开关闭节点
                LOG_INFO("文件夹关闭，抓取到当前文件夹路径为：{}", current_path.generic_string());
                double_click_get_path = current_path;
                return;
            }
        }

        LOG_CRITICAL("文件数建立失败");
        double_click_get_path = last_path;
        return;
    }

//-----------------------------------------------------------------------------
//                   文件复制(粘贴)、剪切、删除、新建立等相关操作
//-----------------------------------------------------------------------------

    // 路径是否存在检测
    bool PathCheck(const std::vector<std::filesystem::path>& file_path)
    {
        bool PathExists = true;
        for (auto it = file_path.begin(); it != file_path.end(); it++)
        {
            if (!std::filesystem::exists(*it))
            {
                LOG_WARN("路径不存在,path:{}", (*it).generic_string());
                PathExists = false;
            }
        }
        if (PathExists)
        {
            LOG_INFO("检测所有路径均存在");
            return true;
        }
        else return false;
    }
    // 新建文件夹
    void AddFolder(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
    {
        std::string new_file_name = file_name;
        if (forced_flag)
        {
            LOG_INFO("AddFolder:强制新建文件夹");
            while (RenameCheck(new_file_name, target_path))
            {
                new_file_name += "-副本";
            }
        }
        std::filesystem::path new_path = target_path / new_file_name;
        std::filesystem::create_directory(new_path);
        LOG_INFO("新建文件夹成功");
        return;
    }
    
    // 新建文本文件
    void AddTextFile(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
    {
        std::string new_file_name = file_name;
        if (forced_flag)
        {
            LOG_INFO("强制新建文本文件");
            while (RenameCheck(new_file_name, target_path))
            {
                new_file_name += "-副本";
            }
        }
        std::filesystem::path new_path = target_path / (file_name + ".txt");
        std::ofstream in(new_path, std::ios::out);
        LOG_INFO("新建文本文件成功");
        return;
    }

    // 重命名检测
    // 返回false可以是重命名也可以是重命名检测失败，返回true说明存在重命名
    bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path)
    {
        if (!PathCheck({ file_path,target_path }))
        {
            LOG_ERROR("存在路径不存在，导致重命名检测失败！");
            return false;
        }

        std::filesystem::path file_to_target_path = target_path;
        std::filesystem::rename(file_to_target_path, file_path);
        if (std::filesystem::exists(file_to_target_path))
        {
            LOG_INFO("文件名：{0}，在路径：{1}中存在同名文件", file_path.filename().generic_string(), target_path.generic_string());
            return true;
        }
        else
        {
            LOG_INFO("未发现重命名");
            return false;
        }
        LOG_ERROR("重命名检测失败！");
        return false;
    }
    // 重命名检测重载
    bool RenameCheck(const std::string& rename, const std::filesystem::path& target_path)
    {
        if (!PathCheck({ target_path }))
        {
            LOG_ERROR("存在路径不存在，导致重命名检测失败！");
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
            LOG_INFO("文件名：{0}，在路径：{1}中存在同名文件", rename, target_path.generic_string());
            return true;
        }
        else
        {
            LOG_INFO("未发现重命名");
            return false;
        }
        LOG_ERROR("重命名检测失败！");
        return false;
    }
    // 复制粘贴文件到文件夹
    void CopyFolderAndFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path)
    {
        if (!PathCheck({ from_path,to_path }))
        {
            LOG_ERROR("存在路径不存在，导致复制操作失败！");
            return;
        }
        if (std::filesystem::is_directory(to_path))
        {
            // 进行重命名检测
            LOG_INFO("复制粘贴操作重命名检测中...");
            if (!RenameCheck(from_path, to_path))
            {
                if (std::filesystem::is_directory(from_path))
                {
                    // 执行文件夹复制操作
                    std::filesystem::copy(from_path, to_path, std::filesystem::copy_options::recursive
                        | std::filesystem::copy_options::update_existing);
                    LOG_INFO("成功复制文件夹 from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
                    return;
                }
                else if (std::filesystem::is_regular_file(from_path))
                {
                    // 执行文件复制操作
                    std::filesystem::copy(from_path, to_path);
                    LOG_INFO("成功复制文件 from_path:{0}--to_path:{1}", from_path.generic_string(), to_path.generic_string());
                    return;
                }
            } 
            else
            {
                LOG_INFO("存在重命名文件");
                return;
            }
        }
        else
        {
            LOG_WARN("复制到的路径不是文件夹 path:{}", to_path.generic_string());
            return;
        }
        LOG_ERROR("复制操作失败！");
        return;
    }
    // 完全删除文件
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
    // 重命名文件夹或文件
    // 注意如果是对文件进行改名输入的rename一定要带扩展名，
    // 因为replace_filename函数会对名字和扩展名进行全部替换
    // 该函数执行强制重命名，并不对重命名进行检测，检测部分需要自己调用
    void RenameFile(const std::string& rename, std::filesystem::path& old_path)
    {
        if (!PathCheck({ old_path }))
        {
            LOG_ERROR("存在路径不存在，导致重命名失败！");
            return;
        }
        old_path.replace_filename(rename);
        LOG_INFO("重命名操作成功！");
        return;
    }

}
