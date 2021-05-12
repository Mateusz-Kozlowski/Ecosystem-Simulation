#include "stdafx.h"
#include "App.h"

int main()
{
	srand(static_cast<unsigned>(time(nullptr)));

	App app;
	app.run();
}
