#include <iostream>
#include <windows.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <conio.h>
#include <ctype.h>
#include <cstdlib>
#define N 11

using namespace std;

struct profile 
{
	string surname; //фамилия
	string name; //имя
	string mid_name; //отчество
	int year; //год рождения
	string gender; //пол
	string city; //место рождения
	string work; //место работы
	string prof; //профессия
	/* указатели на предыдущий (pointer_previous)
	и следующий (pointer_next) элементы
	Заранее обнулены, чтобы не было необходимости постоянно их корректировать */
	profile* pPrev = NULL; 
	profile* pNext = NULL;
};

profile* head, * tail;
profile* pred, * sled;

int cap = 0;

int i_check();             //ф-ия проверки стандартного потока ввода
void enter();              //ф-ия добавления элемента(ов) и ввода (case: 1)
int choice();              //ф-ия выбора
void list();               //ф-ия управления указателями (вызывается в enter())
int f_enter(int count);    //ф-ия ввода их файла (case: 1)
void demand();             //ф-ия спраки, содержит требования к файлам
void del_cause_err(int num); //ф-ия удаления из-за ошибки
int login_check();         //проверка наличия пользователя
void rem_el();			   //ф-ия удаления элемента "remove element" (case: 2)
void display();		       //ф-ия вывода (case: 3)
void young();			   //ф-ия поиска младшего (case: 4)
void searchProf();		   //ф-ия поиска по профессии (case: 5)
void city_work(int key);   //ф-ия поиска по месту рождения/работы, в зависимости от кода (case: 6,7)
void sort(int code);	   //ф-ия сортировки, тип зависит от кода (case: 8)
void first_swap(profile* first, profile* follow, profile* center); //ф-ия замены первых
void last_swap(profile* early, profile* last, profile* center); //ф-ия замены последних
void pointSort(profile* func_buf); //ф-ия сортировки указателей
void workAmount();		   //ф-ия подсчета кол-ва работников (case: 9)
void file_save();		   //ф-ия сохранения в файл (case: 10)
int del();				   //ф-ия очистки памяти (case: 0)
int to_digit(string func_str); //ф-ия проверки типов данных

int main()
{
	SetConsoleCP(1251); //установка кодировки потоков ввода/вывода
	SetConsoleOutputCP(1251);
	system("color F0"); //установка цвета консоли и текста

	int main_key;

	do //меню программы
	{
	    cout << "Выберите действие:\n1. Добавление элемента;\n2. Удаление элемента;\n3. Вывод таблицы;\n4. Поиск младшего пользователя;\n5. Поиск людей по профессии;\n6. Поиск людей по месту рождения\n7. Поиск по месту работы;"
		    << "\n8. Сортировка по году рождения;\n9. Кол-во людей на предприятии;\n10. Сохранить данные в файл;\n0. Выход" << endl;
		main_key = i_check();
		switch (main_key)
		{
		case 1: enter(); break;
		case 2: if (cap == 0) cout << "Вы ещё ничего не ввели!" << endl;
			else rem_el(); break;
		case 3: if (cap == 0) cout << "Вы ещё ничего не ввели!" << endl;
			else display(); break;
		case 4: if (cap == 0) cout << "Вы еще ничего не ввели!" << endl;
			  else young();  break;
		case 5: if (cap == 0) cout << "Вы еще ничего не ввели!" << endl;
			  else searchProf(); break;
		case 6: if (cap == 0) cout << "Вы еще ничего не ввели!" << endl;
			  else city_work(main_key); break;
		case 7: if (cap == 0) cout << "Вы еще ничего не ввели!" << endl;
			  else city_work(main_key); break;
		case 8: if (cap == 0) cout << "Вы еще ничего не ввели!" << endl;
			  else 
		      {
			      cout << "Введите тип сортировки:\n1. Возрастающая (сначала - старше);\n2. Убывающая (сначала - младше)." << endl;
				  sort(choice());
		      } break;
		case 9: if (cap == 0) cout << "Вы еще ничего не ввели!" << endl;
			  else workAmount(); break;
		case 10: if (cap == 0) cout << "Вы не вводили данные." << endl;
			   else file_save(); break;
		case 0: if (cap > 0) main_key = del(); break;
		case -1: break;
		default: cout << "Неправильный код. Повторите попытку:" << endl;
	    }
	} while (main_key != 0);
}

void enter()
{
	int count; //кол-во вводимых данных
	
retry:

	cout << "Вы собираетесь вводить данные?:\n1. Да;\n2. Нет, вернуться." << endl;
	if (choice() == 2) return;

	do //ввод кол-ва записей
	{
		cout << "Сколько записей вы хотите добавить?" << endl;
		count = i_check();
		if (count <= 0 && count != -1) cout << "Вы ввели неправильное кол-во. ";
	} while (count <= 0);

	cout << "Каким образом вы желаете ввeсти данные?\n1. Вручную;\n2. Из файла." << endl; //выбор способа ввода
	
	if (choice() == 1) //ввод с клавиатуры
	{
		for (int i = 0; i < count; i++)
		{
			list();

			cout << "--------------" << cap << " пользователь-----------------" << endl;
			cout << "Введите фамилию пользователя:" << endl;
			cin >> tail->surname;
			cout << "Введите имя пользователя:" << endl;
			cin >> tail->name;
			cout << "Введите отчество пользователя:" << endl;
			cin >> tail->mid_name;
			if (login_check() == 2)
			{
				del_cause_err(i);
				goto skip_login;
			}

			do
			{
				cout << "Введите год рождения пользователя:" << endl;
				tail->year = i_check();
				if ((tail->year < 1955 || tail->year > 2006) && tail->year != -1) cout << "В таком возрасте люди официально не работают!" << endl;
			} while (tail->year < 1955 || tail->year > 2006);

			cout << "Введите пол пользователя:\n1. Мужской;\n2. Женский." << endl;
			if (choice() == 1) tail->gender = "Мужской";
			else tail->gender = "Женский";

			cout << "Введите место рождения пользователя:" << endl;
			cin >> tail->city;
			cout << "Введите место работы пользователя:" << endl;
			cin.get(); //предотвращение пропуска getline
			getline(cin, tail->work);
			cout << "Введите должность пользователя:" << endl;
			cin.clear(); //очистка буфера cin
			getline(cin, tail->prof);

			if (i < count - 1)
			{
				cout << "Данные сохранены!";
			skip_login:
				cout << "Продолжить ввод?:\n1. Да;\n2. Нет, закончить." << endl;
				if (choice() == 2) break;
			}	
		}

		system("pause");
	}
	else //вызов ф-ии ввода из файла
	{
		if (f_enter(count) == 3) goto retry; //возврат в начало функции
		else return; //возврат в меню
	}
}

int f_enter(int count)
{
	ifstream fin; //объект потока ввода из файла
	string fname;
	int ent_esc;
	string s_year;

	cout << "ВНИМАНИЕ!!! Вы выбрали ввод из файла. Файл должен иметь определенное форматирование и правильные данные."
		<< "\nА при наличии неверных данных запись будет удалена или введена ошибочно.";
	while (true)
	{
		cout << "\n1. Продолжить; \n2. Посмотреть требования; \n3. Вернуться." << endl;
		do
		{
			ent_esc = i_check();
			if ((ent_esc < 1 || ent_esc > 3) && ent_esc != -1) cout << "Неправильный код. Повторите попытку:" << endl;
		} while (ent_esc < 1 || ent_esc > 3);

		if (ent_esc == 3) return 3;
		else if (ent_esc == 2) demand();
		else break;
	}
	
	cout << "Введите полный путь к файлу:" << endl;
	cin >> fname;
	fin.open(fname);
	if (!fin.is_open()) cout << "Ошибка открытия файла!" << endl; //проверка на ошибку открытия
	else
	{
		for (int i = 0; i < count; i++) //заполнение
		{
			list();

			fin >> tail->surname >> tail->name >> tail->mid_name;
			if (login_check() == 2) //проверка существования пользователя
			{
				//пропуск записей, иначе они останутся и вызовут ошибку
				fin >> s_year >> tail->gender >> tail->city;
				fin.ignore(1);
				getline(fin, tail->work);
				getline(fin, tail->prof);
				del_cause_err(i); //удаление записи
				continue; //пропуск шага цикла
			}

			fin >> s_year;
			tail->year = to_digit(s_year);
			if (tail->year < 1955 || tail->year > 2006)
			{
				//пропуск данных
				if (tail->year != -1) cout << "Неправильные данные." << endl;
				fin >> tail->gender >> tail->city;
				fin.ignore(1);
				getline(fin, tail->work);
				getline(fin, tail->prof);
				del_cause_err(i); //удаление записи
				continue;
			}
			
			fin >> tail->gender;
			if (tail->gender != "Мужской" && tail->gender != "Женский")
			{
				fin >> tail->city;
				fin.ignore(1);
				getline(fin, tail->work);
				getline(fin, tail->prof);
				del_cause_err(i);
				continue;
			}
			fin >> tail->city;
			fin.ignore(1);
			getline(fin, tail->work);
			getline(fin, tail->prof);
		}
		cout << "Копирование из файла прошло успешно ." << endl;
		fin.close();
	}

	system("pause");
	return 0;
}

void list()
{
	if (cap == 0) //обработка случая создания первого объекта
	{
		head = tail = new profile; //создание начального объекта
		cap++; //инкремент счетчика объектов
	}
	else //обработка создания остальных объектов
	{
		pred = tail;
		tail = new profile; //создание конечного объекта
		tail->pPrev = pred;
		pred->pNext = tail;
		cap++; //инкремент счетчика объектов
	}
}

int login_check()
{
	int log_error = 0;

	if (cap == 1) return log_error;
	pred = head;

	while (pred != tail) //поиск совпадений
	{
		if (pred->surname == tail->surname && pred->name == tail->name && pred->mid_name == tail->mid_name) log_error = 1;
		pred = pred->pNext;
	}

	if (log_error == 1) //если найдено совпадение
	{
		cout << "Уже есть пользователь с таким именем (" << tail->surname << " " << tail->name << " " << tail->mid_name << "). Желаете продолжить:\n1. Да\n2. Нет, удалить" << endl;
		return choice();
	}

	return log_error; 
}

void del_cause_err(int num)
{
	pred = tail->pPrev;
	delete tail; //удаление последнего
	tail = pred;
	tail->pNext = NULL;
	cout << "В записи №" << num + 1 << " была найдена ошибка или повтор. Запись удалена." << endl;
	--cap;
}

void demand()
{
	cout << "Требования к файлу:\n1. Между каждый пунктом должен быть пробел;\n"
		<< "2. Место работы и должность должны быть на отдельных строках;\n"
		<< "3. Должен соблюдаться порядок: Фамилия, Имя, Отчество, Год рождениия, Пол, Место рождения, Место работы, Должность;\n"
		<< "4. Год рождения должен быть больше 1955 и меньше 2006 (люди другого возраста не могут быть официально трудоустроены);\n"
		<< "5. Пол может быть только \"Мужской\" или \"Женский\";\n"
		<< "6. Файл должен иметь кодировку ANSI;\n"
		<< "7. Не должно быть пустых строк.\n";

	system("pause"); //Задержка работы, возобновляется при нажатии любой кнопки

	cout << "Производится ввод из файла.";
}

void display()
{
	int i = 1;
	sled = head; //установка управляющего указателя на первый объект

	cout << "Будет произведен вывод таблицы. Вы хотите продолжить?\n1. Да\n2. Нет, вернуться" << endl;
	if (choice() == 2) return;

	cout << " № |" << setw(N) << left << "  Фамилия" << "|" << setw(N) << "    Имя" << "|" << setw(12) << "  Отчество" << "|" << setw(14) << " Год рождения" << "|" << setw(9) << "   Пол" << "|" << setw(2*N)
		<< "    Место рождения" << "|" << setw(18) << "   Место работы" << "|" << setw(N) << " Профессия" << endl;

	do
	{
		cout << setw(3) << left << i << "|" << setw(N) << sled->surname << "|" << setw(N) << sled->name << "|" << setw(12) << sled->mid_name << "|" << setw(14)
			<< sled->year << "|" << setw(9) << sled->gender << "|" << setw(2 * N) << sled->city << "|" << setw(18) << sled->work << "|" << setw(N) << sled->prof << endl;
		sled = sled->pNext; //переход на следующий элемент
		i++;
	} while (sled != NULL);

	system("pause");
}

void file_save()
{
	ofstream fout; //объект потока вывода в файл
	string fname;
	int i = 1;
	sled = head; //установка управляющего указателя на первый объект

	cout << "Вы собираетесь сохранить данные в файл. Вы хотите продолжить?\n1. Да\n2. Нет, вернуться" << endl;	
	if (choice() == 2) return;

	cout << "Введите полный путь к файлу:" << endl;
	cin >> fname;
	
	fout.open(fname);
	if (!fout.is_open()) cout << "Ошибка открытия файла." << endl; //проверка ошибки открытия файла
	else
	{
		cout << "В к каком формате вы хотите вывести данные?\n1. В стандартном;\n2. В виде таблицы." << endl;

		if (choice() == 1)
		{
			do
			{
				fout << sled->surname << " " << sled->name << " " << sled->mid_name << " " << sled->year << " " << sled->gender << " " << sled->city << endl << sled->work << endl << sled->prof << endl;
				sled = sled->pNext;
			} while (sled != NULL);
		}
		else
		{
			fout << " № |" << setw(N) << left << "  Фамилия" << "|" << setw(N) << "    Имя" << "|" << setw(12) << "  Отчество" << "|" << setw(14) << " Год рождения" << "|" << setw(9) << "   Пол" << "|" << setw(2*N)
				<< " Место рождения" << "|" << setw(18) << "   Место работы" << "|" << setw(N) << " Профессия" << endl;
			do
			{
				fout << setw(3) << left << i << "|" << setw(N) << sled->surname << "|" << setw(N) << sled->name << "|" << setw(12) << sled->mid_name << "|" << setw(14)
					<< sled->year << "|" << setw(9) << sled->gender << "|" << setw(2*N) << sled->city << "|" << setw(18) << sled->work << "|" << setw(N) << sled->prof << endl;
				sled = sled->pNext;
			} while (sled != NULL);
		}
		cout << "Копирование произошло успешно" << endl;
	}
	fout.close();

	system("pause");
}

int del()
{
	cout << "Будут удалены все элементы, если вы их не сохранили. Вы уверены, что хотите выйти?:\n1. Да;\n2. Вернуться обратно." << endl;
	if (choice() == 2) return -1;

	pred = tail;
	for (; cap > 0; cap--) //освобождение памяти
	{
		pred = pred->pPrev;
		delete tail;
		tail = pred;
	}

	cout << "Все данные были успешно удалены." << endl;
	
	return 0;
}

void rem_el()
{
	int num;

	cout << "Далее будет производиться удаление элемента. Хотите продолжить:\n1. Да;\n2. Нет, перейти в главное меню." << endl;
	if (choice() == 2) return;

	while (true)
	{
		cout << "Введите номер элемента, который желаете удалить (1-" << cap << "):" << endl;
		do
		{
			num = i_check();
			if ((num <= 0 || num > cap) && num != -1) cout << "У вас всего " << cap << " Элементов. Введите заново:" << endl;
		} while (num <= 0 || num > cap);
		
		if (num == 1) //удаление первого элемента
		{
			cout << "Процесс удаления необратим. Вы уверены, что хотите удалить " << num << "-й профиль (" << head->surname << " " << head->name
				<< " " << head->mid_name << ")?:\n1. Да;\n2. Нет." << endl; //подтвверждение
			if (choice() == 2) return;
			sled = head->pNext;
			delete head;
			head = sled;
			head->pPrev = NULL;
		}
		else if (num == cap) //удаление последнего
		{
			cout << "Процесс удаления необратим. Вы уверены, что хотите удалить " << num << "-й профиль (" << tail->surname << " " << tail->name
				<< " " << tail->mid_name << ")?:\n1. Да;\n2. Нет." << endl; //подтверждение
			if (choice() == 2) return;
			pred = tail->pPrev;
			delete tail;
			tail = pred;
			tail->pNext = NULL;
		}
		else //удаление произвольного
		{
			pred = head;
			for (int i = 0; i < num - 2; i++)
				pred = pred->pNext;
			sled = pred->pNext->pNext;
			cout << "Процесс удаления необратим. Вы уверены, что хотите удалить " << num << "-й профиль (" << pred->pNext->surname << " " << pred->pNext->name
				<< " " << pred->pNext->mid_name << ")?:\n1. Да;\n2. Нет." << endl; //подтверждение
			if (choice() == 2) return;
			delete sled->pPrev;
			sled->pPrev = pred;
			pred->pNext = sled;
		}
		cap--;

		cout << "Элемент успешно удален. Вы хотите удалить еще элементы?:\n1. Да\n2. Нет." << endl;
		if (choice() == 2) return;
	}
}

void young()
{
	cout << "Функция поиска младшего работника. Хотите продолжить?\n1. Да;\n2. Нет, вернуться." << endl;
	if (choice() == 2) return;

	int min_age;
	min_age = head->year; //установка наименьшего возраста
	sled = pred = head;
	for (int i = 1; i < cap; i++)
	{
		sled = sled->pNext; //переход по элементам
		if (sled->year > min_age) 
		{
			min_age = sled->year;
			pred = sled;
		}
	}
	cout << "Самый молодой пользователь - " << pred->surname << " " << pred->name << " " << pred->mid_name << endl;

	system("pause");
}

void searchProf()
{
	int i = 1;
	sled = head; //установка управляющего указателя на первый объект
	string str;

	cout << "Будет произведен поиск по профессии. Желаете продолжить?\n1. Да;\n2. Нет." << endl;
	if (choice() == 2) return;

	cout << "Введите профессию для поиска:" << endl;
	cin.ignore(1);
	getline(cin, str);
	
	
	cout << " № |" << setw(N) << left << "  Фамилия" << "|" << setw(N) << "    Имя" << "|" << setw(12) << "  Отчество" << "|" << setw(14) << " Год рождения" << "|" << setw(9) << "   Пол" << "|" << setw(2*N)
		<< "    Место рождения" << "|" << setw(18) << "   Место работы" << "|" << setw(N) << " Профессия" << endl;

	do
	{
		if (sled->prof == str)
		{
			cout << setw(3) << left << i << "|" << setw(N) << sled->surname << "|" << setw(N) << sled->name << "|" << setw(12) << sled->mid_name << "|" << setw(14)
				<< sled->year << "|" << setw(9) << sled->gender << "|" << setw(2*N) << sled->city << "|" << setw(18) << sled->work << "|" << setw(N) << sled->prof << endl;
			i++;
		}
		sled = sled->pNext; //переход на следующий элемент
	} while (sled != NULL);

	if (i == 1) cout << "— — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —\nПо Вашему запросу ничего не найдено!" << endl;	

	system("pause");
}

void city_work(int key)
{
	int i = 1;
	sled = head; //установка управляющего указателя на первый объект
	string str;

	if (key == 6)
	{
		cout << "Будет произведен поиск по месту рождения. Продолжить?\n1. Да;\n2. Вернуться в меню." << endl;
		if (choice() == 2) return;
		
		cout << "Введите место рождения для поиска:" << endl;
	}
	else
	{
		cout << "Будет произведен поиск по работы. Продолжить?\n1. Да;\n2. Вернуться в меню." << endl;
		if (choice() == 2) return;
		
		cout << "Введите место работы для поиска:" << endl;
	}
	cin.ignore(1);
	getline(cin, str);

	if (key == 6)
	{
		do
		{
			if (sled->city == str)
			{
				cout << i << ". " << sled->surname << " " << sled->name << " " << sled->mid_name << endl;
				i++;
			}
			sled = sled->pNext; //переход на следующий элемент
		} while (sled != NULL);
	}
	else
	{
		do
		{
			if (sled->work == str)
			{
				cout << i << ". " << sled->surname << " " << sled->name << " " << sled->mid_name << endl;
				i++;
			}
			sled = sled->pNext; //переход на следующий элемент
		} while (sled != NULL);
	}

	if (i == 1) cout << "— — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — — —\nПо Вашему запросу ничего не найдено!" << endl;

	system("pause");
}

void sort(int code)
{
	int procces = 0;
	int min, max, err = 0;
	pred = head;
	sled = pred->pNext;
	profile* buf = NULL;
	
	if (code == 1) cout << "Далее будет выполена сортировка по возрастанию (сначала - старше). Продолжить?\n1. Да;\n2. Нет." << endl;
	else cout << "Далее будет выполена сортировка по убыванию (первый - младше). Продолжить?\n1. Да;\n2. Нет." << endl;
	if (choice() == 2) return;

	if (cap == 1) cout << "У вас только одна учетная запись. Сортировка невозможна!!!";
	else
	{
		while (pred != NULL)
		{
			min = max = pred->year; //установка значения для сравнения
			buf = NULL;
			while (sled != NULL)
			{
				if (min > sled->year && code == 1) //для сортировки по возрастанию
				{
					min = sled->year;
					buf = sled;
				}
				if (max < sled->year && code == 2) //для сортировки по убыванию
				{
					max = sled->year;
					buf = sled;
				}
				sled = sled->pNext;
			}
			if (buf != NULL)
			{
				pointSort(buf);
				err++; //для определения наличия замен на каждом шаге
				procces = 1; //для определения наличия замен за всю функцию
			}
			/*Блок перехода на следцющий элемент
			Еслиэлемент последний, то перехода второй части не будет
			В случае если была перестановка на этом шаге, то все данные заменятся на первоначальные*/
			pred = pred->pNext;
			if (pred != NULL) sled = pred->pNext;
			if (((pred == NULL || sled == NULL) && err > 0))
			{
				pred = head;
				sled = pred->pNext;
				err = 0;
			}
		}
	}
	if (procces == 0) cout << "Данные уже осортированы." << endl;
	else cout << "Данные успешно отсортированы." << endl;

	system("pause");
}

void pointSort(profile* func_buf)
{
	profile* in_buf; //еще одна переменная для сохранения адреса
	/*Блок для смены соседних элементов
    В данном случае нужно менять указатели от 2-ух до 4-ех элементов:*/
	if (pred->pNext == func_buf)
	{
		first_swap(sled, func_buf, pred);

		last_swap(pred, sled, func_buf);

		pred->pPrev = func_buf;
		func_buf->pNext = pred;
	}
	/*Блок для смены элементов, между которыми есть только один элемент
	В данном случае нужно менять указатели от 3-ох до 5-и элементов*/
	else if (pred->pNext->pNext == func_buf)
	{
		sled = pred->pNext;
		sled->pPrev = func_buf;
		sled->pNext = pred;

		first_swap(sled, func_buf, pred);
		sled = pred->pNext;
		pred->pPrev = sled;

		last_swap(pred, sled, func_buf);
		sled = pred->pPrev;
		func_buf->pNext = sled;
	}
	/*Блок для смены элементов, между которыми больше одного элемента
	В данном случае нужно менять  указатели от 4-х до 6-ти элементов*/
	else
	{
		sled = pred->pNext;
		sled->pPrev = func_buf;

		sled = in_buf = func_buf->pPrev;
		sled->pNext = pred;
		
		first_swap(sled, func_buf, pred);
		pred->pPrev = in_buf;

		in_buf = pred->pNext;
		last_swap(pred, sled, func_buf);
		func_buf->pNext = in_buf;
	}
}

void first_swap(profile* first, profile* follow, profile* center)
{
	first = (center->pPrev != NULL) ? center->pPrev : NULL;

	if (first != NULL) //Замена указателей в случае, когда есть элементы до сравниваемых
	{
		first->pNext = follow;
		follow->pPrev = first;
	}
	else //Замена указателей, если сравниваются 1-ый и произвольный элементы
	{
		head = follow;
		follow->pPrev = NULL;
	}
}

void last_swap(profile* early, profile* last, profile* center)
{
	last = (center->pNext != NULL) ? center->pNext : NULL;

	if (last != NULL) //Замена указателей в случае, когда после сравниваемых есть еще элементы
	{
		last->pPrev = early;
		early->pNext = last;
	}
	else //Замена указателей, если сравнивается последний элемент
	{
		tail = early;
		tail->pNext = NULL;
	}
}

void workAmount()
{
	string company; //хранит пользовательский ввод
	int amount = 0;

	cout << "Работает функция подсчета кол-ва работников. Продолжить?\n1. Да;\n2. Нет." << endl;
	if (choice() == 2) return;

	cout << "Введите припдриятие, чтобы узнать кол-во работников:" << endl;
	cin.ignore(1);
	getline(cin, company);
	
	sled = head; //установка указателя на первый элемент
	while (sled != NULL) 
	{
		if (sled->work == company) amount++; //если совпадает - увеличивается кол-во
		sled = sled->pNext; //переход на следующий элемент
	}

	if (amount != 0) cout << "На предприятии " << company << " работают(ет) " << amount << " рабочих(ий)" << endl; //выводит кол-во
	else cout << "Нет зарегистрированных пользователей с данного предприятия" << endl; //Если совпадений нет - выводит сообщение

	system("pause");
}

int i_check() //для случаев проверки в стандартном потоке ввода cin
{
	string str;
	//строка для перевода в число, размер равен 5, потому что стоит ограничение в 4 символа в поле. В большем кол-ве нет нужды
	cin >> str;
	return to_digit(str);
}

int to_digit(string func_str) /*Функция проверяет строку на символы, а затем преобразует в число если ошибок нет*/
{
	int err = 0, digit = -1;
	char ch_mas[5];
	/*Блок try..catch нужен для предотвращения критических ошибок
	Ключевое слово throw нужно чтобы вызвать ошибку до того, как она могла бы повлиять на исполнение кода*/
	try
	{
		if (func_str.length() > 4) throw exception("Превышен предел поля (максимум 4 символов). Не забывайте, что это поле предназначено для цифр.\n"); //помещает сообщение об ошибке
		for (int i = 0; func_str[i]; i++)
		{
			if (func_str[i] < 0) throw exception("Был получен неопознанный символ. Скорее всего символ кириллицы.\n");
			else if (isdigit(func_str[i]) != 0)
			{
				ch_mas[i] = func_str[i]; //перенос символа в строку символов
			}
			else err++;
		}
	}
	catch (const exception& ex)
	{
		cout << ex.what(); //выводит сообщение об ошибке
		return digit; //возвращает код ошибки
	}

	if (err == 0)
	{
		digit = atoi(ch_mas);
		return digit; //возвращает число 
	}
	else
	{
		cout << "Ошибка ввода. Вы ввели сторонний сивол (буквы, -+/* и т.д.). В этом поле должно быть целое число." << endl;
		return digit; //возвращает ощибку
	}
}

int choice()
{
	int ex_key;

	do
	{
		ex_key = i_check(); //вызов функции проверки ввода
		if (ex_key != 1 && ex_key != 2 && ex_key != -1) cout << "Вы ввели неправильный код. Попробуйте еще раз:" << endl;
	} while (ex_key != 1 && ex_key != 2); //допускает только ответ 1 или 2

	return ex_key;
}