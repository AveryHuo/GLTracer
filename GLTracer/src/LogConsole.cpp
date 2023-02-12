#include "LogConsole.h"
#include "imgui/imgui_internal.h"

#include <chrono>

// Portable helpers
static int   Stricmp(const char* s1, const char* s2) { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char* Strdup(const char* s) { IM_ASSERT(s); size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s) { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }
// In C++11 you'd be better off using lambdas for this sort of forwarding callbacks
static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
    LogConsole* console = (LogConsole*)data->UserData;
    return console->TextEditCallback(data);
}

static void GetLogTimeStr(std::string *strTime) {
    auto now = std::chrono::system_clock::now();
    //通过不同精度获取相差的毫秒数
    uint64_t dis_millseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()
        - std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count() * 1000;
    time_t tt = std::chrono::system_clock::to_time_t(now);
    auto time_tm = localtime(&tt);
    char cstrTime[25] = {};
    sprintf(cstrTime, "%d-%02d-%02d %02d:%02d:%02d %03d", time_tm->tm_year + 1900,
        time_tm->tm_mon + 1, time_tm->tm_mday, time_tm->tm_hour,
        time_tm->tm_min, time_tm->tm_sec, (int)dis_millseconds);
    strTime->append(cstrTime);
}

void LogConsole::ClearLog()
{
    for (int i = 0; i < Items.Size; i++)
        free(Items[i]);
    Items.clear();
}

void LogConsole::AddLog(LOG_TYPE logType, const char* fmt, ...) IM_FMTARGS(2)
{
    // FIXME-OPT
    
    std::string timeStr = "";
    GetLogTimeStr(&timeStr);
    std::string destFmt = "["+timeStr+"]";
    if (logType == LOG_TYPE_ERROR) {
        destFmt.append("[error] ");
    }else if (logType == LOG_TYPE_WARNING) {
        destFmt.append("[warning] ");
    }else if (logType == LOG_TYPE_INFO) {
        destFmt.append("[info] ");
    }
    destFmt.append(fmt);
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, IM_ARRAYSIZE(buf), destFmt.c_str(), args);
    buf[IM_ARRAYSIZE(buf) - 1] = 0;
    va_end(args);
    Items.push_back(Strdup(buf));
}

void LogConsole::Draw(const char* title, bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin(title, p_open))
    {
        ImGui::End();
        return;
    }

    // As a specific feature guaranteed by the library, after calling Begin() the last Item represent the title bar.
    // So e.g. IsItemHovered() will return true when hovering the title bar.
    // Here we create a context menu only available from the title bar.
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Close Console"))
            *p_open = false;
        ImGui::EndPopup();
    }

    ImGui::TextWrapped(
        "This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
        "implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
    ImGui::TextWrapped("Enter 'HELP' for help.");

    // TODO: display items starting from the bottom

    //if (ImGui::SmallButton("Add Debug Text")) { AddLog(LOG_TYPE_INFO,"%d some text", Items.Size); AddLog(LOG_TYPE_INFO, "some more text"); AddLog(LOG_TYPE_INFO, "display very important message here!"); }
    //ImGui::SameLine();
    //if (ImGui::SmallButton("Add Debug Error")) { AddLog(LOG_TYPE_ERROR,"something went wrong"); }
    //ImGui::SameLine();
    //if (ImGui::SmallButton("Add Debug Warning")) { AddLog(LOG_TYPE_WARNING, "this is a warnning!"); }
    //ImGui::SameLine();
   


    ImGui::PushID(0);
    if (bFilterError) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.5f, 1.0f));
        if (ImGui::Button("Error")) { bFilterError = !bFilterError; }
        ImGui::PopStyleColor(3);
    }
    else {
        if (ImGui::Button("Error")) { bFilterError = !bFilterError; }
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(1);
    if (bFilterWarning) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.8f, 0.8f, 1.0f));
        if (ImGui::Button("Warning")) { bFilterWarning = !bFilterWarning; }
        ImGui::PopStyleColor(3);
    }
    else {
        if (ImGui::Button("Warning")) { bFilterWarning = !bFilterWarning; }
    }
    
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID(2);
    if (bFilterInfo) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
        if (ImGui::Button("Info")) { bFilterInfo = !bFilterInfo; }
        ImGui::PopStyleColor(3);
    }
    else {
        if (ImGui::Button("Info")) { bFilterInfo = !bFilterInfo; }
    }
    ImGui::PopID();
    ImGui::SameLine();

    if (ImGui::Button("Clear")) { ClearLog(); }
    ImGui::SameLine();

    bool copy_to_clipboard = ImGui::Button("Copy");


    ImGui::Separator();

    // Options menu
    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable("Clear")) ClearLog();
            ImGui::EndPopup();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        if (copy_to_clipboard)
            ImGui::LogToClipboard();
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            bool has_type_filter = (bFilterError || bFilterWarning || bFilterInfo);
            if (has_type_filter) {
                if(!((bFilterError && strstr(item, "[error]")) || (bFilterWarning && strstr(item, "[warning]"))
                || (bFilterInfo && strstr(item, "[info]"))))
                    continue;
            }
            
            if (!Filter.PassFilter(item))
                continue;

            // Normally you would store more information in your item than just a string.
            // (e.g. make Items[] an array of structure, store color/type etc.)
            ImVec4 color;
            bool has_color = false;
            if (strstr(item, "[error]")) { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
            else if (strstr(item, "[warning]")) { color = ImVec4(0.4f, 1.0f, 1.0f, 1.0f); has_color = true; }
            else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
            if (has_color)
                ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(item);
            if (has_color)
                ImGui::PopStyleColor();
        }
        if (copy_to_clipboard)
            ImGui::LogFinish();

        if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
            ImGui::SetScrollHereY(1.0f);
        ScrollToBottom = false;

        ImGui::PopStyleVar();
    }
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf), input_text_flags, &TextEditCallbackStub, (void*)this))
    {
        char* s = InputBuf;
        Strtrim(s);
        if (s[0])
            ExecCommand(s);
        strcpy(s, "");
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void LogConsole::ExecCommand(const char* command_line)
{
    AddLog(LOG_TYPE_INFO, "# %s\n", command_line);

    // Insert into history. First find match and delete it so it can be pushed to the back.
    // This isn't trying to be smart or optimal.
    HistoryPos = -1;
    for (int i = History.Size - 1; i >= 0; i--)
        if (Stricmp(History[i], command_line) == 0)
        {
            free(History[i]);
            History.erase(History.begin() + i);
            break;
        }
    History.push_back(Strdup(command_line));

    // Process command
    if (Stricmp(command_line, "CLEAR") == 0)
    {
        ClearLog();
    }
    else if (Stricmp(command_line, "HELP") == 0)
    {
        AddLog(LOG_TYPE_INFO, "Commands:");
        for (int i = 0; i < Commands.Size; i++)
            AddLog(LOG_TYPE_INFO, "- %s", Commands[i]);
    }
    else if (Stricmp(command_line, "HISTORY") == 0)
    {
        int first = History.Size - 10;
        for (int i = first > 0 ? first : 0; i < History.Size; i++)
            AddLog(LOG_TYPE_INFO, "%3d: %s\n", i, History[i]);
    }
    else
    {
        AddLog(LOG_TYPE_INFO, "Unknown command: '%s'\n", command_line);
    }

    // On command input, we scroll to bottom even if AutoScroll==false
    ScrollToBottom = true;
}



int LogConsole::TextEditCallback(ImGuiInputTextCallbackData* data)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
    case ImGuiInputTextFlags_CallbackCompletion:
    {
        // Example of TEXT COMPLETION

        // Locate beginning of current word
        const char* word_end = data->Buf + data->CursorPos;
        const char* word_start = word_end;
        while (word_start > data->Buf)
        {
            const char c = word_start[-1];
            if (c == ' ' || c == '\t' || c == ',' || c == ';')
                break;
            word_start--;
        }

        // Build a list of candidates
        ImVector<const char*> candidates;
        for (int i = 0; i < Commands.Size; i++)
            if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
                candidates.push_back(Commands[i]);

        if (candidates.Size == 0)
        {
            // No match
            AddLog(LOG_TYPE_INFO, "No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
        }
        else if (candidates.Size == 1)
        {
            // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
            data->InsertChars(data->CursorPos, candidates[0]);
            data->InsertChars(data->CursorPos, " ");
        }
        else
        {
            // Multiple matches. Complete as much as we can..
            // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
            int match_len = (int)(word_end - word_start);
            for (;;)
            {
                int c = 0;
                bool all_candidates_matches = true;
                for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                    if (i == 0)
                        c = toupper(candidates[i][match_len]);
                    else if (c == 0 || c != toupper(candidates[i][match_len]))
                        all_candidates_matches = false;
                if (!all_candidates_matches)
                    break;
                match_len++;
            }

            if (match_len > 0)
            {
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
            }

            // List matches
            AddLog(LOG_TYPE_INFO, "Possible matches:\n");
            for (int i = 0; i < candidates.Size; i++)
                AddLog(LOG_TYPE_INFO, "- %s\n", candidates[i]);
        }

        break;
    }
    case ImGuiInputTextFlags_CallbackHistory:
    {
        // Example of HISTORY
        const int prev_history_pos = HistoryPos;
        if (data->EventKey == ImGuiKey_UpArrow)
        {
            if (HistoryPos == -1)
                HistoryPos = History.Size - 1;
            else if (HistoryPos > 0)
                HistoryPos--;
        }
        else if (data->EventKey == ImGuiKey_DownArrow)
        {
            if (HistoryPos != -1)
                if (++HistoryPos >= History.Size)
                    HistoryPos = -1;
        }

        // A better implementation would preserve the data on the current input line along with cursor position.
        if (prev_history_pos != HistoryPos)
        {
            const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
            data->DeleteChars(0, data->BufTextLen);
            data->InsertChars(0, history_str);
        }
    }
    }
    return 0;
}
