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
		if (forced_flag == false)LOG_INFO("NewBuildFolderAndFile:�½��ļ���");
		else LOG_INFO("NewBuildFolderAndFile:ǿ�ƽ����ļ���");

		if (file_format == FileOperate::FileFormat_Directory)
		{
			// ǿ�ƴ����ļ���
			if (forced_flag)
			{
				FileOperate::AddFolder(file_name, target_path, forced_flag);
				return true;
			}

			if (FileOperate::RenameCheck(file_name, target_path))
			{
				return false;
			}
			FileOperate::AddFolder(file_name, target_path);
			return true;
		}
		else if (file_format == FileOperate::FileFormat_TextFile)
		{
			// ǿ�ƴ����ı��ļ�
			if (forced_flag)
			{
				FileOperate::AddFolder(file_name, target_path, forced_flag);
				return true;
			}
			if (FileOperate::RenameCheck(file_name, target_path))
			{
				return false;
			}
			FileOperate::AddTextFile(file_name, target_path);
			return true;
		}
	}





   





}