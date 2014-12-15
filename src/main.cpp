#include "ash.hpp"

int main(int ac, char **av)
{
	if(ac != 2)	return -1;

	Ash ash;
	ash.setScript(av[1]);
	ash.run();
}
