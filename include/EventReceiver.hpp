#ifndef EVENTRECEIVER_H
#define EVENTRECEIVER_H

#include <irrlicht.h>

//Define controls
#define KEY_UP irr::KEY_KEY_Z
#define KEY_DOWN irr::KEY_KEY_S
#define KEY_LEFT irr::KEY_KEY_Q
#define KEY_RIGHT irr::KEY_KEY_D
#define KEY_JUMP irr::KEY_KEY_K
#define KEY_ATTACK irr::KEY_SPACE

class EventReceiver : public irr::IEventReceiver
{
  public:

    EventReceiver()
    {
      for ( irr::u32 i=0; i < irr::KEY_KEY_CODES_COUNT; ++i )
        {
        KeyIsDown[i] = false;
        KeyIsPressed[i] = false;
        }
    }

    // This is the one method that we have to implement
    virtual bool OnEvent(const irr::SEvent& event)
    {
      // Remember whether each key is down or up
      if ( event.EventType == irr::EET_KEY_INPUT_EVENT )
        {
//        if( event.KeyInput.PressedDown )
//          {
//          if( !KeyIsDown[event.KeyInput.Key] )
//            {
//            KeyIsPressed[event.KeyInput.Key] = true;
//            }
//          else
//            {
//            KeyIsPressed[event.KeyInput.Key] = false;
//            }
//          }

        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        }
      return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown( irr::EKEY_CODE keyCode ) const
    {
      return KeyIsDown[keyCode];
    }

    // This is used to check whether a key is being held down
//    virtual bool IsKeyPressed( irr::EKEY_CODE keyCode )
//    {
//      if( KeyIsPressed[keyCode] && !KeyIsDown[keyCode])
//        KeyIsPressed[keyCode] = false;
//      // Set keys to press if another key is down
//      // WARNING: should check any other active key than arrows if some.
//      // This is used because the OnEvent() method miss to set key pressed
//      // if we press another key just after having pressed one.
//      if( numberOfKeysDown() > 1 && KeyIsPressed[keyCode] )
//        KeyIsPressed[keyCode] = false;

//      return KeyIsPressed[keyCode];
//    }

    //Indicates if arrow keys are down
    bool IsArrowDown()
    {
     return( KeyIsDown[irr::KEY_KEY_Z] ||
       KeyIsDown[irr::KEY_KEY_S] ||
       KeyIsDown[irr::KEY_KEY_Q] ||
       KeyIsDown[irr::KEY_KEY_D] );
    }

    //Indicates if arrow keys are pressed
    int numberOfKeysDown()
    {
      int count = 0;
      for ( irr::u32 i=0; i < irr::KEY_KEY_CODES_COUNT; ++i )
        {
        if( KeyIsDown[i] )
          {
          ++count;
          }
        }

     return( count );
    }

  private:

    // We use this array to store the current state of each key
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    // Check if keys are pressed
    bool KeyIsPressed[irr::KEY_KEY_CODES_COUNT];

};

#endif // EVENTRECEIVER_H
