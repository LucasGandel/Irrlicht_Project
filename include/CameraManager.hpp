#ifndef CAMERAMANAGER_HPP
#define CAMERAMANAGER_HPP

#include <irrlicht.h>
#include "PathFinder.hpp"

class CameraManager
{
  public:
    //! Constructor
    CameraManager()
    {}

    //Add FPS camera
    void addFPSCameraToScene( irr::scene::ISceneManager* sceneManager )
    {
      camera = sceneManager->addCameraSceneNodeFPS( 0, 100.0f, 1.2f );

      camera->setPosition(irr::core::vector3df(2700*2,255*2,2600*2));
      camera->setTarget(irr::core::vector3df(2397*2,343*2,2700*2));
      camera->setFarValue(42000.0f);
    }

    //Add Static camera
    void addStaticCameraToScene( irr::scene::ISceneManager* sceneManager )
    {
      sceneManager->addCameraSceneNode( 0,          //Parent Node
        irr::core::vector3df( 40.0f, 10.0f, 15.0f ),//Camera position
        irr::core::vector3df( 0.0f, 9.0f, 15.0f ) );//Camera orientation
    }

    //Camera
    irr::scene::ICameraSceneNode* camera;

};

#endif // CAMERAMANAGER_HPP
