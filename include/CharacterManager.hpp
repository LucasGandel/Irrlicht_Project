#ifndef CHARACTERMANAGER_HPP
#define CHARACTERMANAGER_HPP

#include <irrlicht.h>
#include "PathFinder.hpp"
#include "EventReceiver.hpp"

class CharacterManager
{
  public:
    //! Constructor
    CharacterManager()
      : animationType( irr::scene::EMAT_STAND ), running( false )
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
      characterNode->setMD2Animation( animationType );
      characterNode->setPosition( irr::core::vector3df( 5400, 510, 5200 ) );

    }

    //Collision Handling
    void addSceneNodeCollision( irr::scene::ISceneManager* sceneManager, irr::scene::ISceneNode* sceneNode )
    {
      //WARNING: For futur development. Not Tested Yet.
      // Create triangle selector
      irr::scene::ITriangleSelector* selector =
        sceneManager->createOctreeTriangleSelector( characterNode->getMesh(), sceneNode, 128 );
      characterNode->setTriangleSelector( selector );

      //Create collision response animator and attach it to the scene node
      irr::scene::ISceneNodeAnimator* animator = sceneManager->createCollisionResponseAnimator(
        selector, sceneNode,
        irr::core::vector3df( 60, 100, 60 ),//Ellipsoid Radius
        irr::core::vector3df( 0, 0, 0 ),//Gravity per second
        irr::core::vector3df( 0, 0, 0) );  //Ellipsoid Translation (Offset)
      selector->drop();
      sceneNode->addAnimator( animator );
      animator->drop();
    }

    void updateAnimationType( EventReceiver* eventReceiver )
    {

      if( ( eventReceiver->IsKeyDown( irr::KEY_DOWN ) ||
            eventReceiver->IsKeyDown( irr::KEY_UP ) ) )
        {
        if( !running )
          {
          animationType = irr::scene::EMAT_RUN;
          characterNode->setMD2Animation( animationType );
          running = true;
        }
        }
      else
      if( running )
        {
         animationType = irr::scene::EMAT_STAND;
         characterNode->setMD2Animation( animationType );
         running = false;
        }
    }

    //Animated Mesh scene node
     irr::scene::IAnimatedMeshSceneNode* characterNode;

    //Animation Type
     irr::scene::EMD2_ANIMATION_TYPE animationType;
     bool running;


};

#endif // CHARACTERMANAGER_HPP
