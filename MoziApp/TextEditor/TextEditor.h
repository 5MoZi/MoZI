#pragma once



#define TEXT_EXTENSION_TXT              ".txt"
#define TEXT_EXTENSION_MARKDOWN         ".md"


class TextEditor
{
public:
    enum TextExtensionFlag_
    {
        TextExtensionFlag_Null,         // δ֪��׺
        TextExtensionFlag_Txt,          // �ı��ļ���׺
        TextExtensionFlag_Markdown      // md�ļ���׺
    };

    TextEditor(const std::filesystem::path c_file_path, const std::filesystem::path c_last_file_path = "",
        const TextEditor::TextExtensionFlag_& c_file_extension_flag = TextExtensionFlag_Null) :
        file_path(c_file_path),
        last_file_path(c_last_file_path),
        file_extension_flag(c_file_extension_flag)
    {};



    void TextEditorBegin();
    void ExtensionCheck();
    void TextEditorHotKeys();
    // ��ʱ���뺯���ṹ��
    struct TextEditorFuncs
    {
        static int MyResizeCallback(ImGuiInputTextCallbackData* data);

        static bool MyInputTextMultiline(const char* label, ImVector<char>* TextContent, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0);
    };

    std::string GetContent();
    void SetContent();
    void SaveText();
    inline void SetMarkdownPath(const std::filesystem::path& c_file_path){ file_path = c_file_path; }

    std::filesystem::path file_path;
    TextExtensionFlag_ file_extension_flag;

private:
    std::filesystem::path last_file_path;
    ImVector<char> TextContent;

};
