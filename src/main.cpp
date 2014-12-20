#include "ash.hpp"
#include <cassert>

int main(int ac, char **av)
{
	assert(ac == 2 || ac == 3);

	Ash ash;
	if(ac == 3)	ash.setTmpFile(av[2]);
	ash.setScript(av[1]);
	ash.run();
}
