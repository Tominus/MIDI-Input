#pragma once
#include <Windows.h>
#include <mmeapi.h>
#pragma comment(lib, "winmm.lib")

class MIDI_Simulator
{
public:
	MIDI_Simulator();
	~MIDI_Simulator();

	void Start_Simulator();
	void Update_Simulator();

private:
	void Find_Window();
	void Find_MIDI_Input();

	void Start_MIDI_Input();

	HWND Get_Window(const char* _windowName);

	void static WINAPI Get_MIDI_Data(HMIDIIN _hmi, UINT _message, DWORD_PTR _dwInstance, DWORD_PTR _dwParam1, DWORD_PTR _dwParam2);

	static void SimulateInput(bool _isPressed, int _input, int _force);


	void TEST_KeyboardSimulation();
	void TEST_MouseSimulation();



private:
	HWND windowHandle;

	UINT deviceID;
	DWORD midiInstance;
	HMIDIIN midiIn;

public:
	static bool _bMouseLeft;
	static bool _bMouseDown;
	static bool _bMouseUp;
	static bool _bMouseRight;

	static bool _bIsInventoryOpen;
	static int _iMouseSpeed;
};