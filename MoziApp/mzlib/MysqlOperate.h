#pragma once


#define DATABASE_USER_NAME				"root"				//���ݿ��û���
#define DATABASE_PASSWORD				"cf200257"			//���ݿ�����
#define DATABASE_NAME					"modata01"			//���ݿ���

#define DATABASE_FILETABLE_NAME			"fileinfo"			//�ļ����ݱ���
#define DATABASE_TEMP_FILETABLE_NAME	"tempfileinfo"		//�ļ�������ת����

namespace MysqlOperate {

	MYSQL* GetMysqlIo();
	void ConnectDatabase();
	void CreateFileDataTable(const std::vector <std::string> database_table_name);
	bool MysqlFilePathCheck(const std::filesystem::path& file_path, const char* database_table_name);
	void AddMysqlFileData(const std::filesystem::path& file_path, const char* database_table_name);
	void PasteMysqlFileData(const std::filesystem::path & from_path, const std::filesystem::path & to_path, const char* database_table_name, const char* temp_table_name);
	void DeleteMysqlFileData(const std::filesystem::path& delete_path, const char* database_table_name);
	void ChangeMysqlFileData(const std::filesystem::path& old_path, const std::filesystem::path& new_path, const char* database_table_name);

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
}


