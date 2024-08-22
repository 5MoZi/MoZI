#pragma once


namespace MoObject
{
    // 日期对象类
	class TimeObject
	{
	public:
		std::string GetCurrentDate();
	private:
		std::string year;		// 年
		std::string month;		// 月
		std::string day;		// 日
	};
    // 获取当前日期函数
	std::string GetCurrentDate();

    // moui 弹窗格式
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

        //拷贝构造函数
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

        float scale;        // 规模

        // 窗口信息
        float window_hight;
        float window_width;

        // 设置弹窗的尺寸和确定按钮的大小
        ImVec2 element_pos;          // 元素位置
        ImVec2 element_space;        // 元素间距
        ImVec2 bar_size;             // 进度条尺寸
        ImVec2 button_size;          // 按钮尺寸

    }MouiPopupStyle;




}



