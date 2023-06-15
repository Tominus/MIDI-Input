#include "MIDI_Simulator.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>

#define Callback
#define Press_Offset 56
#define Key_Offset 36

bool MIDI_Simulator::_bMouseDown = false;
bool MIDI_Simulator::_bMouseLeft = false;
bool MIDI_Simulator::_bMouseRight = false;
bool MIDI_Simulator::_bMouseUp = false;
bool MIDI_Simulator::_bIsInventoryOpen = false;
int MIDI_Simulator::_iMouseSpeed = 40;

MIDI_Simulator::MIDI_Simulator()
{
    windowHandle = nullptr;
    deviceID = 0;
    midiInstance = 0;
    midiIn = 0;
}

MIDI_Simulator::~MIDI_Simulator()
{
    midiInStop(midiIn);
    midiInClose(midiIn);
}

void MIDI_Simulator::Start_Simulator()
{
    //Find_Window();
    Find_MIDI_Input();
    Start_MIDI_Input();
}

void MIDI_Simulator::Update_Simulator()
{
    POINT vMousePosition = { 0, 0 };
    GetCursorPos(&vMousePosition);
    const SHORT mask = 32768;

    POINT vMiddleScreen = { 1920 / 2, 1080 / 2 };


    bool bContinue = true;
    while (bContinue)
    {
        if (_bIsInventoryOpen)
            GetCursorPos(&vMousePosition);

        if (GetKeyState(VK_F6) & mask)
            bContinue = false;

        POINT _offset = {};

        if (MIDI_Simulator::_bMouseLeft)
            _offset.x -= _iMouseSpeed;

        if (MIDI_Simulator::_bMouseDown)
            _offset.y += _iMouseSpeed;

        if (MIDI_Simulator::_bMouseUp)
            _offset.y -= _iMouseSpeed;

        if (MIDI_Simulator::_bMouseRight)
            _offset.x += _iMouseSpeed;

        if (_offset.x || _offset.y)
        {
            if (_bIsInventoryOpen)
                SetCursorPos(vMousePosition.x + _offset.x, vMousePosition.y + _offset.y);
            else
                SetCursorPos(vMiddleScreen.x + _offset.x, vMiddleScreen.y + _offset.y);
        }
        
        Sleep(1);
    }
}

void MIDI_Simulator::Find_Window()
{
    while (windowHandle == nullptr)
    {
        std::cout << "Enter window name" << std::endl;

        std::string _windowName;
        std::getline(std::cin, _windowName);
        windowHandle = Get_Window(_windowName.c_str());
    }
}

void MIDI_Simulator::Find_MIDI_Input()
{
    UINT _numberOfInput = 0;
    MMRESULT _result = 1;

    while (_result != 0)
    {
        _numberOfInput = midiInGetNumDevs();

        if (_numberOfInput == 0)
        {
            std::cout << "No MIDI input detected. Please make sure connection is valid" << std::endl;
            Sleep(1000);
        }
        else
        {
            std::cout << "MIDI input detected" << std::endl;
            _result = midiInOpen(&midiIn, deviceID, (DWORD_PTR)(void*)Get_MIDI_Data, midiInstance, CALLBACK_FUNCTION);

            if (_result != 0)
            {
                std::cout << "An connection error occur : error " << _result << std::endl;
            }
            else
            {
                std::cout << "Connection success !" << std::endl;
            }
        }
    }
}

void MIDI_Simulator::Start_MIDI_Input()
{
    MMRESULT _result = midiInStart(midiIn);

    if (_result != 0)
    {
        std::cout << "MIDI start failed with code error : " << _result << std::endl;
    }
    else
    {
        std::cout << "MIDI start success !" << std::endl;
    }

}

HWND MIDI_Simulator::Get_Window(const char* _windowName)
{
    HWND _window = nullptr;

    std::wstring _w;
    std::copy(_windowName, _windowName + strlen(_windowName), back_inserter(_w));
    const WCHAR* _wchar = _w.c_str();

    _window = FindWindow(NULL, _wchar);
    if (_window)
    {
        std::cout << "Window with name (" << _windowName << ") was found." << std::endl;
        SetForegroundWindow(_window);
    }
    else
    {
        std::cout << "Window with name (" << _windowName << ") is missing" << std::endl;
    }

    return _window;
}

Callback void MIDI_Simulator::Get_MIDI_Data(HMIDIIN _hmi, UINT _message, DWORD_PTR _dwInstance, DWORD_PTR _dwParam1, DWORD_PTR _dwParam2)
{
    if (_message == MIM_DATA)
    {
        std::stringstream _stream;
        _stream << std::hex << _dwParam1;
        std::string _string(_stream.str());

        if (_string.length() < 6)
        {
            _string.insert(_string.begin(), '0');
        }

        const char* _result = _string.c_str();

        const char _velocityResult[2] = { _result[0], _result[1] };
        const char _keyResult[2] = { _result[2], _result[3] };

        int _velocity = strtol(_velocityResult, NULL, 16);
        int _key = strtol(_keyResult, NULL, 16) - Key_Offset;
        const bool _press = (_result[4] - Press_Offset);

        //HWND _window = FindWindow(NULL, TEXT("Minecraft 1.20.1 - Singleplayer"));

        //if (_press)
            //PostMessage(_window, WM_KEYDOWN, VK_SPACE, 0);
            //keybd_event(VK_SPACE, 0, 0, 0);
        //else
            //keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
        //    PostMessage(_window, WM_KEYUP, VK_SPACE, 0);

        SimulateInput(_press, _key, _velocity);
    }
}

void MIDI_Simulator::SimulateInput(bool _isPressed, int _input, int _force)
{
    switch (_input)
    {
    case 0:
    {
        keybd_event(VK_ESCAPE, 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        MIDI_Simulator::_bIsInventoryOpen = false;
        MIDI_Simulator::_iMouseSpeed = 40;
        break;
    }
    case 14:
    {
        keybd_event(VK_LCONTROL, 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 16:
    {
        keybd_event('Q', 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 17:
    {
        keybd_event('S', 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 18:
    {
        keybd_event('Z', 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 19:
    {
        keybd_event('D', 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 20:
    {
        if (_isPressed)
        {
            MIDI_Simulator::_bIsInventoryOpen = !MIDI_Simulator::_bIsInventoryOpen;
            MIDI_Simulator::_iMouseSpeed = MIDI_Simulator::_bIsInventoryOpen ? 20 : 40;
        }
        keybd_event('E', 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 21:
    {
        keybd_event(VK_SPACE, 0, _isPressed ? 0 : KEYEVENTF_KEYUP, 0);
        break;
    }
    case 26:
    {
        INPUT _input = {};
        _input.type = INPUT_MOUSE;
        _input.mi.dwFlags = _isPressed ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
        SendInput(1, &_input, sizeof(INPUT));
        break;
    }
    case 27:
    {
        if (_isPressed)
        {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, 1000, 0);
        }
        break;
    }
    case 28:
    {
        MIDI_Simulator::_bMouseLeft = _isPressed;
        break;
    }
    case 29:
    {
        MIDI_Simulator::_bMouseDown = _isPressed;
        break;
    }
    case 30:
    {
        MIDI_Simulator::_bMouseUp = _isPressed;
        break;
    }
    case 31:
    {
        MIDI_Simulator::_bMouseRight = _isPressed;
        break;
    }
    case 32:
    {

        if (_isPressed)
        {
            mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -1000, 0);
        }
        break;
    }
    case 33:
    {
        INPUT _input = {};
        _input.type = INPUT_MOUSE;
        _input.mi.dwFlags = _isPressed ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
        SendInput(1, &_input, sizeof(INPUT));
        break;
    }

    default:
        break;
    }
}

void MIDI_Simulator::TEST_KeyboardSimulation()
{
    keybd_event(VK_LSHIFT, 0, 0, 0);
    Sleep(1);

    keybd_event('A', 0, 0, 0);
    Sleep(1);
    keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
    Sleep(1);

    keybd_event(VK_LSHIFT, 0, KEYEVENTF_KEYUP, 0);
    Sleep(1);
}

void MIDI_Simulator::TEST_MouseSimulation()
{
    POINT vMousePosition = { 0, 0 };
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
    }
}