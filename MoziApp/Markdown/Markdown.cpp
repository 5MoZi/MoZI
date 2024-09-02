#include "mopch.h"
#include "Markdown.h"
#include "imgui_markdown.h"   
// Following includes for Windows LinkCallback

#define WIN32_LEAN_AND_MEAN
#include "Shellapi.h"

namespace Markdown
{
    static ImGui::MarkdownConfig mdConfig;

    //void GetUrlImage(const char* url, int& len)
    //{
    //    static std::string later_url = { 0 };
    //    std::string new_url = url;
    //    if (later_url != new_url)
    //    {
    //        int nmlen = MultiByteToWideChar(CP_ACP, 0, url, len + 1, NULL, 0);//如果函数运行成功，并且cchWideChar为零，
    //        //返回值是接收到待转换字符串的缓冲区所需求的宽字符数大小。
    //        wchar_t* buffer = new wchar_t[nmlen];
    //        MultiByteToWideChar(CP_ACP, 0, url, len + 1, buffer, nmlen);
    //        HRESULT hr = URLDownloadToFile(NULL, buffer, _T("C:\\Users\\MoZI\\Desktop\\testvsgit\\name.png"), 0, NULL);
    //        bool ret = MoImage::LoadTextureFromFile("C:\\Users\\MoZI\\Desktop\\testvsgit\\name.png", &my_image_texture,
    //            &my_image_width, &my_image_height);
    //        later_url = new_url;
    //    }

    //}

    void MarkdownLinkCallback(ImGui::MarkdownLinkCallbackData data_)
    {
        std::string url(data_.link, data_.linkLength);
        if (!data_.isImage)
        {
            ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
        }
    }

    inline ImGui::MarkdownImageData MarkdownImageCallback(ImGui::MarkdownLinkCallbackData data_)
    {
        // In your application you would load an image based on data_ input. Here we just use the imgui font texture.
        ImTextureID image = ImGui::GetIO().Fonts->TexID;
        // > C++14 can use ImGui::MarkdownImageData imageData{ true, false, image, ImVec2( 40.0f, 20.0f ) };
        ImGui::MarkdownImageData imageData;
        imageData.isValid = true;
        imageData.useLinkCallback = false;
        imageData.user_texture_id = image;
        imageData.size = ImVec2(40.0f, 20.0f);

        // For image resize when available size.x > image width, add
        ImVec2 const contentSize = ImGui::GetContentRegionAvail();
        if (imageData.size.x > contentSize.x)
        {
            float const ratio = imageData.size.y / imageData.size.x;
            imageData.size.x = contentSize.x;
            imageData.size.y = contentSize.x * ratio;
        }

        return imageData;
    }
    void LoadFonts(ImFont* H1, ImFont* H2, ImFont* H3)
    {
        mdConfig.headingFormats[0] = { H1, true };
        mdConfig.headingFormats[1] = { H2, true };
        mdConfig.headingFormats[2] = { H3, false };
    }

    void ExampleMarkdownFormatCallback(const ImGui::MarkdownFormatInfo& markdownFormatInfo_, bool start_)
    {
        // Call the default first so any settings can be overwritten by our implementation.
        // Alternatively could be called or not called in a switch statement on a case by case basis.
        // See defaultMarkdownFormatCallback definition for furhter examples of how to use it.
        ImGui::defaultMarkdownFormatCallback(markdownFormatInfo_, start_);

        switch (markdownFormatInfo_.type)
        {
            // example: change the colour of heading level 2
        case ImGui::MarkdownFormatType::HEADING:
        {
            if (markdownFormatInfo_.level == 2)
            {
                if (start_)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                }
                else
                {
                    ImGui::PopStyleColor();
                }
            }
            break;
        }
        default:
        {
            break;
        }
        }
    }

    void MarkdownBegin(const std::string& markdown_)
    {
        mdConfig.linkCallback = MarkdownLinkCallback;
        mdConfig.tooltipCallback = NULL;
        mdConfig.imageCallback = MarkdownImageCallback;
        mdConfig.linkIcon = ICON_FA_LINK;
        mdConfig.userData = NULL;
        mdConfig.formatCallback = ExampleMarkdownFormatCallback;
        ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);
    }
}
