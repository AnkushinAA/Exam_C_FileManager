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
		FILE* File; //��������� �� ����� � ����������� ������
		//�������� ������������� ����� � ����� ������
		int i = 1;
		while (1) //������� �������� ����� �� ������
		{
			if (!fopen_s(&File, s, "r")) {
				cout << "���� � ����� ������ ��� ����������!" << endl;
				str.insert(str.rfind("."), to_string(i));
				i++;
				fclose(File); //��������� ����
			}
			else break;
			strcpy_s(s, str.c_str());
		}
		cout << str << endl;
		fopen_s(&File, s, "w"); //�������� ����� �� ������(��������)
		if (!File)
		{
			cout << "������ �������� �����!" << endl;
			return;
		}
		fclose(File); //��������� ����
		cout << "���� ������� ������!" << endl;
	}
	void Creat_D(string str)
	{
		char Name[MAX_PATH];
		strcpy_s(Name, str.c_str());
		if (_mkdir(Name) == -1)
		{
			cout << "������ �������� ����������! ��������� ��� � ����!" << endl;
			return;
		}
		else cout << "���������� ������� �������!" << endl;
	}
	void Copy_F(string str, string strpath) // ��� ����� ���������� � ��������� � ������
	{
		//��������� �� �����
		FILE* Src = nullptr, * Dest = nullptr;
		char s[MAX_PATH];
		strcpy_s(s, str.c_str());
		name = str.substr(str.rfind("\\"));
		//��������� ������������ ���� � ��������(��������) ������ �� ������
		if (fopen_s(&Src, s, "rb"))
		{
			cout << "���� �� ������! ��������� ��� � ���� � �����!" << endl;
			return;
		}
		//�������� ���������� �����
		int hSrc = _fileno(Src);
		//���������� ������ �����
		int Size = _filelength(hSrc);
		//�������� ������ ��� ����� ��� ����������� �����������
		char* Data = new char[Size];
		if (!Data)
		{
			cout << "������ ��������� ������, ��� �������� ������ ��� ����������� �����!" << endl;
			fclose(Src); //�������� �����
			return;
		}
		//���������(�������) ���� ���������� � �������� ������ �� ������		
		strcpy_s(s, strpath.c_str());
		if (fopen_s(&Dest, s, "wb"))
		{
			cout << "������ ����������� �����! ��������� ��� � ���� ����������!" << endl;
			fclose(Src); //�������� �����
			delete[] Data;
			return;
		}
		//����� ���������� ������
		int DataValue = 0;
		//��������� ������
		while (!feof(Src)) //���� �� ����� �� ����� �����
		{
			//��������� ������ � ������ Data, ����������� ����� ������ � ������ � ������� ��� � DataValue
			DataValue = fread(Data, sizeof(char), Size, Src);
			//���������� ������ � ���� ����������
			if (DataValue)fwrite(Data, sizeof(char), DataValue, Dest);
			else break; //���� ����������� ������ ������� �� �����
		}
		//��������� �����
		fclose(Src);
		fclose(Dest);
		//����������� ������, ���������� ��� �����
		delete[] Data;
		cout << "���� ������� ����������!" << endl;
	}
	void Del_F(string N)
	{
		char Name[MAX_PATH];
		strcpy_s(Name, N.c_str());
		//������� ���� � ��������� ���������
		if (remove(Name))
		{
			cout << "������ �������� �����! ��������� ��� � ������ �� ����!" << endl;
			return;
		}
		else cout << "���� ������� ������!" << endl;
	}
	void Copy_D(string Source, string Destination) // source - ��� ��������� ����� � ������ �����, destination - ��� � ���� ����� ����������
	{
		static int CountDir = 0; //���-�� ������������� ����������
		static int CountFile = 0; //���-�� ������������� ������
		Source += "\\*.*";//��������� ����� ��� ������ ���� ������ � ������������ � ����������
		char Path[MAX_PATH]{};
		strcpy_s(Path, Source.c_str());  //�������� ������� ���� � ��� �����		 
		_finddata_t Find; //������ ��������� ��� �������� �����������
		int Done = _findfirst(Path, &Find);
		//���� Done ����� -1 ������ ���
		int MayWeWork = Done;
		if (MayWeWork == -1)
		{
			cout << "���������� �� ����������!" << endl;
			return;
		}
		//������� ���������� ����������
		Creat_D(Destination);
		while (MayWeWork != -1)
		{
			if (strcmp(Find.name, ".") && strcmp(Find.name, "..")) //��������� �������� .  � ..
			{
				//��������� ���� ��� ����� �������
				if (Find.attrib & _A_SUBDIR) //���� ���� ������� �������� ����������
				{
					//�������� �����
					//��������� ������ ��� �������� ���� � �������� � ���������
					char SubDirSrc[MAX_PATH];
					//���������� ���� � �������� ��� ���������
					strcpy_s(SubDirSrc, Source.c_str());
					strcat_s(SubDirSrc, "\\");
					strcat_s(SubDirSrc, Find.name); //��������� ��� ��������
					//��������� ������ ��� �������� ���� � �������� � ����������
					char SubDirDest[MAX_PATH];
					//���������� ���� � �������� ��� ����������
					strcpy_s(SubDirDest, Destination.c_str());
					strcat_s(SubDirDest, "\\");
					strcat_s(SubDirDest, Find.name);
					CountDir++;
					Copy_D(SubDirSrc, SubDirDest); //�������� ������� ��� ����������� ��������
				}
				else //����� ����
				{
					//�������� �����
					//��������� ������ ��� �������� ���� � ����� � ���������
					char FileSrc[MAX_PATH];
					//���������� ���� � �������� ��� ���������
					strcpy_s(FileSrc, Source.c_str());
					strcat_s(FileSrc, "\\");
					strcat_s(FileSrc, Find.name); //��������� ��� ��������
					//��������� ������ ��� �������� ���� � �������� � ����������
					char FileDest[MAX_PATH];
					//���������� ���� � �������� ��� ����������
					strcpy_s(FileDest, Destination.c_str());
					strcat_s(FileDest, "\\");
					strcat_s(FileDest, Find.name);
					CountFile++;
					Copy_F(FileSrc, FileDest); //�������� ������� ��� ����������� �����
				}
			}
			MayWeWork = _findnext(Done, &Find);
		}
		_findclose(Done);
		cout << "�����������: " << CountDir << " ���������� � " << CountFile << " ������!" << endl;
	}
	void Del_D(string str)
	{
		cout << "�������� ���� ��� �������� - " << str << endl;
		static int CountDir = 0; //���-�� ��������� ����������
		static int CountFile = 0; //���-�� ��������� ������
		string temp;
		temp= str + "\\*.*";//��������� ����� ��� ������ ���� ������ � ������������ � ����������
		char Path[MAX_PATH]{};
		strcpy_s(Path, temp.c_str());  //�������� ������� ���� � ��� �����		 
		_finddata_t Find; //������ ��������� ��� �������� �����������
		long Done = _findfirst(Path, &Find);
		//���� Done ����� -1 ������ ���
		int MayWeWork = Done;
		if (MayWeWork == -1)
		{
			cout << "���������� �����!" << endl;			
		}
		while (MayWeWork != -1)
		{
			if (MayWeWork == -1)break;
			if (strcmp(Find.name, ".") && strcmp(Find.name, "..")) //��������� �������� .  � ..
			{
				//��������� ���� ��� ����� �������
				if (Find.attrib & _A_SUBDIR) //���� ���� ������� �������� ����������
				{
					//�������� �����
					//��������� ������ ��� �������� ���� � �������� � ���������
					char SubDirSrc[MAX_PATH];
					//���������� ���� � �������� ��� ���������
					strcpy_s(SubDirSrc, str.c_str());
					strcat_s(SubDirSrc, "\\");
					strcat_s(SubDirSrc, Find.name); //��������� ��� ��������					
					CountDir++;
					Del_D(SubDirSrc); //�������� �������������� ��� �������� ��������
				}
				else //����� ����
				{
					//�������� �����
					//��������� ������ ��� �������� ���� � ����� � ���������
					char FileSrc[MAX_PATH];
					//���������� ���� � �������� ��� ���������
					strcpy_s(FileSrc, str.c_str());
					strcat_s(FileSrc, "\\");
					strcat_s(FileSrc, Find.name); //��������� ��� ��������					
					CountFile++;
					Del_F(FileSrc); //�������� ������� ��� ����������� �����
				}
				
			}
			MayWeWork = _findnext(Done, &Find);
			
		}
		_findclose(Done);
		cout << "�������: " << CountDir << " ���������� � " << CountFile << " ������!" << endl;
		//������� ���������� � ��������� ���������
		if (_rmdir(str.c_str()) == -1)
		{
			cout << "������ �������� ����������! ��������� ��� ����� �� ����������!" << endl;
			return;
		}
		else
		{			
			cout << "����� �������" << endl;
		}
	}

}; 
class Fileactions : public ARG
{
public:
	void Creat_File()
	{
		cout << "������� ��� �����: ";
		cin >> name;
		string str = path + name;		
		Creat_F(str);

	}
	void Copy_File()
	{
		string str;
		cout << "������� �������� ����������� �����:" << endl;
		cin >> str;
		str = path + str;
		cout << "������� ����� ���� �����:" << endl;
		string strpath;
		cin >> strpath;
		strpath += name;
		Copy_F(str, strpath);

	}
	void Rename_File()
	{
		//������� � ����� ��� �����
		char OldName[MAX_PATH];
		char NewName[MAX_PATH];
		cout << "������� ������ ��� �����: ";
		string old, n;
		cin >> old;
		cout << "������� ����� ��� �����: ";
		cin >> n;
		old = path + old;
		n = path + n;
		strcpy_s(OldName, old.c_str());
		strcpy_s(NewName, n.c_str());
		//�������������� � ��������
		if (rename(OldName, NewName)) //���������� 0 ���� ������� � ��� ������ ���� ���
		{
			cout << "������ �������������� �����! ��������� ��� � ������ �� ����!" << endl;
			return;
		}
		else cout << "���� ������� ������������!" << endl;
		//����� ������� ���� �� ������� �����
		//�������� � ���� ����� ��� � �������� � ������� rename
	}
	void Delete_File()
	{
		//��� ����� ��� ��������		
		cout << "������� ��� �����: ";
		string N;
		cin >> N;
		N = path + N;
		Del_F(N);
	}
	void Mov_File()
	{
		string str, strpath;
		cout << "������� ��� ������������� �����: ";
		cin >> str;
		cout << "������� ���� ��� ����������� �����: ";
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
		cout << "������� ��� �����: ";
		cin >> str;
		str = path + str;
		Creat_D(str);
	}
	void Copy_Dir()
	{
		string str, dest;
		cout << "������� ��� ����������: ";
		getline(cin, str);
		cout << "������� ���� � ���������� ��� �����������: ";
		getline(cin, dest);
		Copy_D(str.insert(0, path), dest);
	}
	void Del_Dir()
	{
		string str;
		cout << "������� ��� ����� ������� ����� ������� ";
		cin>>str;
		_finddata_t Find; //������ ��������� ��� �������� �����������
		int Done = _findfirst((str.insert(0, path)).c_str(), &Find);
		if (Done == -1)
		{
			cout << "����� ����� ���" << endl;
		}
		else
		{
			Del_D(str);
		}
		_findclose(Done);
	}
	void Show_Dir()
	{
		//������ ���� � ������ ��� ������ ����� ����������� ����������
		string str;
		str = path + "\\*.*";
		system("cls");
		//������� ������ ���������, �������� ������� � ������ � �����������
		_finddata_t Find;
		//�������� �����
		int Done = _findfirst(str.c_str(), &Find);
		//���� Done ����� -1 ������ ���
		int MayWeWork = Done;
		if (MayWeWork == -1)
		{
			cout << "���������� �����!" << endl;
			return;
		}
		cout << "���������� ����������: " << path << endl;
		int CountDir = 0;
		int CountFile = 0;
		while (MayWeWork != -1)
		{
			if (strcmp(Find.name, ".") && strcmp(Find.name, "..")) //��������� �������� .  � ..
			{
				//��������� ���������� ��
				if (Find.attrib & _A_SUBDIR) //���� ������� �������� ����������
				{
					cout << "����������: \t|\t";
					CountDir++;
				}
				else
				{
					cout << "����: \t\t|" << Find.size << "\t";
					CountFile++;
				}
				if (Find.size <= 999999)cout << "\t";
				cout << "|" << Find.name << endl;
			}
			MayWeWork = _findnext(Done, &Find);
		}
		cout << "���-�� ������: " << CountFile << " ���-�� ����������: " << CountDir << endl;
		_findclose(Done);
	}
	void Rename_Dir()
	{
		string str, dest;
		cout << "������� ������ ��� �����" << endl;
		cin>> str;
		cout << "������� ����� ��� �����" << endl;
		cin>>dest;
		//�������������� � ��������
		if (rename((str.insert(0, path)).c_str(), (dest.insert(0, path)).c_str())) //���������� 0 ���� ������� � ��� ������ ���� ���
		{
			cout << "������ �������������� ����������! ��������� ������ � ����� ���� � ��� ����������!" << endl;
			return;
		}
		else cout << "���������� ������� �������������!" << endl;
	}
	void Mov_Dir()
	{
		string str, dest;
		cout << "������� ��� ���������� �����:";
		cin>>str;
		cout << "������� ����� ����:";
		cin>>dest;
		Copy_D(str.insert(0, path), dest);
		Del_D(str);
	}
	void Travel()
	{
		string choice;
		cout << "������� ��� �������� ��� Back"<<endl;
		cin>> choice;
		if (choice == "back")
		{
			path = (path.substr(path.rfind("\\")));
		}
		else
		{
			_finddata_t Find; //������ ��������� ��� �������� �����������
			string pathNext = choice.insert(0, path);			
			int Done = _findfirst(pathNext.c_str(), &Find);
			if (Done == -1)
			{
				cout << "����� ����� ���" << endl;
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
			cout << "crf   // �������� �����" << endl
				<< "crd   // �������� �����" << endl
				<< "cpf   // ����������� �����" << endl
				<< "cpd   // ����������� �����" << endl
				<< "rnf   // �������������� �����" << endl
				<< "rnd   // �������������� �����" << endl
				<< "delf  // �������� �����" << endl
				<< "deld  // �������� �����" << endl
				<< "movf  // ����������� �����" << endl
				<< "movd  // ����������� �����" << endl
				<< "cd    // ������� �� ������" << endl
				<< "show  // ���������� �����" << endl
				<< "exit  // �����" << endl;
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