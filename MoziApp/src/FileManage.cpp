#include "mopch.h"
#include "FileOperate.h"
#include "FileManage.h"
#include "MoIcon.h"
#include "Log.h"


#include "MysqlOperate.h"



namespace FileManage {
	 
	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path,const bool& forced_flag)
	{
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		if (forced_flag == false)LOG_INFO("NewBuildFolderAndFile:新建文件中...");
		else LOG_INFO("NewBuildFolderAndFile:强制建立文件中...");

		std::filesystem::path new_file_path;
		std::string file_extension = "";


		if (file_format == FileOperate::FileFormat_Directory)
		{
			// 强制创建文件夹
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
		else //if (file_format == FileOperate::FileFormat_TextFile)
		{

			switch (file_format)
			{
			case FileOperate::FileFormat_TextFile:	file_extension = FILE_EXTENSION_TXT; break;
			case FileOperate::FileFormat_WordFile:	file_extension = FILE_EXTENSION_WORD; break;
			case FileOperate::FileFormat_PptFile:	file_extension = FILE_EXTENSION_PPT; break;
			default: break;
			}
			// 强制创建文本文件
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

	void CompleteDeleteFile(const std::filesystem::path& target_path)
	{
		LOG_INFO("CompleteDeleteFile:正在删除文件：{}...", target_path.generic_string());
		static FileOperate::FolderMap folder_maper;
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		// 需要注意文件删除顺序，资源管理器页面要在最后删除
		LOG_INFO("在FileMap删除文件中...");
		folder_maper.DeleteFolderPath(target_path);
		LOG_INFO("在Mysql数据表：{}，删除文件中...", DATABASE_FILETABLE_NAME);
		fileinfo_mysql_table.DeleteMysqlFileData(target_path);
		LOG_INFO("在资源管理页面删除文件中...");
		FileOperate::DeleteFolderOrFile(target_path);

	}

	FileOperate::FileOperateReturnFlag CopyPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path)
	{
		LOG_INFO("CopyPasteFile:复制粘贴文件中...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		if (FileOperate::SpecialCopyFolderAndFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_OperateSuccess)
		{
			fileinfo_mysql_table.CopyPasteMysqlFileData(from_path, to_path);
			return FileOperate::FileOperateReturnFlag_OperateSuccess;
		}
		else if (FileOperate::SpecialCopyFolderAndFile(from_path, to_path) == FileOperate::FileOperateReturnFlag_ExistRename)
		{
			return FileOperate::FileOperateReturnFlag_ExistRename;
		}

		return FileOperate::FileOperateReturnFlag_OperateFailure;
	}

	FileOperate::FileOperateReturnFlag CutPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path)
	{
		LOG_INFO("CutPasteFile:剪切粘贴文件中...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		// 剪切操作为：先复制在完全删除
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

		return FileOperate::FileOperateReturnFlag_OperateFailure;
	}



	void DeleteFileToBin(const std::filesystem::path& file_path)
	{

	}



}