#include "mopch.h"
#include "Markdown.h"
#include "imgui_markdown.h" 
#include "MoImage.h"
#include "Log.h"
#include "fonts.h"
// Following includes for Windows LinkCallback

#define WIN32_LEAN_AND_MEAN
#include "Shellapi.h"




namespace Markdown
{
    // 注意路径一定不能用下面这种方式初始化，血泪教训。一但这样初始化
    // 程序就会卡在链接的时候，主函数都进不去。
    // static std::filesystem::path current_markdown_path = { 0 }; 
    static std::filesystem::path current_markdown_path = "";

    static ImGui::MarkdownConfig mdConfig;
    struct ImageInformation
    {
        GLuint image_num;
        int image_width;
        int image_height;
    };
    static std::map<std::filesystem::path, ImageInformation>image_old_down;
    static std::map<std::filesystem::path, ImageInformation>image_new_down;
  
    // 下载网络上图片用到
    //#include <tchar.h>
    //#include <Urlmon.h>
    //#pragma comment(lib,"urlmon.lib")
    //void GetUrlImage(const char* url, int& len)
    //{
    //    static std::string later_url = { 0 };
    //    std::string new_url(url, len);
    //    if (later_url != new_url)
    //    {
    //        GLuint my_image_texture = 0;
    //        int nmlen = MultiByteToWideChar(CP_ACP, 0, new_url.c_str(), len + 1, NULL, 0);//如果函数运行成功，并且cchWideChar为零，
    //        //返回值是接收到待转换字符串的缓冲区所需求的宽字符数大小。
    //        wchar_t* buffer = new wchar_t[nmlen]; 
    //        MultiByteToWideChar(CP_ACP, 0, new_url.c_str(), len + 1, buffer, nmlen);
    //        HRESULT hr = URLDownloadToFile(NULL, buffer, _T("C:\\Users\\Chen\\Desktop\\test\\name.png"), 0, NULL);
    //        bool ret = MoImage::LoadTextureFromFile("C:\\Users\\Chen\\Desktop\\test\\name.png", &my_image_texture,
    //            &my_image_width, &my_image_height);
    //        later_url = new_url;
    //    }
    //}

    void GetPathImage(const std::filesystem::path& image_path)
    {
        if(image_old_down.count(image_path)==0)
        {
            ImageInformation image_inf;
            bool ret = MoImage::LoadTextureFromFile(image_path.generic_string().c_str(), &image_inf.image_num,
                &image_inf.image_width, &image_inf.image_height);
            image_new_down.insert({ image_path, image_inf });
        }
        else
        {
            image_new_down.insert({ image_path, image_old_down[image_path]});
        }
            
    }

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
        ImGui::MarkdownImageData imageData;
        std::string new_path_str(data_.link, data_.linkLength);
        std::filesystem::path new_path = new_path_str;
        // 判断图片路径是否存在，不存在就直接跳过
        if (std::filesystem::exists(new_path))
        {
            if (new_path.extension() == ".png")
            {
                GetPathImage(new_path);
                imageData.isValid = true;
                imageData.useLinkCallback = false;
                imageData.user_texture_id = (void*)(intptr_t)image_new_down[new_path].image_num;
                imageData.size = ImVec2(image_new_down[new_path].image_width, image_new_down[new_path].image_height);

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
            else
            {
                imageData.isValid = false;
                return imageData;
            }
        }
        else
        {
            imageData.isValid = false;
            return imageData;
        }

    }

    void LoadFonts(ImFont* H1, ImFont* H2, ImFont* H3)
    {
        mdConfig.headingFormats[0] = { H1, true };
        mdConfig.headingFormats[1] = { H2, true };
        mdConfig.headingFormats[2] = { H3, true };
    }
    void LoadFonts()
    {
        ImFont* markdown_heading_fonts = Fonts::GetMarkdownHeadingFonts();
        if (markdown_heading_fonts)
        {
            mdConfig.headingFormats[0] = { markdown_heading_fonts, true };
            mdConfig.headingFormats[1] = { markdown_heading_fonts, true };
            mdConfig.headingFormats[2] = { markdown_heading_fonts, true };
        }
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

    void MarkdownBegin(const std::string& markdown_, const std::filesystem::path& current_open_path)
    { 

        if (current_markdown_path != current_open_path)
        {
            LOG_INFO("MarkdownBegin：markdown文件发生转换");
            image_old_down.clear();
        }


        current_markdown_path = current_open_path;

        LoadFonts();        // 加载字体
        mdConfig.linkCallback = MarkdownLinkCallback;
        mdConfig.tooltipCallback = NULL;
        mdConfig.imageCallback = MarkdownImageCallback;
        mdConfig.linkIcon = ICON_FA_LINK;
        mdConfig.userData = NULL;
        //mdConfig.formatCallback = NULL;

        ImGui::Markdown(markdown_.c_str(), markdown_.length(), mdConfig);

        // 查看image_old_down = image_new_down二者内容代码
        //if (ImGui::IsKeyPressed(ImGuiKey_Keypad0))
        //{
        //    std::cout << "image_old_down" << std::endl;
        //    for (auto iter = image_new_down.begin(); iter != image_new_down.end(); ++iter)
        //    {
        //        std::cout << iter->first << " => " << iter->second.image_num << '\n';
        //    }

        //    std::cout << "image_new_down" << std::endl;
        //    for (auto iter = image_new_down.begin(); iter != image_new_down.end(); ++iter)
        //    {
        //        std::cout << iter->first << " => " << iter->second.image_num << '\n';
        //    }
        //}

        image_old_down = image_new_down;
        image_new_down.clear();
    }
}
