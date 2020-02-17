#ifndef BADDY_H
#define BADDY_H

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
class Baddy
{
	public:
		/* Basic Functions */
		Baddy();	//basic constructor
		void battleCycle();				//do a common cycle of battle
		int getSpeed();					//get the current Speed value of the player
		core::position2d<s32> gfxPos();
		core::rect<s32> gfxFramePos();
		void setSPD(int num);			//set the Speed/MaxSpeed of the player  (1 ~ 100)
		void cycle();					//do a common idle cycle, regeneration, called by battleCycle
		void checkCaps();				//ensure all caps on the stats are ok and fix them
		void moveCost();				//takes away SP for the cost of a single movement
		void commandMove();				//sub function of battleCycle
		void commandMenu();				//sub function of battleCycle
		bool commandAccept();			//sub function of battleCycle

		/* Graphics Vars */
		Stance gfx;					//current graphic
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
 		double XP;
		int dmgRequest;

		/* Command Vars */
		void scriptGo();				//let's play a single line of script
		void scriptBegin(const char * name);	//call a script, input it, begin the animation
		bool scriptMove(f32 X, f32 Y);		//get closer to an X, Y position.  Returns true if already there
		FILE * scriptFile;				//the open file to read with actions
		char actString[256];			//the current string for readin a file
		bool fileUse;					//is a script file in use
		int actTimer;					//amount of time left for this act string
		int gfxNum;						//in accordance with the request of the gfx, this is the index of the graphic
		int target;
		bool acting;					//are you performing an action?
		core::position2d<f32> savedPos;	//original place before an action
		int direction;					//1 for right, -1 for left
		int baddyNum;					//self baddy number, 1 ~ 4
		irr::core::stringw attMenu[16];	//for the time being, menu names for attacks
};
/* initialize the character */
Baddy::Baddy()
{
	baddyNum = 1;		//set the player to 1 by default
	target = 1;
	gfx = stance_Idle;	//start out as Idle
	fileUse = false;	//have no file be open
	actTimer = 0;		//set the timer to 0
	for(int i = 0;i < 256;i++) actString[i] = '-';	//begin with no action character
	pos.X = 400;pos.Y = 250;	//starting position
	acting = false;		//begin not performing an act
	direction = -1;		//begin looking left
	dmgRequest = -1;

	//give all default stats
	HP = 20; MP = 40; SP = 20; 
	ATT = 20; DEF = 5; 
	SPD = 4; ASP = 4; 
	MAG = 4; XP = 1;

	MHP = HP; MMP = MP; MSP = SP; MATT = ATT; MDEF = DEF; MSPD = SPD; MASP = ASP; MMAG = MAG; 

	for(int i = 0;i < 16;i++)	/* Default all attacks to Hit*/
		{attMenu[i] = (irr::core::stringw) "Hit";}

}
/* return the final position of drawing the character */
core::position2d<s32> Baddy::gfxPos()
{
	return core::position2d<s32>((s32)pos.X - 100, (s32)pos.Y - 100);	//return both positions
}
/* hand out which sprite index is draw */
core::rect<s32> Baddy::gfxFramePos()
{
	int index = (time/100) % 5;	//which image index to use based on time
	core::rect<s32> result = core::rect<s32>(
		index * 200,
		gfx * 200,
		(index + 1) * 200,
		(gfx + 1) * 200);
	return result;	//give it out
}
/* show the world the true SPD value */
int Baddy::getSpeed()
{
	return 50 + SPD;
}
/* Set the MaxSpeed/Speed of the player */
void Baddy::setSPD(int num)
{
	SPD = num;
	MSPD = num;
}
/* that damn regeneration cycle */
void Baddy::cycle()
{
	if(time % 10 == 0)
	{
		actTimer--; //decrease act Timer
		if(fileUse == true)	scriptGo();
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
void Baddy::checkCaps()
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
void Baddy::moveCost()
{
	if(time % 200 == 0) SP--;
}
/* Standard battle cycle */
void Baddy::battleCycle()
{
	badyPos[baddyNum - 1].X = pos.X;
	badyPos[baddyNum - 1].Y = pos.Y;

	cycle();	//run one cycle... maybe

	if(SP > MSP * 0.5) //if SP has been charged enough, do an attack
		{commandMenu();}
}
/* part of battleCycle */
void Baddy::commandMenu()
{
	//determine which action would be best at this time.
	target = 1 + (rand() % 2);
	commandAccept();
}
/* part of battleCycle */
void Baddy::commandMove()
{
	/*==========================Run Mode=============================*/
	gfx = stance_Run; //nothing special yet
}
/* part of battleCycle */
bool Baddy::commandAccept()
{
	if(SP > 5)
	{
		SP -= 5;
		scriptBegin("Hit");
	}
	return true;
}
/* part of battleCycle */
void Baddy::scriptGo()
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
		else if(currentStr.compare(0,9,"Enemy Hit") == 0) //end the timer for an offense attack
		{
			dmgRequest = (int)(ATT * (0.8 + (rand() % 4)/10));
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
		wantX += plyrPos[target - 1].X;
		//find y want
		for(int i = 0;i < 4;i++) tmp[i] = actString[i + 26];
		f32 wantY = (f32) std::atoi(tmp);
		wantY += plyrPos[target - 1].Y;
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
bool Baddy::scriptMove(f32 wantX, f32 wantY)
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
void Baddy::scriptBegin(const char * name)
{
	//create the full string that holds the filepath
	std::string strn1 = "FFP/skill/baddy/";
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
	}
}
#endif