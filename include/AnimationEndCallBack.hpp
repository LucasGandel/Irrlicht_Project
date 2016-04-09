#ifndef ANIMATIONENDCALLBACK_HPP
#define ANIMATIONENDCALLBACK_HPP

#include <irrlicht.h>

class AnimationEndCallBack : public irr::scene::IAnimationEndCallBack
{
  public:

    AnimationEndCallBack()
        :animationEnd(false)

    {}

    // This is used to check whether a key is being held down
    virtual void OnAnimationEnd( irr::scene::IAnimatedMeshSceneNode *node )
    {
      animationEnd=true;
      node->setLoopMode( true );
    }

    bool animationEnd;
};


#endif // ANIMATIONENDCALLBACK_HPP
