#include "mopch.h"
#include "TextEditor.h"




void TextEditor::TextEditorBegin()
{
    ImGui::BeginChild("MarkEditor", ImVec2(-FLT_MIN, -FLT_MIN), ImGuiChildFlags_Border, 
        ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar);

    // 只有符合设定的文件类型才能使用TextEditor
    ExtensionCheck();
    if (file_extension_flag == TextExtensionFlag_Null)
    {
        ImGui::EndChild();
        return;
    }
    
    SetContent();               // 设定文本内容
    TextEditorHotKeys();        // 快捷键设置

    if (TextContent.empty())
        TextContent.push_back(0);
    TextEditor::TextEditorFuncs::MyInputTextMultiline("##TextEditor", &TextContent, ImVec2(-FLT_MIN, -FLT_MIN));


    ImGui::EndChild();
}

void TextEditor::ExtensionCheck()
{
    if (std::filesystem::is_regular_file(file_path))
    {
        std::filesystem::path text_extension = file_path.extension();
        if (text_extension == TEXT_EXTENSION_TXT)
            file_extension_flag = TextExtensionFlag_Txt;
        else if (text_extension == TEXT_EXTENSION_MARKDOWN)
            file_extension_flag = TextExtensionFlag_Markdown;
        else
            file_extension_flag = TextExtensionFlag_Null;
    }
    else 
        file_extension_flag= TextExtensionFlag_Null;
}

void TextEditor::SetContent()
{
    // 保证一个文件路径只获取一次内容即可
    if (last_file_path != file_path)
    {
        std::ifstream open_text(file_path);
        if (open_text.good())
        {
            TextContent.clear();
            std::string str((std::istreambuf_iterator<char>(open_text)), std::istreambuf_iterator<char>());
            for (auto chr : str)
            {
                TextContent.push_back(chr);
            }
            // 要有终止符号
            TextContent.push_back('\0');
        }
        open_text.close();       //关闭文件
        last_file_path = file_path;
    }
}

std::string TextEditor::GetContent()
{
    std::string Content;
    for (auto it = TextContent.begin(); it != TextContent.end(); it++)
        Content += *it;
    return Content;
}

void TextEditor::SaveText()
{
    std::ofstream open_text(file_path);
    if (open_text.good())
    {
        std::string Content;
        for (auto it = TextContent.begin(); it != TextContent.end(); it++)
            Content += *it;
        open_text << Content;
        open_text.close();
    }
}

void TextEditor::TextEditorHotKeys()
{
    ImGuiIO& io = ImGui::GetIO();
    auto shift = io.KeyShift;
    auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
    auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;

    if (ImGui::IsWindowFocused())
    {
        if (ImGui::IsWindowHovered())
            ImGui::SetMouseCursor(ImGuiMouseCursor_TextInput);
        if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGuiKey_S))
            SaveText();
    }
}

int TextEditor::TextEditorFuncs::MyResizeCallback(ImGuiInputTextCallbackData* data)
{
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        ImVector<char>* my_str = (ImVector<char>*)data->UserData;
        IM_ASSERT(my_str->begin() == data->Buf);
        my_str->resize(data->BufSize);
        data->Buf = my_str->begin();
    }
    return 0;
}

bool TextEditor::TextEditorFuncs::MyInputTextMultiline(const char* label, ImVector<char>* my_str, const ImVec2& size, ImGuiInputTextFlags flags)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    return ImGui::InputTextMultiline(label, my_str->begin(), (size_t)my_str->size(), size, flags | ImGuiInputTextFlags_CallbackResize, TextEditor::TextEditorFuncs::MyResizeCallback, (void*)my_str);
}