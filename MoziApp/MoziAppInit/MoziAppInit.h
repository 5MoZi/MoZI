#pragma once

// �������
#define ThemeColorNum 3					// ������ɫ������


class MoziAppInit
{
public:
	MoziAppInit();
	bool* GetThemeColorChanger();


private:
	bool* theme_color_changer = new bool[ThemeColorNum]();		// ������ɫ�任����������ĳ����ɫ����ʱ������ɫ�±���Ϊ1����Ϊ0;


	void ThemeColorInit();		// ������ɫ��ʼ��
	void StyleInit();			// ��ʽ��ʼ��

};

MoziAppInit* GetMoziAppInitIO();

