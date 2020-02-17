#ifndef ENUMS_H
#define ENUMS_H

/* Irrlicht includes */
#include <irrlicht.h>
#include "driverChoice.h"
using namespace irr;
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//enums
enum Player
{
	Sean = 0,
	AD = 1,
	Mario = 2,
	Sam = 3
};

enum Stance
{
    stance_Idle = 0,
    stance_Run = 1,
	stance_Defend = 2,
	stance_Evade = 3,
	stance_Item = 4,
	stance_Damage = 5,
	stance_Dead = 6,
	stance_Cast = 7,
	stance_Other = 8
};

enum MenuType
{
	Menu_Offend = 0,
	Menu_Defend = 1,
	Menu_Move = 2,
	Menu_Item = 3
};

enum PageNum
{
	Page_1 = 0,
	Page_2 = 1,
	Page_3 = 2,
	Page_4 = 3
};

enum PressType
{
	PressType_NotHeld = 0,
	PressType_Press = 1,
	PressType_Held = 2,
	PressType_Released = 3
};

enum ControlType
{
	CtrlType_Keyboard = 0,
	CtrlType_Mouse = 1,
	CtrlType_Gamepad = 2,
	CtrlType_Null = 3
};

enum Control
{
	Ctrl_Up = 0,
	Ctrl_Right = 1,
	Ctrl_Down = 2,
	Ctrl_Left = 3,
	Ctrl_A = 4,
	Ctrl_B = 5,
	Ctrl_Y = 6,
	Ctrl_X = 7,
	Ctrl_L = 8,
	Ctrl_R = 9,
	Ctrl_ZL = 10,
	Ctrl_ZR = 11,
	Ctrl_Start = 12,
	Ctrl_Select = 13
};

//Structures
struct ControlSet
{
	ControlType ctrlType;
	unsigned char keys[14];
	int button[14];
	int ctrlDown[14];
};

struct GfxFrame
{
	core::rect<s32> pos[8][4];
	core::position2d<s32> offset[8][4];
	int cap[8];
};


//Functions
irr::core::stringw addString(irr::core::stringw str1, irr::core::stringw str2)
{
	return (irr::core::stringw)(str1 + str2);
}

irr::core::stringw addString(irr::core::stringw str1, int str2)
{
	return (irr::core::stringw)(str1 + ((irr::core::stringw) str2));
}

void convertStringToChar(char * filePath, std::string strn1, std::string strn2, std::string strn3)
{
	strn2 = strn2.append(strn3);
	strn1 = strn1.append(strn2);
	for(int i = 0;i < 256;i++)//dump it into a char array
	{
		filePath[i] = strn1[i];
	}
	std::cout << "Current Filename:" << filePath << std::endl;
}

//globals
irrklang::ISoundEngine* engine;
u32 time;
f32 frameDT;
core::position2d<f32> plyrPos[4];	//position on the map, feetwise
core::position2d<f32> badyPos[4];	//position on the map, feetwise
		
#endif