#pragma once


#define DATABASE_USER_NAME				"root"				//���ݿ��û���
#define DATABASE_PASSWORD				"cf200257"			//���ݿ�����
#define DATABASE_NAME					"mzdata01"			//���ݿ���

#define DATABASE_FILETABLE_NAME			"fileinfo"			//�ļ����ݱ���


namespace MysqlOperate {
	class FileDataStream
	{
	public:
		FileDataStream(const std::filesystem::path& file_path);
		// ���ز���
		inline virtual const std::string GetName() const { return file_name; }
		inline virtual const std::string GetCreateDate() const { return file_create_date; }
		inline virtual const std::string GetPath() const { return file_path; }

		// ���ò���
		inline virtual void SetName(const std::string& c_file_name) { file_name = c_file_name; }
		inline virtual void SetCreateDate(const std::string& c_file_create_date) { file_create_date = c_file_create_date; }
		inline virtual void SetPath(const std::string& c_file_path) { file_path = c_file_path; }
	private:
		// ������Ϣ
		std::string file_name;				// �ļ�������
		std::string file_create_date;			// ����ʱ��
		std::string file_path;				// �ļ���·��
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
		static MYSQL mysql;	// ���ݿ���
		static MYSQL_RES* res;	// ��ѯ�����
		static MYSQL_ROW row;	// ��¼�ṹ��
	};



}


