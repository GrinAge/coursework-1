#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

#define ColisionConst 0.12 //Макрос, що відповідає за ступінь колізії
#define PI 3.14159265
#define RenderDistanse 900//Макрос, що відповідає за дальність обзору героя

using namespace sf;

namespace Functions {
	Image ColisionMapImage;
	Image RaycastMapImage;
	void ColisionProcesing(CircleShape& circle, int ftime) {
		float x0 = circle.getPosition().x, y0 = circle.getPosition().y;
		//Задаемо точки перевірки колізії
		float UpColisionX[8] = { x0 + 6,x0 + 7,x0 + 8,x0 + 9,x0 + 10,x0 + 11,x0 + 12,x0 + 13 }, UpColisionY = y0;
		float DownColisionX[8] = { x0 + 6,x0 + 7,x0 + 8,x0 + 9,x0 + 10,x0 + 11,x0 + 12,x0 + 13 }, DownColisionY = y0 + 19;
		float LeftColisionY[8] = { y0 + 6,y0 + 7,y0 + 8,y0 + 9,y0 + 10,y0 + 11,y0 + 12,y0 + 13 }, LeftColisionX = x0;
		float RightColisionY[8] = { y0 + 6,y0 + 7,y0 + 8,y0 + 9,y0 + 10,y0 + 11,y0 + 12,y0 + 13 }, RightColisionX = x0 + 19;
		float LUColisionX[4] = { x0 + 1,x0 + 2,x0 + 3,x0 + 4 }, LUColisionY[4] = { y0 + 4, y0 + 3, y0 + 2,y0 + 1 };
		float RUColisionX[4] = { x0 + 18,x0 + 17,x0 + 16,x0 + 15 }, RUColisionY[4] = { y0 + 4, y0 + 3, y0 + 2,y0 + 1 };
		float LDColisionY[4] = { y0 + 18,y0 + 17,y0 + 16,y0 + 15 }, LDColisionX[4] = { x0 + 4, x0 + 3, x0 + 2,x0 + 1 };
		float RDColisionX[4] = { x0 + 18,x0 + 17,x0 + 16,x0 + 15 }, RDColisionY[4] = { y0 + 15, y0 + 16, y0 + 17,y0 + 18 };
		//Перевіряємо точки на колізію
		for (int i = 0; i < 4; i++)
		{
			if (ColisionMapImage.getPixel(LUColisionX[i], LUColisionY[i]) != Color::Black) {
				circle.move(ColisionConst * ftime, ColisionConst * ftime);
				return;
			}
			if (ColisionMapImage.getPixel(RUColisionX[i], RUColisionY[i]) != Color::Black) {
				circle.move((-ColisionConst) * ftime, ColisionConst * ftime);
				return;
			}
			if (ColisionMapImage.getPixel(LDColisionX[i], LDColisionY[i]) != Color::Black) {
				circle.move(ColisionConst * ftime, (-ColisionConst) * ftime);
				return;
			}
			if (ColisionMapImage.getPixel(RDColisionX[i], RDColisionY[i]) != Color::Black) {
				circle.move((-ColisionConst) * ftime, (-ColisionConst) * ftime);
				return;
			}
		}
		for (int i = 0; i < 8; i++)
		{
			if (ColisionMapImage.getPixel(UpColisionX[i], UpColisionY) != Color::Black) {
				circle.move(0, ColisionConst * ftime);
				return;
			}
			if (ColisionMapImage.getPixel(DownColisionX[i], DownColisionY) != Color::Black) {
				circle.move(0, (-ColisionConst) * ftime);
				return;
			}
			if (ColisionMapImage.getPixel(RightColisionX, RightColisionY[i]) != Color::Black) {
				circle.move((-ColisionConst) * ftime, 0);
				return;
			}
			if (ColisionMapImage.getPixel(LeftColisionX, LeftColisionY[i]) != Color::Black) {
				circle.move(ColisionConst * ftime, 0);
				return;
			}
		}
		return;

	}
	struct CoordXY {
		float x, y;
	};
	CoordXY LineProcesor(float dx, float dy, float Alfa) {
		CoordXY res;
		bool f = 1;
		float k;
		k = tan(Alfa * PI / 180);//знаходимо кутовий коефіціент рівняння прямої
		Color TmpColor = Color::Black;
		float x = 0, x0 = 0, y = 0, hegi;
		hegi = sqrt(x * x + y * y);//Довжина прямої
		if (Alfa <= 45 && Alfa != 0)
		{
			while (hegi <= RenderDistanse && TmpColor == Color::Black) {
				x = x + 1 / k;
				y = k * x;
				hegi = sqrt(x * x + y * y);
				while (x0 <= x && f)
				{
					TmpColor = RaycastMapImage.getPixel(dx + x0, dy + y);
					x0++;
					if (TmpColor != Color::Black)f = 0;
				}
				f = 1;

			}
			res.x = x0;
			res.y = y;
		}
		if (Alfa >= 135 && Alfa < 180)
		{
			while (hegi <= RenderDistanse && TmpColor == Color::Black) {
				x = x + 1 / k;
				y = k * x;
				hegi = sqrt(x * x + y * y);
				while (x0 <= -x && f)
				{
					TmpColor = RaycastMapImage.getPixel(dx + (-x0), dy + y);
					x0++;
					if (TmpColor != Color::Black)f = 0;
				}
				f = 1;

			}
			res.x = -x0;
			res.y = y;
		}
		if (Alfa > 45 && Alfa < 135)
		{
			while (hegi <= RenderDistanse && TmpColor == Color::Black) {
				x = x + 1 / k;
				y = k * x;
				hegi = sqrt(x * x + y * y);
				TmpColor = RaycastMapImage.getPixel(dx + x, dy + y);
			}
			res.x = x;
			res.y = y;
		}
		if (Alfa == 0 || Alfa == 360)
		{
			while (x <= RenderDistanse && TmpColor == Color::Black) {
				x ++;
				TmpColor = RaycastMapImage.getPixel(dx + x, dy + y);
			}
			res.x = x;
			res.y = y;
		}
		if (Alfa == 180)
		{
			while (x <= RenderDistanse && TmpColor == Color::Black) {
				x++;
				TmpColor = RaycastMapImage.getPixel(dx - x, dy + y);
			}
			res.x = -x;
			res.y = y;
		}
		if (Alfa > 180 && Alfa < 225)
		{
			Alfa -= 180;
			while (hegi <= RenderDistanse && TmpColor == Color::Black) {
				x = x + 1 / k;
				y = k * x;
				hegi = sqrt(x * x + y * y);
				while (x0 <= x && f)
				{
					TmpColor = RaycastMapImage.getPixel(dx - x0, dy - y);
					x0++;
					if (TmpColor != Color::Black)f = 0;
				}
				f = 1;

			}
			res.x = -x0;
			res.y = -y;
			
		}
		if (Alfa >= 225 && Alfa <=315)
		{
			Alfa = 360 - Alfa;
			while (hegi <= RenderDistanse && TmpColor == Color::Black) {
				x = x + 1 / k;
				y = k * x;
				hegi = sqrt(x * x + y * y);
				TmpColor = RaycastMapImage.getPixel(dx - x, dy - y);
			}
			res.x = -x;
			res.y = - y;
		}
		if (Alfa > 315 && Alfa < 360)
		{
			Alfa = 360 -Alfa;
			k = tan(Alfa * PI / 180);
			while (hegi <= RenderDistanse && TmpColor == Color::Black) {
				x = x + 1 / k;
				y = k * x;
				hegi = sqrt(x * x + y * y);
				while (x0 <= x && f)
				{
					TmpColor = RaycastMapImage.getPixel(dx + x0, dy - y);
					x0++;
					if (TmpColor != Color::Black)f = 0;
				}
				f = 1;

			}
			res.x = x0;
			res.y = -y;

		}
		return res;
	}
	CoordXY LetsGetMove(float Alfa, float v) {
		CoordXY res;
		float k, speed;
		if (Alfa > 360)Alfa -= 360;
		k = tan(Alfa * PI / 180);
		if (Alfa <= 90) {
			res.x = v / sqrt(1 + k);
			res.y = k * res.x;
		}
		if (Alfa > 90 && Alfa<=135) {
			res.x = - (v / sqrt(-(1 + k)));
			res.y = k * res.x;
		}
		if (Alfa > 135 && Alfa<180) {
			res.x = -(v / sqrt(1 + k));
			res.y = k * res.x;
		}
		if (Alfa > 180 && Alfa <= 270) {
			res.x = -(v / sqrt(1 + k));
			res.y = k * res.x;
		}
		if (Alfa > 270 && Alfa <= 315) {
			res.x = v / sqrt(-(1 + k));
			res.y = k * res.x;
		}
		if (Alfa > 315 && Alfa < 360) {
			res.x = v / sqrt(1 + k);
			res.y = k * res.x;
		}
		if (Alfa == 180) {
			res.x = -v;
			res.y = 0;
		}
		if (Alfa == 0 || Alfa == 360) {
			res.x = v ;
			res.y = 0;
		}
		//Корегуємо модуль швидкості
		speed = sqrt(res.x * res.x + res.y * res.y);
			speed = 1/(speed/v);
			res.x *= speed;
			res.y *= speed;
		return res;
	}
	float height(CoordXY xy){
		float h, y, l = sqrt(xy.x * xy.x + xy.y * xy.y);
		y = 410 * (1 / l)*30;
		if (l >= RenderDistanse)y = 0;
		h = 2*y;
		return h;
	}
}
