#ifndef CAMERAMANAGER_HPP
#define CAMERAMANAGER_HPP

#include <irrlicht.h>
#include "PathFinder.hpp"
#include "EventReceiver.hpp"
#include "CharacterManager.hpp"
#include "Math.hpp"
#include <iostream>

class CameraManager
{
  public:
    //! Constructor
    CameraManager()
      :FarValue( 42000.0f ), setCameraToMeshOrientation( true ),
       moveCameraCursor( false ), d_Interpolate( 1.0f ),
       x_Rotation( 0.0f ), y_Rotation( 0.0f )
    {}

    //Add 3er person camera
    void add3rdPersonCameraToScene( irr::scene::ISceneManager* sceneManager )
    {
      //WARNING : Call move3rdPersonCameraControl() in the main loop to trigger.
      cameraNode = sceneManager->addCameraSceneNode( 0, //Parent Node
        irr::core::vector3df( 0.0f, 0.0f, 0.0f ),       //CamDOWNera position
        irr::core::vector3df( 0.0f, 0.0f, 0.0f ),       //Camera orientation
        0,                                              //Camera Id
        true );                                         //Set Active
      cameraNode->setFarValue( FarValue );
    }

    //Add FPS camera
    void addFPSCameraToScene( irr::scene::ISceneManager* sceneManager )
    {
      cameraNode = sceneManager->addCameraSceneNodeFPS( 0, 100.0f, 1.2f );

      cameraNode->setPosition(irr::core::vector3df(2700*2,255*2,2600*2));
      cameraNode->setTarget(irr::core::vector3df(2397*2,343*2,2700*2));
      cameraNode->setFarValue( FarValue );
    }

    //Add Static camera
    void addStaticCameraToScene( irr::scene::ISceneManager* sceneManager )
    {
      cameraNode = sceneManager->addCameraSceneNode( 0, //Parent Node
        irr::core::vector3df( 40.0f, 10.0f, 15.0f ),    //Camera position
        irr::core::vector3df( 0.0f, 9.0f, 15.0f ) );    //Camera orientation
      cameraNode->setFarValue( FarValue );
    }

    //Timer tick
    void timerTick( irr::IrrlichtDevice* device )
    {
    float deltaTime = device->getTimer()->getTime() - time;

    timer += deltaTime/100000.0f;
    }

    //Move 3rd person camera to its character
    void UpdateCamera( irr::IrrlichtDevice* device,
      EventReceiver* eventReceiver, CharacterManager characterManager )
    {
      //Get cursor mouvement
      irr::core::position2d<irr::f32> cursorPos =
        device->getCursorControl()->getRelativePosition();

      float cursorSensibility = 256.0f;
      float dx = ( cursorPos.X - 0.5 ) * cursorSensibility;
      float dy = ( cursorPos.Y - 0.5 ) * cursorSensibility;

      //Update Rotation
      y_Rotation += dx;
      x_Rotation += dy;

      clamp( &x_Rotation, -90, 10 );
      clampAsAngle( &y_Rotation );

      //Reset Cursor position
      device->getCursorControl()->setPosition( 0.5f, 0.5f );

      //Set Camera to 3rd person orientation
      if( checkOrientation( device, eventReceiver->IsArrowDown(), dx, dy, characterManager.y_MeshRotation ) )
        {
        replaceCameraToMesh(characterManager.y_MeshRotation);
        }

      //3rd person position
      irr::core::vector3df characterNodePosition = characterManager.characterNode->getPosition();
      //Camera Zoom
      float Zoom = 70.0f;

      //Camera Position and Orientation
      irr::core::vector3df cameraPos = characterNodePosition + irr::core::vector3df( Zoom, Zoom, 0 );
      cameraPos.rotateXYBy( x_Rotation, characterNodePosition );
      cameraPos.rotateXZBy( -y_Rotation, characterNodePosition );

      //Update
      cameraNode->setPosition( cameraPos );
      cameraNode->setTarget( irr::core::vector3df( characterNodePosition.X, characterNodePosition.Y, characterNodePosition.Z ) );

    }

    //Set camera orientation to mesh orientation
    void replaceCameraToMesh(float y_MeshRotation)
    {
      if( abs(y_MeshRotation-y_Rotation) >= 1.0f )
        {
        //Determine delta rotation to perform
        //**WARNING: Could be improve by doing calculation only on first pass
        // No need to recalculate angle at every frame for interpolation**
        float yOmega = y_MeshRotation - y_Rotation;
        clampAsAngle( &yOmega );

        float xOmega = -25.0f - x_Rotation;
        clampAsAngle( &xOmega );

        //Interpolate rotation between mesh and camera
        irr::core::vector3df d_Rotation(xOmega,yOmega, 0);
        irr::core::vector3df cameraRotation(x_Rotation,y_Rotation,0);
        irr::core::vector3df currentRotation;
        currentRotation.interpolate(irr::core::vector3df(0.0f,0.0f,0.0f),d_Rotation,d_Interpolate);

        //Rotate camera to mesh
        x_Rotation += currentRotation.X;
        y_Rotation +=currentRotation.Y;

        //Update interpolation step
        d_Interpolate -= 0.0005f;
        }
      else
        {
        //Reset interpolation parameters
        d_Interpolate = 1.0f;
        setCameraToMeshOrientation = false;
        }
    }

    //Check orientation condition to replace camera to mesh orientation
    bool checkOrientation( irr::IrrlichtDevice* device,
      bool arrowDown, float dx, float dy, float y_MeshRotation )
    {
      // Check Mesh-Camera angle when Mesh is moving and camera is not moved
      // with cursor
      if( abs(y_MeshRotation-y_Rotation) >= 50.0f &&
        !setCameraToMeshOrientation &&
        arrowDown &&
        !moveCameraCursor )
        {
        setCameraToMeshOrientation = true;
        }

      // Check if cursor has moved
      if( dx > 0 || dy > 0)
        {
        setCameraToMeshOrientation = false;
        moveCameraCursor = true;
        timer = 0.0f;
        time = device->getTimer()->getTime();
        }

      // Reset camera orientation after a certain time if it has been moved
      // with cursor
      if( moveCameraCursor )
        {
        timerTick( device );
        if( timer > 2.0f )
          {
          setCameraToMeshOrientation =true;
          moveCameraCursor = false;
          timer = 0.0f;
          }
        }

      return setCameraToMeshOrientation;
    }


    //Camera parameters
    irr::scene::ICameraSceneNode* cameraNode;
    float FarValue;

    //3rd Person Camera parameters
    // Replace camera to mesh direction parameters
    bool setCameraToMeshOrientation;
    bool moveCameraCursor;
    float d_Interpolate;
    // Rotation parameters
    float x_Rotation;
    float y_Rotation;

    // Time parameters
    float time;
    float timer;


};

#endif // CAMERAMANAGER_HPP
