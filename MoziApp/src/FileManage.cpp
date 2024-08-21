#include "mopch.h"
#include "FileOperate.h"
#include "FileManage.h"
#include "MoIcon.h"
#include "Log.h"


#include "MysqlOperate.h"



namespace FileManage {
	 

	static FileOperate::FolderMap folder_maper;

	bool NewBuildFile(const std::filesystem::path& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag)
	{
		//static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		if (forced_flag == false)LOG_INFO("NewBuildFile���½��ļ���......");
		else LOG_INFO("NewBuildFile��ǿ�ƽ����ļ���......");

		std::filesystem::path new_file_path;
		std::string file_extension = "";

		// �����ļ���
		if (file_format == FileOperate::FileFormat_Directory)
		{
			// ǿ�ƴ����ļ���
			if (forced_flag)
			{
				new_file_path = FileOperate::AddFile(file_name, target_path, forced_flag);
				MysqlOperate::AddMysqlFileData(new_file_path, DATABASE_FILETABLE_NAME);
				return true;
			}
			if (FileOperate::RenameCheck(file_name, target_path))
			{
				return false;
			}
			new_file_path = FileOperate::AddFile(file_name, target_path);
			MysqlOperate::AddMysqlFileData(new_file_path, DATABASE_FILETABLE_NAME);
			return true;
		}
		// �����ļ�
		else
		{
			// ʶ���ļ���׺
			file_extension = FileOperate::CheckFileExtension(file_format);
			// ǿ�ƴ����ı��ļ�
			if (forced_flag)
			{
				new_file_path = FileOperate::AddFile(file_name.generic_string() + file_extension, target_path, forced_flag);
				MysqlOperate::AddMysqlFileData(new_file_path, DATABASE_FILETABLE_NAME);
				return true;
			}
			if (FileOperate::RenameCheck(file_name.generic_string() + file_extension, target_path))
			{
				return false;
			}
			new_file_path = FileOperate::AddFile(file_name.generic_string() + file_extension, target_path);
			MysqlOperate::AddMysqlFileData(new_file_path, DATABASE_FILETABLE_NAME);
			return true;
		}
	}

	// ������Ҫ���ļ������������������Ŀ���ļ�target_path����Ϊ����
	bool FileManageRenameFile(const std::filesystem::path& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
	{
		FileOperate::FileFormat file_format = FileOperate::CheckFileFormat(target_path);

		if (forced_flag == false)LOG_INFO("RenameFolderAndFile:�������ļ���...");
		else LOG_INFO("RenameFolderAndFile:ǿ���������ļ���...");

		std::filesystem::path new_file_path = target_path;
		std::string file_extension = "";

		if (file_format == FileOperate::FileFormat_Directory)
		{
			// ǿ�ƴ����ļ���
			if (forced_flag)
			{
				new_file_path = FileOperate::RenameFile(file_name, target_path);
				MysqlOperate::ChangeMysqlFileData(target_path, new_file_path, DATABASE_FILETABLE_NAME);
				folder_maper.ChangeFolderPath(target_path, new_file_path);
				return true;
			}
			// ���������
			if (FileOperate::RenameCheck(file_name.generic_string() + file_extension, target_path, true))
			{
				return false;
			}
			new_file_path = FileOperate::RenameFile(file_name, target_path);
			MysqlOperate::ChangeMysqlFileData(target_path, new_file_path, DATABASE_FILETABLE_NAME);
			folder_maper.ChangeFolderPath(target_path, new_file_path);
			return true;
		}
		else
		{

			// ʶ���ļ���׺
			file_extension = FileOperate::CheckFileExtension(file_format);
			// ǿ�ƴ����ı��ļ�
			if (forced_flag)
			{
				new_file_path = FileOperate::RenameFile(file_name.generic_string() + file_extension, target_path);
				MysqlOperate::ChangeMysqlFileData(target_path, new_file_path, DATABASE_FILETABLE_NAME);
				folder_maper.ChangeFolderPath(target_path, new_file_path);
				return true;
			}
			if (FileOperate::RenameCheck(file_name.generic_string() + file_extension, target_path, true))
			{
				return false;
			}
			new_file_path = FileOperate::RenameFile(file_name.generic_string() + file_extension, target_path);
			MysqlOperate::ChangeMysqlFileData(target_path, new_file_path, DATABASE_FILETABLE_NAME);
			folder_maper.ChangeFolderPath(target_path, new_file_path);
			return true;
		}
	}

	bool FileManagePasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& copy_cut_flag, const bool& force_flag)
	{
		LOG_INFO("FileManagePasteFile��ճ���ļ���...");

		if (force_flag == false)
		{
			if (FileOperate::RenameCheck(from_path, to_path))
			{
				return false;
			}
			FileOperate::PasteFile(from_path, to_path);
			MysqlOperate::PasteMysqlFileData(from_path, to_path, DATABASE_FILETABLE_NAME, DATABASE_TEMP_FILETABLE_NAME);
			// ���Ʋ�������Ҫɾ��ԭ�ļ��������в�����Ҫ
			if(copy_cut_flag == true)
			{
				CompleteDeleteFile(from_path);
			}
			return true;
		}
		else if (force_flag == true)
		{
			std::filesystem::path new_file_path = FileOperate::PasteFile(from_path, to_path, true);
			MysqlOperate::PasteMysqlFileData(from_path, new_file_path, to_path, DATABASE_FILETABLE_NAME, DATABASE_TEMP_FILETABLE_NAME);
			// ���Ʋ�������Ҫɾ��ԭ�ļ��������в�����Ҫ
			if (copy_cut_flag == true)
			{
				CompleteDeleteFile(from_path);
			}
			return true;
		}
	}

	// ɾ��������վ������һ�����в���������������FileManagePasteFile����
	void DeleteFileToBin(const std::filesystem::path& file_path)
	{
		LOG_INFO("DeleteFileToBin��ɾ���ļ�������վ��...");
		if (FileManagePasteFile(file_path, MOZI_RECYCLE_BIN_PATH, 1) == false)
		{
			FileManagePasteFile(file_path, MOZI_RECYCLE_BIN_PATH, 1, 1);
		}
		LOG_INFO("DeleteFileToBin��ɾ���ļ�������վ���");
	}

	void CompleteDeleteFile(const std::filesystem::path& target_path)
	{
		LOG_INFO("CompleteDeleteFile:����ɾ���ļ���{}...", target_path.generic_string());
		static FileOperate::FolderMap folder_maper;

		// ��Ҫע���ļ�ɾ��˳����Դ������ҳ��Ҫ�����ɾ��
		LOG_INFO("CompleteDeleteFile����FileMapɾ���ļ���...");
		folder_maper.DeleteFolderPath(target_path);
		LOG_INFO("CompleteDeleteFile����Mysql���ݱ�{}��ɾ���ļ���...", DATABASE_FILETABLE_NAME);
		if (target_path == MOZI_RECYCLE_BIN_PATH)
			MysqlOperate::DeleteMysqlFileData(target_path, DATABASE_FILETABLE_NAME, 1);
		else
			MysqlOperate::DeleteMysqlFileData(target_path, DATABASE_FILETABLE_NAME);
		LOG_INFO("CompleteDeleteFile������Դ����ҳ��ɾ���ļ���...");
		FileOperate::DeleteFolderOrFile(target_path);
	}

	// �������ط�����ļ�����Դ��������
	void SelectFile(const std::filesystem::path& current)
	{
		std::filesystem::path choice_path;
		wchar_t szPath[MAX_PATH];
		memset(szPath, 0, MAX_PATH);	//�������

		BROWSEINFO bi;
		bi.hwndOwner = NULL; // ӵ�иöԻ���Ĵ��ھ��
		bi.pidlRoot = NULL;
		bi.pszDisplayName = szPath;
		bi.lpszTitle = L"��ѡ����Ҫ���ļ���";
		bi.ulFlags = BIF_BROWSEINCLUDEFILES;
		bi.lpfn = NULL;
		bi.lParam = 0;
		bi.iImage = 0;

		//����ѡ��Ŀ¼�Ի��� 
		LPITEMIDLIST lp = SHBrowseForFolder(&bi);
		if (lp && SHGetPathFromIDList(lp, szPath))
		{
			choice_path = szPath;
			FileManagePasteFile(choice_path, current, 0, 1);
		}
	}
}