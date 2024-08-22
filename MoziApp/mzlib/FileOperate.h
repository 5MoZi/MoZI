#pragma once

//-----------------------------------------------------------------------------
//                               �ļ�·���궨��
//-----------------------------------------------------------------------------


//#define BETA_VERSION			// �ڲ��궨��
//#define PUBLIC_VERSION		// �����궨��

/*****�����������Ŀ·��*****/
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// �������·��
#define PROJECT_PATH			std::filesystem::current_path()							// ��Ŀ·��

//#define STORAGE_PATH			"C:\\Users\\MoZI\\Desktop\\MoZI\\MoZI0.1\\storage"
//#define MoZI_RECYCLE_BIN_PATH	"C:\\Users\\MoZI\\Desktop\\MoZI\\MoZI0.1\\storage\\����վ"		// ����վ·��
#define STORAGE_PATH			(SOLUTION_PATH/"storage").generic_u8string().c_str()			// �洢�ļ�·��
#define MOZI_RECYCLE_BIN_PATH	(SOLUTION_PATH/"RecyleBin").generic_u8string().c_str()			// ����վ·��

/*****����·��*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// ��������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()
#endif


#ifdef PUBLIC_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path().parent_path()			// �������·��


#define STORAGE_PATH			(SOLUTION_PATH/"storage").generic_u8string().c_str()			// �洢�ļ�·��
#define MOZI_RECYCLE_BIN_PATH	(SOLUTION_PATH/"RecyleBin").generic_u8string().c_str()			// ����վ·��
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// ��������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

#endif

#define BIN_CODER_AMOUNT 65536


// ר���ļ�
#define MOZI_EXTENSION_Plan ".mzplan"		// Ŀ����Ϣ��չ��
#define MOZI_EXTENSION_PLAN ".mzplan"		// Ŀ����Ϣ��չ��
#define MOZI_EXTENSION_TARGET ".mztarget"	// Ŀ����Ϣ��չ��
#define MOZI_RECYCLE_BIN "����վ"


// �ļ���չ���궨��
#define FILE_EXTENSION_FOLDER	""			// �ı�����չ�����ļ�����Ϊ��ֵ��
#define FILE_EXTENSION_TXT		".txt"		// �ı��ļ���չ��
#define FILE_EXTENSION_WORD		".docx"		// word�ĵ���չ��
#define FILE_EXTENSION_PPT		".pptx"		// ppt��չ��

namespace FileOperate {

	enum FileFormat
	{
		// δʶ�����ļ�
		FileFormat_UnknownFileFormat,		

		/*******�ļ�������********/
		// Ŀ¼
		FileFormat_Directory,		

		/*******�ļ�����********/
		// �Ѿ�ʶ�����ļ���ʽ�������������е��ļ�
		FileFormat_AllKnownFile,		
		// �ı����ļ�
		FileFormat_TextFile,
		FileFormat_WordFile,
		FileFormat_PdfFile,
		// ͼƬ���ļ�
		FileFormat_PngFile,
		FileFormat_JpgFile,
		FileFormat_XlsxFile,	
		FileFormat_PptFile,	
		// ��Ƶ���ļ�
		FileFormat_Mp3File,
		// ��Ƶ���ļ�
		FileFormat_Mp4File,

		/*******MOZIAPPר���ļ�����********/
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

		// ��ȡ˫������ļ���·��
		void BuildFolderTree(const std::filesystem::path& current_path,
			const bool& tree_node_open, std::filesystem::path& double_click_get_path);
	private:
		static std::map<std::filesystem::path, bool> folder_map;
	};


	// ʶ���ļ���ʽ
	FileOperate::FileFormat CheckFileFormat(const std::filesystem::path& file_path);
	// ʶ���ļ���׺
	const std::string CheckFileExtension(const FileOperate::FileFormat& file_format);

	// �ļ���ǰ��ͼ��
	const std::string TreeFileIconConnect(const std::filesystem::path& file_path);

	const std::string GetFileIcon(const std::filesystem::path& file_path);
	const std::string GetFileIcon(const FileOperate::FileFormat& file_format);
	// �ַ�����UTF8��ת
	std::string UTF8_To_string(const std::string& str);
	std::string string_To_UTF8(const std::string& str);



//-----------------------------------------------------------------------------
//                   �ļ�����(ճ��)�����С�ɾ�����½�������ز���
//-----------------------------------------------------------------------------
	// ɨ���ļ��к������Ż����µ�һ����ļ���·�����ļ�·��
	void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path,
		const std::filesystem::path& current_path);
	// ·���Ƿ���ڼ��
	bool PathCheck(const std::vector<std::filesystem::path>& file_path);
	// ���������
	bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path,
		const bool& parent_check_flag = false);
	// �½��ļ�
	std::filesystem::path AddFile(const std::filesystem::path& file_name, 
		const std::filesystem::path& target_path, const bool& forced_flag = false);
	// ճ���ļ�
	std::filesystem::path PasteFile(const std::filesystem::path& from_path,
		const std::filesystem::path& to_path, const bool& forced_flag = false);
	// ��ȫɾ���ļ�
	bool CompleteDeleteFile(const std::filesystem::path& filepath);
	// �������ļ�
	std::filesystem::path RenameFile(const std::filesystem::path& rename, 
		const std::filesystem::path& old_path);
	// ��ȫɾ���ļ�
	void DeleteFolderOrFile(const std::filesystem::path& filepath);

	int SetBinFileCoder();
	bool ReturnBinFileCoder(const int& current_bin_coder);

}

