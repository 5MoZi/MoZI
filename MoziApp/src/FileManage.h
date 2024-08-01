#pragma once




namespace FileManage {


	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag = false);


}