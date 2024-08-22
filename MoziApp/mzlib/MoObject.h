#pragma once


namespace MoObject
{
    // ���ڶ�����
	class TimeObject
	{
	public:
		std::string GetCurrentDate();
	private:
		std::string year;		// ��
		std::string month;		// ��
		std::string day;		// ��
	};
    // ��ȡ��ǰ���ں���
	std::string GetCurrentDate();

    // moui ������ʽ
    typedef struct MouiPopupStyle
    {
    public:
        MouiPopupStyle(const float& c_scale = 0, const float& w_h = 0, const float& w_w = 0, const ImVec2& c_pos = ImVec2(0, 0),
            const ImVec2& c_space = ImVec2(0, 0), const ImVec2& c_bar = ImVec2(0, 0), const ImVec2& c_button = ImVec2(0, 0))
            :scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};

        //MzuiPopupStyle(const float& c_scale, const float& w_h, const float& w_w, const ImVec2& c_pos,
        //    const ImVec2& c_space,const ImVec2& c_bar, const ImVec2& c_button)
        //    :scale(c_scale), window_hight(w_h), window_width(w_w), element_pos(c_pos), element_space(c_space), bar_size(c_bar), button_size(c_button) {};

        inline MouiPopupStyle& GetMouiStyle() { return *this; }
        void ChangeStlyeSize(const float& change_scale);

        //�������캯��
        MouiPopupStyle(const MouiPopupStyle& c)
        {
            scale = c.scale;
            window_hight = c.window_hight;
            window_width = c.window_width;
            element_pos = c.element_pos;
            element_space = c.element_space;
            bar_size = c.bar_size;
            button_size = c.button_size;
        }

        float scale;        // ��ģ

        // ������Ϣ
        float window_hight;
        float window_width;

        // ���õ����ĳߴ��ȷ����ť�Ĵ�С
        ImVec2 element_pos;          // Ԫ��λ��
        ImVec2 element_space;        // Ԫ�ؼ��
        ImVec2 bar_size;             // �������ߴ�
        ImVec2 button_size;          // ��ť�ߴ�

    }MouiPopupStyle;




}



