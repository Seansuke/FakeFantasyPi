#ifndef CHARA_H
#define CHARA_H

/* Irrlicht includes */
#include <irrlicht.h>
#include "driverChoice.h"
using namespace irr;
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

/* Personal Includes */
#include "include\enums.h"
#include "include\controls.h"

/* define the header for the class */
class Chara
{
	public:
		/* Basic Functions */
		Chara();	//basic constructor
		void battleCycle();				//do a common cycle of battle
		core::position2d<s32> gfxPos();	//position of the graphics if asked to be drawn on the map
		core::rect<s32> gfxFramePos();	//the rectangle positions of each individual sprite
		int getSpeed();					//get the current Speed value of the player
		void setSPD(int num);			//set the Speed/MaxSpeed of the player  (1 ~ 100)
		void cycle();					//do a common idle cycle, regeneration, called by battleCycle
		void checkCaps();				//ensure all caps on the stats are ok and fix them
		void moveCost();				//takes away SP for the cost of a single movement
		void commandMove();				//sub function of battleCycle
		void commandMenu();				//sub function of battleCycle
		bool commandAccept();			//sub function of battleCycle

		/* Graphics Vars */
		Stance gfx;					//current graphic
		GfxFrame frame;				//current frame for graphic
		core::position2d<f32> pos;	//position on the map, feetwise
		/* Stats Vars */
		int HP; int MHP;
		int MP;	int MMP;
		int SP; int MSP;
		int ATT; int MATT;
		int DEF; int MDEF;
		int SPD; int MSPD;
		int ASP; int MASP;
		int MAG; int MMAG;
 		double XP; double MXP;
		int dmgRequest;

		/* Command Vars */
		void scriptGo();				//let's play a single line of script
		void scriptBegin(const char * name);	//call a script, input it, begin the animation
		bool scriptMove(f32 X, f32 Y);		//get closer to an X, Y position.  Returns true if already there
		FILE * scriptFile;				//the open file to read with actions
		bool timingUsing;				//whether the attack timing of [Y] button is on or not
		bool timingSuccess;				//whether the attack timing of [Y] was a success
		char actString[256];			//the current string for readin a file
		bool fileUse;					//is a script file in use
		int actTimer;					//amount of time left for this act string
		bool move;						//are you moving?
		irr::core::stringw gfxRequest;	//request MAIN to open the attack/defence graphic file
		int gfxNum;						//in accordance with the request of the gfx, this is the index of the graphic
		bool acting;					//are you performing an action?
		core::position2d<f32> savedPos;	//original place before an action
		int direction;					//1 for right, -1 for left
		int target;						//1 ~ 4
		int menuPos;					//0 ~ 3, holds the current position of the cursor
		int subMenu;					//0 ~ 3, Offense, Defense, Move, Item
		int subMenuPage;				//0 ~ 3, for each page number
		int plyr;						//self player number, 1 ~ 4
		Controller ctrl;				//current controller state
		bool human;						//true for human p[layer, false for not

		//menu
		irr::core::stringw attMenu[16];	//for the time being, menu names for attacks
		irr::core::stringw defMenu[16];
		irr::core::stringw itemMenu[16];
		irr::core::stringw hotKeys[4];

		/* Control functions */
		bool Press(Control control);		//check to see if a control is being pressed
		bool Held(Control control);			//check to see if a control is being held
		bool Released(Control control);		//check to see if a control is being released
};
/* initialize the character */
Chara::Chara()
{
	plyr = 1;			//set the player to 1 by default
	dmgRequest = -1;
	target = 1;
	gfx = stance_Idle;	//start out as Idle
	fileUse = false;	//have no file be open
	actTimer = 0;		//set the timer to 0
	for(int i = 0;i < 256;i++) actString[i] = '-';	//begin with no action character
	pos.X = 200;pos.Y = 250;	//starting position
	move = false;		//start out not in Move Mode
	acting = false;		//begin not performing an act
	direction = 1;		//begin looking right
	menuPos = 0;		//start at the top of the menu position
	subMenu = -1;		//in no submenu
	subMenuPage = 0;	//no page of the submenu
	gfxRequest = "none";//no request yet
	human = false;

	for(int j = 0;j <= 7;j += 1)	//frames locations and offsets on the sprite sheet
	{
		frame.cap[j] = 4;	//All GFX Frame caps set to 4
		for(int i = 0;i <= 3;i += 1)
		{
			frame.pos[j][i].UpperLeftCorner.X = i*100;
			frame.pos[j][i].UpperLeftCorner.Y = j*100;
			frame.pos[j][i].LowerRightCorner.X = (i+1)*100;
			frame.pos[j][i].LowerRightCorner.Y = (j+1)*100;
			frame.offset[j][i].X = i*100 + 50;
			frame.offset[j][i].Y = j*100 + 50;
		}
	}

	//give all default stats
	HP = 100; MHP = 100; MP = 100; MMP = 100; SP = 100; MSP = 100;
	ATT = 10; MATT = 10; DEF = 10; MDEF = 10;
	SPD = 10; MSPD = 10; ASP = 10; MASP = 10;
	MAG = 10; MMAG = 10; XP = 0; MXP = 0;

	for(int i = 0;i < 16;i++)	/* Clear all names for menus */
	{
		attMenu[i] = (irr::core::stringw) "---";
		defMenu[i] = (irr::core::stringw) "---";
		itemMenu[i] = (irr::core::stringw) "---";
		if(i < 4)
		{
			hotKeys[i] = (irr::core::stringw) "---";
		}
	}

	attMenu[0] = (irr::core::stringw) "Slash";
	/*attMenu[1] = (irr::core::stringw) "Thrust";
	attMenu[2] = (irr::core::stringw) "Eclipse";
	attMenu[3] = (irr::core::stringw) "Jump Attack";
	attMenu[4] = (irr::core::stringw) "Cyclone";
	attMenu[5] = (irr::core::stringw) "Blade Beam";
	*/
	defMenu[0] = (irr::core::stringw) "Idle";
	/*defMenu[1] = (irr::core::stringw) "Protect";
	defMenu[2] = (irr::core::stringw) "Wall";
	defMenu[3] = (irr::core::stringw) "Counter";
	defMenu[4] = (irr::core::stringw) "Barrier";
	defMenu[5] = (irr::core::stringw) "Grand Divide";

	itemMenu[0] = (irr::core::stringw) "Prostate Glove";
	itemMenu[1] = (irr::core::stringw) "Apple";
	itemMenu[2] = (irr::core::stringw) "Water";
	itemMenu[3] = (irr::core::stringw) "M&Ms";
	itemMenu[4] = (irr::core::stringw) "Green Tea";
	itemMenu[5] = (irr::core::stringw) "Cookie";*/
}
/* return the final position of drawing the character */
core::position2d<s32> Chara::gfxPos()
{
	int index = (time/100) % frame.cap[gfx];	//which image index to use based on time
	s32 gfxPosX = (s32)pos.X + (frame.pos[gfx][index].UpperLeftCorner.X - frame.offset[gfx][index].X);	//pos + (upLeft - gfxOffset) = finalPos
	s32 gfxPosY = (s32)pos.Y + (frame.pos[gfx][index].UpperLeftCorner.Y - frame.offset[gfx][index].Y);	//pos + (upLeft - gfxOffset) = finalPos
	return core::position2d<s32>(gfxPosX, gfxPosY);	//return both positions
}
/* hand out which sprite index is draw */
core::rect<s32> Chara::gfxFramePos()
{
	int index = (time/100) % frame.cap[gfx];	//sprite index is time dependant
	return frame.pos[gfx][index];	//give it out
}
/* show the world the true SPD value */
int Chara::getSpeed()
{
	return 50 + SPD;
}
/* Set the MaxSpeed/Speed of the player */
void Chara::setSPD(int num)
{
	SPD = num;
	MSPD = num;
}
/* that damn regeneration cycle */
void Chara::cycle()
{
	if(time % 10 == 0)
	{
		actTimer--; //decrease act Timer
		if(fileUse == true)	scriptGo();
	}
	if(HP <= 0)	//if dead, return
	{
		gfx = stance_Dead;
		HP = 0;
		return;
	} 
	if(time % 500 == 0 && SP < MSP) SP++;	/* Special SP regen */
	checkCaps();	/* Cap everything*/
	if(time % 1500 != 0) return;	/*check the time available to perform a single cycle*/
	/* It's time for a single cycle to be performed */
	if(HP > MHP) HP--;
	if(MP > MMP) MP--;
	if(SP > MSP) SP--;
	if(ATT > MATT) ATT--;
	if(DEF > MDEF) DEF--;
	if(SPD > MSPD) SPD--;
	if(ASP > MASP) ASP--;
	if(MAG > MMAG) MAG--;
	if(HP < MHP) HP++;
	if(MP < MMP) MP++;
	if(SP < MSP) SP++;
	if(ATT < MATT) ATT++;
	if(DEF < MDEF) DEF++;
	if(SPD < MSPD) SPD++;
	if(ASP < MASP) ASP++;
	if(MAG < MMAG) MAG++;
	checkCaps();	/* Cap everything... again*/
}
/* Check all caps and fix them */
void Chara::checkCaps()
{
	/* Cap everything*/
	if(HP > MHP * 1.5) HP = (int)(MHP * 1.5);
	if(MP > MMP * 1.5) MP = (int)(MMP * 1.5);
	if(SP > MSP * 1.5) SP = (int)(MSP * 1.5);
	if(ATT > MATT * 1.5) ATT = (int)(MATT * 1.5);
	if(DEF > MDEF * 1.5) DEF = (int)(MDEF * 1.5);
	if(SPD > MSPD * 1.5) SPD = (int)(MSPD * 1.5);
	if(ASP > MASP * 1.5) ASP = (int)(MASP * 1.5);
	if(MAG > MMAG * 1.5) MAG = (int)(MMAG * 1.5);
	if(HP < 0) HP = 0;
	if(MP < 0) MP = 0;
	if(SP < 0) SP = 0;
	if(MHP < 0) MHP = 0;
	if(MMP < 0) MMP = 0;
	if(MSP < 0) MSP = 0;
	if(ATT < MATT * 0.5) ATT = (int)(MATT * 0.5);
	if(DEF < MDEF * 0.5) DEF = (int)(MDEF * 0.5);
	if(SPD < MSPD * 0.5) SPD = (int)(MSPD * 0.5);
	if(ASP < MASP * 0.5) ASP = (int)(MASP * 0.5);
	if(MAG < MMAG * 0.5) MAG = (int)(MMAG * 0.5);
}
/* Take away SP for a movement */
void Chara::moveCost()
{
	if(time % 200 == 0) SP--;
}
/* All control functions */
bool Chara::Press(Control control)
{
	return ctrl.press(plyr, control);
}
bool Chara::Held(Control control)
{
	return ctrl.held(plyr, control);
}
bool Chara::Released(Control control)
{
	return ctrl.released(plyr, control);
}
/* Standard battle cycle */
void Chara::battleCycle()
{
	plyrPos[plyr - 1].X = pos.X;
	plyrPos[plyr - 1].Y = pos.Y;
	cycle();	//run one cycle... maybe
	if(move == true) commandMove();
	else commandMenu();

	//attack timing
	if(timingUsing == true && timingSuccess == false)
	{
		if(Press(Ctrl_Y) == true)
		{
			timingSuccess = true;
			engine->play2D("FFP/sfx/Accept.wav", false);
		}
	}
}
/* part of battleCycle */
void Chara::commandMenu()
{
	/*==========================Menu Mode=============================*/
	if(Press(Ctrl_Up)) {menuPos--;engine->play2D("FFP/sfx/Cursor.wav", false);}
	else if(Press(Ctrl_Down)) {menuPos++;engine->play2D("FFP/sfx/Cursor.wav", false);}
	if(menuPos < Menu_Offend) menuPos = Menu_Item;
	if(menuPos > Menu_Item) menuPos = Menu_Offend;

	//cycle through sub memu's pages
	if(subMenu != -1)
	{
		if(Press(Ctrl_Right)) {subMenuPage++;engine->play2D("FFP/sfx/Cursor.wav", false);}
		else if(Press(Ctrl_Left)) {subMenuPage--;engine->play2D("FFP/sfx/Cursor.wav", false);}
		if(subMenuPage < Page_1) subMenuPage = Page_4;
		if(subMenuPage > Page_4) subMenuPage = Page_1;
	}

	//choose option
	if(Press(Ctrl_A))
	{
		if(!commandAccept()) return;
	}
	if(Press(Ctrl_B))
	{
		//Return back to previous menu
		engine->play2D("FFP/sfx/Cancel.wav", false);
		if(move == false) subMenu = -1;
		return;
	}
}
/* part of battleCycle */
void Chara::commandMove()
{
	/*==========================Run Mode=============================*/
	gfx = stance_Idle;

	//vertical movement
	if(Held(Ctrl_Up) && (pos.Y > 64) && (SP > 0))
	{
		pos.Y -= getSpeed() * frameDT;
		gfx = stance_Run;
		moveCost();
	}
	else if(Held(Ctrl_Down) && (pos.Y < 300) && (SP > 0))
	{
		pos.Y += getSpeed() * frameDT;
		gfx = stance_Run;
		moveCost();
	}

	//horizontal
	if(Held(Ctrl_Left) && (pos.X > 32) && (SP > 0))
	{
		pos.X -= getSpeed() * frameDT;
		gfx = stance_Run;
		moveCost();
	}
	else if(Held(Ctrl_Right) && (pos.X < 608) && (SP > 0))
	{
		pos.X += getSpeed() * frameDT;
		gfx = stance_Run;
		moveCost();
	}

	//end run mode
	if(Press(Ctrl_B))
	{
		gfx = stance_Idle;
		move = false;
	}
}
/* part of battleCycle */
bool Chara::commandAccept()
{
	if(HP <= 0) {return false;} //if dead, return
	/* Not within a submenu?*/
	if(subMenu == -1)
	{
		switch(menuPos)
		{
			case Menu_Move:
				if(acting == false)
				{
					//enter Move Mode
					move = true;
					engine->play2D("FFP/sfx/Accept.wav", false);
					return 0;
				}
				else
				{
					//currently in action, cannot use another skill
					engine->play2D("FFP/sfx/Cancel.wav", false);
				}
			break;
			default:
				//enter a submenu
				subMenu = menuPos;
				engine->play2D("FFP/sfx/Accept.wav", false);
				return 0;
			break;
		}
	}
	else
	{
		if(acting == false)
		{
			//perform an action
			switch(subMenu)
			{
				case Menu_Offend:
					if(SP > 20)
					{
						SP -= 20;
						scriptBegin("off_0");
					}
					return 0;
				break;
				case Menu_Defend:
					scriptBegin("def_0");
					return 0;
				break;
				case Menu_Item:
					//scriptBegin("item_0");
					return 0;
				break;
			}
		}
		else
		{
			//currently in action, cannot use another skill
			engine->play2D("FFP/sfx/Cancel.wav", false);
		}
	}
	return true;
}
/* part of battleCycle */
void Chara::scriptGo()
{
	if(fileUse == false) return; //error checking
	if(std::feof(scriptFile) && actTimer <= 0)	//close up shop
	{
		std::fclose(scriptFile); //close the file properly first
		fileUse = false;	//keep in mind there is no file
		acting = false;		//we're not performing the action anymore
		gfx = stance_Idle;	//reset graphic
		for(int i = 0;i < 256;i++) actString[i] = '-';	//no action characters
		return;	//get outta dodge
	}
	std::string currentStr = actString;//tmp string to help out
	if(actTimer <= 0)	//check if ready for next action
	{
		for(int i = 0;i < 256;i++) actString[i] = ' ';	//empty the string
		std::fgets(actString,256,scriptFile);//get some string
		currentStr = actString;//toss it in the string
		std::cout << actString << std::endl;
		//One time only runs
		if(currentStr.compare(0,6,"gfx = ") == 0) //set gfx to anything
		{
			if(currentStr.compare(6,4,"Idle") == 0) {gfx = stance_Idle;}
			else if(currentStr.compare(6,3,"Run") == 0) {gfx = stance_Run;}
			else if(currentStr.compare(6,6,"Defend") == 0) {gfx = stance_Defend;}
			else if(currentStr.compare(6,5,"Evade") == 0) {gfx = stance_Evade;}
			else if(currentStr.compare(6,4,"Item") == 0) {gfx = stance_Item;}
			else if(currentStr.compare(6,6,"Damage") == 0) {gfx = stance_Damage;}
			else if(currentStr.compare(6,4,"Dead") == 0) {gfx = stance_Dead;}
			else if(currentStr.compare(6,4,"Cast") == 0) {gfx = stance_Cast;}
			else	//special rules if it is a custom graphics chart
			{
				gfx = stance_Other;
				char tmp[2] = "";
				for(int i = 0;i < 2;i++) tmp[i] = actString[i + 6]; //read the gfx number from the script
				gfx = stance_Other;
				gfxNum = std::atoi(tmp);
			}
			actTimer = 1; //rock and roll
		}
		else if(currentStr.compare(0,7,"gfxtime") == 0) //custom act time for changing graphics
		{
			actTimer = 2; //small gfx time
		}
		else if(currentStr.compare(0,7,"time = ") == 0) //certain time interval
		{
			char tmp[4] = "";
			for(int i = 0;i < 4;i++) tmp[i] = actString[i + 7]; //find the time
			actTimer = std::atoi(tmp); //parse the time
		}
		else if(currentStr.compare(0,7,"SP Gain") == 0) //gain SP
		{
			SP++;
			actTimer = 1;
		}
		else if(currentStr.compare(0,7,"Animate") == 0) //request an animation to show
		{
			actTimer = 1;
		}
		else if(currentStr.compare(0,10,"End Timing") == 0) //end the timer for an offense attack
		{
			actTimer = 1;
			timingUsing = false;
			if(timingSuccess == true)
			{
				dmgRequest = (int)(ATT * (1.3 + (rand() % 4)/10));
			}
			dmgRequest = (int)(ATT * (0.8 + (rand() % 4)/10));
		}
		else if(currentStr.compare(0,12,"Start Timing") == 0) //begin the timer for an offense attack
		{
			actTimer = 1;
			timingSuccess == false;
			timingUsing = true;
		}
	}
	currentStr = actString;//dump it in a string
	//repetitious actions
	if(currentStr.compare(0,20,"Move towards enemy. ") == 0) //move towards the enemy placement
	{
		actTimer = 9999;//keep the timer running to ensure no loss
		gfx = stance_Run;
		//find x want
		char tmp[4] = "";
		for(int i = 0;i < 4;i++) tmp[i] = actString[i + 21];
		f32 wantX = (f32) std::atoi(tmp);
		wantX += badyPos[target - 1].X;
		//find y want
		for(int i = 0;i < 4;i++) tmp[i] = actString[i + 26];
		f32 wantY = (f32) std::atoi(tmp);
		wantY += badyPos[target - 1].Y;
		if(scriptMove(wantX,wantY) == true)	//check if in place
		{
			actTimer = 0;	//we're in the wanted place, leave
			gfx = stance_Idle;
		}
	}
	else if(currentStr.compare(0,9,"Move back") == 0)  //move back to your original position
	{
		actTimer = 9999;	//keep the timer running to ensure no loss
		gfx = stance_Run;
		if(scriptMove(savedPos.X,savedPos.Y) == true)
		{
			actTimer = 0;	//we're in the wanted place, leave
			gfx = stance_Idle;
		}

	}
}
/* Move towards a certain position on the field */
bool Chara::scriptMove(f32 wantX, f32 wantY)
{
	if(pos.X == wantX && pos.Y == wantY)	//we're there!
	{
		return true;
	}
	else
	{
		if(abs(pos.X - wantX) > 1000 * frameDT)	//get closer x
		{
			if(pos.X < wantX) pos.X += 1000 * frameDT;
			else pos.X -= 1000 * frameDT;
		}
		else pos.X = wantX;
		if(abs(pos.Y - wantY) > 1000 * frameDT)	//get closer y
		{
			if(pos.Y < wantY) pos.Y += 1000 * frameDT;
			else pos.Y -= 1000 * frameDT;
		}
		else pos.Y = wantY;
	}
	return false;
}
/* part of battleCycle */
void Chara::scriptBegin(const char * name)
{
	//create the full string that holds the filepath
	gfxRequest = name;
	std::string strn1 = "FFP/skill/sean/";
	std::string strn2 = name;
	strn2 = strn2.append(".txt");
	strn1 = strn1.append(strn2);
	//dump it into a char array
	char filePath[50];
	for(int i = 0;i < 50;i++)
	{
		filePath[i] = strn1[i];
	}
	//open the text file
	std::cout << filePath << std::endl;
	if((scriptFile = std::fopen(filePath,"r")) == NULL)
	{
		std::cout << "failed\n";
	}
	else
	{
		savedPos.X = pos.X;
		savedPos.Y = pos.Y;
		acting = true;
		fileUse = true;
		actTimer = 0;
		timingUsing = false;
		timingSuccess = false;
	}
}
#endif