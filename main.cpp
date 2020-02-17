/** Fake Fantasy Pi **/

#include <irrlicht.h>
#include <irrKlang.h>
#pragma comment(lib, "irrKlang.lib")
#include "driverChoice.h"
using namespace irr;
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

/* Personal Include */
#include "include\eventReceiver.h"
#include "include\chara.h"
#include "include\baddy.h"
#include "include\enums.h"
#include "include\controls.h"

int main()
{
	Chara plyr[2];  //create 2 characters
	plyr[Sean] = Chara();
	plyr[Sean].setSPD(40);  // let's set his Speed/MaxSpeed to 40 (/100)
	plyr[Sean].ATT += 5;
	plyr[Sean].DEF += 5;
	plyr[Sean].human = true;
	plyr[Sean].plyr = 1;
	plyr[AD] = Chara();
	plyr[AD].setSPD(10);
	plyr[AD].pos.X = 50;
	plyr[AD].human = true;
	plyr[AD].plyr = 2;
	plyr[AD].DEF = 4;
	plyr[AD].MAG += 10;
	plyr[AD].HP = 70;
	plyr[AD].MHP = 70;
	plyr[AD].MP = 150;
	plyr[AD].MMP = 150;

	Baddy bads = Baddy();

	Controller ctrl = Controller(); //create a player set of controllers
	bool fullscreen = false;	//just setup the fullscreen mode

	engine = irrklang::createIrrKlangDevice();	//sound driver
	if (!engine) return 0;	//failsafe

	MyEventReceiver receiver;	//create input receiver

	video::E_DRIVER_TYPE driverType = irr::video::E_DRIVER_TYPE(5);		//driverChoiceConsole();  if wants a custom console type
	if (driverType==video::EDT_COUNT) return 1;							//failsafe for errors
	IrrlichtDevice *device = createDevice(driverType, core::dimension2d<u32>(640, 480), 16, fullscreen, false, false, &receiver);	//setup the drivertype, size of window, color bit amount,  fullscreen?, idk, idk, address of the input reciever
	if (device == 0) return 1;											// failsafe for errors
	device->setWindowCaption(L"Fake Fantasy 3.14159265 (Pi)");			//make the window look nice ^^
	video::IVideoDriver* driver = device->getVideoDriver();				//grab the video driver

	/* input textures for drawing */
	video::ITexture* bg_texture = driver->getTexture("FFP/gfx/bg/grass.png");
	driver->makeColorKeyTexture(bg_texture, core::position2d<s32>(0,0));
	
	video::ITexture* icon_texture = driver->getTexture("FFP/gfx/icon.png");
	driver->makeColorKeyTexture(bg_texture, core::position2d<s32>(0,0));

	video::ITexture* player_req_texture[2];
	video::ITexture* player_texture[2];

	player_req_texture[0];
	player_texture[0] = driver->getTexture("FFP/gfx/sean/main.png");
	driver->makeColorKeyTexture(player_texture[0], core::position2d<s32>(0,0));

	player_req_texture[1];
	player_texture[1] = driver->getTexture("FFP/gfx/ad/main.png");
	driver->makeColorKeyTexture(player_texture[1], core::position2d<s32>(0,0));

	for(int k = 0;k < 2;k++)
	{
		std::string name = "sean";
		if(k==1) name = "ad";
		FILE * scriptFile;//put all frame caps on the character
		char filePath[256] = "";
		convertStringToChar(filePath, "FFP/gfx/",name,"/main.txt");
		if((scriptFile = std::fopen(filePath,"r")) != NULL)
		{
			for(int i = 0;i < 8;i++)
			{
				char char_parser[6] = "";
				std::fgets(char_parser,256,scriptFile);//get some string
				for(int j = 0;j < 5;j++){char_parser[j] = '0';}
				plyr[k].frame.cap[i] = std::atoi(char_parser);
			}
			std::fclose(scriptFile);
		}
		else
		{
			std::cout << "File failed to open" << std::endl;
		}
	}

	video::ITexture* slime_texture = driver->getTexture("FFP/gfx/baddy/slime/main.png");
	driver->makeColorKeyTexture(slime_texture, core::position2d<s32>(0,0));

	/* Fonts */
	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	gui::IGUIFont* font2 = device->getGUIEnvironment()->getFont("FFP/font/fonthaettenschweiler.bmp");

	/* Prepare a nicely filtering 2d render mode for special cases. */
	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
	driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;

	u32 then = device->getTimer()->getTime();  //get a timer to ensure proper frame-rate
	irr::EKEY_CODE keys1[14] = {KEY_UP,KEY_RIGHT,KEY_DOWN,KEY_LEFT, KEY_KEY_A,KEY_KEY_Z,KEY_KEY_X,KEY_KEY_S, KEY_KEY_Q,KEY_KEY_W, KEY_KEY_1,KEY_KEY_2, KEY_KEY_C,KEY_KEY_V};	//create default keyboard keys
	irr::EKEY_CODE keys2[14] = {KEY_KEY_I,KEY_KEY_O,KEY_KEY_L,KEY_KEY_K, KEY_KEY_H,KEY_KEY_N,KEY_KEY_M,KEY_KEY_J, KEY_KEY_Y,KEY_KEY_U, KEY_KEY_6,KEY_KEY_7, KEY_KEY_8,KEY_KEY_9};	//create default keyboard keys
	int button1[14] = {0,0,0,0, 2,1,3,4, 5,7, 6,8, 9,10};	//create default gamepad keys
	ctrl.ControllerSet(1, CtrlType_Keyboard, keys1, button1);	//Player 1's keys are set.
	ctrl.ControllerSet(2, CtrlType_Keyboard, keys2, button1);	//Player 2's keys are set.
	ctrl.ControllerSet(3, CtrlType_Keyboard, keys2, button1);	//Player 3's keys are set.
	ctrl.ControllerSet(4, CtrlType_Keyboard, keys2, button1);	//Player 4's keys are set.
	bool running = true;	//keep the game going
	core::rect<s32> bg_frame(0,0,640,300);	/* setup the bg's frame location */
	video::SColor color_normal = video::SColor(255,255,255,255);
	video::SColor color_black = video::SColor(255,0,0,0);
	video::SColor color_red = video::SColor(255,255,0,0);
	video::SColor color_green = video::SColor(255,0,255,0);
	video::SColor color_blue = video::SColor(255,0,0,255);
	//engine->play2D("FFP/midi/El Hazard - Battle.it", true);	//play bgm

	//game time
	while(device->run() && driver && running == true)
	{
		if (device->isWindowActive() && running == true)
		{
			/* Deal with cycle routines */
			time = device->getTimer()->getTime();	// get the timer
			frameDT = (f32)(time - then) / 1000.f;	// Recieve how much change in time there has been
			then = time;	//recycle 'then' to find the change in time next while loop
						
			/* Ensure buttons are up to date */
			for(int j = 0;j < 255;j++)
			{
				if(receiver.IsKeyDown((irr::EKEY_CODE) j)) ctrl.handleKeypress((irr::EKEY_CODE) j, -1, -1);
				else ctrl.handleKeyrelease((irr::EKEY_CODE) j, -1, -1);
			}

			/*=================player control==================*/
			for(int num = 0;num < 2;num++)
			{
				plyr[num].ctrl = ctrl;		//give the player the updated controls
				plyr[num].battleCycle();	//ensure the player goes through the battle cycle

				if(plyr[num].gfxRequest != "none")  //check if player is requesting a new gfx sheet
				{
					irr::io::path strn[5];
					strn[0] = "FFP/gfx/";
					if(num==0) {strn[1] = "sean";}
					if(num==1) {strn[1] = "ad";}
					strn[2] = "/";
					strn[3] = plyr[num].gfxRequest;
					strn[4] = ".png";
					for(int i = 1;i < 5;i++){strn[0].append(strn[i]);}
					player_req_texture[num] = driver->getTexture(strn[0]);
					driver->makeColorKeyTexture(player_req_texture[num], core::position2d<s32>(0,0));
					plyr[num].gfxRequest = "none";
				}
			}
			bads.battleCycle();	//ensure the baddy goes through the battle cycle

			/*=================damage requests==================*/
			for(int i = 0; i < 2; i++)
			{
				if(plyr[i].dmgRequest != -1)
				{
					engine->play2D("FFP/sfx/Damage.wav", false);
					std::cout << "Damage: " << plyr[i].dmgRequest - (bads.DEF * 0.5) << "\n";
					if(plyr[i].dmgRequest - (int)(bads.DEF * 0.25) > 1)
						{bads.HP -= plyr[i].dmgRequest - (int)(bads.DEF * 0.5);}
					plyr[i].dmgRequest = -1;
				}
			}
			if(bads.dmgRequest != -1)
			{
				engine->play2D("FFP/sfx/Damage.wav", false);
				std::cout << "Damage: " << bads.dmgRequest - (plyr[bads.target - 1].DEF * 0.5) << "\n";
				if(bads.dmgRequest - (int)(plyr[bads.target - 1].DEF * 0.25) > 1)
					{plyr[bads.target - 1].HP -= bads.dmgRequest - (int)(plyr[bads.target - 1].DEF * 0.25);}
				bads.dmgRequest = -1;
			}

			/*=================begin drawing===================*/
			driver->beginScene(true, true, video::SColor(255,120,102,136));	//open the scene
			driver->draw2DImage(bg_texture, core::position2d<s32>(0,0),bg_frame, 0, color_normal, true);	// draw bg

			for(int num = 0;num < 2;num++)
			{
				// draw shadow
				if(plyr[num].move)
				{
					core::position2d<s32> m = core::position2d<s32>((s32)plyr[num].pos.X,(s32)plyr[num].pos.Y);
					driver->draw2DRectangle(video::SColor(165,255 - (time) % 255,255 - (time) % 255,255 - (time) % 255),
						core::rect<s32>(m.X-20, m.Y-10, m.X+20, m.Y+10));
				}
				else
				{
					core::position2d<s32> m = core::position2d<s32>((s32)plyr[num].pos.X,(s32)plyr[num].pos.Y);
					driver->draw2DRectangle(video::SColor(165,0,0,0),
						core::rect<s32>(m.X-20, m.Y-10, m.X+20, m.Y+10));
				}

				if(plyr[num].gfx == stance_Other)
				{
					irr::core::rect<irr::s32> currentFramePos;
					currentFramePos.UpperLeftCorner.X = 0;
					currentFramePos.LowerRightCorner.X = 200;
					currentFramePos.UpperLeftCorner.Y = plyr[num].gfxNum * 200;
					currentFramePos.LowerRightCorner.Y = (plyr[num].gfxNum + 1) * 200;

					irr::core::vector2d<irr::s32> currentGfxPos;
					currentGfxPos.X = plyr[num].pos.X - 100;
					currentGfxPos.Y = plyr[num].pos.Y - 100;

					driver->draw2DImage(player_req_texture[num], currentGfxPos, currentFramePos , 0,	color_normal, true);	// draw player performing special action
				}
				else
				{
					driver->draw2DImage(player_texture[num], plyr[num].gfxPos(), plyr[num].gfxFramePos(), 0,	color_normal, true);	// draw player
				}

				int menuXoffset = num * 640 / 4;
				// draw bars
				driver->draw2DRectangle(video::SColor(200 + time % 55,255,50,50),	core::rect<s32>(menuXoffset+10, 360, menuXoffset+10 + (int)(100.0 * plyr[num].HP / (double)plyr[num].MHP), 370));
				if(plyr[num].HP > plyr[num].MHP) driver->draw2DRectangle(video::SColor(200 + time % 55,155,60,60),	core::rect<s32>(menuXoffset+110, 360, menuXoffset+10 + (int)(100.0 * plyr[num].HP / (double)plyr[num].MHP), 370));
				driver->draw2DRectangle(video::SColor(200 + time % 55,50,255,50),	core::rect<s32>(menuXoffset+10, 370, menuXoffset+10 + (int)(100.0 * plyr[num].MP / (double)plyr[num].MMP), 380));
				if(plyr[num].MP > plyr[num].MMP) driver->draw2DRectangle(video::SColor(200 + time % 55,60,155,60),	core::rect<s32>(menuXoffset+110, 370, menuXoffset+10 + (int)(100.0 * plyr[num].MP / (double)plyr[num].MMP), 380));
				driver->draw2DRectangle(video::SColor(200 + time % 55,50,50,255),	core::rect<s32>(menuXoffset+10, 380, menuXoffset+10 + (int)(100.0 * plyr[num].SP / (double)plyr[num].MSP), 390));
				if(plyr[num].SP > plyr[num].MMP) driver->draw2DRectangle(video::SColor(200 + time % 55,60,60,155),	core::rect<s32>(menuXoffset+110, 380, menuXoffset+10 + (int)(100.0 * plyr[num].SP / (double)plyr[num].MSP), 390));
				driver->draw2DRectangle(video::SColor(100,155,155,155),	core::rect<s32>(menuXoffset+10, 360, menuXoffset+110, 390));
				if (font)
				{
					font->draw(addString("HP:",plyr[num].HP),core::rect<s32>(menuXoffset+50,360,menuXoffset+145,370),color_black);
					font->draw(addString("MP:",plyr[num].MP),core::rect<s32>(menuXoffset+50,370,menuXoffset+145,380),color_black);
					font->draw(addString("SP:",plyr[num].SP),core::rect<s32>(menuXoffset+50,380,menuXoffset+145,390),color_black);
				}

				// draw menu
				if(!plyr[num].move) driver->draw2DRectangle(color_blue, core::rect<s32>(menuXoffset+15, 400 + plyr[num].menuPos*20, menuXoffset+150, 420 + plyr[num].menuPos*20));	//draw the cursor
				if (font2 && font)
				{
					irr::core::stringw name = "Sean";
					if(num==1) name = "AD";
					font2->draw(name,core::rect<s32>(menuXoffset+70,320,menuXoffset+145,340),color_normal);	//draw the player's text for the name
					switch(plyr[num].subMenu)	//draw the current amount of text for menu
					{
						case -1:
							font2->draw(L"OFFENSE",core::rect<s32>(menuXoffset+20,400,menuXoffset+145,420),color_normal);
							font2->draw(L"DEFENSE",core::rect<s32>(menuXoffset+20,420,menuXoffset+145,440),color_normal);
							font2->draw(L"MOVE   ",core::rect<s32>(menuXoffset+20,440,menuXoffset+145,460),color_normal);
							font2->draw(L"ITEM   ",core::rect<s32>(menuXoffset+20,460,menuXoffset+145,480),color_normal);
						break;
						case 0:
							for(int i = 0;i < 4;i++)
								font2->draw(plyr[num].attMenu[plyr[num].subMenuPage*4 + i],core::rect<s32>(menuXoffset+20,400 + 20 * i,menuXoffset+145,420 + 20*i),color_normal);
						break;
						case 1:
							for(int i = 0;i < 4;i++)
								font2->draw(plyr[num].defMenu[plyr[num].subMenuPage*4 + i],core::rect<s32>(menuXoffset+20,400 + 20 * i,menuXoffset+145,420 + 20*i),color_normal);
						break; 
						case 3:
							for(int i = 0;i < 4;i++)
								font2->draw(plyr[num].itemMenu[plyr[num].subMenuPage*4 + i],core::rect<s32>(menuXoffset+20,400 + 20 * i,menuXoffset+145,420 + 20*i),color_normal);
						break;
					}
					/* draw page tabs */
					if(plyr[num].subMenu != -1)	//only draw while in a submenu
					{
						driver->draw2DRectangle(color_red, core::rect<s32>(menuXoffset+15, 390, menuXoffset+55, 400));	//draw the bg for the page menu
						driver->draw2DRectangle(color_blue, core::rect<s32>(menuXoffset+15 + plyr[num].subMenuPage*10, 390, menuXoffset+25 + plyr[num].subMenuPage*10, 400));	//draw the current page viewing
					}
				}
			}

			// draw monster
			driver->draw2DImage(slime_texture, bads.gfxPos(),
				bads.gfxFramePos(), 0,
				color_normal, true);

			if (font)	font->draw(L"Fake Fantasy Pi Battle Demo", core::rect<s32>(130,10,300,50), color_normal);	// draw some text
			
			/* Finally draw a half-transparent rect under the mouse cursor. */
			core::position2d<s32> m = device->getCursorControl()->getPosition();	//find the position of the mouse
			driver->draw2DRectangle(video::SColor(100,255,255,255),	core::rect<s32>(m.X-20, m.Y-20, m.X+20, m.Y+20));

			driver->endScene();	//end the drawing scene

			if(receiver.IsKeyDown(KEY_ESCAPE) == true) running = false;	///* Escape the main program */
		}
	}

	/* Clean up shop! */
	device->drop();
	engine->drop();
	return 0;
}