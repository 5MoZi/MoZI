#pragma once

//-----------------------------------------------------------------------------
//                               �ļ�·���궨��
//-----------------------------------------------------------------------------


/*****�����������Ŀ·��*****/
#ifdef BETA_VERSION
#define SOLUTION_PATH           std::filesystem::current_path().parent_path()			// �������·��
#define PROJECT_PATH			std::filesystem::current_path()							// ��Ŀ·��

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
#define MOZI_RECYCLE_BIN "����վ"


// ������ļ���׺�Ĳ���
// 1������#define���������ڵ��ļ���չ
// 2��enum FileFormat��Ӷ�Ӧ��ö������
// 3����static std::map<std::filesystem::path, FileFormat> file_format_map����Ӷ�Ӧ��ӳ��
// 4����const std::string TreeFileIconConnect������const std::string GetFileIcon�Լ������غ�����������Ӧ�Ĺ���
// 5����Moicon.h����Ӷ�Ӧ���ļ���iconͼ��

// �ļ���չ���궨��
#define FILE_EXTENSION_FOLDER		""			// �ı�����չ�����ļ�����Ϊ��ֵ��
#define FILE_EXTENSION_TXT			".txt"		// �ı��ļ���չ��
#define FILE_EXTENSION_WORD			".docx"		// word�ĵ���չ��
#define FILE_EXTENSION_PPT			".pptx"		// ppt��չ��
#define FILE_EXTENSION_MARKDOWN		".md"		// Markdown��չ��
#define FILE_EXTENSION_PDF		    ".pdf"		// pdf��չ��
#define FILE_EXTENSION_PNG		    ".png"		// pngͼƬ��չ��
#define FILE_EXTENSION_JPG		    ".jpg"		// jpgͼƬ��չ��
#define FILE_EXTENSION_EXCEL	    ".xlsx"		// excel�����չ��
#define FILE_EXTENSION_MP3	        ".mp3"		// mp3��չ��
#define FILE_EXTENSION_MP4	        ".mp4"		// mp4��չ��


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
		FileFormat_MarkdownFile,
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

	// ʶ���ļ���ʽ
	FileOperate::FileFormat CheckFileFormat(const std::filesystem::path& file_path);
	// ʶ���ļ���׺
	const std::string CheckFileExtension(const FileOperate::FileFormat& file_format);

	// �ļ���ǰ��ͼ��
	const std::string TreeFileIconConnect(const std::filesystem::path& file_path);

	// ��ö�Ӧ�ļ����͵�ͼ��
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

//-----------------------------------------------------------------------------
//								����վ�����������
//-----------------------------------------------------------------------------
	int SetBinFileCoder();
	bool ReturnBinFileCoder(const int& current_bin_coder);

}

