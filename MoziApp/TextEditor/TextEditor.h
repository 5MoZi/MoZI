#pragma once



// ����չeditor��ʹ�õ��ļ�����ʱ����Ҫ�����������ط����
// 1��#define ��������Ҫ���ļ���׺
// 2���� enum TextExtensionFlag����������͵�ö��
// 3����const std::map<std::string, TextExtensionFlag> text_extension_map��������͵�ӳ��
#define EDITOR_ALLOW_EXTENSION_TXT              ".txt"
#define EDITOR_ALLOW_EXTENSION_MARKDOWN         ".md"


class TextEditor
{
public:
    enum TextExtensionFlag
    {
        TextExtensionFlag_Null,         // δ֪��׺
        TextExtensionFlag_Txt,          // �ı��ļ���׺
        TextExtensionFlag_Markdown      // md�ļ���׺
    };

    const std::map<std::filesystem::path, TextExtensionFlag> text_extension_map
    {
        {EDITOR_ALLOW_EXTENSION_TXT,        TextExtensionFlag_Txt},
        {EDITOR_ALLOW_EXTENSION_MARKDOWN,   TextExtensionFlag_Markdown}
    };

    TextEditor(const std::filesystem::path c_file_path, const std::filesystem::path c_last_file_path = "",
        const TextEditor::TextExtensionFlag& c_file_extension_flag = TextExtensionFlag_Null) :
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
    inline void SetEditorFilePath(const std::filesystem::path& c_file_path){ file_path = c_file_path; }

    std::filesystem::path file_path;
    TextExtensionFlag file_extension_flag;
    float font_scale = 1.0f;
private:
    std::filesystem::path last_file_path;
    ImVector<char> TextContent;

};
