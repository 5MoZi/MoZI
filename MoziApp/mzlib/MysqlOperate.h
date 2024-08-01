#pragma once


#define DATABASE_USER_NAME				"root"				//数据库用户名
#define DATABASE_PASSWORD				"cf200257"			//数据库密码
#define DATABASE_NAME					"mzdata01"			//数据库名

#define DATABASE_FILETABLE_NAME			"fileinfo"			//文件数据表名


namespace MysqlOperate {
	class FileDataStream
	{
	public:
		FileDataStream(const std::filesystem::path& file_path);
		// 返回参数
		inline virtual const std::string GetName() const { return file_name; }
		inline virtual const std::string GetCreateDate() const { return file_create_date; }
		inline virtual const std::string GetPath() const { return file_path; }

		// 设置参数
		inline virtual void SetName(const std::string& c_file_name) { file_name = c_file_name; }
		inline virtual void SetCreateDate(const std::string& c_file_create_date) { file_create_date = c_file_create_date; }
		inline virtual void SetPath(const std::string& c_file_path) { file_path = c_file_path; }
	private:
		// 基本信息
		std::string file_name;				// 文件夹名称
		std::string file_create_date;			// 创建时间
		std::string file_path;				// 文件夹路径
	};

	class MysqlData
	{
	public:
		MysqlData(const char* database_table_name, const char* database_user_name, const char* database_password, const char* database_name);
		void ConnectDatabase();
		void CloseDatabase();

		void AddMysqlFileData(const std::filesystem::path& file_path);
		void DeleteMysqlFileData(const std::filesystem::path& delete_path);
		void ChangeMysqlFileData(const std::filesystem::path& old_path, const MysqlOperate::FileDataStream& file_data);

		bool MysqlFilePathCheck(const std::filesystem::path& file_path);
	private:
		const char* database_table_name;

		static const char* database_user_name;
		static const char* database_password;
		static const char* database_name;
		static MYSQL mysql;	// 数据库类
		static MYSQL_RES* res;	// 查询结果集
		static MYSQL_ROW row;	// 记录结构体
	};



}


