#include "mopch.h"
#include "MysqlOperate.h"
#include "Log.h"
#include "MoObject.h"

namespace MysqlOperate {

	const char* MysqlData::database_user_name;
	const char* MysqlData::database_password;
	const char* MysqlData::database_name;
	MYSQL_RES* MysqlData::res;	// ��ѯ�����
	MYSQL_ROW MysqlData::row;	// ��¼�ṹ��
	MYSQL MysqlData::mysql;	// ���ݿ���


	std::string StringToUtf8(const std::string str)
	{
		std::filesystem::path strpath = str;
		return strpath.generic_u8string();
	}


	MYSQL* GetMysqlIo()
	{
		return &MysqlData::mysql;
	}


	MysqlData::MysqlData(const char* c_database_user_name, const char* c_database_password,
		const char* c_database_name)
	{
		// ���ݳ�ʼ��
		database_user_name = c_database_user_name;
		database_password = c_database_password;
		database_name = c_database_name;
		// ���ݿ�����
		ConnectDatabase();
		LOG_INFO("Mysql���ݿ��ʼ�����");
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

	MysqlTable::MysqlTable(const char* c_database_table_name)
	{
		one_build_flag = false;
		database_table_name = c_database_table_name;
		LOG_INFO("MysqlTable::MysqlTable���������ݱ���...");
		CreateDataTable();
	}

	// �������ݱ��
	void MysqlTable::CreateDataTable()
	{
		LOG_INFO("MysqlData::CreateDataTable�����ݱ���ڼ����...");
		res = mysql_list_tables(&mysql, database_table_name);
		if (mysql_fetch_row(res) == NULL)
		{
			LOG_INFO("MysqlData::CreateDataTable�����ݱ����ڣ��������ݱ�");
			//���ҵ�������ͷŽ�����ռ�
			mysql_free_result(res);						
			// �������ݱ���
			char create_datatable[512];
			snprintf(create_datatable, 512, "create table if not exists %s (file_name varchar(200) not null,create_date char(12),file_path varchar(500) primary key);", database_table_name);
			mysql_query(&mysql, create_datatable);
			LOG_INFO("MysqlData::CreateDataTable�����ݱ�{}�������", database_table_name);
			return;
		}
		else
		{
			LOG_INFO("MysqlData::CreateDataTable�����ݱ��Ѵ���");
			return;
		}
		LOG_CRITICAL("MysqlData::CreateDataTable�����ݱ���ʧ��");
		return;
	}
	
	// ������ݿ��Ƿ���ͬ����·��
	bool MysqlTable::MysqlFilePathCheck(const std::filesystem::path& file_path)
	{
		if (!std::filesystem::exists(file_path))
		{
			LOG_INFO("������ݿ⣺�ļ�·��������,path:{}", file_path.generic_string());
			return false;
		}
		char check_path[256];
		snprintf(check_path, 256, "select * from %s where file_path ='%s';", database_table_name, file_path.generic_u8string().c_str());
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
	void MysqlTable::AddMysqlFileData(const std::filesystem::path& file_path)
	{
		if (MysqlFilePathCheck(file_path))
		{
			LOG_WARN("���Mysql���ݣ�����ͬ·�������ݣ��޷�����������ݲ���");
			return;
		}
		FileDataStream file_data(file_path);

		char get_data[2000];
		snprintf(get_data, 2000, "insert into %s(file_name,create_date,file_path)values('%s','%s','%s');",
			database_table_name, file_data.GetName().c_str(), file_data.GetCreateDate().c_str(), file_data.GetPath().c_str());
		mysql_query(&mysql, get_data);
		
		LOG_INFO("���Mysql���ݣ���������ӳɹ�");
		return;
	}

	// ����ճ������
	void MysqlTable::CopyPasteMysqlFileData(const std::filesystem::path& from_path, const std::filesystem::path& to_path)
	{
		LOG_INFO("MysqlTable::CopyPasteMysqlFileData:��Mysql����ճ���ļ���...");

		if (MysqlFilePathCheck(to_path/ from_path.filename()))
		{
			LOG_WARN("����ճ��Mysql���ݣ�����ͬ·�������ݣ��޷����и���ճ�����ݲ���");
			return;
		}
		FileDataStream file_data(to_path / from_path.filename());

		char get_data[2000];
		snprintf(get_data, 2000, "insert into %s(file_name,create_date,file_path)values('%s','%s','%s');",
			database_table_name, file_data.GetName().c_str(), file_data.GetCreateDate().c_str(), file_data.GetPath().c_str());
		mysql_query(&mysql, get_data);

		LOG_INFO("����ճ��Mysql���ݣ����ݸ���ճ���ɹ�");
		return;
	}

	// ɾ������
	void MysqlTable::DeleteMysqlFileData(const std::filesystem::path& delete_path)
	{
		if (!MysqlFilePathCheck(delete_path))
		{
			LOG_WARN("ɾ��Mysql���ݣ������ڸ�·�������ݣ��޷�����ɾ�����ݲ���");
			return;
		}
		char sql[2000];
		snprintf(sql, 2000, "delete from %s where file_path='%s';", database_table_name, delete_path.generic_u8string().c_str());
		mysql_query(&mysql, sql);
		LOG_INFO("ɾ��Mysql���ݣ�����ɾ���ɹ�");
		return;
	}

	// �޸�����
	void MysqlTable::ChangeMysqlFileData(const std::filesystem::path& old_path, const MysqlOperate::FileDataStream& file_data)
	{
		if (!MysqlFilePathCheck(old_path))
		{
			LOG_WARN("�޸�Mysql���ݣ������ڸ�·�������ݣ��޷������޸����ݲ���");
			return;
		}
		char sql[2000];
		snprintf(sql, 2000, "update %s set file_name='%s',create_date='%s',file_path='%s' where file_path='%s';",
			database_table_name, file_data.GetName().c_str(), file_data.GetCreateDate().c_str(), 
			file_data.GetPath().c_str(), old_path.generic_string().c_str());
		mysql_query(&mysql, sql);
		LOG_INFO("�޸�Mysql���ݣ������޸ĳɹ�");
		return;
	}

	// ��������ʼ��
	FileDataStream::FileDataStream(const std::filesystem::path& new_file_path)
	{
		file_name = new_file_path.filename().generic_u8string();
		file_create_date = StringToUtf8(MoObject::GetCurrentDate());
		file_path = new_file_path.generic_u8string();
		LOG_INFO("Mysql����������ʼ���ɹ�");
	}

}
