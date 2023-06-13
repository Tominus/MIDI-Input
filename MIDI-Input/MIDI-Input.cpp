#include <iostream>
#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    HWND hFore = FindWindow(NULL, TEXT("GitHub Desktop"));
    if (hFore)
    {
        SetForegroundWindow(hFore);
        Sleep(1000);
        
        /*keybd_event(VK_LCONTROL, 0, 0, 0);
        keybd_event(VK_LCONTROL, 0, KEYEVENTF_KEYUP, 0);*/

        Sleep(500);

        keybd_event(VK_LSHIFT, 0, 0, 0);
        Sleep(1);

        keybd_event('A', 0, 0, 0);
        Sleep(1);
        keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
        Sleep(1);

        keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
        Sleep(1);

        /*
        POINT vMousePosition = {};
        GetCursorPos(&vMousePosition);
        const SHORT mask = 32768;

        bool bContinue = true;
        while (bContinue)
        {
            if (GetKeyState(VK_ESCAPE) & mask)
                bContinue = false;

            if (GetKeyState(VK_LEFT) & mask)
                --vMousePosition.x;

            if (GetKeyState(VK_RIGHT) & mask)
                ++vMousePosition.x;

            if (GetKeyState(VK_DOWN) & mask)
                ++vMousePosition.y;

            if (GetKeyState(VK_UP) & mask)
                --vMousePosition.y;

            SetCursorPos(vMousePosition.x, vMousePosition.y);
            Sleep(1);
        }*/
    }
    return 69;
}