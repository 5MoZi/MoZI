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

        // ������ʾ��־λ
        void StopDisplay();     // ֹͣ��ʾ
        void BeginDisplay();    // ��ʼ��ʾ
        void ReverseDisplay();     // ��ת��ʾ��־λ
        // ��������أ���ʾ����������ֱ��mde::ddp("xxx")����
        void operator()(const char* fmt, ...)IM_FMTARGS(2);


        // ��������
        static int   Stricmp(const char* s1, const char* s2);
        static int   Strnicmp(const char* s1, const char* s2, int n);
        static char* Strdup(const char* s);
        static void  Strtrim(char* s);
        void ExecCommand(const char* command_line);

        static int TextEditCallbackStub(ImGuiInputTextCallbackData* data);
        int TextEditCallback(ImGuiInputTextCallbackData* data);
    };

    // ȫ�ֱ������ṩ������ϵͳʹ��
    extern DubugDisplay ddp;
}


