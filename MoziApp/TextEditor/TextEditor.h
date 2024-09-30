#pragma once



// 当扩展editor可使用的文件类型时，需要再下面三个地方添加
// 1、#define 定义所需要的文件后缀
// 2、在 enum TextExtensionFlag内添加新类型的枚举
// 3、在const std::map<std::string, TextExtensionFlag> text_extension_map添加新类型的映射
#define EDITOR_ALLOW_EXTENSION_TXT              ".txt"
#define EDITOR_ALLOW_EXTENSION_MARKDOWN         ".md"


class TextEditor
{
public:
    enum TextExtensionFlag
    {
        TextExtensionFlag_Null,         // 未知后缀
        TextExtensionFlag_Txt,          // 文本文件后缀
        TextExtensionFlag_Markdown      // md文件后缀
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
    // 临时输入函数结构体
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
