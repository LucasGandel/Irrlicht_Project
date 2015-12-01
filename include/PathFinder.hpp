#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include "Project_Config.h"
#include "irrString.h"

class PathFinder
{
  public:

    PathFinder()
    {}

    // Return full path to the file stored in media file
    irr::core::stringc getFullMediaPath(const char* filename)
    {
      irr::core::stringc media_DIR = MEDIA_DIR;
      return media_DIR + filename;
    }

  private:
};

#endif // PATHFINDER_HPP

