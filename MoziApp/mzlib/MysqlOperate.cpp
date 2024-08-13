#include "mopch.h"
#include "MysqlOperate.h"
#include "Log.h"
#include "MoObject.h"

namespace MysqlOperate {


//-----------------------------------------------------------------------------
//                               Mysql数据库相关操作
//-----------------------------------------------------------------------------
	// 数据库初始化
	static MYSQL mysql;
	MYSQL_RES* res;	// 查询结果集
	MYSQL_ROW row;	// 记录结构体
	// 连接数据库
	void ConnectDatabase()
	{
		LOG_INFO("Mysql数据库连接中......");
		//初始化数据库句柄
		mysql_init(&mysql);
		//设置数据库编码方式
		mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");

		//连接数据库
		if (mysql_real_connect(&mysql, "localhost", DATABASE_USER_NAME, DATABASE_PASSWORD, DATABASE_NAME, 3306, NULL, 0) == NULL)
		{
			LOG_CRITICAL("Mysql数据库连接失败，失败原因：{}", mysql_error(&mysql));
			return;
		}
		LOG_INFO("Mysql数据库连接成功，数据库名：{}", DATABASE_NAME);
		return;
	}
	
	// 创建文件数据表，可以创建多个
	void CreateFileDataTable(const std::vector <std::string> database_table_name)
	{
		LOG_INFO("MysqlData::CreateDataTable：数据表存在检测中......");
		for (int i = 0; i < database_table_name.size(); i++)
		{
			res = mysql_list_tables(&mysql, database_table_name[i].c_str());
			if (mysql_fetch_row(res) == NULL)
			{
				LOG_INFO("MysqlData::CreateDataTable：数据表不存在，创建数据表");
				//查找到结果：释放结果集空间
				mysql_free_result(res);
				// 进行数据表建立
				char create_datatable[512];
				snprintf(create_datatable, 512, "create table if not exists %s (file_name varchar(200) not null,create_date char(12),file_path varchar(500) primary key);", database_table_name[i].c_str());
				mysql_query(&mysql, create_datatable);
				LOG_INFO("MysqlData::CreateDataTable：数据表{}创建完成", database_table_name[i]);
			}
			else
			{
				LOG_INFO("MysqlData::CreateDataTable：数据表{}已存在", database_table_name[i]);
				mysql_free_result(res);
			}
		}
	}

	// 检测数据库是否有同样的路径
	bool MysqlFilePathCheck(const std::filesystem::path& file_path, const char* database_table_name)
	{
		LOG_INFO("MysqlFilePathCheck：在Mysql检测是否存在相同路径......");
		char check_path[256];
		snprintf(check_path, 256, "select * from %s where file_path ='%s';", database_table_name, file_path.generic_u8string().c_str());
		mysql_query(&mysql, check_path);
		// 无检测到相同路径直接进行添加数据
		res = mysql_store_result(&mysql);
		if (mysql_fetch_row(res))
		{
			LOG_INFO("MysqlFilePathCheck：检测数据库：存在同路径的数据");
			mysql_free_result(res);						//查找到结果：释放结果集空间
			return true;
		}
		else
		{
			LOG_INFO("MysqlFilePathCheck：检测数据库：不存在同路径的数据");
			mysql_free_result(res);						//查找到结果：释放结果集空间
			return false;
		}
	}

	// 添加数据
	void AddMysqlFileData(const std::filesystem::path& file_path, const char* database_table_name)
	{
		LOG_INFO("AddMysqlFileData：在Mysql添加文件中......");
		if (MysqlFilePathCheck(file_path, database_table_name))
		{
			LOG_WARN("AddMysqlFileData：添加Mysql数据：存在同路径的数据，无法进行添加数据操作");
			return;
		}
		FileDataStream file_data(file_path);

		char get_data[2000];
		snprintf(get_data, 2000, "insert into %s(file_name,create_date,file_path)values('%s','%s','%s');",
			database_table_name, file_data.GetName().c_str(), file_data.GetCreateDate().c_str(), file_data.GetPath().c_str());
		mysql_query(&mysql, get_data);

		LOG_INFO("AddMysqlFileData：添加Mysql数据：新数据添加成功");
		return;
	}

	// 粘贴数据
	void PasteMysqlFileData(const std::filesystem::path& from_path, const std::filesystem::path& to_path, const char* database_table_name, const char* temp_table_name)
	{
		LOG_INFO("PasteMysqlFileData：在Mysql粘贴文件中......");

		if (MysqlFilePathCheck(to_path / from_path.filename(), database_table_name))
		{
			LOG_WARN("PasteMysqlFileData：粘贴Mysql数据：存在同路径的数据，无法进行复制粘贴数据操作");
			return;
		}
		FileDataStream new_file_data(to_path / from_path.filename());
		char sql_cmd[2000];

		snprintf(sql_cmd, 2000, "insert into %s(file_name,create_date,file_path)values('%s','%s','%s');",
			database_table_name, new_file_data.GetName().c_str(), new_file_data.GetCreateDate().c_str(), new_file_data.GetPath().c_str());
		mysql_query(&mysql, sql_cmd);
		// 注意：文件不需要进行该操作，因为文件下没有其余的文件
		// 由于在更改mysql之前以及修改了文件名所以不能用std::filesystem::is_directory进行判断
		if (from_path.extension() == "")
		{
			LOG_INFO("PasteMysqlFileData：文件递归粘贴中......");
			// 第一步：如果文件夹from_path下有文件，则将其下的所有文件复制到临时表中
			snprintf(sql_cmd, 2000, "insert into %s select * from %s where file_path like '%%%s/%%';",
				temp_table_name, database_table_name, from_path.generic_u8string().c_str());
			mysql_query(&mysql, sql_cmd);
			// 第二步：修改路径，利用字符串替换，将原有的文件路径的一部分替换为新路径
			snprintf(sql_cmd, 2000, "update %s set file_path=replace(file_path,'%s','%s') where file_path like '%%%s%%';",
				temp_table_name, from_path.generic_u8string().c_str(),
				new_file_data.GetPath().c_str(), from_path.generic_u8string().c_str());
			mysql_query(&mysql, sql_cmd);
			// 第三步：更新所有文件的日期，全部变为new_file_data创建日期
			snprintf(sql_cmd, 2000, "update %s set create_date = '%s';",
				temp_table_name, new_file_data.GetCreateDate().c_str());
			mysql_query(&mysql, sql_cmd);
			// 第四步：将临时表的数据复制回原表中
			snprintf(sql_cmd, 2000, "insert into %s select * from %s;",
				database_table_name, temp_table_name);
			mysql_query(&mysql, sql_cmd);
			// 第五步：清空临时表，以备下一次使用
			snprintf(sql_cmd, 2000, "truncate table %s;", temp_table_name);
			mysql_query(&mysql, sql_cmd);
			LOG_INFO("PasteMysqlFileData：文件递归粘贴完成");
		}

		LOG_INFO("PasteMysqlFileData：粘贴Mysql数据：数据粘贴成功");
		return;
	}

	// 删除数据
	void DeleteMysqlFileData(const std::filesystem::path& delete_path, const char* database_table_name)
	{
		LOG_INFO("DeleteMysqlFileData：在Mysql删除文件中......");
		if (!MysqlFilePathCheck(delete_path, database_table_name))
		{
			LOG_WARN("DeleteMysqlFileData：删除Mysql数据：不存在该路径的数据，无法进行删除数据操作");
			return;
		}
		char sql[2000];
		snprintf(sql, 2000, "delete from %s where file_path like'%%%s%%';", database_table_name, delete_path.generic_u8string().c_str());
		mysql_query(&mysql, sql);
		LOG_INFO("DeleteMysqlFileData：删除Mysql数据：数据删除成功");
		return;
	}

	// 修改数据
	void ChangeMysqlFileData(const std::filesystem::path& old_path, const std::filesystem::path& new_path, const char* database_table_name)
	{
		LOG_INFO("ChangeMysqlFileData：在Mysql修改文件中......");
		if (!MysqlFilePathCheck(old_path, database_table_name))
		{
			LOG_WARN("修改Mysql数据：不存在该路径的数据，无法进行修改数据操作");
			return;
		}
		char sql[2000];


		snprintf(sql, 2000, "update %s set file_name='%s',file_path='%s' where file_path='%s';",
			database_table_name, new_path.filename().generic_u8string().c_str(), new_path.generic_u8string().c_str(), old_path.generic_u8string().c_str());
		mysql_query(&mysql, sql);

		// 如果是文件夹，则需要将文件夹下所有的文件路径都进行替换
		// 由于在更改mysql之前以及修改了文件名所以不能用std::filesystem::is_directory进行判断
		if (old_path.extension() == "")
		{
			snprintf(sql, 2000, "update %s set file_path=replace(file_path,'%s','%s') where file_path like '%%%s%%';",
				database_table_name, old_path.generic_u8string().c_str(),
				new_path.generic_u8string().c_str(), old_path.generic_u8string().c_str());
		}
		mysql_query(&mysql, sql);
		LOG_INFO("ChangeMysqlFileData：修改Mysql数据：数据修改成功");
		return;
	}

	MYSQL* GetMysqlIo()
	{
		return &mysql;
	}


//-----------------------------------------------------------------------------
//                               Mysql文件数据流相关操作
//-----------------------------------------------------------------------------
	// 数据流初始化
	FileDataStream::FileDataStream(const std::filesystem::path& new_file_path)
	{
		LOG_INFO("FileDataStream：创建Mysql文件数据流");
		file_name = new_file_path.filename().generic_u8string();
		std::filesystem::path strpath = MoObject::GetCurrentDate();
		file_create_date = strpath.generic_u8string();
		file_path = new_file_path.generic_u8string();
		LOG_INFO("FileDataStream：Mysql数据流创建成功");
	}

}
