#pragma once




namespace FileManage {


	bool NewBuildFolderAndFile(const std::string& file_name, const FileOperate::FileFormat& file_format,
		const std::filesystem::path& target_path, const bool& forced_flag = false);

	void CompleteDeleteFile(const std::filesystem::path& target_path);

	FileOperate::FileOperateReturnFlag CopyPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag = 0);
	FileOperate::FileOperateReturnFlag CutPasteFile(const std::filesystem::path& from_path, const std::filesystem::path& to_path,
		const bool& force_flag = 0);
	void DeleteFileToBin(const std::filesystem::path& file_path);

	bool RenameFolderAndFile(const std::string& file_name, std::filesystem::path& target_path, const bool& forced_flag = false);
}