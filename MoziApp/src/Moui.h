#pragma once


namespace Moui {


    // UI������ɫ
    enum ThemeColor
    {
        ThemeColor_Light = 0,		// ��ɫ
        ThemeColor_Dark = 1,		// ��ɫ
        ThemeColor_Classic = 2	    // ������ɫ
    };

    void MouiInit();

	void DynamicDPI(GLFWwindow* window, ImGuiIO& io);

    const std::string IconAndChinese(const std::string str1, const std::string str2, const int type = 1);

    void ChangeThemeColor();
    // �½��ļ����ļ��е���
    void AddNewFilePopup(bool& open_popup, const char* popup_name, const FileOperate::FileFormat& file_format, const std::filesystem::path& current_path,
        const float& current_scale);

    void RenameFilePopup(bool& open_popup, const char* popup_name,
        const std::filesystem::path& current_path, const float& current_scale);
    void PasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale, const bool& copy_cut_flag = false);

    void AddNewFileAndFolderPopup(bool& open_popup, const char* popup_name, const FileOperate::FileFormat& file_format,
        const std::filesystem::path& current_path, const float& current_scale);
    void CopyPasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale);
    void CutPasteFilePopup(bool& open_popup, const char* popup_name, const std::filesystem::path& from_path,
        const std::filesystem::path& to_path, const float& current_scale);
    //void RenameFilePopup(bool& open_popup, const char* popup_name,
    //    std::filesystem::path& current_path, const float& current_scale);
}
