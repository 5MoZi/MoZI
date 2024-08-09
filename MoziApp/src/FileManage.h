#pragma once




namespace FileManage {

	bool NewBuildFile(const std::filesystem::path& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag = false);
	bool FileManageRenameFile(const std::filesystem::path& file_name, const std::filesystem::path& target_path,
		const bool& forced_flag = false);
	bool FileManagePasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& copy_cut_flag = false, const bool& force_flag = false);

	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag = false);

	void CompleteDeleteFile(const std::filesystem::path& target_path);

	FileOperate::FileOperateReturnFlag CopyPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag = 0);
	FileOperate::FileOperateReturnFlag CutPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag = 0);
	void DeleteFileToBin(const std::filesystem::path& file_path);

}