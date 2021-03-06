#include "SledgeHost.h"


#include <uslscore/USFileSys.h>

#ifndef HAS_MOAILUA
#define HAS_MOAILUA
#include <lua-headers/moai_lua.h>
#endif

#define SLEDGE_HOST_USE_LUAEXT

#ifdef SLEDGE_HOST_USE_LUAEXT
#include <aku/AKU-luaext.h>
#endif

#include <aku/AKU-untz.h>

#ifdef SLEDGE_HOST_USE_AUDIOSAMPLER
#include <aku/AKU-audiosampler.h>
#endif

#ifdef WIN32
//#include <glut.h>
#include <FolderWatcher-win.h>
#else
//#include <GLUT/glut.h>
#endif

#ifdef __APPLE__
#include <FolderWatcher-mac.h>
#include <OpenGL/OpenGL.h>
#include <moaiext-macosx/SFSAkuInit.h>
#endif

static bool sDynamicallyReevaluateLuaFiles = false;

static unsigned int s_timerInterval = 0;

#ifndef SDL_STATICWRAPPER_VAR
#define SDL_STATICWRAPPER_VAR
void* CurrentSledgeHostInstance = NULL;
#endif


SledgeHost::SledgeHost(int argc, char** arg)
{
	memset(&m_WindowPos, 0, sizeof(vec2u));
	memset(&m_WindowSize, 0, sizeof(vec2u));
	memset(&m_WindowTitle, 0, WINDOWTITLE_LENGTH);

	m_SDLWindow = NULL;

	m_InputManager = new SledgeInputManager();

	makeActive();

	// @todo	un-dumb this
	char* lastScript = NULL;

	// 2013/3/10	look for files if no args given
	//printf("argc count: %d\n", argc);
	if(argc == 1)
	{
		int foundFileIdx = -1;
		char* testfilenames[] = {
			"test/main.lua",
			"main.lua",
			"main.lb",
			"Resources/main.lua",
			"Resources/main.lb"
		};
		for (int i = 0; i < 5; ++i)
		{
#ifdef _DEBUG
			printf("%d: %s...", i, testfilenames[i]);
#endif
			if(USFileSys::CheckFileExists (testfilenames[i]))
			{
				//AKURunScript(testfilenames[i]);
				lastScript = testfilenames[i];
				foundFileIdx = i;
#ifdef _DEBUG
				printf("\tfound\n");
#endif
				break;
			} else {
#ifdef _DEBUG
				printf("\tnot found\n");
#endif
			}
		}

		if(foundFileIdx == -1)
		{
			// signal quit?
		}
	} else {

		for ( int i = 1; i < argc; ++i ) {
			char* thisarg = arg [ i ];
			if ( strcmp( thisarg, "-e" ) == 0 ) {
				sDynamicallyReevaluateLuaFiles = true;
			}
			else if ( strcmp( thisarg, "-s" ) == 0 && ++i < argc ) {
				char* script = arg [ i ];
				AKURunString ( script );
			}
			else {
				//AKURunScript ( thisarg );
				lastScript = thisarg;
			}
		}
	}

	// Detect desired different CWD and switch if necessary.
	int scr = 0;
	if(lastScript != NULL)
	{
		// detect script directory
		printf("script path given: %s\n", lastScript);
#ifdef WIN32
		//setStartupDir(lastScript);
		scr = winhostext_SetWorkingDirectory(lastScript);
#endif
	}

	// Init needs to happen after CWD switch, if it happens.
	doInit();

	AKURunBytecode ( moai_lua, moai_lua_SIZE );

	REGISTER_LUA_CLASS ( SledgeCore );
	REGISTER_LUA_CLASS ( SledgeInputHandler );
	SledgeInputHandler::SetManager(m_InputManager);

	REGISTER_LUA_CLASS (SledgeGraphicsHandler);
	
	
	// Finally, run the script.
	AKUSetArgv ( arg );
	if(lastScript != NULL)
	{
#ifdef WIN32
		if(scr > 0)
		{
			AKURunScript(&(lastScript[strlen(lastScript) - scr]));
		} else {
			AKURunScript(lastScript);
		}
#else 
		AKURunScript(lastScript);
#endif
	}


	if ( lastScript && sDynamicallyReevaluateLuaFiles ) {
#ifdef WINDOWS
		winhostext_WatchFolder ( lastScript );
#elif __APPLE__
//		FIXME: FWWatch isn't linkable at the moment. need to fix this.
//		FWWatchFolder( lastScript );
#endif
	}

	double simstep = AKUGetSimStep();

	m_TimerInterval = (unsigned int)(AKUGetSimStep() * 1000.0);
	m_TimerInterval3 = m_TimerInterval * 2;

	m_Counter = 0;

	runGame();
}


SledgeHost::~SledgeHost()
{
	// teardown
	AKUFinalize();
	delete m_InputManager;
}

bool SledgeHost::doInit()
{
	// initialize SDL
	SDL_Init(
		SDL_INIT_VIDEO |
		SDL_INIT_TIMER |
		SDL_INIT_JOYSTICK |
		SDL_INIT_GAMECONTROLLER |
		SDL_INIT_HAPTIC
	);

	// refresh the AKU context
	m_AkuContext = AKUGetContext();
	if(m_AkuContext) {
		AKUDeleteContext(m_AkuContext);
	}
	m_AkuContext = AKUCreateContext();

	//AKUInitMemPool ( 100 * 1024 * 1024 );

	// initialise AKU modules
	// @todo add more AKU things here
	#ifdef SLEDGE_HOST_USE_LUAEXT
	AKUExtLoadLuacrypto ();
	AKUExtLoadLuacurl ();
	AKUExtLoadLuafilesystem ();
	AKUExtLoadLuasocket ();
	AKUExtLoadLuasql ();
	#endif

	AKUUntzInit();

	#ifdef SLEDGE_HOST_USE_AUDIOSAMPLER
	//AKUAudioSamplerInit();
	#endif


	// set AKU input configuration and reserve AKU input devices 
	m_InputManager->doAKUInit();
	
#ifdef WIN32
	MOAIEnvironment& environment = MOAIEnvironment::Get ();

	// 2013/06/17: Load info.xml
	SledgeCore::LoadInfoXML( "info.xml", &environment );

#elif __APPLE__
	[SFSAkuInit MoaiTypesInit];
	[SFSAkuInit MoaiEnvironmentInit];
#endif
	
	AKUSetFunc_OpenWindow (&SledgeHost::AKUCallbackWrapper_OpenWindowFunc);

	return true;
}

void SledgeHost::runGame()
{
	SDL_Event event;
	unsigned int tick_start = SDL_GetTicks();
	unsigned int tick_end = 0;
	unsigned int tick_delta = 0;
	unsigned int tick_wait = 0;
	bool bGameRunning = true;
	m_DeltaTime = 0.0;
	if(m_SDLWindow != NULL) {
		m_ActiveTimer = SDL_AddTimer(
			m_TimerInterval,
			&SledgeHost::SDLCallbackWrapper_OnTickFunc,//_onTick,
			&m_TimerInterval
		);	
		
		while (bGameRunning)
		{
			m_InputManager->blankKeybFlags();

			while(SDL_PollEvent(&event))
			{
				switch(event.type) {
				case SDL_KEYDOWN:
				case SDL_KEYUP:
					if(event.key.keysym.sym == SDLK_F4 && (event.key.keysym.mod == KMOD_LALT || event.key.keysym.mod == KMOD_RALT))
					{
						bGameRunning = false;						
					} else {
						m_InputManager->inputNotify_onKeyDown(&(event.key));
					}

					break;

				case SDL_MOUSEMOTION:
					m_InputManager->inputNotify_onMouseMove(&(event.motion));
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					m_InputManager->inputNotify_onMouseButton(&(event.button));
					break;
				case SDL_USEREVENT:
					//Render updates not being received and updates not tied to render causes stuttering
					//ProcessUserEvent(event.user.code);
					break;

				case SDL_CONTROLLERAXISMOTION:
				case SDL_CONTROLLERBUTTONDOWN:
				case SDL_CONTROLLERBUTTONUP:
					break;

				case SDL_QUIT:
					SDL_RemoveTimer(m_ActiveTimer);
					bGameRunning = false;
					break;
				default:
					break;
				}
			};

			AKUUpdate();
			AKURender();
			SDL_GL_SwapWindow(m_SDLWindow);

			m_InputManager->doOnTick();

			// Update the clock.
			tick_end = SDL_GetTicks();
			tick_delta = tick_end - tick_start;
			m_DeltaTime = ((double)tick_delta) / 1000.0;
			//printf("Tick: %i - %f\n", tick_delta, m_DeltaTime);
			tick_start = tick_end;
		}
	}
}

void SledgeHost::makeActive()
{
	CurrentSledgeHostInstance = (void*)this;
}

void SledgeHost::AKUCallback_OpenWindowFunc
	(const char* title, int width, int height)
{
	m_WindowPos.x = 180;
	m_WindowPos.y = 100;

	m_WindowSize.x = width;
	m_WindowSize.y = height;
	
	// Set up a window if we don't have one yet.
	if(m_SDLWindow == NULL)
	{
		// configure SDL's GL settings.
		// @todo	assess whether this is actually a good idea
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		// bring up the window
		if((m_SDLWindow = SDL_CreateWindow(
			title,
			m_WindowPos.x,
			m_WindowPos.y,
			m_WindowSize.x,
			m_WindowSize.y,
			SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
			)) == NULL)
		{
			SDL_Quit();
			return;
		}
		
		SDL_SetWindowTitle(m_SDLWindow, title);

		// create the gl context
		m_SDLGLContext = SDL_GL_CreateContext(m_SDLWindow);

		// tell AKU about what we just did
		AKUDetectGfxContext ();
		AKUSetScreenSize (
			m_WindowSize.x,
			m_WindowSize.y
		);		
		AKUSetViewSize (
			m_WindowSize.x,
			m_WindowSize.y
		);

		int vsync = SDL_GL_SetSwapInterval(1);
		if (vsync == 0) {
			printf("VSync enabled!\n");
		} else {
			printf("VSync not enableable!\n");
		}
	}

	SledgeGraphicsHandler::SetWindow(m_SDLWindow);


	// 2013/06/17: Get additional environment info
	SledgeCore::GetAdditionalHWInfo( &(MOAIEnvironment::Get()) );

}

void SledgeHost::AKUCallbackWrapper_OpenWindowFunc
	(const char* title, int width, int height)
{
	SledgeHost* obj = (SledgeHost*)CurrentSledgeHostInstance;

	obj->AKUCallback_OpenWindowFunc(title, width, height);
}

void SledgeHost::_doAkuUpdate()
{
	AKUUpdate();

	if ( sDynamicallyReevaluateLuaFiles ) {		
#ifdef _WIN32
		winhostext_Query ();
#elif __APPLE__
//		FWReloadChangedLuaFiles ();
#endif
	}
}

void SledgeHost::_doAkuRender()
{
	AKURender();
	SDL_GL_SwapWindow(m_SDLWindow);
}

void SledgeHost::ProcessUserEvent(int p_type)
{
	switch (p_type)
	{
	case SDLUserEventType::UET_RENDER:
		_doAkuRender();
		break;
	case SDLUserEventType::UET_UPDATE:
		_doAkuUpdate();
		break;
	default:
		break;
	}
}

unsigned int SledgeHost::SDLCallback_OnTickFunc(unsigned int millisec, void* param)
{
	UNUSED (millisec);

	// re-register the timer
	m_TimerInterval2 =
		(unsigned int)(AKUGetSimStep() * 1000.0);
	m_ActiveTimer = SDL_AddTimer(
		m_TimerInterval2,
		&SledgeHost::SDLCallbackWrapper_OnTickFunc,//_onTick,
		&m_TimerInterval2
		);

	// use SDL_PushEvent to enqueue AKU_Update because AKU for whatever reason
	// isn't threadsafe; this SDL callback runs on a separate thread so we
	// can't do it here.
	SDL_Event event;

	event.type = SDL_USEREVENT;
	event.user.code = SDLUserEventType::UET_UPDATE;
	SDL_PushEvent(&event);

	return 0;
}

unsigned int SledgeHost::SDLCallbackWrapper_OnTickFunc(unsigned int millisec, void* param)
{
	SledgeHost* obj = (SledgeHost*)CurrentSledgeHostInstance;
	return obj->SDLCallback_OnTickFunc(millisec, param);
}