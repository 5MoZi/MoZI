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
		if (forced_flag == false)LOG_INFO("NewBuildFolderAndFile:�½��ļ���...");
		else LOG_INFO("NewBuildFolderAndFile:ǿ�ƽ����ļ���...");

		std::filesystem::path new_file_path;

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
		else if (file_format == FileOperate::FileFormat_TextFile)
		{
			// ǿ�ƴ����ı��ļ�
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