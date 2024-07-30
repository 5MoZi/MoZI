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

namespace FileManage {

	enum FileFormat
	{
		FileFormat_UnknownFile,		// 未识别类文件

		/*******文件夹类型********/
		FileFormat_Directory,		// 目录

		/*******文件类型********/
		FileFormat_AllKnownFile,		// 已经识别类文件格式，代表下面所有的文件
		// 文本类文件
		FileFormat_TextFile,
		FileFormat_WordFile,
		FileFormat_PdfFile,
		// 图片类文件
		FileFormat_PngFile,
		FileFormat_JpgFile,
		FileFormat_XlsxFile,	// EXCEL文件
		FileFormat_PptFile,	// PPT文件
		// 音频类文件
		FileFormat_Mp3File,
		// 视频类文件
		FileFormat_Mp4File,

		/*******MOZIAPP专有文件类型********/
		FileFormat_MzPlanFile,
		FileFormat_MzTargetFile
	};

	// 识别文件格式
	FileFormat CheckFileType(const std::filesystem::path& file_path);
	// 文件树前的图标
	const std::string TreeFileIconConnect(const std::filesystem::path& file_path);

	// 扫描文件夹函数，放回其下第一层的文件夹路径和文件路径
	void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path, const std::filesystem::path& current_path);

	class FolderMap
	{
	public:
		void AddFolder(const std::filesystem::path& current_path, const bool& tree_node_open);
		void DeleteFolder(const std::filesystem::path& current_path);
		void LookFolderMap();

		// 获取双击后的文件夹路径
		const std::filesystem::path& BuildFolderTree(const std::filesystem::path& current_path, const bool& tree_node_open);
	private:
		static std::map<std::filesystem::path, bool> folder_map;
	};



}