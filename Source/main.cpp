#ifdef _DEBUG
#include <vld.h>
#endif
#include "Headers/Core.h"

/*
								Proxima b
   .::.
   _::_
 _/____\_        ()
 \      /      <~--~>
  \____/        \__/         <>_
  (____)       (____)      (\)  )                        __/"""\
   |  |         |  |        \__/      WWWWWW            ]___ 0  }
   |__|         |  |       (____)      |  |       __        /   }
  /    \        |__|        |  |       |  |      (  )     /~    }
 (______)      /____\       |__|       |__|       ||      \____/
(________)    (______)     /____\     /____\     /__\     /____\
/________\   (________)   (______)   (______)   (____)   (______)

*/

int main()
{
	Core core;
    core.Init();
	core.Run();
	return 0;
}