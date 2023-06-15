#include <crtdbg.h>

#include "MIDI_Simulator.h"


int main(int argc, char* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(162);

    MIDI_Simulator* _midiSimulator = new MIDI_Simulator();

    _midiSimulator->Start_Simulator();

    _midiSimulator->Update_Simulator();

    delete _midiSimulator;

    return 69;
}