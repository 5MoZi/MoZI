#include "mopch.h"
#include "FileOperate.h"
#include "FileManage.h"
#include "MoIcon.h"
#include "Log.h"


#include "MysqlOperate.h"



namespace FileManage {
	 
	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag)
	{
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		if (forced_flag == false)LOG_INFO("NewBuildFolderAndFile:�½��ļ���...");
		else LOG_INFO("NewBuildFolderAndFile:ǿ�ƽ����ļ���...");

		std::filesystem::path new_file_path;
		std::string file_extension = "";


		if (file_format == FileOperate::FileFormat_Directory)
		{
			// ǿ�ƴ����ļ���
			if (forced_flag)
			{
				new_file_path = FileOperate::AddFolder(file_name, target_path, forced_flag);
				fileinfo_mysql_table.AddMysqlFileData(new_file_path);
				return true;
			}
			if (FileOperate::RenameCheck(file_name, target_path))
			{
				return false;
			}
			new_file_path = FileOperate::AddFolder(file_name, target_path);
			fileinfo_mysql_table.AddMysqlFileData(new_file_path);
			return true;
		}
		else 
		{

			switch (file_format)
			{
			case FileOperate::FileFormat_TextFile:	file_extension = FILE_EXTENSION_TXT; break;
			case FileOperate::FileFormat_WordFile:	file_extension = FILE_EXTENSION_WORD; break;
			case FileOperate::FileFormat_PptFile:	file_extension = FILE_EXTENSION_PPT; break;
			default: break;
			}
			// ǿ�ƴ����ı��ļ�
			if (forced_flag)
			{
				new_file_path = FileOperate::AddTextFile(file_name, file_extension, target_path, forced_flag);
				fileinfo_mysql_table.AddMysqlFileData(new_file_path);
				return true;
			}
			if (FileOperate::RenameCheck(file_name+ file_extension, target_path))
			{
				return false;
			}
			new_file_path = FileOperate::AddTextFile(file_name, file_extension, target_path);
			fileinfo_mysql_table.AddMysqlFileData(new_file_path);
			return true;
		}
	}

	// ������Ҫ���ļ������������������Ŀ���ļ�target_path����Ϊ����
	bool RenameFolderAndFile(const std::string& file_name, std::filesystem::path& target_path, const bool& forced_flag)
	{
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

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
				new_file_path =FileOperate::RenameFile(file_name, target_path, forced_flag);
				fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
				return true;
			}
			// ���������
			std::filesystem::path file_to_target_path = target_path.parent_path();
			file_to_target_path /= file_name;
			if (std::filesystem::exists(file_to_target_path))
			{
				LOG_INFO("����������");
				return false;
			}

			new_file_path = FileOperate::RenameFile(file_name, target_path);
			fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
			return true;
		}
		else
		{

			switch (file_format)
			{
			case FileOperate::FileFormat_TextFile:	file_extension = FILE_EXTENSION_TXT; break;
			case FileOperate::FileFormat_WordFile:	file_extension = FILE_EXTENSION_WORD; break;
			case FileOperate::FileFormat_PptFile:	file_extension = FILE_EXTENSION_PPT; break;
			default: break;
			}
			// ǿ�ƴ����ı��ļ�
			if (forced_flag)
			{
				FileOperate::RenameFile(file_name + file_extension, target_path, forced_flag);
				fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
				return true;
			}
			std::filesystem::path file_to_target_path = target_path.parent_path();
			file_to_target_path /= (file_name + file_extension);
			if (std::filesystem::exists(file_to_target_path))
			{
				LOG_INFO("����������");
				return false;
			}
			FileOperate::RenameFile(file_name + file_extension, target_path);
			fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
			return true;
		}
	}
	void CompleteDeleteFile(const std::filesystem::path& target_path)
	{
		LOG_INFO("CompleteDeleteFile:����ɾ���ļ���{}...", target_path.generic_string());
		static FileOperate::FolderMap folder_maper;
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		// ��Ҫע���ļ�ɾ��˳����Դ������ҳ��Ҫ�����ɾ��
		LOG_INFO("��FileMapɾ���ļ���...");
		folder_maper.DeleteFolderPath(target_path);
		LOG_INFO("��Mysql���ݱ�{}��ɾ���ļ���...", DATABASE_FILETABLE_NAME);
		fileinfo_mysql_table.DeleteMysqlFileData(target_path);
		LOG_INFO("����Դ����ҳ��ɾ���ļ���...");
		FileOperate::DeleteFolderOrFile(target_path);

	}

	FileOperate::FileOperateReturnFlag CopyPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag)
	{
		LOG_INFO("CopyPasteFile:����ճ���ļ���...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);
		if (force_flag == false)
		{
			if (FileOperate::SpecialCopyFolderAndFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_OperateSuccess)
			{
				fileinfo_mysql_table.CopyPasteMysqlFileData(from_path, to_path);
				return FileOperate::FileOperateReturnFlag_OperateSuccess;
			}
			else if (FileOperate::SpecialCopyFolderAndFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_ExistRename)
			{
				return FileOperate::FileOperateReturnFlag_ExistRename;
			}
		}
		else if (force_flag == true)
		{
			std::filesystem::path new_file_path = FileOperate::CopyAndCutForcedRenameFile(from_path, to_path);
			fileinfo_mysql_table.CopyPasteMysqlFileData(new_file_path, to_path);
			return FileOperate::FileOperateReturnFlag_OperateSuccess;
		}

		return FileOperate::FileOperateReturnFlag_OperateFailure;
	}

	FileOperate::FileOperateReturnFlag CutPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag)
	{
		LOG_INFO("CutPasteFile:����ճ���ļ���...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);
		if (force_flag == false)
		{
			// ���в���Ϊ���ȸ�������ȫɾ��
			if (FileOperate::SpecialCopyFolderAndFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_OperateSuccess)
			{
				fileinfo_mysql_table.CopyPasteMysqlFileData(from_path, to_path);
				CompleteDeleteFile(from_path);
				return FileOperate::FileOperateReturnFlag_OperateSuccess;
			}
			else if (FileOperate::SpecialCopyFolderAndFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_ExistRename)
			{
				return FileOperate::FileOperateReturnFlag_ExistRename;
			}
		}
		else if (force_flag == true)
		{
			std::filesystem::path new_file_path = FileOperate::CopyAndCutForcedRenameFile(from_path, to_path);
			fileinfo_mysql_table.CopyPasteMysqlFileData(new_file_path, to_path);
			CompleteDeleteFile(from_path);
			return FileOperate::FileOperateReturnFlag_OperateSuccess;
		}

		return FileOperate::FileOperateReturnFlag_OperateFailure;
	}

	void DeleteFileToBin(const std::filesystem::path& file_path)
	{
		LOG_INFO("DeleteFileToBin:ɾ���ļ�������վ��...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);
		// �ֱ�ȡ�ļ�����չ�����ļ���
		std::filesystem::path new_file_extension = file_path.extension();
		std::filesystem::path new_file_name = file_path.stem();
		while (FileOperate::RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), MOZI_RECYCLE_BIN_PATH))
		{
			new_file_name += "-����";
		}
		// �����·������Ҫע���·�����ļ�ϵͳ�в������ڣ����Բ�����filesystem��һЩ�Դ���
		// ��⺯�����ᵼ�³��ִ�����Ϊ·�������ڵĴ���
		std::filesystem::path new_file_path = file_path.parent_path() / (new_file_name.generic_string() + new_file_extension.generic_string());
		std::cout << new_file_path << std::endl;
		// �ļ��и���ʹ��recursiveѡ��
		if (new_file_extension.generic_string() == "")
		{
			// ִ���ļ��и��Ʋ���
			std::filesystem::copy(file_path, (MOZI_RECYCLE_BIN_PATH / new_file_path.filename()), std::filesystem::copy_options::recursive);
			fileinfo_mysql_table.CopyPasteMysqlFileData(new_file_path, MOZI_RECYCLE_BIN_PATH);
			CompleteDeleteFile(file_path);
			LOG_INFO("�ɹ�ɾ���ļ��� from_path:{}", file_path.generic_string());
		}
		else
		{
			// ִ���ļ����Ʋ���
			std::filesystem::copy(file_path, (MOZI_RECYCLE_BIN_PATH / new_file_path.filename()));
			fileinfo_mysql_table.CopyPasteMysqlFileData(new_file_path, MOZI_RECYCLE_BIN_PATH);
			CompleteDeleteFile(file_path);
			LOG_INFO("�ɹ�ɾ���ļ� from_path:{0}", file_path.generic_string());
		}
	}

	void RenameFile(const std::filesystem::path& file_path)
	{

	}

}