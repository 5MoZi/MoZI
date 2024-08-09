#pragma once

//-----------------------------------------------------------------------------
//                               文件路径宏定义
//-----------------------------------------------------------------------------

/*****解决方案与项目路径*****/

#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// 解决方案路径
#define PROJECT_PATH			std::filesystem::current_path()							// 项目路径

//#define STORAGE_PATH			"C:\\Users\\MoZI\\Desktop\\MoZI\\MoZI0.1\\storage"
//#define MoZI_RECYCLE_BIN_PATH	"C:\\Users\\MoZI\\Desktop\\MoZI\\MoZI0.1\\storage\\回收站"		// 回收站路径
#define STORAGE_PATH			(SOLUTION_PATH/"storage").generic_u8string().c_str()				// 存储文件路径
#define MOZI_RECYCLE_BIN_PATH	(SOLUTION_PATH/"RecyleBin").generic_u8string().c_str()	// 回收站路径

/*****字体路径*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// 英文字体
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// 中文字体
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

// 专有文件
#define MOZI_EXTENSION_Plan ".mzplan"		// 目标信息扩展名
#define MOZI_EXTENSION_PLAN ".mzplan"		// 目标信息扩展名
#define MOZI_EXTENSION_TARGET ".mztarget"	// 目标信息扩展名
#define MOZI_RECYCLE_BIN "回收站"


// 文件扩展名宏定义
#define FILE_EXTENSION_FOLDER	""			// 文本夹扩展名（文件夹则为空值）
#define FILE_EXTENSION_TXT		".txt"		// 文本文件扩展名
#define FILE_EXTENSION_WORD		".docx"		// word文档扩展名
#define FILE_EXTENSION_PPT		".pptx"		// ppt扩展名

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


	enum FileOperateReturnFlag
	{
		FileOperateReturnFlag_ExistRename,
		FileOperateReturnFlag_OperateSuccess,
		FileOperateReturnFlag_OperateFailure
	};

	class FolderMap
	{
	public:
		void AddFolderPath(const std::filesystem::path& current_path, const bool& tree_node_open);
		void DeleteFolderPath(const std::filesystem::path& current_path);
		void ChangeFolderPath(const std::filesystem::path& old_path, const std::filesystem::path& new_path);
		void LookFolderMap();

		// 获取双击后的文件夹路径
		void BuildFolderTree(const std::filesystem::path& current_path,
			const bool& tree_node_open, std::filesystem::path& double_click_get_path);
	private:
		static std::map<std::filesystem::path, bool> folder_map;
	};


	// 识别文件格式
	FileOperate::FileFormat CheckFileFormat(const std::filesystem::path& file_path);
	// 识别文件后缀
	const std::string CheckFileExtension(const FileOperate::FileFormat& file_format);

	// 文件树前的图标
	const std::string TreeFileIconConnect(const std::filesystem::path& file_path);
	// 扫描文件夹函数，放回其下第一层的文件夹路径和文件路径
	void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path, const std::filesystem::path& current_path);

	// 字符串与UTF8互转
	std::string UTF8_To_string(const std::string& str);
	std::string string_To_UTF8(const std::string& str);



//-----------------------------------------------------------------------------
//                   文件复制(粘贴)、剪切、删除、新建立等相关操作
//-----------------------------------------------------------------------------

	// 路径是否存在检测
	bool PathCheck(const std::vector<std::filesystem::path>& file_path);
	// 重命名检测
	bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path, const bool& parent_check_flag = false);
	// 新建文件
	std::filesystem::path AddFile(const std::filesystem::path& file_name, const std::filesystem::path& target_path, const bool& forced_flag = false);
	// 粘贴文件
	//bool PasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path);
	std::filesystem::path PasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path, const bool& forced_flag = false);
	// 完全删除文件
	bool CompleteDeleteFile(const std::filesystem::path& filepath);
	// 重命名文件
	std::filesystem::path RenameFile(const std::filesystem::path& rename, const std::filesystem::path& old_path);

	// 新建文件夹
	std::filesystem::path AddFolder(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag = false);
	// 新建文本文件
	std::filesystem::path AddTextFile(const std::string& file_name, const std::string& file_extension,
		const std::filesystem::path& target_path, const bool& forced_flag = false);


	// 复制粘贴文件
	void CopyFolderAndFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path);
	// 专有的复制粘贴文件
	FileOperate::FileOperateReturnFlag SpecialCopyFolderAndFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path);

	// 完全删除文件
	void DeleteFolderOrFile(const std::filesystem::path& filepath);
	// 重命名文件夹或文件
	// 注意如果是对文件进行改名输入的rename一定要带扩展名，
	// 因为replace_filename函数会对名字和扩展名进行全部替换
	// 该函数执行强制重命名，并不对重命名进行检测，检测部分需要自己调用
	std::filesystem::path RenameFile(const std::string& rename, std::filesystem::path& old_path, const bool& forced_flag = false);


	std::filesystem::path CopyAndCutForcedRenameFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path);
}

