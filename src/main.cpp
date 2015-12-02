#include <irrlicht.h>

#include "Project_Config.h"
#include "../include/EventReceiver.hpp"
#include "../include/TerrainManager.hpp"
#include "../include/CameraManager.hpp"

int main(void)
{
  //Event Receiver
  EventReceiver eventReceiver;

  // Device creation
  irr::IrrlichtDevice* device = irr::createDevice(
    irr::video::EDT_OPENGL, // API = OpenGL
    irr::core::dimension2d<irr::u32>( 800, 600 ), // Window Size 640x480p
    32, false, false, false, &eventReceiver ); // 32 bits/pixel, FullScreen, StencilBuffer, Vsync, Receiver

  // Create Video Driver
  irr::video::IVideoDriver* driver =
    device->getVideoDriver();
  // Create SceneManager
  irr::scene::ISceneManager* sceneManager =
    device->getSceneManager();

  //Set Cursor visibility
  device->getCursorControl()->setVisible( true );

  // Add FPS camera
  CameraManager cameraManager;
  cameraManager.addFPSCameraToScene( sceneManager );

  // Add terrain scene node
  TerrainManager terrainManager;
  terrainManager.addTerrainToScene( sceneManager, driver );
  terrainManager.addSceneNodeCollision( sceneManager, cameraManager.camera );

  //Set font color ( A (transparency), R, G, B )
  irr::video::SColor color( 255, 255, 255, 255);

  //Rendering loop
  while (device->run())
  {
    //Close window if the Escape key is pressed
    if(eventReceiver.IsKeyDown( irr::KEY_ESCAPE))
    {
      device->closeDevice();
    }
    //Draw scene
    driver->beginScene( true, true, color );
    sceneManager->drawAll ();
    driver->endScene ();
  }

  device->drop (); //Free memory

  return EXIT_SUCCESS;
}
