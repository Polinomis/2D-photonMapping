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

sf::Vector2i rotate(sf::Vector2i v, float angle)
{
	float rad = angle/180*PI;
	
	sf::Vector2i vrot;
	vrot.x = cos(rad) * v.x - sin(rad) * v.y;
	vrot.y = sin(rad) * v.x + cos(rad) * v.y;

	return vrot;
}

sf::Vector2f rotate(sf::Vector2f v, float angle)
{
	float rad = angle/180*PI;
	
	sf::Vector2f vrot;
	vrot.x = cos(rad) * v.x - sin(rad) * v.y;
	vrot.y = sin(rad) * v.x + cos(rad) * v.y;

	return vrot;
}

sf::Vector2f getNormal(sf::Vector2f v, sf::Vector2i point, sf::Image &opticalImage)
{
	sf::Color cpoint = opticalImage.getPixel(point.x,point.y);
	printf("vecteur: %f , %f\n",v.x,v.y);
	printf("point: %i , %i\n", point.x, point.y);	
	printf("color point : %i,%i,%i\n",cpoint.r,cpoint.g,cpoint.a);
	
	sf::Vector2f normal(0.,0.);
	// normalise
	
	float norm = sqrt(v.x*v.x+v.y*v.y); 
	
	printf("\nnorm: %f\n ", norm);
	v /= norm;	
	printf("vecteur: %f , %f\n",v.x,v.y);
	// mobile vector and point
	
	sf::Vector2i itA, itB;
	//float angle = (v.y>0) ? acos(v.x):-acos(v.x);

	sf::Vector2i Apoint;
	sf::Vector2i Av;
	Av.x = (v.x == 0) ? 0 : v.x/abs(v.x);
	Av.y = (v.y == 0) ? 0 : v.y/abs(v.y);
	printf("Av: %i , %i | ", Av.x, Av.y);	
	Apoint.x = point.x-Av.x;
	Apoint.y = point.y-Av.y;
	sf::Color cApoint = opticalImage.getPixel(Apoint.x,Apoint.y);

	sf::Vector2i Bpoint = Apoint;
	sf::Color cBpoint = cApoint; 

	printf("Apoint: %i , %i | ", Apoint.x, Apoint.y);	
	printf("Color Apoint : %i,%i,%i\n",cApoint.r,cApoint.g,cApoint.a);
	printf("Bpoint: %i , %i | ", Bpoint.x, Bpoint.y);	
	printf("Color Apoint : %i,%i,%i\n",cBpoint.r,cBpoint.g,cBpoint.a);

	if(cApoint == cpoint) return normal; // if same surface
	// mobile iteration
	
	int nbIteration(14);
	int nbPixel = nbIteration;
	int move = 0;
	bool findA = false;
	bool findB = false;
	sf::Vector2i ApointNormal[nbIteration];
	sf::Vector2i BpointNormal[nbIteration];

	// 1 ere iteration //////////////////////////////////////////////////

	
	printf("--------------1-------------------\n");
	if(Apoint.x != point.x && Apoint.y != point.y) // if diagonal
	{
		if(Av == sf::Vector2i(1,-1)) // v going down right 
		{
			printf("DOWN RIGHT\n");
			itA = DOWN;
			itB = RIGHT;
		}
		else if(Av == sf::Vector2i(-1,-1)) // down left 
		{
			printf("DOWN LEFT\n");
			itA = LEFT;
			itB = DOWN;
		}
		else if(Av == sf::Vector2i(-1,1)) // up right 
		{
			printf("UP RIGHT\n");
			itA = RIGHT;
			itB = UP;
		}
		else // up left
		{
			printf("UP LEFT\n");
			itA = UP;
			itB = LEFT;
		}	
			
		opticalImage.setPixel(Apoint.x,Apoint.y,sf::Color::Green);
		
		// iteration 1 itA
		
		printf("------%i------\n", move);	
		printf("itA : %i, %i\n", itA.x, itA.y);
		printf("Apoint : %i, %i\n", Apoint.x, Apoint.y);

		while(move < 4 && !findA)
		{
			move++;
			Apoint += itA;	
			if(opticalImage.getPixel(Apoint.x,Apoint.y) == cpoint) // Surface
			{
				opticalImage.setPixel(Apoint.x,Apoint.y,sf::Color::Yellow);
				findA = true;
				itA = -itA;
				ApointNormal[0] = Apoint;
			}
			else
			{
				if(move % 2 == 0) itA = rotate(itA, 90);
			}

			printf("------%i------\n", move);	
			printf("Apoint : %i, %i\n", Apoint.x, Apoint.y);
			printf("itA : %i, %i\n", itA.x, itA.y);
		}
		
		// iteration 1 itB
		
		move = 0;
		findB = false;
		printf("------%iB-----\n", move);	
		printf("itA : %i, %i\n", itA.x, itA.y);
		printf("ApointA : %i, %i\n", Apoint.x, Apoint.y);

		while(move < 4 && !findB)
		{
			move++;
			Bpoint += itB;	
			if(opticalImage.getPixel(Bpoint.x,Bpoint.y) == cpoint) // Surface
			{
				opticalImage.setPixel(Bpoint.x,Bpoint.y,sf::Color::Yellow);
				findB = true;
				itB = -itB;
				BpointNormal[0] = Bpoint;
			}
			else
			{
				if(move % 2 == 0) itB = rotate(itB, -90);
			}

			printf("------%i------\n", move);	
			printf("BpointB : %i, %i\n", Bpoint.x, Bpoint.y);
			printf("itB : %i, %i\n", itA.x, itA.y);
		}
	}
	else
	{
		if(Av == UP) // v going up
		{
			printf("UP\n");
			itA = RIGHT;
			itB = LEFT;
		}
		else if(Av == DOWN) // down 
		{
			printf("DOWN\n");
			itA = LEFT;
			itB = RIGHT;
		}
		else if(Av == LEFT) // left 
		{
			printf("LEFT\n");
			itA = UP;
			itB = DOWN;
		}
		else // right
		{
			printf("RIGHT\n");
			itA = DOWN;
			itB = UP;
		}	
			
		opticalImage.setPixel(Apoint.x,Apoint.y,sf::Color::Green);
		
		// iteration 1 itA
		
		printf("------%iA-----\n", move);	
		printf("itA : %i, %i\n", itA.x, itA.y);
		printf("Apoint : %i, %i\n", Apoint.x, Apoint.y);

		while(move < 4 && !findA)
		{
			move++;
			Apoint += itA;	
			if(opticalImage.getPixel(Apoint.x,Apoint.y) == cpoint) // Surface
			{
				opticalImage.setPixel(Apoint.x,Apoint.y,sf::Color::Yellow);
				findA = true;
				itA = -itA;
				ApointNormal[0] = Apoint;
			}
			else
			{
				opticalImage.setPixel(Apoint.x,Apoint.y,sf::Color::Red);
				if(move % 1 == 0) itA = rotate(itA, 90);
			}

			printf("------%i------\n", move);	
			printf("ApointA : %i, %i\n", Apoint.x, Apoint.y);
			printf("itA : %i, %i\n", itA.x, itA.y);
		}
	
		// iteration 1 itB
	   
		move = 0;
		printf("\n------%iB-----\n", move);	
		printf("itB : %i, %i\n", itB.x, itB.y);
		printf("BpointB : %i, %i\n", Bpoint.x, Bpoint.y);

		while(move < 4 && !findB)
		{
			move++;
			Bpoint += itB;	
			if(opticalImage.getPixel(Bpoint.x,Bpoint.y) == cpoint) // Surface
			{
				opticalImage.setPixel(Bpoint.x,Bpoint.y,sf::Color::Yellow);
				findB = true;
				itB = -itB;
				BpointNormal[0] = Bpoint;
			}
			else
			{
				opticalImage.setPixel(Bpoint.x,Bpoint.y,sf::Color::Blue);
				if(move % 1 == 0) itB = rotate(itB, -90);
			}

			printf("------%i------\n", move);	
			printf("Bpoint: %i, %i\n", Bpoint.x, Bpoint.y);
			printf("itB : %i, %i\n", itB.x, itB.y);
		}
	}
	// 2 -> nbIteration ///////////////////////////////////////////////////
	
	int iteration = 1;

	// iteration A

	while(iteration < nbIteration && findA)
	{
		printf("----------------%iA---------------\n",iteration);
		findA = false;
		move = 0;
		while(move < 5 && !findA)
		{
			move++;
			// reverse C
			if(move == 1)Apoint+= itA;
			else if(move == 2)Apoint += rotate(itA,90);
			else if(move == 5)Apoint += rotate(itA,-90);
			else Apoint += -itA;

			if(opticalImage.getPixel(Apoint.x,Apoint.y) == cpoint) // surface
			{
				opticalImage.setPixel(Apoint.x,Apoint.y,sf::Color::Yellow);
				findA = true;
				ApointNormal[iteration] = Apoint;
			}
			printf("------%i------\n", move);	
			printf("ApointA : %i, %i\n", Apoint.x, Apoint.y);
			printf("itA : %i, %i\n", itA.x, itA.y);
		}
		if(!findA) // if not pixel find
		{
			nbPixel = iteration;
		}
		iteration++;
	}
	
	
	iteration = 1;

	// iteration B

	while(iteration < nbPixel && findA)
	{
		printf("----------------%iB---------------\n",iteration);
		findB = false;
		move = 0;
		while(move < 5 && !findB)
		{
			move++;
			// reverse C
			if(move == 1)Bpoint+= itB;
			else if(move == 2)Bpoint += rotate(itB,-90);
			else if(move == 5)Bpoint += rotate(itB,90);
			else Bpoint += -itB;

			if(opticalImage.getPixel(Bpoint.x,Bpoint.y) == cpoint) // surface
			{
				opticalImage.setPixel(Bpoint.x,Bpoint.y,sf::Color::Yellow);
				findB = true;
				BpointNormal[iteration] = Bpoint;
			}
			printf("------%i------\n", move);	
			printf("Bpoint : %i, %i\n", Bpoint.x, Bpoint.y);
			printf("itB : %i, %i\n", itB.x, itB.y);
		}
		iteration++;
	}
	
	for(int i=0; i<nbPixel;i++)
	{
		int dx = ApointNormal[i].x - BpointNormal[i].x;	
		int dy = ApointNormal[i].y - BpointNormal[i].y;

		printf("dx : %i, dy : %i\n",dx,dy);
		normal.x += dx; 
		normal.y += dy; 
	}
	normal.x /= nbPixel;
	normal.y /= nbPixel;
	return normal;
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

	sf::CircleShape cir1(10);
	cir1.setPosition(windowSize.x/4,windowSize.y/2);
	cir1.setFillColor(sf::Color(0,255,255));


	// creation of the optic image
	sf::RenderTexture opticRenderTexture;
	opticRenderTexture.create(windowSize.x,windowSize.y);

	opticRenderTexture.draw(rec1);
	opticRenderTexture.draw(cir1);
	opticRenderTexture.display();

	sf::Image opticImage = opticRenderTexture.getTexture().copyToImage();
		
	// normal
	sf::Vector2f vec(-5,0);
	sf::Vector2i point = (sf::Vector2i) /*sf::Vector2i(windowSize.x/2,windowSize.y/2);*/ (cir1.getPosition() + cir1.getPoint(11));	
		
	sf::RectangleShape recPoint(sf::Vector2f(3,3));
	recPoint.setFillColor(sf::Color(0,0,255,128));
	recPoint.setPosition((sf::Vector2f)point);

	float viewRatio = 0.1f;
	sf::View view((sf::Vector2f)point,sf::Vector2f(1600*viewRatio, 900*viewRatio));
	window.setView(view);		
	sf::Vector2f normal = getNormal(vec,point,opticImage);
	printf("normal: %f , %f", normal.x, normal.y);
	sf::VertexArray normalVertice(sf::Lines,2);
	normalVertice[0].position = (sf::Vector2f)point-normal;
	normalVertice[1].position = (sf::Vector2f)point+normal;
	// main loop
	
	sf::Sprite opticSprite;
	sf::Texture opticTexture;
	opticTexture.loadFromImage(opticImage);
	opticSprite.setTexture(opticTexture);
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
		window.draw(opticSprite);
		//window.draw(rec1);
		//window.draw(cir1);
		window.draw(recPoint);
		window.draw(normalVertice);
        window.display();
    }

    return 0;
}
