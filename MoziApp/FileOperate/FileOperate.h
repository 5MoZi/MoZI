#pragma once

//-----------------------------------------------------------------------------
//                               文件路径宏定义
//-----------------------------------------------------------------------------


/*****解决方案与项目路径*****/
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// 解决方案路径
#define PROJECT_PATH			std::filesystem::current_path()							// 项目路径

#define STORAGE_PATH			(SOLUTION_PATH/"storage").generic_u8string().c_str()			// 存储文件路径
#define MOZI_RECYCLE_BIN_PATH	(SOLUTION_PATH/"RecyleBin").generic_u8string().c_str()			// 回收站路径

/*****字体路径*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// 中文字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()
#endif


#ifdef PUBLIC_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path().parent_path()			// 解决方案路径


#define STORAGE_PATH			(SOLUTION_PATH/"storage").generic_u8string().c_str()			// 存储文件路径
#define MOZI_RECYCLE_BIN_PATH	(SOLUTION_PATH/"RecyleBin").generic_u8string().c_str()			// 回收站路径
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// 中文字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif

#define BIN_CODER_AMOUNT 65536

// 专有文件
#define MOZI_RECYCLE_BIN "回收站"


// 添加新文件后缀的操作
// 1、利用#define的添加相对于的文件扩展
// 2、enum FileFormat添加对应的枚举类型
// 3、在static std::map<std::filesystem::path, FileFormat> file_format_map中添加对应的映射
// 4、在const std::string TreeFileIconConnect函数、const std::string GetFileIcon以及其重载函数中添加相对应的功能
// 5、在Moicon.h中添加对应的文件的icon图标

// 文件扩展名宏定义
#define FILE_EXTENSION_FOLDER		""			// 文本夹扩展名（文件夹则为空值）
#define FILE_EXTENSION_TXT			".txt"		// 文本文件扩展名
#define FILE_EXTENSION_WORD			".docx"		// word文档扩展名
#define FILE_EXTENSION_PPT			".pptx"		// ppt扩展名
#define FILE_EXTENSION_MARKDOWN		".md"		// Markdown扩展名
#define FILE_EXTENSION_PDF		    ".pdf"		// pdf扩展名
#define FILE_EXTENSION_PNG		    ".png"		// png图片扩展名
#define FILE_EXTENSION_JPG		    ".jpg"		// jpg图片扩展名
#define FILE_EXTENSION_EXCEL	    ".xlsx"		// excel表格扩展名
#define FILE_EXTENSION_MP3	        ".mp3"		// mp3扩展名
#define FILE_EXTENSION_MP4	        ".mp4"		// mp4扩展名


namespace FileOperate {

	enum FileFormat
	{
		// 未识别类文件
		FileFormat_UnknownFileFormat,		

		/*******文件夹类型********/
		// 目录
		FileFormat_Directory,		

		/*******文件类型********/
		// 已经识别类文件格式，代表下面所有的文件
		FileFormat_AllKnownFile,		
		// 文本类文件
		FileFormat_TextFile,
		FileFormat_WordFile,
		FileFormat_PdfFile,
		FileFormat_MarkdownFile,
		// 图片类文件
		FileFormat_PngFile,
		FileFormat_JpgFile,
		FileFormat_XlsxFile,	
		FileFormat_PptFile,	
		// 音频类文件
		FileFormat_Mp3File,
		// 视频类文件
		FileFormat_Mp4File,

		/*******MOZIAPP专有文件类型********/
		FileFormat_MzPlanFile,
		FileFormat_MzTargetFile
	};

	// 识别文件格式
	FileOperate::FileFormat CheckFileFormat(const std::filesystem::path& file_path);
	// 识别文件后缀
	const std::string CheckFileExtension(const FileOperate::FileFormat& file_format);

	// 文件树前的图标
	const std::string TreeFileIconConnect(const std::filesystem::path& file_path);

	// 获得对应文件类型的图标
	const std::string GetFileIcon(const std::filesystem::path& file_path);
	const std::string GetFileIcon(const FileOperate::FileFormat& file_format);

	// 字符串与UTF8互转
	std::string UTF8_To_string(const std::string& str);
	std::string string_To_UTF8(const std::string& str);



//-----------------------------------------------------------------------------
//                   文件复制(粘贴)、剪切、删除、新建立等相关操作
//-----------------------------------------------------------------------------
	// 扫描文件夹函数，放回其下第一层的文件夹路径和文件路径
	void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path,
		const std::filesystem::path& current_path);
	// 路径是否存在检测
	bool PathCheck(const std::vector<std::filesystem::path>& file_path);
	// 重命名检测
	bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path,
		const bool& parent_check_flag = false);
	// 新建文件
	std::filesystem::path AddFile(const std::filesystem::path& file_name, 
		const std::filesystem::path& target_path, const bool& forced_flag = false);
	// 粘贴文件
	std::filesystem::path PasteFile(const std::filesystem::path& from_path,
		const std::filesystem::path& to_path, const bool& forced_flag = false);
	// 完全删除文件
	bool CompleteDeleteFile(const std::filesystem::path& filepath);
	// 重命名文件
	std::filesystem::path RenameFile(const std::filesystem::path& rename, 
		const std::filesystem::path& old_path);
	// 完全删除文件
	void DeleteFolderOrFile(const std::filesystem::path& filepath);

//-----------------------------------------------------------------------------
//								回收站重名编号问题
//-----------------------------------------------------------------------------
	int SetBinFileCoder();
	bool ReturnBinFileCoder(const int& current_bin_coder);

}

