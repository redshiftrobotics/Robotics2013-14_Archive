#pragma config(Sensor, S1,     ,               sensorI2CCustom)
#pragma config(Sensor, S3,     Color,          sensorI2CCustom)
#pragma config(Sensor, S4,     ,               sensorTouch)
#pragma config(Sensor, S2,     ,               sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../libraries/Servos.h"
#include "../libraries/Arm.c"
#include "../libraries/Motors.h"
#include "../libraries/Menu.c"
#include "JoystickDriver.c"

//declared update arm task
task UpdateArm;

string RampOrientation;
//POSSIBLE VALUES: "Left", "Right"
string RampY;
//POSSIBLE VALUES: "Half", "On", "Over"
string RampX;
//POSSIBLE VALUES: "Left", "Middle", "Right"
bool Rotate90;

int Multiplier = 1;

bool WaitForStartBool = false;

void RampOrientationMenu()
{
	string t = "Ramp Orientation?";
	string a = "Left";
	string b = "Right";
	int MenuReturn = Menu(t, a, b);

	if(MenuReturn == 0)
	{
		RampOrientation = "Left";
	}
	else
	{
		RampOrientation = "Right";
	}
}

void RampXMenu()
{
	string t = "Ramp X?";
	string a = "Left";
	string b = "Middle";
	string c = "Right";
	int MenuReturn = Menu(t, a, b, c);

	if(MenuReturn == 0)
	{
		RampX = "Left";
	}
	else if(MenuReturn == 1)
	{
		RampX = "Middle";
	}
	else
	{
		RampX = "Right":
	}
}

void WaitForStartMenu()
{
	string t = "Wait for Start";
	string b = "Yes";
	string c = "No";
	int MenuReturn = Menu(t, b, c);

	if(MenuReturn == 0)
	{
		WaitForStartBool = true;
	}
	else if(MenuReturn == 1)
	{
		WaitForStartBool = false;
	}
}

void RampYMenu()
{
	string t = "Ramp Y?";
	string a = "Half";
	string b = "On";
	string c = "Over";
	int MenuReturn = Menu(t, a, b, c);

	if(MenuReturn == 0)
	{
		RampY = "Half";
	}
	else if(MenuReturn == 1)
	{
		RampY = "On";
	}
	else
	{
		RampY = "Over":
	}
}

void RotateMenu()
{
	string t = "Rotate 90?";
	string a = "Yes";
	string b = "No";
	int MenuReturn = Menu(t, a, b);

	if(MenuReturn == 0)
	{
		Rotate90 = true;
	}
	else
	{
		Rotate90 = false;
	}
}

void DisplayProgramSettings()
{
	eraseDisplay();

	nxtDisplayString(1, "Ramp Side: %s", RampOrientation);
	nxtDisplayString(2, "Ramp X: %s", RampX);
	nxtDisplayString(3, "Ramp Y: %s", RampY);
	nxtDisplayString(4, "Rotation: %i", Rotate90);
}

void Move(int RightPower, int LeftPower)
{
	Motors_SetSpeed(S1, 2, 1, RightPower);
	Motors_SetSpeed(S1, 2, 2, -LeftPower);
}

task main()
{
	//starts tasks
	StartTask(UpdateArm);

	//turn off joystick debug
	bDisplayDiagnostics = false;

	//lets the used pick the program
	WaitForStartMenu();
	RampOrientationMenu();
	RampXMenu();
	RampYMenu();
	RotateMenu();
	DisplayProgramSettings();

	//waits for start it bool is set
	if(WaitForStartBool)
	{
		waitForStart();
	}

	//sets arm to go up
	Arm_SetSpeed(-100);
	sleep(2800);

	//moves forward
	Move(40 * Multiplier, 40 * Multiplier);
	sleep(600 / Multiplier);
	Move(0, 0);

	//stop arm
	Arm_SetSpeed(0);

	//spit out block
	Motors_SetSpeed(S1, 3, 1, -100);
	sleep(1000);
	Motors_SetSpeed(S1, 3, 1, 0);

	//moves backward
	Move(-40, -40);
	sleep(200);
	Move(0, 0);

	//turns
	if(RampOrientation == "Left")
	{
		Move(-40, 40);
	}
	else if(RampOrientation == "Right")
	{
		Move(40, -40);
	}
	sleep(750);
	Move(0, 0);

	//moves arm down
	Arm_SetSpeed(80);

	//moves backward
	Move(-40 * Multiplier, -40 * Multiplier);
	if(RampOrientation == "Left")
	{
		if(RampX == "Right")
		{
			sleep(750 / Multiplier);
		}
		else if(RampX == "Middle")
		{
			sleep(1200 / Multiplier);
		}
		else if(RampX == "Left")
		{
			sleep(1750 / Multiplier);
		}
	}
	else if(RampOrientation == "Right")
	{
		if(RampX == "Right")
		{
			sleep(1750 / Multiplier);
		}
		else if(RampX == "Middle")
		{
			sleep(1200 / Multiplier);
		}
		else if(RampX == "Left")
		{
			sleep(750 / Multiplier);
		}
	}
	Move(0, 0);

	//turns onto ramp
	if(RampOrientation == "Left")
	{
		Move(-35, 0);
	}
	else if(RampOrientation == "Right")
	{
		Move(0, -40);
	}
	sleep(1350);
	Move(0, 0);

	//moves onto ramp
	Move(-40 * Multiplier, -40 * Multiplier);
	if(RampY == "Half")
	{
		sleep(900 / Multiplier);
	}
	else if(RampY == "On")
	{
		sleep(1350 / Multiplier);
	}
	else if(RampY == "Over")
	{
		sleep(2100 / Multiplier);
	}

	//stops robot and arm
	Move(0, 0);
	Arm_SetSpeed(0);
}

task UpdateArm()
{
	while(true)
	{
		Arm_Update();
	}
}
