#include "ash.hpp"
#include <cassert>

int main(int ac, char **av)
{
	assert(ac == 2);

	Ash ash;
	ash.setScript(av[1]);
	ash.run();
}
