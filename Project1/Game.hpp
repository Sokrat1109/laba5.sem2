#pragma once
#include <string>
#include <thread>
#include "Circle.hpp"
#include <math.h>

namespace mt
{

	const float pi = acos(-1);

	class Game
	{
		int m_width;
		int m_height;
		std::string m_capture;
		mt::Circle* m_c;
		int m_n;
		sf::RenderWindow m_window;

	public:
		Game(int width, int height, const std::string& capture)
		{
			m_width = width;
			m_height = height;
			m_capture = capture;
		}

		void Setup(int n)
		{
			m_n = n;
			m_window.create(sf::VideoMode(m_width, m_height), m_capture);

			srand(time(0));

			m_c = new mt::Circle[m_n];
			for (int i = 0; i < m_n; i++)
			{
				int r = rand() % 100 + 1;
				int x = rand() % (1000 - 2 * r) + r;
				int y = rand() % (600 - 2 * r) + r;
				int v = rand() % 50 + 10;
				float alfa = rand() % 7;

				int flag = 1;
				for (int j = 0; j < i; j++) {
					if (sqrt((m_c[j].X() - x) * (m_c[j].X() - x) + (m_c[j].Y() - y) * (m_c[j].Y() - y)) <= (m_c[j].R() + r)) {
						i--;
						flag = 0;
						break;
					}
				}
				if (flag == 1) {
					m_c[i].Setup(x, y, r, v, alfa);
				}
			}
		}

		void TouchBorder(Circle& obj)
		{
			float x = obj.X();
			float y = obj.Y();
			float r = obj.R();

			if (x + r >= m_width || x - r <= 0)
			{
				obj.Alfa(pi - obj.Alfa());
			}

			if (y + r >= m_height || y - r <= 0)
			{
				obj.Alfa(2 * pi - obj.Alfa());
			}

		}

		void TouchCircle(Circle& obj1, Circle& obj2)
		{
			float x1 = obj1.X();
			float y1 = obj1.Y();
			float r1 = obj1.R();
			float v1 = obj1.V();
			float alf1 = obj1.Alfa();

			float x2 = obj2.X();
			float y2 = obj2.Y();
			float r2 = obj2.R();
			float v2 = obj2.V();
			float alf2 = obj2.Alfa();

			if (sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1)) <= (r1 + r2)) {

				/*float dx1 = v1 * cos(alf1);
				float dy1 = v1 * sin(alf1);
				float dx2 = v2 * cos(alf2);
				float dy2 = v2 * sin(alf2);

				float p1_x = x2 - x1;
				float p2_x = y2 - y1;
				float p1_y = -p2_x;
				float p2_y = -p1_x;

				float betta = acos(abs(p1_x) / sqrt(p1_x * p1_x + p2_x * p2_x));
				float delta1 = alf1 - betta;
				float delta2 = alf2 - betta;

				float vx1 = cos(delta1) * v1;
				float vy1 = sin(delta1) * v1;

				float vx2 = cos(delta1) * v2;
				float vy2 = sin(delta1) * v2;

				float tmp = vx1;
				vx1 = vx2;
				vx2 = tmp;

				delta1 = atan(tan(vy1 / vx1));
				delta2 = atan(tan(vy2 / vx2));*/
				obj1.V(v2);
				obj2.V(v1);

				obj1.Alfa(alf2);
				obj2.Alfa(alf1);
			}
		}

		void LifeCycle()
		{
			sf::Clock clock;

			while (m_window.isOpen())
			{
				sf::Event event;
				while (m_window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						m_window.close();
				}


				float dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				for (int i = 0; i < m_n; i++)
					m_c[i].Move(dt);

				for (int i = 0; i < m_n; i++) {
					for (int j = i + 1; j < m_n; j++) {
						TouchCircle(m_c[i], m_c[j]);
					}
				}

				for (int i = 0; i < m_n; i++)
					TouchBorder(m_c[i]);
				

				m_window.clear();
				for (int i = 0; i < m_n; i++)
					m_window.draw(m_c[i].Get());
				m_window.display();
			}
		}
	};

}