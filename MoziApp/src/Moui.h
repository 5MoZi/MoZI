#pragma once


namespace Moui {


    // UI主题颜色
    enum ThemeColor
    {
        ThemeColor_Light = 0,		// 白色
        ThemeColor_Dark = 1,		// 黑色
        ThemeColor_Classic = 2	    // 经典紫色
    };

    void MouiInit();

	void DynamicDPI(GLFWwindow* window, ImGuiIO& io);

    const std::string IconAndChinese(const std::string str1, const std::string str2, const int type = 1);

    void ChangeThemeColor();



}
