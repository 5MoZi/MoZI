#include "mopch.h"
#include "MoObject.h"
#include "Log.h"

namespace MoObject
{
	std::string TimeObject::GetCurrentDate()
	{
		// ��ȡϵͳʱ��
		time_t timep;
		time(&timep);
		tm p;
		localtime_s(&p, &timep);
		char buff[50];
		year = std::to_string(1900 + p.tm_year);
		month = std::to_string(1 + p.tm_mon);
		day = std::to_string(p.tm_mday);
		LOG_INFO("��ȡ��ǰ������...");
		return (year + "-" + month + "-" + day);
	}

	std::string GetCurrentDate()
	{
		// ��ȡϵͳʱ��
		time_t timep;
		time(&timep);
		tm p;
		localtime_s(&p, &timep);
		std::string year = std::to_string(1900 + p.tm_year);
		std::string month = std::to_string(1 + p.tm_mon);
		std::string day = std::to_string(p.tm_mday);
		LOG_INFO("��ȡ��ǰ������...");
		return (year + "-" + month + "-" + day);
	}

	void MouiPopupStyle::ChangeStlyeSize(const float& change_scale)
	{
		float change_rate = change_scale / scale;   // �������

		// ���ݱ����޸Ķ�Ӧ�ߴ�
		window_hight = window_hight * change_rate;
		window_width = window_width * change_rate;
		element_pos = ImVec2(element_pos.x * change_rate, element_pos.y * change_rate);
		element_space = ImVec2(element_space.x * change_rate, element_space.y * change_rate);
		bar_size = ImVec2(bar_size.x * change_rate, bar_size.y * change_rate);
		button_size = ImVec2(button_size.x * change_rate, button_size.y * change_rate);

		// ˢ�¹�ģ
		scale = change_scale;
	}




}
