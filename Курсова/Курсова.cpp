#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Header.h"


#define PI 3.14159265



using namespace sf;
using namespace Functions;




int main()
{
	RectangleShape slice[640];//Масив прямокутників
	Image MenuImage;
	MenuImage.loadFromFile("images/Menu.png");
	Texture MenuTexture;
	MenuTexture.loadFromImage(MenuImage);
	Sprite MenuSprite;
	MenuSprite.setTexture(MenuTexture);
	MenuSprite.setPosition(0, 0);
	Texture ColisionMapTexture;
	Sprite ColisionMapSprite;
	CircleShape HeroColisonRadius(10);//Тіло героя
	Clock GameClock;
	VertexArray line(Lines, 1280);//Масив ліній
	ColisionMapImage.loadFromFile("images/ColisionMap.png");
	RaycastMapImage.loadFromFile("images/ColisionMap.png");
	ColisionMapTexture.loadFromImage(ColisionMapImage);
	ColisionMapSprite.setTexture(ColisionMapTexture);
	HeroColisonRadius.setFillColor(Color::Green);
	HeroColisonRadius.setOutlineThickness(2);
	HeroColisonRadius.setOutlineColor(Color::White);
	float VievX, VievY, VievRadius;
	HeroColisonRadius.setPosition(640, 360);
	float deg = -45,move =0;//змінна deg відповідає за початок поля зору а move за напрям руху
	float FaidBlack = 255;//змінна відповідає за ефект затемнення
	CoordXY MoveCoord, MoveCoordLR;
	bool KursowaWorks = 1;
	while (KursowaWorks) {
		Event event;
		RenderWindow menu(VideoMode(800, 800), "Menu");//Виконуємо рендр вікна меню
		while (menu.isOpen()) 
		{
			while (menu.pollEvent(event))
			{
				if (event.type == Event::Closed)
					menu.close();
			}
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				menu.close();
				KursowaWorks = 0;
			}
			if (Keyboard::isKeyPressed(Keyboard::Enter)) {
						menu.close();
			}
			menu.clear();
			menu.draw(MenuSprite);
			menu.display();
		}
		if (KursowaWorks) {
			RenderWindow map(VideoMode(1280, 720), "Map");//Виконуємо рендр вікна мапи
			RenderWindow window(VideoMode(1280, 720), "Reder window");//Виконуємо рендр вікна результату роботи
			while (window.isOpen())
			{
				float GameTime = GameClock.getElapsedTime().asMicroseconds();//створюемо  змінну часу та  задаємо їй минулий час
				GameClock.restart();//Перезапускаемо час
				GameTime /= 800;
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();
				}
				MoveCoord = LetsGetMove(move, 0.1);//Знаходимо напрям руху вперед
				MoveCoordLR = LetsGetMove(move + 90, 0.1);//Знаходимо напрям руху вправо
				if (Keyboard::isKeyPressed(Keyboard::Up)) {
					HeroColisonRadius.move(MoveCoord.x * GameTime, MoveCoord.y * GameTime);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down)) {
					HeroColisonRadius.move(-(MoveCoord.x) * GameTime, -(MoveCoord.y) * GameTime);
				}
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					HeroColisonRadius.move(-(MoveCoordLR.x) * GameTime, -(MoveCoordLR.y) * GameTime);
				}
				if (Keyboard::isKeyPressed(Keyboard::Right)) {
					HeroColisonRadius.move(MoveCoordLR.x * GameTime, MoveCoordLR.y * GameTime);
				}
				if (Keyboard::isKeyPressed(Keyboard::Q)) {
					deg -= 0.1 * GameTime;
					move -= 0.1 * GameTime;
				}
				if (Keyboard::isKeyPressed(Keyboard::E)) {
					deg += 0.1 * GameTime;
					move += 0.1 * GameTime;
				}
				// корегуемо значення deg та move якщо вони виходять за діапазон [0;360]
				if (deg < 0)deg = 360 + deg;
				if (deg > 360)deg -= 360;
				if (move < 0)move = 360 + move;
				if (move > 360)move -= 360;
				ColisionProcesing(HeroColisonRadius, GameTime * 0.9);//викликаемо функцію, що відповідае за колізію
				//присвоємо VievX та VievY координати центру героя
				VievX = HeroColisonRadius.getPosition().x + 10;
				VievY = HeroColisonRadius.getPosition().y + 10;
				VievRadius = 0;
				CoordXY dote;
				//цикл, що відповідає за малюваня ліній
				for (int i = 0; i < 1280; i++) {
					line[i].position = Vector2f(VievX, VievY);
					dote = LineProcesor(VievX, VievY, deg);//знаходимо довжину лінії
					slice[i / 2].setSize(Vector2f(2, height(dote)));//знаходимо висоту та координати прямокутника
					slice[i / 2].setPosition(i, 410 - height(dote) / 2);
					FaidBlack = 255 - sqrt(dote.x * dote.x + dote.y * dote.y) / 3;//0бчилюемо затемнення
					if (FaidBlack < 0)FaidBlack = 0;
					slice[i / 2].setFillColor(Color::Color(0, 255, 0, FaidBlack));//Даємо прямокутнику колір, що залежить від затемнення
					FaidBlack = 255;
					deg += 0.0703125;
					if (deg < 0)deg = 360 + deg;
					if (deg > 360)deg -= 360;
					line[++i].position = Vector2f(dote.x + VievX, dote.y + VievY);
					deg += 0.0703125;
					if (deg < 0)deg = 360 + deg;
					if (deg > 360)deg -= 360;
				}
				//Малюємо кадр у вікнах результату та мапи
				map.clear();
				map.draw(ColisionMapSprite);
				map.draw(HeroColisonRadius);
				map.draw(line);
				map.display();
				window.clear();
				for (int i = 0; i < 640; i++)
				{
					window.draw(slice[i]);
				}
				window.display();
				deg -= 90;

			}
		}
	}
	return 0;
}