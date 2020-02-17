/*Programmer: Sean Higgins 
*/
#ifndef CONTROLLER_H
#define CONTROLLER_H

/* Irrlicht includes */
#include <irrlicht.h>
#include "driverChoice.h"
using namespace irr;
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

/* Personal Includes*/

#include "include\enums.h"

class Controller
{
	private:
		//functions

	public:
		//definitions
		float mouseSensitivity;
		float keySensitivity;
		float joySensitivity;
		ControlSet ctrl[4];
		int maxController;

		//functions
		Controller();
		~Controller();

		void handleIdle();
		void ControllerSet(int player, ControlType ctrlType, irr::EKEY_CODE keys[], int button[]);
		void clearController(int i);
		bool press(int player, int ctrl);
		bool held(int player, int ctrl);
		bool released(int player, int ctrl);
		bool combo(int player, int num, int num2);
		void handleJoystick(unsigned int buttonMask, int x, int y, int z);
		void handleKeypress(irr::EKEY_CODE key, int x, int y);
		void handleKeyrelease(irr::EKEY_CODE key, int x, int y);
		void handleMouse(int x, int y);
		void handleClick(int button, int state, int x, int y);
};


//intialize all enemy data
Controller::Controller()
{
	mouseSensitivity = 200.0f;
	keySensitivity = 0.8f;
	joySensitivity = 5000.0f;

	maxController = 4;
	for(int i = 0;i < maxController;i++)
	{
		clearController(i);
	}
}

//destruct
Controller::~Controller()
{
}

//handle idle
void Controller::handleIdle() 
{
	for(int i = 0;i < maxController;i++)
	{
		for(int j = 0;j < 14;j++)
		{
			if(ctrl[i].ctrlDown[j] == PressType_Press)
			{
				ctrl[i].ctrlDown[j] = PressType_Held;
			}
			else if(ctrl[i].ctrlDown[j] == PressType_Released)
			{
				ctrl[i].ctrlDown[j] = PressType_NotHeld;
			}
		}
	}
}
//setup a new ctrlr
void Controller::ControllerSet(int player, ControlType ctrlType, irr::EKEY_CODE keys[], int button[])
{
	player--;
	ctrl[player].ctrlType = ctrlType;
	for(int i = 0;i < 14;i++)
	{
		ctrl[player].keys[i] = keys[i];
		ctrl[player].button[i] = button[i];
		ctrl[player].ctrlDown[i] = 0;
	}
}

//clear a complete ctrlr
void Controller::clearController(int i)
{
	ctrl[i].ctrlType = CtrlType_Null;
	for(int j = 0;j < 14;j++)
	{
		ctrl[i].keys[j] = 0;
		ctrl[i].button[j] = 0;
		ctrl[i].ctrlDown[j] = 0;
	}
}

//check whether a button has just been pressed
bool Controller::press(int player, int request)
{
	player--;
	return (ctrl[player].ctrlDown[request] == PressType_Press);
}

//check whether a button is pressed
bool Controller::held(int player, int request)
{
	player--;
	return (ctrl[player].ctrlDown[request] == PressType_Press || ctrl[player].ctrlDown[request] == PressType_Held);
}

//check whether a button has just been released
bool Controller::released(int player, int request)
{
	player--;
	return (ctrl[player].ctrlDown[request] == PressType_Released);
}

bool Controller::combo(int player, int num, int num2)
{
	player--;
	return (press(player, num) && held(player, num2)) || (held(player, num) && press(player, num2));
}
//Handles all key presses
void Controller::handleJoystick(unsigned int buttonMask, int x, int y, int z)
{
	for(int i = 0;i < maxController;i++)
	{
		if(ctrl[i].ctrlType == CtrlType_Gamepad)
		{
			ctrl[i].ctrlDown[Ctrl_Up] = false;
			ctrl[i].ctrlDown[Ctrl_Left] = false;
			ctrl[i].ctrlDown[Ctrl_Right] = false;
			ctrl[i].ctrlDown[Ctrl_Down] = false;

			if(y < -300) ctrl[i].ctrlDown[Ctrl_Up] = true;
			else if(y > 300) ctrl[i].ctrlDown[Ctrl_Down] = true;
			if(x > 300)	ctrl[i].ctrlDown[Ctrl_Right] = true;
			else if(x < -300) ctrl[i].ctrlDown[Ctrl_Left] = true;

			for(int j = 0;j < 16;j++)
			{
				for(int k = Ctrl_A;k < 14;k++)
				{
					if(ctrl[i].button[k] == j)
					{
						if(buttonMask & (1 << j))
						{
							if(ctrl[i].ctrlDown[k] == PressType_NotHeld)
							{
								ctrl[i].ctrlDown[k] = PressType_Press;
							}
							else
							{
								ctrl[i].ctrlDown[k] = PressType_Held;
							}
						}
						else
						{
							if(ctrl[i].ctrlDown[k] == PressType_Held || ctrl[i].ctrlDown[k] == PressType_Press)
							{
								ctrl[i].ctrlDown[k] = PressType_Released;
							}
							else
							{
								ctrl[i].ctrlDown[k] = PressType_NotHeld;
							}
						}
					}
				}
			}
		}
	}
}

//Handles all key presses
void Controller::handleKeypress(irr::EKEY_CODE key, int x, int y) 
{
	for(int i = 0;i < maxController;i++)
	{
		if(ctrl[i].ctrlType == CtrlType_Keyboard)
		{
			for(int k = 0;k < 14;k++)
			{
				if(ctrl[i].keys[k] == key)
				{
					if(ctrl[i].ctrlDown[k] == PressType_NotHeld)
					{
						ctrl[i].ctrlDown[k] = PressType_Press;
					}
					else
					{
						ctrl[i].ctrlDown[k] = PressType_Held;
					}
					k = 15;//boot out
					i = maxController + 1;
				}
			}
		}
	}
}

//Handles all key releases
void Controller::handleKeyrelease(irr::EKEY_CODE key, int x, int y)
{
	for(int i = 0;i < maxController;i++)
	{
		if(ctrl[i].ctrlType == CtrlType_Keyboard)
		{
			for(int k = 0;k < 14;k++)
			{
				if(ctrl[i].keys[k] == key)
				{
					if(ctrl[i].ctrlDown[k] == PressType_Held || ctrl[i].ctrlDown[k] == PressType_Press)
					{
						ctrl[i].ctrlDown[k] = PressType_Released;
					}
					else
					{
						ctrl[i].ctrlDown[k] = PressType_NotHeld;
					}
					k = 15;//boot out
					i = maxController + 1;
				}
			}
		}
	}
}

//handles all cursor movement
void Controller::handleMouse(int x, int y)
{
	//mouse x
	//mouse y
}

//handles all mouse clicks
void Controller::handleClick(int button, int state, int x, int y) 
{
	switch(state)
	{
		case 0://button pressed DOWN
			//button
		break;
		case 1://button pressed DOWN
			//button
		break;
	}
}

#endif