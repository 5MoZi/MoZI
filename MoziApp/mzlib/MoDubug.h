#pragma once



namespace MoDebug
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


        DubugDisplay()
        {
            ClearLog();
            memset(InputBuf, 0, sizeof(InputBuf));
            HistoryPos = -1;

            // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
            Commands.push_back("HELP");
            Commands.push_back("HISTORY");
            Commands.push_back("CLEAR");
            Commands.push_back("CLASSIFY");
            AutoScroll = true;
            ScrollToBottom = false;
            AddLog("Welcome to Dear ImGui!");
        }
        ~DubugDisplay()
        {
            ClearLog();
            for (int i = 0; i < History.Size; i++)
                free(History[i]);
        }

        // Portable helpers
        static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
        static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
        static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
        static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }


        void ClearLog()
        {
            for (int i = 0; i < Items.Size; i++)
                free(Items[i]);
            Items.clear();
        }

        void AddLog(const char* fmt, ...) IM_FMTARGS(2)
        {
            // FIXME-OPT
            char buf[1024];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
            buf[IM_ARRAYSIZE(buf) - 1] = 0;
            va_end(args);
            Items.push_back(Strdup(buf));
        }
	};
}


