#include "DBLayer.h"

DataBase* DataBase::_instance_ptr_db = nullptr;

DataBase::DataBase()
{
	this->_con = nullptr;
	this->_driver = nullptr;
	this->_hwnd = nullptr;
	this->_pstmt = nullptr;
	this->_result = nullptr;
}

DataBase::~DataBase()
{
	delete _con;
	delete _pstmt;
	delete _result;
}

string DataBase::WideToUTF8(const wchar_t* wideStr)
{
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	string result(utf8Size, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &result[0], utf8Size, nullptr, nullptr);
	return result.c_str();
}


void DataBase::DebugMessage(const char* Message1, const char* Message2)
{
	if (_hwnd != nullptr)
	{
		MessageBoxA(_hwnd, Message1, Message2, MB_OK);
	}
}

bool DataBase::ConnectToDB(bool Debugging)
{
	try
	{
		_driver = get_driver_instance();
		_con = _driver->connect(WideToUTF8(_settings->_mainSettings.server.c_str()), WideToUTF8(_settings->_mainSettings.userName.c_str()), WideToUTF8(_settings->_mainSettings.password.c_str()));
		if (Debugging)
			DebugMessage("�� ����������", "�������!");
		return true;
	}
	catch (SQLException e)
	{
		if (Debugging)
			DebugMessage("�� ������� ������������ � ������� ��!", "������!");
		return false;
	}
}

bool DataBase::LoadDatabase(bool Debugging)
{
	if (ConnectToDB())
	{
		Statement* pstmt = _con->createStatement();
		_result = pstmt->executeQuery("SHOW DATABASES");
		while (_result->next()) {
			if (_result->getString(1) == WideToUTF8(_settings->_mainSettings.databaseName.c_str())) {
				delete pstmt;
				_con->setSchema(WideToUTF8(_settings->_mainSettings.databaseName.c_str()));
				if (Debugging)
					DebugMessage(WideToUTF8(_settings->_mainSettings.databaseName.c_str()).c_str(), "����������");
				return true;
			}
		}
		if (Debugging)
			MessageBox(_hwnd, _settings->_mainSettings.databaseName.c_str(), L"�� �� ����������", MB_ICONERROR);
		return false;
	}
	return false;
}

bool DataBase::HasLogin(const string Login, bool Debugging)
{
	_pstmt = _con->prepareStatement("SELECT _login FROM _employees WHERE _login = '" + Login + "';");
	_result = _pstmt->executeQuery();
	string tmp;
	while (_result->next())
	{
		tmp += _result->getString(1);
	}
	if (Debugging)
	{
		tmp.size() > 0 ? DebugMessage(tmp.c_str(), "���� ����� ������������") : DebugMessage(Login.c_str(), "���� ������ ������ � ��");
	}
	return tmp.size() > 0;
}

string DataBase::GetPassword(string Login, bool Debugging)
{
	_pstmt = _con->prepareStatement("SELECT _password FROM _employees WHERE _login = '" + Login + "';");
	_result = _pstmt->executeQuery();
	string tmp;
	while (_result->next())
	{
		tmp += _result->getString(1);
	}
	if (Debugging)
		DebugMessage(tmp.c_str(), "����������� ������");
	return tmp;
}

bool DataBase::ValidateCredentials(const string Login, const string Password, bool Debugging)
{
	string Log, Pass;
	if (HasLogin(Login))
	{
		Log = Login;
		Pass = GetPassword(Login);
		if (Password == Pass)
		{
			if (Debugging)
				DebugMessage("", "���� ��������");
			return true;
		}
	}
	return false;
}

eRole DataBase::GetRole(string Login, bool Debugging)
{
	_pstmt = _con->prepareStatement("SELECT _role_id FROM _employees WHERE _login = '" + Login + "';");
	_result = _pstmt->executeQuery();
	int tmp = -1;
	if (_result->next())
	{
		tmp = _result->getInt(1);
		if (Debugging)
			DebugMessage("���� ���������", "�������");
	}

	switch (tmp)
	{
	case 1: return eRole::Administrator;
	case 2: return eRole::Manager;
	case 3: return eRole::Master;
	case 4: return eRole::Analitic;
	default: return eRole::ConnectedDB;
	}
}

int DataBase::GetId(string Login, bool Debugging)
{
	int tmp = -1;
	_pstmt = _con->prepareStatement("SELECT id FROM _employees WHERE _login = ?;");
	_pstmt->setString(1, Login);
	_result = _pstmt->executeQuery();
	while (_result->next())
	{
		tmp = _result->getInt(1);
	}
	if (Debugging)
	{
		tmp != -1 ? DebugMessage("ID �������", "������� ���������") : DebugMessage("������", "������ �������� ID �� �������");
	}
	return tmp;
}

string DataBase::SimpleHash(const string& input)
{
	hash<string> hasher;
	size_t hash = hasher(input);
	return to_string(hash);
}

bool DataBase::Connect(bool Debugging)
{
	return LoadDatabase(Debugging);
}

bool DataBase::RegisterUser(const string login, const string password, eRole role,
	string fio, string bithday, string pasport, string bank, bool Debugging)
{
	if (!_con || _con->isClosed())
	{
		if (Debugging)
		{
			MessageBox(nullptr, L"��� ��������� ���������� � ��", L"������!", MB_ICONERROR);
		}
		return false;
	}

	try
	{
		Statement* stmt = _con->createStatement();
		_result = stmt->executeQuery("SELECT _login FROM _employees WHERE _login = '" + login + "';");
		if (_result->next())
		{
			if (Debugging) {
				MessageBoxA(nullptr, "������������ � ����� ������� ��� ����������", "������!", MB_ICONERROR);
			}
			delete stmt;
			return false;
		}
		delete stmt;

		_pstmt = _con->prepareStatement("INSERT INTO _employees (_login, _password, _role_id, _fio, _birthData, _pasport, _bank) VALUES (?, ?, ?, ?, ?, ?, ?);");
		_pstmt->setString(1, login);
		_pstmt->setString(2, SimpleHash(password));
		_pstmt->setInt(3, role);
		_pstmt->setString(4, fio);
		_pstmt->setString(5, bithday);
		_pstmt->setString(6, pasport);
		_pstmt->setString(7, bank);
		_pstmt->executeUpdate();
		return true;
	}

	catch (const SQLException& e) {
		if (Debugging) {
			string errorMsg = "������ SQL [" + std::to_string(e.getErrorCode()) + "]: " + e.what();
			MessageBoxA(nullptr, errorMsg.c_str(), "������ ����������", MB_ICONERROR);
		}
		return false;
	}

	catch (...) {
		if (Debugging) {
			MessageBox(nullptr, L"����������� ������ ��� ���������� ������������", L"������!", MB_ICONERROR);
		}
		return false;
	}
}

bool DataBase::HasAdministrator(bool Debugging)
{
	bool out = false;
	_pstmt = _con->prepareStatement("SELECT _login FROM _employees WHERE _role_id = 1");
	_result = _pstmt->executeQuery();
	string tmp;
	while (_result->next())
	{
		tmp += _result->getString(1);
	}

	out = tmp.size() > 0;
	if (Debugging)
	{
		if (out)
			DebugMessage("���������", "����");
		else DebugMessage("�� ���������", "����");
	}
	return out;
}

bool DataBase::InLogin(bool Debugging)
{
	if (ValidateCredentials(WideToUTF8(_settings->_userSettings.login.c_str()), WideToUTF8(_settings->_userSettings.password.c_str()), Debugging))
	{
		_settings->_userSettings.role = GetRole(WideToUTF8(_settings->_userSettings.login.c_str()), Debugging);
		_settings->_userSettings.inLogin = true;
		return true;
	}
	return false;
}

vector<string> DataBase::LoadTitle(string table, bool Debugging)
{
	vector<string> tmp;
	_pstmt = _con->prepareStatement("SELECT _title from " + table + " ORDER BY id;");
	_result = _pstmt->executeQuery();

	while (_result->next())
	{
		tmp.push_back(_result->getString(1) + "\r\n");
	}
	if (Debugging)
	{
		tmp.size() > 0 ? DebugMessage(table.c_str(), "������� ���������") : DebugMessage(table.c_str(), "������ �������� �������");
	}
	return tmp;
}
