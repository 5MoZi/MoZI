#include "mopch.h"
#include "Log.h"
#include "MoObject.h"
#include "MysqlOperate.h"
namespace MysqlOperate {

	const char* MysqlData::database_user_name;
	const char* MysqlData::database_password;
	const char* MysqlData::database_name;
	MYSQL_RES* MysqlData::res;	// ��ѯ�����
	MYSQL_ROW MysqlData::row;	// ��¼�ṹ��
	MYSQL MysqlData::mysql;	// ���ݿ���


	MysqlData::MysqlData(const char* c_database_table_name, const char* c_database_user_name, 
		const char* c_database_password, const char* c_database_name)
	{
		// ���ݳ�ʼ��
		database_table_name = c_database_table_name;
		database_user_name = c_database_user_name;
		database_password = c_database_password;
		database_name = c_database_name;
		// ���ݿ�����
		ConnectDatabase();
	}

	// �������ݿ�
	void MysqlData::ConnectDatabase()
	{
		//��ʼ�����ݿ���
		mysql_init(&mysql);
		//�������ݿ���뷽ʽ
		mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");
		//�������ݿ�
		if (mysql_real_connect(&mysql, "localhost", database_user_name, database_password, database_name, 3306, NULL, 0) == NULL)
		{
			LOG_CRITICAL("Mysql���ݿ�����ʧ�ܣ�ʧ��ԭ��{}", mysql_error(&mysql));
			return;
		}
		LOG_INFO("Mysql���ݿ����ӳɹ������ݿ�����{}", database_name);
		return;
	}
	// �ر����ݿ�
	void MysqlData::CloseDatabase()
	{
		mysql_close(&mysql);		//�ر����ݿ�
	}

	// ������ݿ��Ƿ���ͬ����·��
	bool MysqlData::MysqlFilePathCheck(const std::filesystem::path& file_path)
	{
		if (!std::filesystem::exists(file_path))
		{
			LOG_WARN("������ݿ⣺�ļ�·��������,path:{}", file_path.generic_string());
			return false;
		}

		char check_path[256];
		snprintf(check_path, 256, "select * from %s where path ='%s';", database_table_name, file_path.generic_string().c_str());
		mysql_query(&mysql, check_path);
		// �޼�⵽��ͬ·��ֱ�ӽ����������
		res = mysql_store_result(&mysql);
		if (mysql_fetch_row(res))
		{
			LOG_INFO("������ݿ⣺����ͬ·��������");
			mysql_free_result(res);						//���ҵ�������ͷŽ�����ռ�
			return true;
		}
		else
		{
			LOG_INFO("������ݿ⣺������ͬ·��������");
			mysql_free_result(res);						//���ҵ�������ͷŽ�����ռ�
			return false;
		}
		LOG_WARN("������ݿ⣺������ݿ�ʧ��");
		mysql_free_result(res);							//���ҵ�������ͷŽ�����ռ�
		return false;

	}

	// �������
	void MysqlData::AddMysqlFileData(const std::filesystem::path& file_path)
	{
		if (MysqlFilePathCheck(file_path))
		{
			LOG_WARN("���Mysql���ݣ�����ͬ·�������ݣ��޷�����������ݲ���");
			return;
		}

		FileDataStream file_data(file_path);

		char get_data[2000];
		snprintf(get_data, 2000, "insert into %s(name,create_date,path)values('%s','%s','%s');",
			file_data.GetName().c_str(), file_data.GetCreateDate().c_str(), file_data.GetPath().c_str());
		mysql_query(&mysql, get_data);
		LOG_INFO("���Mysql���ݣ���������ӳɹ�");
		return;
	}

	// ɾ������
	void MysqlData::DeleteMysqlFileData(const std::filesystem::path& delete_path)
	{
		if (!MysqlFilePathCheck(delete_path))
		{
			LOG_WARN("ɾ��Mysql���ݣ������ڸ�·�������ݣ��޷�����ɾ�����ݲ���");
			return;
		}
		char sql[2000];
		snprintf(sql, 2000, "delete from %s where path='%s';", database_table_name, delete_path.generic_u8string().c_str());
		mysql_query(&mysql, sql);
		LOG_INFO("ɾ��Mysql���ݣ�����ɾ���ɹ�");
		return;
	}

	// �޸�����
	void MysqlData::ChangeMysqlFileData(const std::filesystem::path& old_path, const MysqlOperate::FileDataStream& file_data)
	{
		if (!MysqlFilePathCheck(old_path))
		{
			LOG_WARN("�޸�Mysql���ݣ������ڸ�·�������ݣ��޷������޸����ݲ���");
			return;
		}
		char sql[2000];
		snprintf(sql, 2000, "update %s set name='%s',create_date='%s',path='%s' where path='%s';",
			database_table_name, file_data.GetName().c_str(), file_data.GetCreateDate().c_str(), 
			file_data.GetPath().c_str(), old_path.generic_string().c_str());
		mysql_query(&mysql, sql);
		LOG_INFO("�޸�Mysql���ݣ������޸ĳɹ�");
		return;
	}

	// ��������ʼ��
	FileDataStream::FileDataStream(const std::filesystem::path& new_file_path)
	{
		file_name = new_file_path.filename().generic_string();
		file_create_date = MoObject::GetCurrentDate();
		file_path = new_file_path.generic_string();
		LOG_INFO("Mysql����������ʼ���ɹ�");
	}

}
