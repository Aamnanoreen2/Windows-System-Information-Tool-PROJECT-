#include <windows.h>
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")


// Function prototypes
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void DisplaySystemTime(HWND hwnd);
void DisplayMemoryInformation(HWND hwnd);
void DisplayOSVersion(HWND hwnd);
void DisplayDiskInformation(HWND hwnd);
void DisplayNetworkInformation(HWND hwnd);
void DrawMemoryGraph(HDC hdc, RECT rect, MEMORYSTATUSEX statex);
void DrawDiskGraph(HDC hdc, RECT rect);
void DisplayRealTimeSystemInfo(HWND hwnd);
void DisplayCPUInformation(HWND hwnd);


// Enumeration to track which graph to display
enum DisplayOption {
    NONE,
    MEMORY,
    DISK
};

// Global variable to store the current display option
DisplayOption currentDisplayOption = NONE;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "OSModuleWindowClass";

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindow(
        "OSModuleWindowClass",
        "Operating System Modules",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 600,
        NULL, NULL, hInstance, NULL);

    // Create buttons for each module
    CreateWindow("BUTTON", "Display System Time ", WS_VISIBLE | WS_CHILD, 50, 50, 200, 30, hwnd, (HMENU)1, hInstance, NULL);
    CreateWindow("BUTTON", "Display Memory Information", WS_VISIBLE | WS_CHILD, 50, 100, 200, 30, hwnd, (HMENU)2, hInstance, NULL);
   // CreateWindow("BUTTON", "Display OS Version", WS_VISIBLE | WS_CHILD, 50, 150, 200, 30, hwnd, (HMENU)3, hInstance, NULL);
    CreateWindow("BUTTON", "Display Disk Information", WS_VISIBLE | WS_CHILD, 50, 150, 200, 30, hwnd, (HMENU)4, hInstance, NULL);
    CreateWindow("BUTTON", "Display Network Information", WS_VISIBLE | WS_CHILD, 50, 200, 200, 30, hwnd, (HMENU)5, hInstance, NULL);
CreateWindow("BUTTON", "Display Real-time System Info", WS_VISIBLE | WS_CHILD, 50, 250, 200, 30, hwnd, (HMENU)6, hInstance, NULL);
CreateWindow("BUTTON", "Display CPU Information", WS_VISIBLE | WS_CHILD, 50, 300, 200, 30, hwnd, (HMENU)7, hInstance, NULL);

    // Display the window
    ShowWindow(hwnd, nCmdShow);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case 1: // Display System Time button
                    DisplaySystemTime(hwnd);
                    currentDisplayOption = NONE; // Reset display option
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;
                case 2: // Display Memory Information button
                    DisplayMemoryInformation(hwnd);
                    currentDisplayOption = MEMORY; // Set display option to memory
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;
                case 3: // Display OS Version button
                    DisplayOSVersion(hwnd);
                    currentDisplayOption = NONE; // Reset display option
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;
                case 4: // Display Disk Information button
                    DisplayDiskInformation(hwnd);
                    currentDisplayOption = DISK; // Set display option to disk
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;
                case 5: // Display Network Information button
                    DisplayNetworkInformation(hwnd);
                    currentDisplayOption = NONE; // Reset display option
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;

                case 6: // Display Real-time System Info button
                    DisplayRealTimeSystemInfo(hwnd);
                    currentDisplayOption = NONE; // Reset display option
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;

                case 7: // Display CPU Information button
                    DisplayCPUInformation(hwnd);
                    currentDisplayOption = NONE; // Reset display option
                    InvalidateRect(hwnd, NULL, TRUE); // Force a repaint
                    break;

            }
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);

            MEMORYSTATUSEX statex;
            statex.dwLength = sizeof(statex);

            if (currentDisplayOption == MEMORY && GlobalMemoryStatusEx(&statex)) {
                DrawMemoryGraph(hdc, rect, statex);
            }

            if (currentDisplayOption == DISK) {
                DrawDiskGraph(hdc, rect);
            }

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void DisplaySystemTime(HWND hwnd) {
    // Launch the Windows built-in clock application
    ShellExecute(NULL, "open", "ms-clock:", NULL, NULL, SW_SHOWDEFAULT);

    // Optionally, you can still display the system time in a message box
    SYSTEMTIME st;
    GetLocalTime(&st);

    std::string message = "Current System Time:\n";
    message += "Year: " + std::to_string(st.wYear) + "\n";
    message += "Month: " + std::to_string(st.wMonth) + "\n";
    message += "Day: " + std::to_string(st.wDay) + "\n";
    message += "Hour: " + std::to_string(st.wHour) + "\n";
    message += "Minute: " + std::to_string(st.wMinute) + "\n";
    message += "Second: " + std::to_string(st.wSecond) + "\n";

    MessageBox(hwnd, message.c_str(), "System Time", MB_OK);
}

void DisplayMemoryInformation(HWND hwnd) {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    if (GlobalMemoryStatusEx(&statex)) {
        std::string message = "Memory Information:\n";
        message += "Total Physical Memory: " + std::to_string(statex.ullTotalPhys) + " bytes\n";
        message += "Available Physical Memory: " + std::to_string(statex.ullAvailPhys) + " bytes\n";

        MessageBox(hwnd, message.c_str(), "Memory Information", MB_OK);
    } else {
        MessageBox(hwnd, "Error retrieving memory information.", "Error", MB_OK | MB_ICONERROR);
    }
}

void DisplayOSVersion(HWND hwnd) {
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (GetVersionEx(&osvi)) {
        std::string versionInfo = "Operating System Version:\n";

        switch (osvi.dwPlatformId) {
            case VER_PLATFORM_WIN32_NT:
                if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
                    versionInfo += "Windows 2000\n";
                else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
                    versionInfo += "Windows XP\n";
                else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
                    versionInfo += "Windows Vista\n";
                else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
                    versionInfo += "Windows 7\n";
                else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
                    versionInfo += "Windows 8\n";
                else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
                    versionInfo += "Windows 8.1\n";
                else if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
                    versionInfo += "Windows 10\n";
                else
                    versionInfo += "Unknown Windows NT Version\n";
                break;

            case VER_PLATFORM_WIN32_WINDOWS:
                if (osvi.dwMinorVersion == 0 && osvi.dwBuildNumber < 1000)
                    versionInfo += "Windows 95\n";
                else if (osvi.dwMinorVersion == 0 && osvi.dwBuildNumber < 1998)
                    versionInfo += "Windows 98\n";
                else if (osvi.dwMinorVersion == 10 && osvi.dwBuildNumber < 2183)
                    versionInfo += "Windows ME\n";
                else
                    versionInfo += "Unknown Windows 9x Version\n";
                break;

            default:
                versionInfo += "Unknown Operating System Version\n";
                break;
        }

        MessageBox(hwnd, versionInfo.c_str(), "Operating System Version", MB_OK);
    } else {
        MessageBox(hwnd, "Error retrieving operating system version information.", "Error", MB_OK | MB_ICONERROR);
    }
}

void DisplayDiskInformation(HWND hwnd) {
    DWORD drives = GetLogicalDrives();

    std::string message = "Disk Information:\n";

    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        if (drives & (1 << (letter - 'A'))) {
            char rootPath[4] = {letter, ':', '\\', '\0'};
            ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

            if (GetDiskFreeSpaceEx(rootPath, &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
                message += "Drive ";
                message += letter;
                message += ":\n";
                message += "Total space: " + std::to_string(totalBytes.QuadPart) + " bytes\n";
                message += "Free space: " + std::to_string(totalFreeBytes.QuadPart) + " bytes\n";
            }
        }
    }

    MessageBox(hwnd, message.c_str(), "Disk Information", MB_OK);
}

void DisplayNetworkInformation(HWND hwnd) {
    std::string command = "ipconfig";
    std::string result;

    // Execute the ipconfig command and capture the output
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        MessageBox(hwnd, "Error executing ipconfig command.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result += buffer;
    }

    _pclose(pipe);

    // Display the result in a message box
    MessageBox(hwnd, result.c_str(), "Network Information", MB_OK);
}

void DisplayRealTimeSystemInfo(HWND hwnd) {

      // Launch a hypothetical system monitoring tool to display real-time system information
   /// ShellExecute(NULL, "open", "system_monitoring_tool.exe", NULL, NULL, SW_SHOWDEFAULT);
 ShellExecute(NULL, "open", "taskmgr.exe", NULL, NULL, SW_SHOWDEFAULT);

    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);

    GlobalMemoryStatusEx(&memStatus);

    double memoryLoad = static_cast<double>(memStatus.dwMemoryLoad);
    DWORDLONG totalPhysMem = static_cast<DWORDLONG>(memStatus.ullTotalPhys);
    DWORDLONG physMemUsed = totalPhysMem - static_cast<DWORDLONG>(memStatus.ullAvailPhys);

    // Calculate CPU usage (Note: this is a basic calculation and may not be accurate)
    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULONGLONG idleTime64 = ((ULONGLONG)idleTime.dwHighDateTime << 32) + idleTime.dwLowDateTime;
        ULONGLONG kernelTime64 = ((ULONGLONG)kernelTime.dwHighDateTime << 32) + kernelTime.dwLowDateTime;
        ULONGLONG userTime64 = ((ULONGLONG)userTime.dwHighDateTime << 32) + userTime.dwLowDateTime;
        ULONGLONG sysTime = idleTime64 + kernelTime64 + userTime64;
        double cpuUsage = 100.0 - ((double)idleTime64 / sysTime * 100.0);

        // Display CPU and memory information
        std::string message = "Real-time System Information:\n";
        message += "CPU Usage: " + std::to_string(cpuUsage) + "%\n";
        message += "Memory Usage: " + std::to_string(memoryLoad) + "%\n";
        message += "Total Physical Memory: " + std::to_string(totalPhysMem) + " bytes\n";
        message += "Used Physical Memory: " + std::to_string(physMemUsed) + " bytes\n";

        MessageBox(hwnd, message.c_str(), "Real-time System Information", MB_OK);
    } else {
        MessageBox(hwnd, "Error retrieving real-time system information.", "Error", MB_OK | MB_ICONERROR);
    }
}

void DisplayCPUInformation(HWND hwnd) {

      // Launch the system's "About" or "System Information" app
    ShellExecute(NULL, "open", "msinfo32.exe", "/categories +SystemSummary", NULL, SW_SHOWDEFAULT);

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    std::string cpuInfo = "CPU Information:\n";
    cpuInfo += "Architecture: ";
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            cpuInfo += "x64 (AMD or Intel)\n";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            cpuInfo += "ARM\n";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            cpuInfo += "x86 (Intel)\n";
            break;
        default:
            cpuInfo += "Unknown\n";
            break;
    }

    DWORD returnLength = 0;
    GetLogicalProcessorInformation(NULL, &returnLength);
    if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
        SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(returnLength);
        if (buffer && GetLogicalProcessorInformation(buffer, &returnLength)) {
            int processorCount = 0;
            for (DWORD i = 0; i < returnLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++) {
                if (buffer[i].Relationship == RelationProcessorPackage)
                    processorCount++;
            }
            cpuInfo += "Number of Processors: " + std::to_string(processorCount) + "\n";
        }
        free(buffer);
    }

    cpuInfo += "Processor Type: ";
    switch (sysInfo.dwProcessorType) {
        case PROCESSOR_INTEL_386:
            cpuInfo += "Intel 386\n";
            break;
        case PROCESSOR_INTEL_486:
            cpuInfo += "Intel 486\n";
            break;
        case PROCESSOR_INTEL_PENTIUM:
            cpuInfo += "Intel Pentium\n";
            break;
        case PROCESSOR_INTEL_IA64:
            cpuInfo += "Intel Itanium\n";
            break;
        case PROCESSOR_AMD_X8664:
            cpuInfo += "AMD x64\n";
            break;
        case PROCESSOR_ARM720: // Use PROCESSOR_ARM720 instead of PROCESSOR_ARM
            cpuInfo += "ARM\n";
            break;
        default:
            cpuInfo += "Unknown\n";
            break;
    }

    cpuInfo += "Number of Cores: " + std::to_string(sysInfo.dwNumberOfProcessors) + "\n";
    cpuInfo += "Processor Level: " + std::to_string(sysInfo.wProcessorLevel) + "\n";
    cpuInfo += "Processor Revision: " + std::to_string(sysInfo.wProcessorRevision) + "\n";

    MessageBox(hwnd, cpuInfo.c_str(), "CPU Information", MB_OK);
}



void DrawMemoryGraph(HDC hdc, RECT rect, MEMORYSTATUSEX statex) {
    // Updated DrawMemoryGraph implementation
    RECT graphRect = {50, 450, 550, 350};
    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    Rectangle(hdc, graphRect.left, graphRect.top, graphRect.right, graphRect.bottom);
    DeleteObject(hPen);

    double totalPhys = static_cast<double>(statex.ullTotalPhys);
    double usedPhys = totalPhys - static_cast<double>(statex.ullAvailPhys);
    double usedPercentage = usedPhys / totalPhys;
    double freePercentage = 1.0 - usedPercentage;
///MEMORY USAGE
    RECT usedRect = graphRect;
    usedRect.right = graphRect.left + static_cast<LONG>((graphRect.right - graphRect.left) * usedPercentage);
    HBRUSH hBrush = CreateSolidBrush(RGB(115, 200, 255));
    FillRect(hdc, &usedRect, hBrush);
    DeleteObject(hBrush);
///FREE MEMORY
    RECT freeRect = graphRect;
    freeRect.left = usedRect.right;
    hBrush = CreateSolidBrush(RGB(100, 0, 220));
    FillRect(hdc, &freeRect, hBrush);
    DeleteObject(hBrush);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    TextOut(hdc, graphRect.left, graphRect.top - 20, "Memory Usage", 12);

    char usedLabel[20], freeLabel[20];
    sprintf_s(usedLabel, "Used Memory: %.1f%%", usedPercentage * 100);
    sprintf_s(freeLabel, "Free Memory: %.1f%%", freePercentage * 100);
    TextOut(hdc, graphRect.left, graphRect.bottom + 5, usedLabel, strlen(usedLabel));
    TextOut(hdc, graphRect.right - 150, graphRect.bottom + 5, freeLabel, strlen(freeLabel));
}





void DrawDiskGraph(HDC hdc, RECT rect) {
    // Draw a detailed disk usage pie chart with a legend
    HBRUSH hBrush;
    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    SelectObject(hdc, hPen);

    int centerX = 345;   //right left   ..agr km value hogi to left ...agr zyada to right
    int centerY = 175;   //upper neeche   //375 ho to neehe issi trah km value ho to upper
    int radius = 90;     //piechart size

    DWORD drives = GetLogicalDrives();
    int totalSectors = 0;
    std::vector<std::pair<char, int>> driveUsage;

    for (char letter = 'A'; letter <= 'Z'; ++letter) {
        if (drives & (1 << (letter - 'A'))) {
            char rootPath[4] = {letter, ':', '\\', '\0'};
            ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;

            if (GetDiskFreeSpaceEx(rootPath, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
                int total = static_cast<int>(totalNumberOfBytes.QuadPart / (1024 * 1024));
                int used = static_cast<int>((totalNumberOfBytes.QuadPart - totalNumberOfFreeBytes.QuadPart) / (1024 * 1024));
                totalSectors += total;
                driveUsage.push_back({letter, used});
            }
        }
    }

    if (totalSectors > 0) {
        double startAngle = 0.0;
        int colorIndex = 0;
        COLORREF colors[] = {RGB(255, 0, 0), RGB(0, 255, 0), RGB(0, 0, 255), RGB(255, 255, 0), RGB(255, 165, 0), RGB(128, 0, 128)};

        for (const auto& drive : driveUsage) {
            char letter = drive.first;
            int usedSectors = drive.second;

            double usedAngle = 360.0 * usedSectors / totalSectors;
            double endAngle = startAngle + usedAngle;
            double startRadians = startAngle * 3.14159 / 180.0;
            double endRadians = endAngle * 3.14159 / 180.0;

            int startX = centerX + static_cast<int>(radius * cos(startRadians));
            int startY = centerY - static_cast<int>(radius * sin(startRadians));
            int endX = centerX + static_cast<int>(radius * cos(endRadians));
            int endY = centerY - static_cast<int>(radius * sin(endRadians));

            hBrush = CreateSolidBrush(colors[colorIndex % 6]);
            SelectObject(hdc, hBrush);
            Pie(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius, startX, startY, endX, endY);
            DeleteObject(hBrush);

            // Draw percentage text
            double midAngle = (startAngle + endAngle) / 2.0;
            double midRadians = midAngle * 3.14159 / 180.0;
            int textX = centerX + static_cast<int>((radius / 1.5) * cos(midRadians));
            int textY = centerY - static_cast<int>((radius / 1.5) * sin(midRadians));
            char percentageText[10];
            sprintf_s(percentageText, "%.1f%%", (100.0 * usedSectors / totalSectors));
            TextOut(hdc, textX, textY, percentageText, strlen(percentageText));

            // Draw legend
            RECT legendRect = {centerX + radius + 20, centerY - radius + 20 + (colorIndex * 20), centerX + radius + 40, centerY - radius + 40 + (colorIndex * 20)};
            hBrush = CreateSolidBrush(colors[colorIndex % 6]);
            FillRect(hdc, &legendRect, hBrush);
            DeleteObject(hBrush);
            FrameRect(hdc, &legendRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

            char legendText[20];
            sprintf_s(legendText, "Drive %c: %.1f%%", letter, (100.0 * usedSectors / totalSectors));
            TextOut(hdc, centerX + radius + 50, centerY - radius + 25 + (colorIndex * 20), legendText, strlen(legendText));

            startAngle = endAngle;
            colorIndex++;
        }

        // Draw the title
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, centerX - 40, centerY - radius - 20, "Disk Usage", 10);
    }

    DeleteObject(hPen);
}
