#pragma once

// 主题相关
#define ThemeColorNum 3					// 主题颜色的数量


class MoziAppInit
{
public:
	MoziAppInit();
	bool* GetThemeColorChanger();


private:
	bool* theme_color_changer = new bool[ThemeColorNum]();		// 主题颜色变换器：当启用某个颜色主题时，该颜色下标置为1其余为0;


	void ThemeColorInit();		// 主题颜色初始化
	void StyleInit();			// 格式初始化

};

MoziAppInit* GetMoziAppInitIO();

