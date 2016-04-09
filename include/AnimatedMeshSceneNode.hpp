#ifndef ANIMATEDMESHSCENENODE_HPP
#define ANIMATEDMESHSCENENODE_HPP

#include <irrlicht.h>

class AnimatedMeshSceneNode : public irr::scene::IAnimatedMeshSceneNode
{
  public:

  //Animation parameters
  irr::scene::EMD2_ANIMATION_TYPE animationType;
  bool run;
  bool attack;
}

#endif // ANIMATEDMESHSCENENODE_HPP
