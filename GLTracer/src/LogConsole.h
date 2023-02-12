#pragma once
#include "imgui/imgui.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

enum LOG_TYPE {
    LOG_TYPE_INFO,
    LOG_TYPE_WARNING,
    LOG_TYPE_ERROR
};

#define LOGE(fmt, ...) LogConsole::Instance().AddLog(LOG_TYPE_ERROR, fmt, __VA_ARGS__)
#define LOGI(fmt, ...) LogConsole::Instance().AddLog(LOG_TYPE_INFO, fmt, __VA_ARGS__)
#define LOGW(fmt, ...) LogConsole::Instance().AddLog(LOG_TYPE_WARNING, fmt, __VA_ARGS__)

class LogConsole
{
private:
    char                  InputBuf[256];
    ImVector<char*>       Items;
    ImVector<const char*> Commands;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImGuiTextFilter       Filter;
    bool                  AutoScroll;
    bool                  ScrollToBottom;
    bool                  bFilterError = false;
    bool                  bFilterInfo = false;
    bool                  bFilterWarning = false;
public:
    void    ClearLog();
    void    AddLog(LOG_TYPE logType,const char* fmt, ...) IM_FMTARGS(2);
    void    Draw(const char* title, bool* p_open);
    void    ExecCommand(const char* command_line);
    int     TextEditCallback(ImGuiInputTextCallbackData* data);

    LogConsole(const LogConsole&) = delete;
    LogConsole& operator=(const LogConsole&) = delete;

    static LogConsole& Instance() {
        static LogConsole m_pInstance;
        return m_pInstance;

    }

    LogConsole()
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
    }
    ~LogConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }
};

