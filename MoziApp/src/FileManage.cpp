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
	bool RenameFolderAndFile(const std::string& file_name, std::filesystem::path& target_path, const bool& forced_flag)
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
				new_file_path =FileOperate::RenameFile(file_name, target_path, forced_flag);
				fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
				return true;
			}
			// 重命名检测
			std::filesystem::path file_to_target_path = target_path.parent_path();
			file_to_target_path /= file_name;
			if (std::filesystem::exists(file_to_target_path))
			{
				LOG_INFO("存在重命名");
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
			// 强制创建文本文件
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
				LOG_INFO("存在重命名");
				return false;
			}
			FileOperate::RenameFile(file_name + file_extension, target_path);
			fileinfo_mysql_table.ChangeMysqlFileData(target_path, new_file_path);
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

	void DeleteFileToBin(const std::filesystem::path& file_path)
	{
		LOG_INFO("DeleteFileToBin:删除文件至回收站中...");
		static MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);
		// 分别取文件的扩展名和文件名
		std::filesystem::path new_file_extension = file_path.extension();
		std::filesystem::path new_file_name = file_path.stem();
		while (FileOperate::RenameCheck(new_file_name.generic_string() + new_file_extension.generic_string(), MOZI_RECYCLE_BIN_PATH))
		{
			new_file_name += "-副本";
		}
		// 组合新路径，需要注意该路径在文件系统中并不存在，所以不能用filesystem中一些自带的
		// 检测函数，会导致出现错误，因为路径不存在的错误
		std::filesystem::path new_file_path = file_path.parent_path() / (new_file_name.generic_string() + new_file_extension.generic_string());
		std::cout << new_file_path << std::endl;
		// 文件夹复制使用recursive选择
		if (new_file_extension.generic_string() == "")
		{
			// 执行文件夹复制操作
			std::filesystem::copy(file_path, (MOZI_RECYCLE_BIN_PATH / new_file_path.filename()), std::filesystem::copy_options::recursive);
			fileinfo_mysql_table.CopyPasteMysqlFileData(new_file_path, MOZI_RECYCLE_BIN_PATH);
			CompleteDeleteFile(file_path);
			LOG_INFO("成功删除文件夹 from_path:{}", file_path.generic_string());
		}
		else
		{
			// 执行文件复制操作
			std::filesystem::copy(file_path, (MOZI_RECYCLE_BIN_PATH / new_file_path.filename()));
			fileinfo_mysql_table.CopyPasteMysqlFileData(new_file_path, MOZI_RECYCLE_BIN_PATH);
			CompleteDeleteFile(file_path);
			LOG_INFO("成功删除文件 from_path:{0}", file_path.generic_string());
		}
	}

	void RenameFile(const std::filesystem::path& file_path)
	{

	}

}