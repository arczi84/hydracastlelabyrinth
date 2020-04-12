#include <SDL/SDL.h>
#include "system.h"

#include <exec/exec.h>
#include <proto/exec.h>

char quitGame = 0;

void Input_KeyEvent(SDL_Event* evt);
void Input_JoyEvent(SDL_Event* evt);
void Input_JoyAxisEvent(SDL_Event* evt);
void Input_JoyHatEvent(SDL_Event* evt);

int PHL_MainLoop()
{
    SDL_Event evt;
    while(SDL_PollEvent(&evt))
    {
        switch(evt.type)
        {
            case SDL_QUIT:
                quitGame = 1;
                return 0;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Input_KeyEvent(&evt);
                break;
            case SDL_JOYAXISMOTION:
                Input_JoyAxisEvent(&evt);
                break;
            case SDL_JOYHATMOTION:
                Input_JoyHatEvent(&evt);
                break;
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                Input_JoyEvent(&evt);
                break;
        }
    }
    if (quitGame == 1) 
    {
		return 0;
	}
	return 1;
}
void PHL_ConsoleInit()
{

}
void PHL_GameQuit()
{
    quitGame = 1;
}

void PHL_ErrorScreen(char* message)
{
    fprintf(stderr, "%s\n", message);
}

char *
strsep (char **stringp, const char *delim)
{
  char *begin, *end;
  begin = *stringp;
  if (begin == NULL)
    return NULL;
  /* Find the end of the token.  */
  end = begin + strcspn (begin, delim);
  if (*end)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = '\0';
      *stringp = end;
    }
  else
    /* No more delimiters; this is the last token.  */
    *stringp = NULL;
  return begin;
}

// get rid of SDL's saga detection because it isn't necessary
// and produce garbage when not fullscreen during splash-logo-video
extern int __real_SDL_VideoInit (const char *driver_name, Uint32 flags);
int __wrap_SDL_VideoInit (const char *driver_name, Uint32 flags)
{
    ULONG bak = SysBase->AttnFlags;
    int ret;

    SysBase->AttnFlags &= ~(1 << 10);
    ret = __real_SDL_VideoInit(driver_name, flags);
    SysBase->AttnFlags = bak;

    return ret;
}
