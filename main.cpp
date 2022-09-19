#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

std::string currentDateTime()
{
	std::time_t t = std::time(nullptr);
	std::tm now;
	localtime_s(&now, &t);
	char buffer[128];
	strftime(buffer, sizeof(buffer), "%d-%m-%Y %X", &now);
	return buffer;
}

std::string card_number;  // Номер карты
std::string card_type;  // Тип карты
float amount;  // Сумма на карте

void log(std::fstream& p_f,
		 std::string   p_card_number,
		 float         p_amount,
		 std::string   p_category_name = "NULL",
		 std::string   p_note          = "NULL")
{
	p_f << p_card_number << ' ' << p_amount << ' ' << p_category_name << ' ' << p_note << ' ' << currentDateTime() << "\n";
}

// Операция добавления денег через банкомат (Наличными)
void oper_add_money_by_atm(std::string p_card_number, float p_amount)
{
	if (p_amount <= 0.0f) {
		cout << "Нельзя вносить отрицательные и нулевые суммы!";
		return;
	}
	std::fstream f("Database/cards.txt");
	bool flag = true;
	if (f.is_open())
	{
		// Читаем данные с файла в строки
		while (!f.eof())
		{
			// Считываем данные с файла в строки
			f >> card_number >> card_type >> amount;
			// Если номер карты нашёлся
			if (card_number == p_card_number) {
				float new_amount = amount + p_amount;
				flag = false;
				// TODO: ЗАПИСЬ В ФАЙЛ
			}

			//cout << card_number << " " << card_type << " " << amount << endl;
		}
	}
	if (flag == true) cout << "Карта не найдена в базе данных!";
}


bool oper_get_money_from_atm(std::string p_card_number, float p_amount)
{
	std::fstream f("Database/cards.txt");
	bool flag = true;
	if (f.is_open())
	{
		while (!f.eof())  // Читаем данные с файла в строки
		{
			f >> card_number >> card_type >> amount;  // Считываем данные с файла в строки
			if (card_number == p_card_number) {       // Если номер карты нашёлся
				if (amount < p_amount) {              // Проверяем хватает ли денег на карте
					cout << "Not enough money";
					return false;
				} else {
					float new_amount = amount + p_amount;
					// TODO: ЗАПИСЬ В ФАЙЛ
					return true;
				}
				flag = false;	
			}
		}
	}
	if (flag == true)
	{
		cout << "Card not exist!";
		return false;
	}
	return false;
}


bool oper_buy_goods
(
	std::string p_card_number,   // Номер катры
	float p_amount,              // Сумма покупки
	std::string p_category_name, // Название категори затрат
	std::string p_note           // Информация о покупке
)
{
	// Списываем со счёта указанную сумму
	if (oper_get_money_from_atm(p_card_number, p_amount))
	{
		// Откинуть в историю операций номер карты, сумму, примечание
		fstream f("Database/card_operations.txt", std::ios::out | std::ios::app);
		if (f.is_open())
		{
			log(f, p_card_number, p_amount, p_category_name, p_note);
			/*f << '\n'
				<< p_card_number   << ' '
				<< p_amount        << ' '
				<< p_category_name << ' '
				<< p_note;*/
			f.close();
			return true;
		}
		else
		{
			cout << "No file Database/card_operations.txt";
		}
	}
	return false;
}

void report_spendings()
{
	std::string p_card_number;
	cout << "CARD_NUMBER = "; cin >> p_card_number;
	fstream f("Database/card_operations.txt", std::ios::in);
	if (f.is_open())
	{
		std::string f_card_num;
		float       f_amount;
		std::string f_oper_type;
		std::string f_note;
		std::string f_doc_date;
		std::string f_doc_time;
		float summ = 0.0f;
		while (!f.eof())
		{
			f >> f_card_num >> f_amount >> f_oper_type >> f_note >> f_doc_date >> f_doc_time;
			if (p_card_number == f_card_num)
			{
				if (f_oper_type == "GOODS")
				{
					summ += f_amount;
				}
			}
		}
		cout << "SUMM = " << summ << endl;
	}
}

int main()
{
	//oper_buy_goods("1234123412341234", 50, "MILK", "1pcs_Moloko_1L");
	//oper_buy_goods("1234123412341234", 50, "TOBACCO", "1pcs_???");
	//oper_buy_goods("1234123412341234", 100, "DRINK", "1psc_Water_5L");

	report_spendings();

	return 0;
}


/*#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <exception>

using namespace std;

unsigned int WIN_W = 640;
unsigned int WIN_H = 480;

sf::IntRect NullRect(0, 0, 0, 0);

// Базовый класс - любой объект в системе
class TObject : public sf::Sprite
{
public:
	TObject() { cout << "TObject()" << endl; }
	virtual ~TObject() { cout << "~TObject()" << endl; }
};

// Класс "Юнит"/"Единица"/"Сущность"
class TUnit : public TObject
{
public:
	TUnit() : TObject() { cout << "TUnit()" << endl; }
	~TUnit() { cout << "~TUnit()" << endl; }
	void draw1(sf::RenderWindow  win) { win.draw(*this); }
	void draw2(sf::RenderWindow* win)
	{
		throw "ERROR_NOT_WINDOW";
	}
	void draw3(sf::RenderWindow &win) { win.draw(*this); }
};

// Класс "Кот"
class TUnitCat : public TUnit
{
	sf::Texture t;
	sf::CircleShape circle;
	float radius = 50;
public:
	TUnitCat() : TUnit(), radius(50.0f)
	{
		cout << "TUnitCat()" << endl;
		if (t.loadFromFile("src/images/cat.png"))
		{
			setTexture(t);
			setTextureRect(sf::IntRect(0, 0, 500, 489));
			setScale(0.25f, 0.25f);
			setOrigin(250, 250);
		}
		
		circle.setRadius(radius);
		circle.setOrigin(radius, radius);
		circle.setOutlineColor(sf::Color::Red);
		circle.setOutlineThickness(5);
		circle.setFillColor(sf::Color(255, 0, 0, 10));
		circle.setPosition(getPosition());
	}
	float getRadius() { return radius; }
	void draw(sf::RenderWindow& win)
	{
		win.draw(*this);
		win.draw(circle);
	}

	void move(float offsetX, float offsetY)
	{
		sf::Sprite::move(offsetX, offsetY);
		circle.move(offsetX, offsetY);
		int x = getPosition().x;
		int y = getPosition().y;
		if (x <     0) throw std::exception("invalid_x: x < 0");
		if (x > WIN_W) throw std::exception("invalid_x: x > win_w");
		if (y <     0) throw std::exception("invalid_y: y < 0");
		if (y > WIN_H) throw std::exception("invalid_y: y > win_h");
	}

	void setPosition(float x, float y)
	{
		sf::Sprite::setPosition(x, y);
		circle.setPosition(x, y);
	}

	~TUnitCat() { cout << "~TUnitCat()" << endl; }
};

class AnimationCat
{
	// Текстура для анимации
	sf::Texture texture;
	// Кадры анимации
	//sf::IntRect frames[8];

	sf::IntRect *_frames;
	int   count_frames; // Кол-во кадров в анимации
	int   cur_frame;
	float timer_next_frame_cur;
	float timer_next_frame_max; // Время необходимое для проигрывания одного кадра
public:
	AnimationCat()
		: cur_frame(0)
		, timer_next_frame_cur(0.00f)
		, timer_next_frame_max(0.00f)
		, _frames(nullptr)
	{
		load();
	}
	void load()
	{
		std::ifstream f("src/ani_cat.txt");
		if (f.is_open())
		{
			string CONST_NAME;
			//---------------------------
			string TEXTURE_PATH;
			int    TEXTURE_COUNT_FRAMES;
			float  ANI_FRAME_TIME;
			int    NUM_FRAME, X, Y, W, H;
			//---------------------------
			// До тех пор, пока не конец файла
			while (!f.eof())
			{
				// Загружаем путь к картинке
				f >> CONST_NAME >> TEXTURE_PATH;
				//cout << CONST_NAME << " = " << TEXTURE_PATH << endl;
				texture.loadFromFile(TEXTURE_PATH);

				// Загружаем кроличество кадров в анимации
				f >> CONST_NAME >> TEXTURE_COUNT_FRAMES;
				//cout << CONST_NAME << " = " << TEXTURE_COUNT_FRAMES << endl;
				count_frames = TEXTURE_COUNT_FRAMES;
				
				_frames = new sf::IntRect[count_frames];

				// Загружаем время для одного кадра анимации
				f >> CONST_NAME >> ANI_FRAME_TIME;
				//cout << CONST_NAME << " = " << ANI_FRAME_TIME << endl;
				timer_next_frame_max = ANI_FRAME_TIME;

				f >> CONST_NAME;
				//cout << CONST_NAME << endl;

				if (CONST_NAME == "BEGIN_FRAME_INFO")
				{
					for (int i = 0; i < count_frames; ++i)
					{
						f >> NUM_FRAME >> X >> Y >> W >> H;
						//cout << NUM_FRAME << " " <<  X << " " << Y << " " << W << " " << H << endl;
						_frames[i] = sf::IntRect(X, Y, W, H);
					}
				}

				f >> CONST_NAME;
				//cout << CONST_NAME << endl;

				break;
			}
		}
		else
		{
			cout << "AnimationCat::load() not found file src/ani_cat.txt " << endl;
		}
	}
	sf::Texture& GetTexture()
	{
		return texture;
	}
	// Получить следующий кадр анимации
	sf::IntRect& GetNextFrame(float time)
	{
		if (_frames == nullptr)
		{
			cout << "AnimationCat::GetNextFrame() not found info frames!";
			return NullRect;
		}

		timer_next_frame_cur += time;

		if (timer_next_frame_cur >= timer_next_frame_max)
		{
			// Переходим на следующий кадр
			cur_frame += 1;
			// Если кадры закончились, проигрываем аниацию заного с первого кадра
			if (cur_frame >= count_frames) cur_frame = 0;
			// Сбрасываем время накопления
			timer_next_frame_cur -= timer_next_frame_max;
		}
		//cout << endl << "cur_frame = " << cur_frame << endl;
		// Возвращаем кадр
		return _frames[cur_frame];
	}
};


bool TestCollisionObject(TUnitCat* obj1, TUnitCat* obj2)
{
	if (!obj1 || !obj2) return false;

	sf::Vector2f p1 = obj1->getPosition();
	sf::Vector2f p2 = obj2->getPosition();
	float r1 = obj1->getRadius();
	float r2 = obj2->getRadius();
	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	//float dist = sqrt(dx*dx + dy*dy);
	float dist2 = dx * dx + dy * dy;

	//cout << "dist = " << dist << endl;

	//if (dist < obj1->getRadius() + obj2->getRadius())
	if (dist2 < (r1+r2)*(r1+r2))
		return true;

	return false;
}

void test_pow(const int* v)
{
	int* temp;
	temp = const_cast<int*>(v);
	*temp = 5;
}

class B
{
public:
	virtual void Test() { cout << "B::Test()" << endl; }
};

class D : public B
{
public:
	void Test() { cout << "D::Test()" << endl; }
};


int main()
{
	// Test dynamic_cast
	B* ptr_b = nullptr;
	B obj_b;  // parent
	D* ptr_d;
	D obj_d;  // child
	ptr_d = dynamic_cast<D*>(&obj_d);
	if (ptr_d)
	{
		ptr_d->Test();
	}
	ptr_b = dynamic_cast<B*>(&obj_d);
	if (ptr_b)
	{
		ptr_b->Test();
	}

	ptr_b = dynamic_cast<B*>(&obj_b);
	if (ptr_b)
	{
		ptr_b->Test();
	}
	ptr_d = dynamic_cast<D*>(&obj_b);
	if (ptr_d)
		cout << "dynamic_cast succesful";
	else
		cout << "dynamic_cast not succesful";
	cout << endl;

	return 0;


	// Test const_cast
	int x = 10;
	test_pow(&x);
	cout << "x=" << x;
	return 0;

	setlocale(LC_ALL, "");

	sf::RenderWindow window(sf::VideoMode(WIN_W, WIN_H), "SFML_Test");
	// Переменные для работы со временем
	sf::Clock clock;
	float time     = 0;
	int   fps      = 0;
	float buf_time = 0;

	TUnitCat cat;

	TUnitCat cat2;
	cat2.setPosition(300, 200);

	std:vector<sf::Sprite*> vec;

	sf::Image image;
	image.loadFromFile("cell.png");

	sf::Texture texture;
	texture.loadFromImage(image);

	bool keys[sf::Keyboard::KeyCount];
	for (int i = 0; i < sf::Keyboard::KeyCount; i++)
		keys[i] = false;

	//sf::Texture texture_cat;
	//texture_cat.loadFromFile("cat.png");

	AnimationCat aniCat;
	sf::Sprite sprite;
	sprite.setTexture(aniCat.GetTexture());
	sprite.setTextureRect(sf::IntRect(500, 0, 500, 489));
	sprite.setScale(0.5f, 0.5f);

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			sf::Sprite* sprite = new sf::Sprite(texture);
			sprite->setPosition(i * 24, j * 24);
			vec.push_back(sprite);
		}
	}

	while (window.isOpen())
	{
		time = clock.getElapsedTime().asSeconds();
		clock.restart();
		buf_time += time;
		fps++;
		// Если прошла 1 секунда
		if (buf_time >= 1.0f)
		{
			//cout << "FPS: " << fps << endl;
			// Сбрасываем одну сикунду из буфера накопления времени
			fps = 0;
			buf_time -= 1.0f;
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			// Нажатие клавиши на клавиатуре
			else if (event.type == sf::Event::KeyPressed)
			{
				keys[event.key.code] = true;
			}
			// Отжатие клавиши на клавиаткуре
			else if (event.type == sf::Event::KeyReleased)
			{
				keys[event.key.code] = false;
			}
		}
		float speed = 100.0f;
		float dx    =   0.0f;
		float dy    =   0.0f;
		if (keys[sf::Keyboard::W]) dy = -speed * time; //cat.move(0, -speed * time);
		if (keys[sf::Keyboard::S]) dy = +speed * time; //cat.move(0, +speed * time);
		if (keys[sf::Keyboard::A]) dx = -speed * time; //cat.move(-speed * time, 0);
		if (keys[sf::Keyboard::D]) dx = +speed * time; //cat.move(+speed * time, 0);
		try
		{
			cat.move(dx, dy);
		}
		catch (std::exception e)
		{
			//cout << e.what() << endl;
			cat.move(-dx, -dy);
		}

		for (int i = 0; i < 1000; i++)
		{
			TestCollisionObject(&cat, &cat2);
		}

		/*if (TestCollisionObject(&cat, &cat2))
		{
			cout << "COLLISION!!!" << endl;
		}*/

		/*catch (...)
		{
			cat.move(-dx, -dy);
		}

		cat.setTextureRect(aniCat.GetNextFrame(time));

		window.clear(sf::Color(200, 200, 200));

		cat.draw(window);
		cat2.draw(window);

		window.display();
	}


	return 0;
}*/