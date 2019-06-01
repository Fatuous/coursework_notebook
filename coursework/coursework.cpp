#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <Windows.h>

using namespace std;

class map_pair {
public:
	class data {
	public:
		string name;
		string surname;
		string birth;
		string phone;
		int id;

		data(){
			id = 0;
		}

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

	map_pair(data key) {
		this->letter = key.surname[0];
		this->info = key;
	}

	bool operator >= (map_pair& p) {
		return this->letter >= p.letter;
	}

};

ostream& operator << (ostream& o, map_pair& p) {
	return o << p.info.surname << " " << p.info.name << "\t\t" << p.info.phone << "\t" << p.info.birth << endl;
}

class stack {
public:
	class stack_node {
	public:
		map_pair key;
		stack_node* tail;
		stack_node() {
			tail = nullptr;
		}
	};

	stack_node* head;

	stack() {
		head = nullptr;
	}

	map_pair pop() {
		map_pair key = head->key;
		head = head->tail;
		return key;
	}

	void push(map_pair key) {
		stack_node* n = new stack_node();
		n->key = key;
		n->tail = head;
		head = n;
	}

	bool empty() {
		return head == nullptr;
	}
};

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

		~node() {
			delete left;
			delete right;
		}

		node(map_pair& key, node* parent) {
			this->key = key;
			this->parent = parent;
			right = nullptr;
			left = nullptr;
		}
	};

	node* root;
	int id;

	multimap() {
		root = nullptr;
		id = 1;
	}

	void clear() {
		id = 1;
		delete root;
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
		key.info.id = id++;
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

	void print_id(node* parent) {
		if (parent->left)
			print_id(parent->left);
		cout << parent->key.info.id << " " << parent->key;
		if (parent->right)
			print_id(parent->right);
	}

	void find(node* parent, string key) {
		if (parent->left)
			find(parent->left, key);
		if (key == parent->key.info.birth
			|| key == parent->key.info.name
			|| key == parent->key.info.surname
			|| key == parent->key.info.phone
			|| key == parent->key.letter) cout << parent->key;
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
	
	void load() {
		ifstream fin("data.txt");
		string s, name, surname, phone, birth;
		map_pair temp;
		streampos old_pos;
		this->clear();
		while (!fin.eof()) {
			old_pos = fin.tellg();
			getline(fin, s);
			if (s == "\0") {
				break;
			}
			else {
				fin.seekg(old_pos);
				fin >> surname >> name >> phone >> birth;
				temp = map_pair(map_pair::data(surname, name, phone, birth));
				this->push(temp);
				fin.ignore();
			}
		}
		fin.close();
		cout << "Данные загружены" << endl;
	}

	void find_id(node* parent, int id) {
		if (parent->left)
			 find_id(parent->left, id);
		if (id == parent->key.info.id) { 
			remove(parent); 
			return; 
		}
		if (parent->right)
			 find_id(parent->right, id);
	}

	void remove(node* Node){
		if (Node->left == nullptr && Node->right == nullptr)
			if (Node == root) 
				root = nullptr;
			else if (Node->parent->left == Node) 
				Node->parent->left = nullptr;
			else 
				Node->parent->right = nullptr;
		else if (Node->left == nullptr || Node->right == nullptr) {
			if (Node->left == nullptr) {
				if (Node == root) 
					root = root->right;
				else if (Node->parent->left == Node) 
					Node->parent->left = Node->right;
				else 
					Node->parent->right = Node->right;
				Node->right->parent = Node->parent;
			}
			else {
				if (Node == root) 
					root = root->left;
				else if (Node->parent->left == Node) 
					Node->parent->left = Node->left;
				else 
					Node->parent->right = Node->left;
				Node->left->parent = Node->parent;
			}
		}
		else {
			Node->key = getleft(Node)->key;
			if (getleft(Node)->parent->left == getleft(Node))
				getleft(Node)->parent->left = nullptr;
			else 
				getleft(Node)->parent->right = nullptr;
		}
	
	}

	node* getleft(node* Node) {
		if (Node->left != nullptr)
			return getleft(Node->left);
		return Node;
	}
	
	void remove() {
		int id;
		if (root) {
			print_id(root);
			cout << "Введите номер записи, которую хотите удалить: ";
			cin >> id;
			find_id(root, id);
		}
		else
			cout << "Записи отсутствуют\n";
	}

	void push_to_stack(stack& x) {
		node* temp = nullptr;
		node* Node = root;
		while (Node != nullptr) {
			if (temp == Node->parent) {
				if (Node->left != nullptr) {
					temp = Node;
					Node = Node->left;
					continue;
				}
				else
					temp = nullptr;
			}
			if (temp == Node->left) {
				x.push(Node->key);
				if (Node->right != nullptr)
				{
					temp = Node;
					Node = Node->right;
					continue;
				}
				else
					temp = nullptr;
			}
			if (temp == Node->right)
			{
				temp = Node;
				Node = Node->parent;
			}
		}
	}

	void push_by_name(node* parent, map_pair& key) {
		if (key.info.surname >= parent->key.info.surname) {
			if (parent->right)
				push_by_name(parent->right, key);
			else
				parent->right = new node(key, parent);
		}
		else {
			if (parent->left)
				push_by_name(parent->left, key);
			else
				parent->left = new node(key, parent);
		}
	}

	void push_by_name(map_pair& key) {
		if (root == nullptr)
			root = new node(key, nullptr);
		else
			push_by_name(root, key);
	}

	void push_by_date(node* parent, map_pair& key) {
		string datekey = key.info.birth;
		string datethis = parent->key.info.birth;
		if (datekey.substr(6) >= datethis.substr(6) 
			|| (datekey.substr(6) == datethis.substr(6) 
				&& datekey.substr(3) >= datethis.substr(3))
			|| (datekey.substr(6) == datethis.substr(6)
				&& datekey.substr(3) == datethis.substr(3)
				&& datekey >= datethis)) {
			if (parent->right)
				push_by_date(parent->right, key);
			else
				parent->right = new node(key, parent);
		}
		else {
			if (parent->left)
				push_by_date(parent->left, key);
			else
				parent->left = new node(key, parent);
		}
	}

	void push_by_date(map_pair& key) {
		if (root == nullptr)
			root = new node(key, nullptr);
		else
			push_by_date(root, key);
	}

	void sort_by_date(stack x) {
		while (!x.empty()) {
			map_pair temp = x.pop();
			push_by_date(temp);
		}
	}

	void sort_by_name(stack x) {
		while (!x.empty()) {
			map_pair temp = x.pop();
			push_by_name(temp);
		}
	}

	void sort() {
		char s;
		system("cls");
		cout << "------------------------------------" << endl;
		cout << "| 1 - Сортировать по фамилии       |" << endl;
		cout << "| 2 - Сортировать по дате рождения |" << endl;
		cout << "------------------------------------" << endl;

		stack Stack;
		push_to_stack(Stack);
		delete root;
		root = nullptr;

		s = _getch();
		switch (s) {
			case '1':
				sort_by_name(Stack);
				break;
			case '2':
				sort_by_date(Stack);
				break;
			default:
				sort();
				break;
		}
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
	cout << "| 6 - удалить запись              |" << endl;
	cout << "| 7 - сортировать записи          |" << endl;
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
		switch (s) {
		case '1':
			menu();
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
			m.load();			
			break;

		case '4':
			menu();
			m.save();
			break;

		case '5':
			menu();
			m.print_id(m.root);
			cout << "Введите искомую фамилию / первую букву фамилии, имя, дату рождения или номер телефона: ";
			cin >> name;
			m.find(name);
			break;

		case '6':
			menu();
			m.remove();
			break;

		case '7':
			m.sort();
			menu();
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