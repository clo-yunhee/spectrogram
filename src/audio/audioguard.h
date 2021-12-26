#ifndef AUDIOGUARD_H
#define AUDIOGUARD_H

#include <portaudio.h>

class AudioGuard {
   public:
    AudioGuard() { Pa_Initialize(); }
    ~AudioGuard() { Pa_Terminate(); }
};

#endif  // AUDIOGUARD_H