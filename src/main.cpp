#include <irrlicht.h>

#include "Project_Config.h"
#include "../include/EventReceiver.hpp"
#include "../include/TerrainManager.hpp"
#include "../include/CameraManager.hpp"
#include "../include/CharacterManager.hpp"

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
  // Set cursor position
  device->getCursorControl()->setPosition( 0.5f, 0.5f );

  // Create SceneManager
  irr::scene::ISceneManager* sceneManager =
    device->getSceneManager();

  //Set Cursor visibility
  device->getCursorControl()->setVisible( false );

  //Add 3rd Person camera
  CameraManager cameraManager;
  cameraManager.add3rdPersonCameraToScene( sceneManager );

  // Add terrain scene node
  TerrainManager terrainManager;
  terrainManager.addTerrainToScene( sceneManager, driver );
  //Here, the terrain create its own triangleSelector and set the cameraNode to collide with it;
  //See Character manager for better approach.
  terrainManager.addSceneNodeCollision( sceneManager, cameraManager.cameraNode );
  terrainManager.addSkyBox( sceneManager, driver );

  //Add mesh
  CharacterManager characterManager;
  characterManager.addCharacterToScene( sceneManager, driver );
  //Here the characterNode stores all triangleSelector for every object it collides with.
  characterManager.addSceneNodeCollision( sceneManager, terrainManager.terrainNode );

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
    //3rd Person mouvement
    cameraManager.UpdateCamera( device, &eventReceiver, characterManager );
    characterManager.UpdateCharacter( &eventReceiver );
//    characterManager.updateAnimationType( &eventReceiver );

    //Draw scene
    driver->beginScene( true, true, color );
    sceneManager->drawAll ();
    driver->endScene ();
  }

  device->drop (); //Free memory

  return EXIT_SUCCESS;
}
