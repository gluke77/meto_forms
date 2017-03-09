#ifndef _AUTOSTART_INCLUDED
#define _AUTOSTART_INCLUDED

class Autostart
{
public:
	enum Action
	{
		Start = 0,
		MovementDetect,
		SgRotate,
		Uzg,
		TuMotor,
		TuPushdown,
		SgPosition,
		E2pMotor,
		E2kMotor,
		E3pMotor,
		E3kMotor,
		E3mMotor,
		Bath,
	};

	enum Check
	{
		AirInSystem = 0,
		SgWater,
		SgTemperature,
		UzgReady,
		E2Water,
		E3Water,
		E2pTemperature,
		E2kTemperature,
		E3pTemperature,
		E3kTemperature,
		E3mTemperature,
	};	

	static const int LineActionCount = Bath + 1;
	static const int SvarkaActionCount = SgPosition + 1;
	static const int LineCheckCount = E3mTemperature + 1;
	static const int SvarkaCheckCount = UzgReady + 1;
};


#endif // _AUTOSTART_INCLUDED

