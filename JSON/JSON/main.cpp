#include "rapidjson-master/include/rapidjson/document.h"
#include "rapidjson-master/include/rapidjson/filereadstream.h"

#include <iostream>
#include <io.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include <cstdio>
#include <conio.h>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;
using namespace rapidjson;

struct MyJsonData
{
	wstring fileName;
	int color;
};

wchar_t g_wcharJsonPath[255];
wstring g_wstrJsonPath;
string g_strJsonPath;
wstring g_strJsonDirectory = L"\\";
vector<wstring> g_curJsonList;
vector<wstring> g_prevJsonList;
int g_curSize;
int g_curSelNumber;
int g_prevSelNumber = 0;
map<int, string> g_mapData;
map<wstring,int> g_mapOuputJsonList;

void ShowJsonFile();

void GetJsonPath()
{
	wchar_t _tempPath[255];
	GetCurrentDirectory(255, _tempPath);
	GetCurrentDirectory(255, g_wcharJsonPath);
	int _len = wcslen(_tempPath);

	wcscat_s(_tempPath, 255, L"\\setting.ini");

	ifstream ifs(_tempPath);
	char c;
	while (ifs.get(c))
	{
		g_strJsonDirectory += c;
	}
	g_strJsonDirectory += L"\\*.json*";

	wcscat_s(g_wcharJsonPath, 255, g_strJsonDirectory.c_str());

	for (const auto& i : g_wcharJsonPath)
	{
		g_strJsonPath += i;
	}
}

void SetJsonList()
{
	_finddata_t fd;
	intptr_t handle;
	wstring strTemp;

	handle = _findfirst(g_strJsonPath.c_str(), &fd);
	g_curJsonList.clear();
	do
	{
		strTemp.clear();
		for (const auto& i : fd.name)
		{
			strTemp += i;
			if (i == '\0')
			{
				break;
			}
		}
		g_curJsonList.push_back(strTemp);
	} while (_findnext(handle, &fd) == 0);

	g_mapOuputJsonList.clear();

	if (g_prevJsonList.size() != 0)
	{
		for (const auto& i : g_curJsonList)
		{
			auto iter = find(g_prevJsonList.begin(), g_prevJsonList.end(), i);
			// ���� �߰���
			if (iter == g_prevJsonList.end())
			{
				g_mapOuputJsonList.insert({ i,2 });
			}
			else
			{
				// ���� �־���
				g_mapOuputJsonList.insert({ i,7 });
			}
		}

		for (const auto& i : g_prevJsonList)
		{
			auto iter = find(g_curJsonList.begin(), g_curJsonList.end(), i);
			// ������
			if (iter == g_curJsonList.end())
			{
				g_mapOuputJsonList.insert({ i,4 });
			}
		}
	}
	else
	{
		for (const auto& i : g_curJsonList)
		{
			// ���� �־���
			g_mapOuputJsonList.insert({ i,7 });
		}
	}

	g_prevJsonList = g_curJsonList;
}

string ParseJsonToString()
{
	wchar_t _charJsonPath[255];
	GetCurrentDirectory(255, _charJsonPath);
	wcscat_s(_charJsonPath, 255, L"\\Data\\");

	wcscat_s(_charJsonPath, 255, g_curJsonList[g_curSelNumber - 1].c_str());

	ifstream ifs(_charJsonPath);

	Document document;
	std::string jsonStr((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	document.Parse(jsonStr.c_str());

	const char* data = document["string"].GetString();
	int size = document["string"].GetStringLength();
	string str;

	for (int i = 0; i < size; ++i)
	{
		str += data[i];
	}

	return str;
}

void ShowAndSelectJsonList()
{
	// ���丮 �ȿ� �ִ� ��� Json������ �̸��� ����Ʈ�� �����Ѵ�.
	SetJsonList();

	g_curSize = g_mapOuputJsonList.size();

	//for (int i = 0; i < g_curSize; ++i)
	//{
	//	wstring temp = g_curJsonList[i];
	//
	//	int wstrLen = temp.size();
	//
	//	cout << i + 1 << " : ";
	//
	//	for (int j = 0; j < wstrLen; ++j)
	//	{
	//		cout << (char)temp[j];
	//	}
	//	cout << endl;
	//}
	int _count = 0;
	for (const auto& i : g_mapOuputJsonList)
	{
		wstring temp = i.first;

		int wstrLen = temp.size();

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << _count + 1 << " : ";

		for (int j = 0; j < wstrLen; ++j)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), i.second);
			cout << (char)temp[j];
		}
		cout << endl;
		_count++;
	}

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout << "������� ������ �������ּ��� : ";

	cin >> g_curSelNumber;

	if (g_curSelNumber > g_curSize)
	{
		cout << endl << "�Է��� ���ڰ� �̻��մϴ�. �ٽ� �Է����ּ��� : ";
		cin >> g_curSelNumber;
		string str = ParseJsonToString();
		cout << str << endl << "===========================================================================================" << endl;
		cout << "���� �������� ���ư����� ���� ��������.";
	}
	else
	{
		system("cls");
		string str = ParseJsonToString();
		cout << str << endl << "===========================================================================================" << endl;
		cout << "���� �������� ���ư����� ���� ��������.";
	}

	// ������ Json������ ��� �����ش�.
	ShowJsonFile();
}

void ShowJsonFile()
{
	wchar_t _charJsonPath[255];
	GetCurrentDirectory(255, _charJsonPath);
	wcscat_s(_charJsonPath, 255, L"\\Data\\");

	wcscat_s(_charJsonPath, 255, g_curJsonList[g_curSelNumber - 1].c_str());

	std::filesystem::path file_path = _charJsonPath;
	std::filesystem::file_time_type last_write_time = std::filesystem::last_write_time(_charJsonPath);

	while (true)
	{
		std::filesystem::file_time_type new_write_time = std::filesystem::last_write_time(_charJsonPath);
		if (new_write_time != last_write_time)
		{
			last_write_time = new_write_time;
			system("cls");
			for (size_t i = 0; i < g_curJsonList[g_curSelNumber - 1].size(); ++i)
			{
				cout << (char)g_curJsonList[g_curSelNumber - 1][i];
			}
			cout << " ������ �����Ǿ����ϴ�. �����̽��ٸ� ������ �ٽ� �ε� �մϴ�.";
			char c = _getch();
			if (c == ' ')
			{
				system("cls");

				cout << g_mapData[g_curSelNumber - 1];

				string str = ParseJsonToString();
				cout << str << endl << "===========================================================================================" << endl;
				cout << "���� �������� ���ư����� ���� ��������.";
				g_mapData.insert({ g_curSelNumber - 1, str });
			}
		}
		else
		{
			if (GetAsyncKeyState(VK_TAB) & 0x8000)
			{
				// ���丮 �ȿ� �ִ� ��� Json������ �����ش�.
				system("cls");
				
				ShowAndSelectJsonList();
			}
		}
	}
}

void main()
{
	// Json ���ϵ��� �ִ� ���丮 �ּҸ� ���´�.
	GetJsonPath();
	
	// ���丮 �ȿ� �ִ� ��� Json������ �����ְ� �����Ѵ�.
	ShowAndSelectJsonList();
}