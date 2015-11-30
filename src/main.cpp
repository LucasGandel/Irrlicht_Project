#include <irrlicht.h>

#include "Project_Config.h"
#include "../include/EventReceiver.hpp"
#include "../include/TerrainManager.hpp"
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

//  //Add Static camera
//  sceneManager->addCameraSceneNode( 0,         //Set camera as static
//    irr::core::vector3df( 40.0f, 10.0f, 15.0f ), // Camera position
//    irr::core::vector3df( 0.0f, 9.0f, 15.0f ) );  // Camera orientation
// add camera
  irr::scene::ICameraSceneNode* camera =
    sceneManager->addCameraSceneNodeFPS(0,100.0f,1.2f);

  camera->setPosition(irr::core::vector3df(2700*2,255*2,2600*2));
  camera->setTarget(irr::core::vector3df(2397*2,343*2,2700*2));
  camera->setFarValue(42000.0f);

  // Add terrain scene node
  TerrainManager terrainManager;
  terrainManager.addTerrainToScene( sceneManager, driver );

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
