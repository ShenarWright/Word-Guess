#include "Application.h"

#ifndef RELEASE
int main()
#else
int WinMain()
#endif // !RELEASE
//int main()
{
	Application app;
	app.run();
}