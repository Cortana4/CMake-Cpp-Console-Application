#include <iostream>
#include <fmt/format.h>

int main(int argC, char* argV[])
{
	std::cout << fmt::format("Hello from {}!\n", "fmt");
	return 0;
}
