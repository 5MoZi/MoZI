#include "mopch.h"
#include "FileManage.h"
#include "MoIcon.h"

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

}