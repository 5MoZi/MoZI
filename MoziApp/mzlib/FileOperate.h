#pragma once

//-----------------------------------------------------------------------------
//                               �ļ�·���궨��
//-----------------------------------------------------------------------------

/*****�����������Ŀ·��*****/

#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// �������·��
#define PROJECT_PATH			std::filesystem::current_path()							// ��Ŀ·��

//#define STORAGE_PATH			"C:\\Users\\MoZI\\Desktop\\MoZI\\MoZI0.1\\storage"
//#define MoZI_RECYCLE_BIN_PATH	"C:\\Users\\MoZI\\Desktop\\MoZI\\MoZI0.1\\storage\\����վ"		// ����վ·��
#define STORAGE_PATH			(SOLUTION_PATH/"storage").generic_u8string().c_str()				// �洢�ļ�·��
#define MOZI_RECYCLE_BIN_PATH	(SOLUTION_PATH/"RecyleBin").generic_u8string().c_str()	// ����վ·��

/*****����·��*****/
#define FONT_ENGLISH_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\arial.ttf").generic_u8string().c_str()				// Ӣ������
#define FONT_CHINESE_BASE_PATH		(SOLUTION_PATH/"lib\\fonts\\zhenghei_chinese.ttf").generic_u8string().c_str()	// ��������
#define ICON_BASE_FILE_PATH			(SOLUTION_PATH/"lib\\fonts\\fa-solid-900.ttf").generic_u8string().c_str()

// ר���ļ�
#define MOZI_EXTENSION_Plan ".mzplan"		// Ŀ����Ϣ��չ��
#define MOZI_EXTENSION_PLAN ".mzplan"		// Ŀ����Ϣ��չ��
#define MOZI_EXTENSION_TARGET ".mztarget"	// Ŀ����Ϣ��չ��
#define MOZI_RECYCLE_BIN "����վ"
namespace FileOperate {

	enum FileFormat
	{
		FileFormat_UnknownFile,		// δʶ�����ļ�

		/*******�ļ�������********/
		FileFormat_Directory,		// Ŀ¼

		/*******�ļ�����********/
		FileFormat_AllKnownFile,		// �Ѿ�ʶ�����ļ���ʽ�������������е��ļ�
		// �ı����ļ�
		FileFormat_TextFile,
		FileFormat_WordFile,
		FileFormat_PdfFile,
		// ͼƬ���ļ�
		FileFormat_PngFile,
		FileFormat_JpgFile,
		FileFormat_XlsxFile,	// EXCEL�ļ�
		FileFormat_PptFile,	// PPT�ļ�
		// ��Ƶ���ļ�
		FileFormat_Mp3File,
		// ��Ƶ���ļ�
		FileFormat_Mp4File,

		/*******MOZIAPPר���ļ�����********/
		FileFormat_MzPlanFile,
		FileFormat_MzTargetFile
	};

	// ʶ���ļ���ʽ
	FileFormat CheckFileType(const std::filesystem::path& file_path);
	// �ļ���ǰ��ͼ��
	const std::string TreeFileIconConnect(const std::filesystem::path& file_path);
	
	// ɨ���ļ��к������Ż����µ�һ����ļ���·�����ļ�·��
	void ScanDirectory(std::vector<std::filesystem::path>& folder_path, std::vector<std::filesystem::path>& file_path, const std::filesystem::path& current_path);


	std::string UTF8_To_string(const std::string& str);
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


//-----------------------------------------------------------------------------
//                   �ļ�����(ճ��)�����С�ɾ�����½�������ز���
//-----------------------------------------------------------------------------
	// �½��ļ���
	std::filesystem::path AddFolder(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag = 0);
	// �½��ı��ļ�
	void AddTextFile(const std::string& file_name, const std::filesystem::path& target_path, const bool& forced_flag = false);


	// ·���Ƿ���ڼ��
	bool PathCheck(const std::vector<std::filesystem::path>& file_path);
	// ���������
	// ����false������������Ҳ���������������ʧ�ܣ�����true˵������������
	bool RenameCheck(const std::filesystem::path& file_path, const std::filesystem::path& target_path);
	// �������������
	bool RenameCheck(const std::string& rename, const std::filesystem::path& target_path);
	// ����ճ���ļ����ļ���
	void CopyFolderAndFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path);
	// ��ȫɾ���ļ�
	void DeleteFolderOrFile(const std::filesystem::path& filepath);
	// �������ļ��л��ļ�
	// ע������Ƕ��ļ����и��������renameһ��Ҫ����չ����
	// ��Ϊreplace_filename����������ֺ���չ������ȫ���滻
	// �ú���ִ��ǿ�������������������������м�⣬��ⲿ����Ҫ�Լ�����
	void RenameFile(const std::string& rename, std::filesystem::path& old_path);

}

