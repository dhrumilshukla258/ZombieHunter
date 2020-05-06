#ifndef  Frame_Rate_Controller_H
#define  Frame_Rate_Controller_H
#include <SDL2.0/SDL_stdinc.h>

class FrameRateController {
public:
	FrameRateController(Uint32 MaxFrameRate);
	~FrameRateController();

	void FrameStart();
	void FrameEnd();
	Uint32 GetFrameTime();
private:
	Uint32 mTickStart;
	Uint32 mTickEnd;
	Uint32 mFrameTime;
	Uint32 mNeededTicksPerFrame;
};
#endif // ! FRC_H
