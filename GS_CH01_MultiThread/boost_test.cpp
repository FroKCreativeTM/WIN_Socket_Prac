#include <iostream>
#include <boost/shared_ptr.hpp>

int main()
{
	boost::shared_ptr<int> spInt;
	spInt.reset(new int(1));
	std::cout << *spInt << std::endl;
	return 0;
}