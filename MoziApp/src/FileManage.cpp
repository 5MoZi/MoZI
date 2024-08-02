#include "mopch.h"
#include "FileOperate.h"
#include "FileManage.h"
#include "MoIcon.h"
#include "Log.h"


#include "MysqlOperate.h"



namespace FileManage {

	//static MysqlOperate::MysqlData mysql_database=
	//static 
	 
	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path,const bool& forced_flag)
	{
		MysqlOperate::MysqlTable fileinfo_mysql_table(DATABASE_FILETABLE_NAME);
		if (forced_flag == false)LOG_INFO("NewBuildFolderAndFile:新建文件中...");
		else LOG_INFO("NewBuildFolderAndFile:强制建立文件中...");

		std::filesystem::path new_file_path;

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
		else if (file_format == FileOperate::FileFormat_TextFile)
		{
			// 强制创建文本文件
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
	}





   





}