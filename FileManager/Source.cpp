// FileManager
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h> 
#include <io.h>   
#include <windows.h>
#include <direct.h> 

using namespace std;

class ARG
{
public:
	string name;
	string path;
	ARG()
	{
		name = " ";
		path = "D:\\";
	}
	void Creat_F(string str)
	{
		char s[MAX_PATH];
		strcpy_s(s, str.c_str());
		FILE* File; //указатель на место в оперативной памяти
		//проверка существования файла с таким именем
		int i = 1;
		while (1) //попытка открытия файла на чтение
		{
			if (!fopen_s(&File, s, "r")) {
				cout << "Файл с таким именем уже существует!" << endl;
				str.insert(str.rfind("."), to_string(i));
				i++;
				fclose(File); //закрываем файл
			}
			else break;
			strcpy_s(s, str.c_str());
		}
		cout << str << endl;
		fopen_s(&File, s, "w"); //открытие файла на запись(создание)
		if (!File)
		{
			cout << "Ошибка создания файла!" << endl;
			return;
		}
		fclose(File); //закрываем файл
		cout << "Файл успешно создан!" << endl;
	}
	void Creat_D(string str)
	{
		char Name[MAX_PATH];
		strcpy_s(Name, str.c_str());
		if (_mkdir(Name) == -1)
		{
			cout << "Ошибка создания директории! Проверьте имя и путь!" << endl;
			return;
		}
		else cout << "Директория успешно создана!" << endl;
	}
	void Copy_F(string str, string strpath) // имя файла начального и конечного с путями
	{
		//указатели на файлы
		FILE* Src = nullptr, * Dest = nullptr;
		char s[MAX_PATH];
		strcpy_s(s, str.c_str());
		name = str.substr(str.rfind("\\"));
		//открываем оригинальный файл в бинарном(двоичном) режиме на чтение
		if (fopen_s(&Src, s, "rb"))
		{
			cout << "Файл не найден! Проверьте имя и путь к файлу!" << endl;
			return;
		}
		//Получаем дескриптор файла
		int hSrc = _fileno(Src);
		//определяем размер файла
		int Size = _filelength(hSrc);
		//выделяем память под буфер для копирования содержимого
		char* Data = new char[Size];
		if (!Data)
		{
			cout << "Ошибка выделения памяти, при создании буфера для копирования файла!" << endl;
			fclose(Src); //закрытие файла
			return;
		}
		//открываем(создаем) файл назначения в бинарном режиме на запись		
		strcpy_s(s, strpath.c_str());
		if (fopen_s(&Dest, s, "wb"))
		{
			cout << "Ошибка копирования файла! Проверьте имя и путь назанчения!" << endl;
			fclose(Src); //закрытие файла
			delete[] Data;
			return;
		}
		//Объем копируемых данных
		int DataValue = 0;
		//считываем данные
		while (!feof(Src)) //пока не дошли до конца файла
		{
			//считываем данные в строку Data, подсчитывая объем данных в строке и помещая его в DataValue
			DataValue = fread(Data, sizeof(char), Size, Src);
			//записываем строку в файл назначения
			if (DataValue)fwrite(Data, sizeof(char), DataValue, Dest);
			else break; //если закончились данные выходим из цикла
		}
		//закрываем файлы
		fclose(Src);
		fclose(Dest);
		//освобождаем память, выделенную под буфер
		delete[] Data;
		cout << "Файл успешно скопирован!" << endl;
	}
	void Del_F(string N)
	{
		char Name[MAX_PATH];
		strcpy_s(Name, N.c_str());
		//удаляем файл и проверяем результат
		if (remove(Name))
		{
			cout << "Ошибка удаления файла! Проверьте имя и закрыт ли файл!" << endl;
			return;
		}
		else cout << "Файл успешно удален!" << endl;
	}
	void Copy_D(string Source, string Destination) // source - имя начальной папки с полным путем, destination - имя и путь папки назначения
	{
		static int CountDir = 0; //кол-во скопированных директорий
		static int CountFile = 0; //кол-во скопированных файлов
		Source += "\\*.*";//добавляем маску для поиска всех файлов и подкатологов в директории
		char Path[MAX_PATH]{};
		strcpy_s(Path, Source.c_str());  //копируем текущий путь и имя папки		 
		_finddata_t Find; //объект структуры для хранения результатов
		int Done = _findfirst(Path, &Find);
		//если Done равен -1 файлов нет
		int MayWeWork = Done;
		if (MayWeWork == -1)
		{
			cout << "Директория не существует!" << endl;
			return;
		}
		//создаем директорию назначения
		Creat_D(Destination);
		while (MayWeWork != -1)
		{
			if (strcmp(Find.name, ".") && strcmp(Find.name, "..")) //исключаем каталоги .  и ..
			{
				//проверяем файл или папка найдена
				if (Find.attrib & _A_SUBDIR) //если поле атрибут содержит директорию
				{
					//копируем папки
					//временная строка для хранения пути к подпапке в источнике
					char SubDirSrc[MAX_PATH];
					//запоминаем путь к подпапке для источника
					strcpy_s(SubDirSrc, Source.c_str());
					strcat_s(SubDirSrc, "\\");
					strcat_s(SubDirSrc, Find.name); //добавляем имя подпапки
					//временная строка для хранения пути к подпапке в назначении
					char SubDirDest[MAX_PATH];
					//запоминаем путь к подпапке для назанчения
					strcpy_s(SubDirDest, Destination.c_str());
					strcat_s(SubDirDest, "\\");
					strcat_s(SubDirDest, Find.name);
					CountDir++;
					Copy_D(SubDirSrc, SubDirDest); //вызываем функцию для копирования подпапки
				}
				else //иначе файл
				{
					//копируем файлы
					//временная строка для хранения пути к файлу в источнике
					char FileSrc[MAX_PATH];
					//запоминаем путь к подпапке для источника
					strcpy_s(FileSrc, Source.c_str());
					strcat_s(FileSrc, "\\");
					strcat_s(FileSrc, Find.name); //добавляем имя подпапки
					//временная строка для хранения пути к подпапке в назначении
					char FileDest[MAX_PATH];
					//запоминаем путь к подпапке для назанчения
					strcpy_s(FileDest, Destination.c_str());
					strcat_s(FileDest, "\\");
					strcat_s(FileDest, Find.name);
					CountFile++;
					Copy_F(FileSrc, FileDest); //вызываем функцию для копирования файла
				}
			}
			MayWeWork = _findnext(Done, &Find);
		}
		_findclose(Done);
		cout << "Скопировано: " << CountDir << " директорий и " << CountFile << " файлов!" << endl;
	}
	void Del_D(string str)
	{
		cout << "Проверка пути для удаления - " << str << endl;
		static int CountDir = 0; //кол-во удаленных директорий
		static int CountFile = 0; //кол-во удаленных файлов
		string temp;
		temp= str + "\\*.*";//добавляем маску для поиска всех файлов и подкатологов в директории
		char Path[MAX_PATH]{};
		strcpy_s(Path, temp.c_str());  //копируем текущий путь и имя папки		 
		_finddata_t Find; //объект структуры для хранения результатов
		long Done = _findfirst(Path, &Find);
		//если Done равен -1 файлов нет
		int MayWeWork = Done;
		if (MayWeWork == -1)
		{
			cout << "Директория Пуста!" << endl;			
		}
		while (MayWeWork != -1)
		{
			if (MayWeWork == -1)break;
			if (strcmp(Find.name, ".") && strcmp(Find.name, "..")) //исключаем каталоги .  и ..
			{
				//проверяем файл или папка найдена
				if (Find.attrib & _A_SUBDIR) //если поле атрибут содержит директорию
				{
					//копируем папки
					//временная строка для хранения пути к подпапке в источнике
					char SubDirSrc[MAX_PATH];
					//запоминаем путь к подпапке для источника
					strcpy_s(SubDirSrc, str.c_str());
					strcat_s(SubDirSrc, "\\");
					strcat_s(SubDirSrc, Find.name); //добавляем имя подпапки					
					CountDir++;
					Del_D(SubDirSrc); //вызываем рекурсифункцию для удаления подпапки
				}
				else //иначе файл
				{
					//копируем файлы
					//временная строка для хранения пути к файлу в источнике
					char FileSrc[MAX_PATH];
					//запоминаем путь к подпапке для источника
					strcpy_s(FileSrc, str.c_str());
					strcat_s(FileSrc, "\\");
					strcat_s(FileSrc, Find.name); //добавляем имя подпапки					
					CountFile++;
					Del_F(FileSrc); //вызываем функцию для копирования файла
				}
				
			}
			MayWeWork = _findnext(Done, &Find);
			
		}
		_findclose(Done);
		cout << "Удалено: " << CountDir << " директорий и " << CountFile << " файлов!" << endl;
		//удаляем директорию и проверяем результат
		if (_rmdir(str.c_str()) == -1)
		{
			cout << "Ошибка удаления директории! Проверьте имя пуста ли директория!" << endl;
			return;
		}
		else
		{			
			cout << "папка удалена" << endl;
		}
	}

}; 
class Fileactions : public ARG
{
public:
	void Creat_File()
	{
		cout << "Введите имя файла: ";
		cin >> name;
		string str = path + name;		
		Creat_F(str);

	}
	void Copy_File()
	{
		string str;
		cout << "Введите название копируемого файла:" << endl;
		cin >> str;
		str = path + str;
		cout << "Введите новый путь файла:" << endl;
		string strpath;
		cin >> strpath;
		strpath += name;
		Copy_F(str, strpath);

	}
	void Rename_File()
	{
		//Текущее и новое имя файла
		char OldName[MAX_PATH];
		char NewName[MAX_PATH];
		cout << "Введите старое имя файла: ";
		string old, n;
		cin >> old;
		cout << "Введите новое имя файла: ";
		cin >> n;
		old = path + old;
		n = path + n;
		strcpy_s(OldName, old.c_str());
		strcpy_s(NewName, n.c_str());
		//переименование и проверка
		if (rename(OldName, NewName)) //возвращает 0 если удалось и код ошибки если нет
		{
			cout << "Ошибка переименования файла! Проверьте имя и закрыт ли файл!" << endl;
			return;
		}
		else cout << "Файл успешно переименован!" << endl;
		//нужно извлечь путь из старого имени
		//добавить к нему новое имя и передать в функцию rename
	}
	void Delete_File()
	{
		//Имя файла для удаления		
		cout << "Введите имя файла: ";
		string N;
		cin >> N;
		N = path + N;
		Del_F(N);
	}
	void Mov_File()
	{
		string str, strpath;
		cout << "Введите имя перемещаемого файла: ";
		cin >> str;
		cout << "Введите путь для перемещения файла: ";
		cin >> strpath;
		Copy_F(str, strpath);
		Del_F(str);
	}
};
class Directactions : public ARG
{
public:
	void Creat_Dir()
	{
		string str;
		cout << "Введите имя папки: ";
		cin >> str;
		str = path + str;
		Creat_D(str);
	}
	void Copy_Dir()
	{
		string str, dest;
		cout << "Введите имя директории: ";
		getline(cin, str);
		cout << "Введите путь к директории для копирования: ";
		getline(cin, dest);
		Copy_D(str.insert(0, path), dest);
	}
	void Del_Dir()
	{
		string str;
		cout << "Введите имя папки которую нужно удалить ";
		cin>>str;
		_finddata_t Find; //объект структуры для хранения результатов
		int Done = _findfirst((str.insert(0, path)).c_str(), &Find);
		if (Done == -1)
		{
			cout << "Такой папки нет" << endl;
		}
		else
		{
			Del_D(str);
		}
		_findclose(Done);
	}
	void Show_Dir()
	{
		//полный путь с маской для вывода всего содержимого директории
		string str;
		str = path + "\\*.*";
		system("cls");
		//создаем объект структуры, хранящей иданные о файлах и директориях
		_finddata_t Find;
		//начинаем поиск
		int Done = _findfirst(str.c_str(), &Find);
		//если Done равен -1 файлов нет
		int MayWeWork = Done;
		if (MayWeWork == -1)
		{
			cout << "Директория пуста!" << endl;
			return;
		}
		cout << "Содержимое директории: " << path << endl;
		int CountDir = 0;
		int CountFile = 0;
		while (MayWeWork != -1)
		{
			if (strcmp(Find.name, ".") && strcmp(Find.name, "..")) //исключаем каталоги .  и ..
			{
				//проверяем директория ли
				if (Find.attrib & _A_SUBDIR) //если атрибут содержит директорию
				{
					cout << "Директория: \t|\t";
					CountDir++;
				}
				else
				{
					cout << "Файл: \t\t|" << Find.size << "\t";
					CountFile++;
				}
				if (Find.size <= 999999)cout << "\t";
				cout << "|" << Find.name << endl;
			}
			MayWeWork = _findnext(Done, &Find);
		}
		cout << "Кол-во файлов: " << CountFile << " Кол-во директорий: " << CountDir << endl;
		_findclose(Done);
	}
	void Rename_Dir()
	{
		string str, dest;
		cout << "Введите старое имя папки" << endl;
		cin>> str;
		cout << "Введите новое имя папки" << endl;
		cin>>dest;
		//переименование и проверка
		if (rename((str.insert(0, path)).c_str(), (dest.insert(0, path)).c_str())) //возвращает 0 если удалось и код ошибки если нет
		{
			cout << "Ошибка переименования директории! Проверьте старый и новый путь и имя директории!" << endl;
			return;
		}
		else cout << "Директории успешно переименована!" << endl;
	}
	void Mov_Dir()
	{
		string str, dest;
		cout << "Введите имя копируемой папки:";
		cin>>str;
		cout << "Введите новый путь:";
		cin>>dest;
		Copy_D(str.insert(0, path), dest);
		Del_D(str);
	}
	void Travel()
	{
		string choice;
		cout << "Введите имя подпапки или Back"<<endl;
		cin>> choice;
		if (choice == "back")
		{
			path = (path.substr(path.rfind("\\")));
		}
		else
		{
			_finddata_t Find; //объект структуры для хранения результатов
			string pathNext = choice.insert(0, path);			
			int Done = _findfirst(pathNext.c_str(), &Find);
			if (Done == -1)
			{
				cout << "Такой папки нет" << endl;
			}
			else
			{
				path = pathNext+"\\";
			}
			_findclose(Done);
		}
	}
};

void MENU()
{
	Fileactions A;
	Directactions B;
	string str;
	while (1)
	{
		A.path = B.path;
		cout << A.path;
		cin >> str;
		if (str == "help" || str == "/?")
		{
			cout << "crf   // создание файла" << endl
				<< "crd   // создание папки" << endl
				<< "cpf   // копирование файла" << endl
				<< "cpd   // копирование файла" << endl
				<< "rnf   // переименование файла" << endl
				<< "rnd   // переименование папки" << endl
				<< "delf  // удаление файла" << endl
				<< "deld  // удаление папки" << endl
				<< "movf  // перемещение файла" << endl
				<< "movd  // перемещение папки" << endl
				<< "cd    // переход по папкам" << endl
				<< "show  // содержимое папки" << endl
				<< "exit  // выход" << endl;
		}
		if (str == "crf")A.Creat_File();
		if (str == "cpf")A.Copy_File();
		if (str == "rnf")A.Rename_File();
		if (str == "delf")A.Delete_File();
		if (str == "movf")A.Mov_File();
		if (str == "crd")B.Creat_Dir();
		if (str == "cpd")B.Copy_Dir();
		if (str == "rnd")B.Rename_Dir();
		if (str == "deld")B.Del_Dir();
		if (str == "movd")B.Mov_Dir();
		if (str == "cd")B.Travel();
		if (str == "show") B.Show_Dir();
		if (str == "exit")exit(666);
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	MENU();
}