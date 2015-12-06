#ifndef CAMERAMANAGER_HPP
#define CAMERAMANAGER_HPP

#include <irrlicht.h>
#include "PathFinder.hpp"
#include "EventReceiver.hpp"
#include <iostream>
class CameraManager
{
  public:
    //! Constructor
    CameraManager()
      :FarValue( 42000.0f ),thirdPersonCamera( false ),
       x_Rotation( 0.0f ), y_Rotation( 0.0f ), y_MeshRotation( 0.0f )
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

      thirdPersonCamera = true;
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
      cameraNode = sceneManager->addCameraSceneNode( 0,          //Parent Node
        irr::core::vector3df( 40.0f, 10.0f, 15.0f ),//Camera position
        irr::core::vector3df( 0.0f, 9.0f, 15.0f ) );//Camera orientation
      cameraNode->setFarValue( FarValue );
    }

    //Move 3rd person camera and its character
    void move3rdPersonCameraControl( irr::IrrlichtDevice* device,
      irr::scene::IAnimatedMeshSceneNode* characterNode, EventReceiver* eventReceiver )
    {
      irr::core::position2d<irr::f32> cursorPos =
        device->getCursorControl()->getRelativePosition();

      float cursorSensibility = 256.0f;
      float dx = ( cursorPos.X - 0.5 ) * cursorSensibility;
      float dy = ( cursorPos.Y - 0.5 ) * cursorSensibility;

      y_Rotation += dx;
      x_Rotation += dy;
      if( x_Rotation < -90 )
      {
        x_Rotation = -90;
      }
      else
      if( x_Rotation > 10 )
      {
        x_Rotation = 10;
      }

      //Reset Cursor position
      device->getCursorControl()->setPosition( 0.5f, 0.5f );

      //Set 3rd person mouvement
      irr::core::vector3df facing(
        sin( ( characterNode->getRotation().Y + 90.0f ) * irr::core::PI/180.0f ),
        0,
        cos( ( characterNode->getRotation().Y + 90.0f ) * irr::core::PI/180.0f )
      );

      float speed = 0.0f;
      if( eventReceiver->IsKeyDown( irr::KEY_DOWN) )
      {
      speed = -4.0f;
      }
      if( eventReceiver->IsKeyDown( irr::KEY_UP) )
      {
      speed = 4.0f;
      }
      if( eventReceiver->IsKeyDown( irr::KEY_LEFT ))
      {
      y_MeshRotation -= 3.0f;
      }
      if( eventReceiver->IsKeyDown( irr::KEY_RIGHT ))
      {
      y_MeshRotation += 3.0f;
      }

      facing.normalize();
      irr::core::vector3df newPos = ( facing * speed ) + characterNode->getPosition();
      characterNode->setPosition( newPos );
      characterNode->setRotation( irr::core::vector3df( 0, y_MeshRotation, 0 ) );

      //Set Camera mouvement
      float Zoom = 70.0f;
      irr::core::vector3df playerPos = characterNode->getPosition();
      irr::core::vector3df cameraPos = characterNode->getPosition() + irr::core::vector3df( Zoom, Zoom, 0 );

      cameraPos.rotateXYBy( x_Rotation, playerPos );
      cameraPos.rotateXZBy( -y_Rotation, playerPos );
      cameraNode->setPosition( cameraPos );
      cameraNode->setTarget( irr::core::vector3df( playerPos.X, playerPos.Y, playerPos.Z ) );
    }


    //Camera parameters
    irr::scene::ICameraSceneNode* cameraNode;

    float FarValue;

    //3rd Person Camera parameters
    bool thirdPersonCamera;
    float x_Rotation;
    float y_Rotation;
    float y_MeshRotation;


};

#endif // CAMERAMANAGER_HPP
