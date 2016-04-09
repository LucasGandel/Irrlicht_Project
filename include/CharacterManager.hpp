#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP

#include <irrlicht.h>
#include "PathFinder.hpp"
#include "EventReceiver.hpp"
#include "AnimationEndCallBack.hpp"
#include "Math.hpp"
#include <iostream>
class CharacterManager
{
  public:
    //! Constructor
    CharacterManager()
      :y_MeshRotation( 0.0f ),
       animationType( irr::scene::EMAT_STAND ), animationSwitch(false)
    {}

    //Add ITerrainSceneNode to scene manager
    void addCharacterToScene( irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver )
    {
      //Path Finder to load texture
      PathFinder pathFinder;

      //Add mesh
      irr::scene::IAnimatedMesh* meshCharacter = sceneManager->getMesh(
        pathFinder.getFullMediaPath( "faerie.md2" ) );
      characterNode = sceneManager->addAnimatedMeshSceneNode( meshCharacter );

      /*
      To let the mesh look a little bit nicer, we change its material. We
      disable lighting because we do not have a dynamic light in here, and
      the mesh would be totally black otherwise. Then we set the frame loop,
      such that the predefined STAND animation is used. And last, we apply a
      texture to the mesh. Without it the mesh would be drawn using only a
      color.

      Mesh mouvement is set by the 3rd person camera manager
      */
      characterNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
      characterNode->setMaterialTexture( 0,
        driver->getTexture( pathFinder.getFullMediaPath( "faerie2.bmp" ) ) );
      characterNode->setMD2Animation( irr::scene::EMAT_STAND );
      characterNode->setPosition( irr::core::vector3df( 5400, 570, 5200 ) );

      metaTriangleSelector = sceneManager->createMetaTriangleSelector();

    }

    //Collision Handling
    void addSceneNodeCollision( irr::scene::ISceneManager* sceneManager, irr::scene::ISceneNode* sceneNode )
    {
      //WARNING: For futur development. Not Tested Yet.
//       Create triangle selector
     irr::scene::ITriangleSelector* selector = sceneNode->getTriangleSelector();
     if(!selector)
     {
       return;
     }
     metaTriangleSelector->addTriangleSelector(selector);
      //Create collision response animator and attach it to the scene node
      animator = sceneManager->createCollisionResponseAnimator(
        metaTriangleSelector, characterNode,
        irr::core::vector3df( 70, 30, 70 ),//Ellipsoid Radius
        irr::core::vector3df( 0, -10, 0 ),//Gravity per second
        irr::core::vector3df( 0, 10, 0) );  //Ellipsoid Translation (Offset)
      selector->drop();
      characterNode->addAnimator( animator );
      animator->drop();
    }

    //Update 3rd person position and orientation
    void updateTransform( EventReceiver* eventReceiver )
    {
      //Mesh Speed
      float speed = 0.0f;

      //KeyBoard Mesh action
      if( eventReceiver->IsKeyDown( KEY_DOWN) )
        {
        speed = -4.0f;
        }
      if( eventReceiver->IsKeyDown( KEY_UP ) )
        {
        speed = 4.0f;
        }
      if( eventReceiver->IsKeyDown( KEY_RIGHT ) )
        {
        y_MeshRotation += 3.0f;
        speed = 3.0f;
        }
      if( eventReceiver->IsKeyDown( KEY_LEFT ) )
        {
        y_MeshRotation -= 3.0f;
        speed = 3.0f;

        }
      if( animationType == irr::scene::EMAT_ATTACK  )
        {
        speed = 0.0f;
        }


      clampAsAngle( &y_MeshRotation );

      //Calculates mesh position
      irr::core::vector3df meshForward(
        sin( ( characterNode->getRotation().Y + 90.0f ) * irr::core::PI/180.0f ),
        0,
        cos( ( characterNode->getRotation().Y + 90.0f ) * irr::core::PI/180.0f ) );

      meshForward.normalize();

      irr::core::vector3df newPos = meshForward * speed + characterNode->getPosition();

      //Update Mesh
      characterNode->setPosition( newPos );
      characterNode->setRotation(
        irr::core::vector3df( 0, y_MeshRotation + 180.0f, 0 ) );
    }

    //Update 3rd person animation
    void updateAnimation( EventReceiver* eventReceiver )
    {
    //Set standing or running
    if( animationType != irr::scene::EMAT_ATTACK &&
      animationType != irr::scene::EMAT_JUMP  )
      {if( (eventReceiver->IsKeyDown( KEY_UP) ||
       eventReceiver->IsKeyDown( KEY_DOWN) ||
       eventReceiver->IsKeyDown( KEY_LEFT) ||
       eventReceiver->IsKeyDown( KEY_RIGHT) ))
       {
       if( animationType != irr::scene::EMAT_RUN)
         {
         animationType = irr::scene::EMAT_RUN;
         animationSwitch = true;
         }
       else
         {
         animationSwitch = false;
         }
       }
     else
       {
       if(animationType != irr::scene::EMAT_STAND)
         {
         animationType = irr::scene::EMAT_STAND;
         animationSwitch = true;
         }
       else
         {
         animationSwitch = false;
         }
       }
    }

    //handle Jump
    if(animationType != irr::scene::EMAT_JUMP )
      {
      if(eventReceiver->IsKeyDown( KEY_JUMP))
        {
        characterNode->setLoopMode( false );
        animationType = irr::scene::EMAT_JUMP ;
        animationSwitch = true;characterNode->setAnimationEndCallback( &jumpEndReceiver );
        animator->jump(3.0f);
        }
      }
    else
      {
      if( jumpEndReceiver.animationEnd )
        {
        animationSwitch =true;
        animationType = irr::scene::EMAT_STAND;
        jumpEndReceiver.animationEnd=false;
         }
       else
         {
         animationSwitch = false;
         }
       }


      //handle attack
      if( animationType != irr::scene::EMAT_ATTACK )
        {
        if(eventReceiver->IsKeyDown( KEY_ATTACK))
          {
          characterNode->setLoopMode( false );
          animationType = irr::scene::EMAT_ATTACK ;
          animationSwitch = true;characterNode->setAnimationEndCallback( &attackEndReceiver );
          }
        }
      else
        {
        if( attackEndReceiver.animationEnd )
          {
          animationSwitch =true;
          animationType = irr::scene::EMAT_STAND;
          attackEndReceiver.animationEnd=false;
          }
        else
          {
          animationSwitch = false;
          }
        }

      //Switch animation
      if(animationSwitch)
        {
          characterNode->setMD2Animation(animationType);
          animationSwitch =false;
        }
    }

    void UpdateCharacter( EventReceiver* eventReceiver   )
    {
        updateTransform( eventReceiver );
        updateAnimation( eventReceiver );
    }

    //Animated Mesh scene node
     irr::scene::IAnimatedMeshSceneNode* characterNode;

    //Collision handling
     irr::scene::ISceneNodeAnimatorCollisionResponse* animator;
     irr::scene::IMetaTriangleSelector * metaTriangleSelector;

     //3rd Person parameters
     // Rotation parameter
     float y_MeshRotation;
     // Animation
     irr::scene::EMD2_ANIMATION_TYPE animationType;
     AnimationEndCallBack attackEndReceiver;
     AnimationEndCallBack jumpEndReceiver;
     bool animationSwitch;


};

#endif // CHARACTERMANAGER_HPP
