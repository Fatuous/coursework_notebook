#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <Windows.h>

/* TODO: 
		1. (4/5) класса
		2. удаление записи
		3. пофиксить считывание из файла (if getline == '\0')
		4. сортировка по разным полям
		5. Улучшить поиск записи (по 1 букве фамилии)
*/
using namespace std;

class map_pair {
public:
	class data {
	public:
		string name;
		string surname;
		string birth;
		string phone;

		data(){}

		data(string surname, string name, string phone, string birth) {
			this->name = name;
			this->surname = surname;
			this->birth = birth;
			this->phone = phone;
		}
	};
	
	string letter;
	data info;

	map_pair(){}

	map_pair(data value) {
		this->letter = value.surname[0];
		this->info = value;
	}

	bool operator >= (map_pair& p) {
		return this->letter >= p.letter;
	}

};

ostream& operator << (ostream& o, map_pair& p) {
	return o << p.info.surname << " " << p.info.name << " \t" << p.info.phone << " \t" << p.info.birth << endl;
}

class multimap {
public:
	class node {
	public:
		node* parent;
		node* right;
		node* left;
		map_pair key;

		node() {
			right = nullptr;
			left = nullptr;
			parent = nullptr;
		}
		node(map_pair& key, node* parent) {
			this->key = key;
			this->parent = parent;
			right = nullptr;
			left = nullptr;
		}
	};

	node* root;

	multimap() {
		root = nullptr;
	}

	void clear() {
		root = nullptr;
	}

	void push(node* parent, map_pair& key) {
		if (key >= parent->key) {
			if (parent->right)
				push(parent->right, key);
			else
				parent->right = new node(key, parent);
		}
		else {
			if (parent->left)
				push(parent->left, key);
			else
				parent->left = new node(key, parent);
		}
	}

	void push(map_pair& key) {
		if (root == nullptr)
			root = new node(key, nullptr);
		else
			push(root, key);
	}

	void print(node* parent) {
		if (parent->left)
			print(parent->left);
		cout << parent->key;
		if (parent->right)
			print(parent->right);
	}

	void print() {
		if (root)
			print(root);
		else 
			cout << "Записи отсутствуют\n";
	}

	void find(node* parent, string key) {
		if (parent->left)
			find(parent->left, key);
		if (key == parent->key.info.birth
			|| key == parent->key.info.name
			|| key == parent->key.info.surname
			|| key == parent->key.info.phone) cout << parent->key;
		if (parent->right)
			find(parent->right, key);
	}

	void find(string key) {
		find(root, key);
	}

	void save(node* x) {
		ofstream fout ("data.txt");
		node* temp = nullptr;
		
		while (x != nullptr) {
			if (temp == x->parent) {
				if (x->left != nullptr) {
					temp = x;
					x = x->left;
					continue;
				}
				else
					temp = nullptr;
			}
			if (temp == x->left) {
				fout << x->key;
				if (x->right != nullptr)
				{
					temp = x;
					x = x->right;
					continue;
				}
				else
					temp = nullptr;
			}
			if (temp == x->right)
			{
				temp = x;
				x = x->parent;
			}
		}
		cout << "Данные успешно сохранены" << endl;
		fout.close();
	}

	void save() {
		save(root);
	}
};

void menu() {
	system("cls");
	cout << "-----------------------------------" << endl;
	cout << "| 1 - добавить запись             |" << endl;
	cout << "| 2 - вывести данные на экран     |" << endl;
	cout << "| 3 - загрузить данные из файла   |" << endl;
	cout << "| 4 - сохранить данные в файл     |" << endl;
	cout << "| 5 - найти записи                |" << endl;
	cout << "| 0 - выход                       |" << endl;
	cout << "-----------------------------------" << endl;
}

int main() {
	setlocale(LC_ALL, "Russian");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	char s;
	string name, surname, phone, birth;
	map_pair temp;
	multimap m;
	bool end = false;

	menu();
	while (end != true) {
		s = _getch();
		ifstream fin("data.txt");
		switch (s) {
		case '1':
			cout << "Введите фамилию, имя, номер телефона и дату рождения:\n\t";
			cin >> surname >> name >> phone >> birth;
			temp = map_pair(map_pair::data(surname, name, phone, birth));
			m.push(temp);

			menu();
			break;

		case '2':
			menu();
			m.print();
			break;

		case '3':
			menu();
			m.clear();
			while (!fin.eof()) {
				fin >> surname >> name >> phone >> birth;
				temp = map_pair(map_pair::data(surname, name, phone, birth));
				m.push(temp);
			}
			fin.close();
			cout << "Данные загружены" << endl;
			break;

		case '4':
			menu();
			m.save();
			break;

		case '5':
			menu();
			cout << "Введите искомую фамилию, имя, дату рождения или номер телефона: ";
			cin >> name;
			m.find(name);
			break;

		case '0':
			end = true;
			break;

		default:
			menu();
			break;
		}
	}
	
	return 0;
}