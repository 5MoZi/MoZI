#pragma once



namespace mde
{
    class DubugDisplay
    {
    public:
        char                  InputBuf[256];
        ImVector<char*>       Items;
        ImVector<const char*> Commands;
        ImVector<char*>       History;
        int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
        ImGuiTextFilter       Filter;
        bool                  AutoScroll;
        bool                  ScrollToBottom;
        bool                  IsDisplay;

        DubugDisplay();
        ~DubugDisplay();

        void ClearLog();
        void AddLog(const char* fmt, ...) IM_FMTARGS(2);

        // 启动显示标志位
        void StopDisplay();     // 停止显示
        void BeginDisplay();    // 开始显示
        void ReverseDisplay();     // 翻转显示标志位
        // 运算符重载，显示函数，调用直接mde::ddp("xxx")即可
        void operator()(const char* fmt, ...)IM_FMTARGS(2);


        // 帮助函数
        static int   Stricmp(const char* s1, const char* s2);
        static int   Strnicmp(const char* s1, const char* s2, int n);
        static char* Strdup(const char* s);
        static void  Strtrim(char* s);
        void ExecCommand(const char* command_line);

        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        int TextEditCallback(ImGuiInputTextCallbackData* data);
    };

    // 全局变量，提供给整个系统使用
    extern DubugDisplay ddp;
}


