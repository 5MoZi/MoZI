#include "mopch.h"
#include "Log.h"
#include "FileOperate.h"
#include "MysqlOperate.h"
#include "MoObject.h"

#include "Moui.h"
#include "MoziPage.h"

#include "Log.h"

#include "MoImage.h"

#include "markdown.h"

#define APP_NAME u8"MoZI"

/*************************关闭终端的方法*******************************/
// 1、打开项目属性，选择连接器中的系统
// 2、在子系统栏中，将控制台换成窗口
// 3、依旧在连接器中选择高级
// 4、在入口点出输入  mainCRTStartup 
/********************************************************************/

/**************生成release版本与debug版本注意事项**********************/
// 1、当生成debug版本时需要再项目属性中、连接器中选择输入
// 2、在输入中选择忽略特定默认库输入：msvcrt.lib
// 3、生成release版本时将上述的输入删除即可
/********************************************************************/
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main()
{
    //-----------------------------------------------------------------------------
    //                                  基础配置
    //-----------------------------------------------------------------------------
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, APP_NAME, nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init(glsl_version);
    //-----------------------------------------------------------------------------
    //                               MOZI初始化设置
    //-----------------------------------------------------------------------------

 
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    MoziPage::MoziAppInit();

    //int my_image_width = 0;
    //int my_image_height = 0;
    //GLuint my_image_texture = 0;
    //bool ret = MoImage::LoadTextureFromFile("C:\\Users\\MoZI\\Desktop\\x.jpg", &my_image_texture, &my_image_width, &my_image_height);
    //IM_ASSERT(ret);
    //-----------------------------------------------------------------------------
    //                               循环主体
    //-----------------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // 动态DIP，依据屏幕的DIP来设定字体
        Moui::DynamicDPI(window, io);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //// 前面都是初始化操作与设定，下面才是程序运行部分
        ///*******************UI运行与程序部分***********************/

        MoziPage::HomePage();
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
 

        //ImGui::Begin("OpenGL Texture Text");
        //ImGui::Text("pointer = %x", my_image_texture);
        //ImGui::Text("size = %d x %d", my_image_width, my_image_height);
        //ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));
        //ImGui::End();
        ///********************************************************/

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
    }

    // Cleanup
    mysql_close(MysqlOperate::GetMysqlIo());		//关闭数据库

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}