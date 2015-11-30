#ifndef TERRAINSCENENODE_HPP
#define TERRAINSCENENODE_HPP

#include <irrlicht.h>
#include "PathFinder.hpp"

class TerrainManager
{
  public:
    //! Constructor
    TerrainManager()
    {}

    //Add ITerrainSceneNode to scene manager
    void addTerrainToScene( irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver )
    {
      createTerrainSceneNode( sceneManager, driver );
      handleCollision( sceneManager );
    }

    //Create TerrainSceneNode
    void createTerrainSceneNode( irr::scene::ISceneManager* sceneManager, irr::video::IVideoDriver* driver )
    {
      //Path Finder to load texture
      PathFinder pathFinder;

      //Create node
      terrain = sceneManager->addTerrainSceneNode(
        pathFinder.getFullMediaPath( "terrain-heightmap.bmp" ),//HeightMap
        0,					                                   //Parent node
        -1,					                                   //Node id
        irr::core::vector3df( 0.f, 0.f, 0.f ),		           //Position
        irr::core::vector3df( 0.f, 0.f, 0.f ),		           //Rotation
        irr::core::vector3df( 40.f, 4.4f, 40.f ),	           //Scale
        irr::video::SColor( 255, 255, 255, 255 ),              //VertexColor
        5,					                                   //MaxLOD
        irr::scene::ETPS_17,				                   //PatchSize
        4 );				                                   //SmoothFactor

      //Set Material
      terrain->setMaterialFlag( irr::video::EMF_LIGHTING, false );
      terrain->setMaterialTexture( 0,
        driver->getTexture( pathFinder.getFullMediaPath( "terrain-texture.jpg" ) ) );
      terrain->setMaterialTexture(1,
        driver->getTexture( pathFinder.getFullMediaPath( "detailmap3.jpg" ) ) );
      terrain->setMaterialType( irr::video::EMT_DETAIL_MAP );
      terrain->scaleTexture( 1.0f, 20.0f );
    }

    //Collision Handling
    void handleCollision ( irr::scene::ISceneManager* sceneManager )
    {
      // Create triangle selector to handle collision
      irr::scene::ITriangleSelector* selector
        = sceneManager->createTerrainTriangleSelector( terrain, 0 );
      terrain->setTriangleSelector( selector );
    }

    //Terrain scene node
    irr::scene::ITerrainSceneNode* terrain;

};
#endif // TERRAINSCENENODE_HPP
