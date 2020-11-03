#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <cstdio>

#define PI 3.14159265359 
// axis vector

sf::Vector2i
	LEFT(-1,0),
	RIGHT(1,0),
	UP(0,1),
	DOWN(0,-1);


sf::Vector2f getNormal(sf::Vector2f v, sf::Vector2i point, sf::Image opticalImage)
{
	printf("vecteur: %f , %f\n",v.x,v.y);
	printf("point: %i , %i\n", point.x, point.y);	

	// normalise
	
	float norm = sqrt(v.x*v.x+v.y*v.y); 
	
	printf("norm: %f\n ", norm);


	v /= norm;	
	// mobile vector 
	
	sf::Vector2i itA, itB;
	float angle = (v.y>0) ? acos(v.x):-acos(v.x);

	printf("angle: %f\n",angle*180/PI);
	if(angle >= -PI/4 && angle <= PI/4) // v going right
	{
		itA = LEFT;
		itB = LEFT;
	}
	else if(angle > PI/4 && angle < 3*PI/4) // Up 
	{
		itA = DOWN;
		itB = DOWN;
	}
	else if(angle > -PI/4 && angle < -3*PI/4) // down 
	{
		itA = UP;
		itB = UP;
	}
	else
	{
		itA = RIGHT;
		itB = RIGHT;
	}

	printf("\nvecteur Inverse : %i;%i\n",itA.x,itA.y);

	sf::Color cpoint = opticalImage.getPixel(point.x,point.y);
	sf::Vector2i Apoint;
	Apoint.x = point.x-v.x;
	Apoint.y = point.y-v.y;
	sf::Color cApoint = opticalImage.getPixel(Apoint.x,Apoint.y);

	printf("Apoint: %i , %i\n", Apoint.x, Apoint.y);	

	printf("color point : %i,%i,%i\n",cpoint.r,cpoint.g,cpoint.a);
	printf("color Apoint : %i,%i,%i\n",cApoint.r,cApoint.g,cApoint.a);
	// mobile iteration
	
	// 1 ere iteration : tout le tour
	
	// iteration suivant : L a l'envers
	int nbIteration(5);
	
	for(int i(0);i<nbIteration; i++)
	{

	}
	
}

int main()
{
	srand(time(0));

	// Window initialisation
	
	float ratio = 0.8f;
    sf::RenderWindow window(sf::VideoMode(1600*ratio, 900*ratio), "SFML works!");
	sf::Vector2u windowSize = window.getSize();
	
	// entities initialisation
	
	sf::RectangleShape rec1(sf::Vector2f(windowSize.x/2,windowSize.y));
	rec1.setPosition(windowSize.x/2,0);
	rec1.setFillColor(sf::Color(255,255,255));

	sf::CircleShape cir1(100);
	cir1.setPosition(windowSize.x/4,windowSize.y/2);
	cir1.setFillColor(sf::Color(0,255,255));


	// creation of the optic image
	sf::RenderTexture opticTexture;
	opticTexture.create(windowSize.x,windowSize.y);

	opticTexture.draw(rec1);
	opticTexture.draw(cir1);
	opticTexture.display();

	sf::Image opticImage = opticTexture.getTexture().copyToImage();
		
	// normal
	sf::Vector2f vec(5,5);
	sf::Vector2i point = (sf::Vector2<int>) (cir1.getPosition() + cir1.getPoint(26));	
	
	sf::RectangleShape recPoint(sf::Vector2f(10,10));
	recPoint.setPosition((sf::Vector2f)point);
		
	sf::Vector2f a = getNormal(vec,point,opticImage);
	// main loop
	
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed	||
					sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        window.clear();
		window.draw(rec1);
		window.draw(cir1);
		window.draw(recPoint);
        window.display();
    }

    return 0;
}
