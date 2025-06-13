#pragma once
#include <Windows.h>
#include <vector>
#include <string.h>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "Settings.h"
using namespace std;
using namespace sql;

struct STable
{
	int id;
	std::string tableLine;

	STable()
	{
		id = -1;
		tableLine = "";
	}
};

class DataBase {

public:
	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;

	static void ReleaseInstance() {
		if (_instance_ptr_db)
			delete _instance_ptr_db;
		_instance_ptr_db = nullptr;
	}

	static DataBase* GetInstance()
	{
		if (_instance_ptr_db == nullptr)
			_instance_ptr_db = new DataBase();
		return _instance_ptr_db;
	}

	void SetParentWindow(HWND hwnd)
	{
		if (hwnd != nullptr)
		{
			if (_hwnd == nullptr)
				delete _hwnd;
			_hwnd = nullptr;
			_hwnd = hwnd;
		}
	}

	string SimpleHash(const string& input);
	bool Connect(bool Debugging = false);
	bool RegisterUser(const string login, const string password, eRole role,
		string fname, string lname, string pname, string email, bool Debugging = false);
	bool HasAdministrator(bool Debugging = false);
	bool InLogin(bool Debugging = false);
	vector<STable> LoadTablePartner(int id = -1, bool Debugging = false);
	vector<string> LoadTitle(string table, bool Debugging = false);
	bool RegisterPartner(string name, string director, string email,
		string number, string adress, string inn, string rating, int type, bool Debugging = false);
	bool DeletePartner(int id, bool Debugging = false);
	bool UpdateRating(string rating, int id, bool Debugging = false);
	vector<STable> LoadTableOrder(int id = -1, bool Debugging = false);
	vector<STable> LoadTableTitle(string table, bool Debugging = false);
	bool UpdateStatusOrder(int idStatus, int id, bool Debugging = false);
	bool CreateOrder(int partner_id, int manager_id, int product_id, int status_id, string total_amount, bool Debugging = false);
	int GetIdUser(string Login, bool Debugging = false);
	bool DeleteOrder(int id, bool Debugging = false);

private:
	static DataBase* _instance_ptr_db;
	HWND _hwnd;
	Settings* _settings = Settings::GetInstance();
	Driver* _driver;
	Connection* _con;
	PreparedStatement* _pstmt;
	ResultSet* _result;

	DataBase();
	~DataBase();

	string WideToUTF8(const wchar_t* wideStr);
	void DebugMessage(const char* Message1, const char* Message2);
	bool ConnectToDB(bool Debugging = false);
	bool LoadDatabase(bool Debugging = false);
	bool HasLogin(const string Login, bool Debugging = false);
	string GetPassword(string Login, bool Debugging = false);
	bool ValidateCredentials(const string Login, const string Password, bool Debugging = false);
	eRole GetRole(string Login, bool Debugging = false);
	vector<int> LoadTableIndex(string table, bool Debugging = false);
	int GetOrderStatus(int id) 
	{
		int Out;
		_pstmt = _con->prepareStatement("SELECT s.id FROM _orders o INNER JOIN _status s ON s.id = o._status_id WHERE o.id = ?;");
		_pstmt->setInt(1, id);
		_result = _pstmt->executeQuery();

		for (int i = 0; _result->next(); i++)
		{
			Out = _result->getInt(1);
		}
		return Out;
	}
};