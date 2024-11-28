#include "ComplexPlane.h"


int main()
{
	VideoMode::getDesktopMode().width / 2;
	VideoMode::getDesktopMode().height / 2;
	
	RenderWindow window;
	ComplexPlane myPlane(0,0);
	Font myfont;
	Text mytext;
	
	bool update = true;
	while (window.isOpen())
	{
		Event event;


		if (event.type == event.Closed) window.close();
		if (event.type == event.MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				cout << "the left button was pressed" << endl;
				cout << "mouse x: " << event.mouseButton.x << endl;
				cout << "mouse y: " << event.mouseButton.y << endl;

				update == true;
			}
		}
	}
	return 0;
}