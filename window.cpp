
#include "window.h"

Window::Window()
{
	windowContext.depthBits = 24;
	windowSize = sf::Vector2i(840,480);
	window.create(sf::VideoMode(windowSize.x,windowSize.y),"Voxel 2.0",sf::Style::Default,windowContext);
	world = new World();
}

Window::~Window()
{
	delete world;
	this->close();
}

void Window::run()
{
	float delta = 0.0f;
	sf::Clock clock;
	camera.setPosition(sf::Vector3f(world->getMap().getSize().x/2,200,world->getMap().getSize().z/2));
	while (window.isOpen())
	{
		window.setFramerateLimit(128);
		window.setView(sf::View(sf::FloatRect(0,0,window.getSize().x,window.getSize().y)));
		delta = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		delta /= 128;
		while (window.pollEvent(windowEvent))
		{
			switch (windowEvent.type)
			{
				case sf::Event::Closed:
					this->close();
					break;
				case sf::Event::Resized:
					glViewport(0,0,window.getSize().x,window.getSize().y);
					break;
				default:
					break;
			}
		}
		window.clear();
		camera.setRenderDistance(world->getRenderDistance());
		camera.run(world,&window,delta);
		world->render(camera.getPosition(),camera.getRaycaster());
		camera.draw(world,&window);
		window.display();
	}
}

void Window::close()
{
	window.close();
	exit(EXIT_SUCCESS);
}
