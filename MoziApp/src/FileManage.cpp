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
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		if (forced_flag == false)LOG_INFO("NewBuildFile：新建文件中......");
		else LOG_INFO("NewBuildFile：强制建立文件中......");

		std::filesystem::path new_file_path;
		std::string file_extension = "";

		// 创建文件夹
		if (file_format == FileOperate::FileFormat_Directory)
		{
			// 强制创建文件夹
			if (forced_flag)
			{
				new_file_path = FileOperate::AddFile(file_name, target_path, forced_flag);
				fileinfo_mysql_table.AddMysqlFileData(new_file_path);
				return true;
			}
			if (FileOperate::RenameCheck(file_name, target_path))
			{
				return false;
			}
			new_file_path = FileOperate::AddFile(file_name, target_path);
			fileinfo_mysql_table.AddMysqlFileData(new_file_path);
			return true;
		}
		// 创建文件
		else
		{
			// 识别文件后缀
			file_extension = FileOperate::CheckFileExtension(file_format);
			// 强制创建文本文件
			if (forced_flag)
			{
				new_file_path = FileOperate::AddFile(file_name.generic_string() + file_extension, target_path, forced_flag);
				fileinfo_mysql_table.AddMysqlFileData(new_file_path);
				return true;
			}
			if (FileOperate::RenameCheck(file_name.generic_string() + file_extension, target_path))
			{
				return false;
			}
			new_file_path = FileOperate::AddFile(file_name.generic_string() + file_extension, target_path);
			fileinfo_mysql_table.AddMysqlFileData(new_file_path);
			return true;
		}
	}

	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag)
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
		else 
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


	// 由于需要对文件进行重命名则，输入的目标文件target_path不能为常量
	bool FileManageRenameFile(const std::filesystem::path& file_name, const std::filesystem::path& target_path, const bool& forced_flag)
	{
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		FileOperate::FileFormat file_format = FileOperate::CheckFileFormat(target_path);

		if (forced_flag == false)LOG_INFO("RenameFolderAndFile:重命名文件中...");
		else LOG_INFO("RenameFolderAndFile:强制重命名文件中...");

		std::filesystem::path new_file_path = target_path;
		std::string file_extension = "";

		if (file_format == FileOperate::FileFormat_Directory)
		{
			// 强制创建文件夹
			if (forced_flag)
			{
				new_file_path = FileOperate::RenameFile(file_name, target_path);
				fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
				folder_maper.ChangeFolderPath(target_path, new_file_path);
				return true;
			}
			// 重命名检测
			if (FileOperate::RenameCheck(file_name.generic_string() + file_extension, target_path, true))
			{
				return false;
			}
			new_file_path = FileOperate::RenameFile(file_name, target_path);
			fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
			folder_maper.ChangeFolderPath(target_path, new_file_path);
			return true;
		}
		else
		{

			// 识别文件后缀
			file_extension = FileOperate::CheckFileExtension(file_format);
			// 强制创建文本文件
			if (forced_flag)
			{
				FileOperate::RenameFile(file_name.generic_string() + file_extension, target_path);
				fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
				folder_maper.ChangeFolderPath(target_path, new_file_path);
				return true;
			}
			if (FileOperate::RenameCheck(file_name.generic_string() + file_extension, target_path, true))
			{
				return false;
			}
			FileOperate::RenameFile(file_name.generic_string() + file_extension, target_path);
			fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
			folder_maper.ChangeFolderPath(target_path, new_file_path);
			return true;
		}
	}


	bool FileManagePasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& copy_cut_flag, const bool& force_flag)
	{
		LOG_INFO("FileManagePasteFile：粘贴文件中...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		if (force_flag == false)
		{
			if (FileOperate::RenameCheck(from_path, to_path))
			{
				return false;
			}
			FileOperate::PasteFile(from_path, to_path);
			fileinfo_mysql_table.PasteMysqlFileData(from_path, to_path);
			// 复制操作不需要删除原文件，而剪切操作需要
			if(copy_cut_flag == true)
			{
				CompleteDeleteFile(from_path);
			}
			return true;
		}
		else if (force_flag == true)
		{
			std::filesystem::path new_file_path = FileOperate::PasteFile(from_path, to_path, true);
			fileinfo_mysql_table.PasteMysqlFileData(new_file_path, to_path);
			// 复制操作不需要删除原文件，而剪切操作需要
			if (copy_cut_flag == true)
			{
				CompleteDeleteFile(from_path);
			}
			return true;
		}
	}

	FileOperate::FileOperateReturnFlag CopyPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag)
	{
		LOG_INFO("CopyPasteFile:复制粘贴文件中...");
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
		LOG_INFO("CutPasteFile:剪切粘贴文件中...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);
		if (force_flag == false)
		{
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


	// 删除至回收站本质是一个剪切操作，因此这里调用FileManagePasteFile即可
	void DeleteFileToBin(const std::filesystem::path& file_path)
	{
		LOG_INFO("DeleteFileToBin：删除文件至回收站中...");
		if (FileManagePasteFile(file_path, MOZI_RECYCLE_BIN_PATH, 1) == false)
		{
			FileManagePasteFile(file_path, MOZI_RECYCLE_BIN_PATH, 1, 1);
		}
		LOG_INFO("DeleteFileToBin：删除文件至回收站完成");
	}

	void CompleteDeleteFile(const std::filesystem::path& target_path)
	{
		LOG_INFO("CompleteDeleteFile:正在删除文件：{}...", target_path.generic_string());
		static FileOperate::FolderMap folder_maper;
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);

		// 需要注意文件删除顺序，资源管理器页面要在最后删除
		LOG_INFO("CompleteDeleteFile：在FileMap删除文件中...");
		folder_maper.DeleteFolderPath(target_path);
		LOG_INFO("CompleteDeleteFile：在Mysql数据表：{}，删除文件中...", DATABASE_FILETABLE_NAME);
		fileinfo_mysql_table.DeleteMysqlFileData(target_path);
		LOG_INFO("CompleteDeleteFile：在资源管理页面删除文件中...");
		FileOperate::DeleteFolderOrFile(target_path);
	}
}