#pragma once
#include <string>
#include <map>
#include "CoreTypes.h"

using namespace std;

class MyButton {
public:
	MyButton() : _x(0), _y(0), _width(0), _height(0), _id(0) {}
	~MyButton() {
		if (_hBrush) DeleteObject(_hBrush);
		if (_hFont) DeleteObject(_hFont);
	}

	MyButton(const MyButton&) = delete;
	MyButton& operator=(const MyButton&) = delete;

	//Инициализация кнопки
	static class Builder {
	private:
		MyButton* _initButton;

	public:
		Builder() : _initButton(new MyButton()) {}
		~Builder()
		{
			delete _initButton;
		}

		Builder& parent(HWND parent) {
			_initButton->_parent = parent;
			return *this;
		}

		Builder& hBrush(HBRUSH hBrush) {
			_initButton->_hBrush = hBrush;
			return *this;
		}

		Builder& text(wstring text) {
			_initButton->_text = text;
			return *this;
		}

		Builder& coordSize(int x, int y, int width, int height) {
			_initButton->_x = x;
			_initButton->_y = y;
			_initButton->_width = width;
			_initButton->_height = height;
			return *this;
		}

		Builder& backGroundColor(SColor bgColor) {
			_initButton->_bgColor = bgColor;
			return *this;
		}

		Builder& textColor(SColor textColor) {
			_initButton->_textColor = textColor;
			return *this;
		}

		Builder& font(HFONT hFont) {
			_initButton->_hFont = hFont;
			return *this;
		}

		Builder& outline(bool outLine) {
			_initButton->_outLine = outLine;
			return *this;
		}

		Builder& id(int id) {
			_initButton->_id = id;
			return *this;
		}

		bool CanCreate() const
		{
			return _initButton &&
				_initButton->_parent &&
				_initButton->_width > 0 &&
				_initButton->_height > 0;
		}

		MyButton* Build()
		{
			if (CanCreate())
			{
				_initButton->UpdateBrush();
				_initButton->CreateButton();
				MyButton* result = _initButton;
				_initButton = nullptr;
				return result;
			}
			return nullptr;
		}
	};

	static Builder Create() {
		return Builder();
	}

public:
	void SetBackgroundColor(const SColor& newColor);
	void SetColorText(const SColor& newColor);
	void SetText(const wstring& newText);
	void DrawButton(DRAWITEMSTRUCT* pDraw);
	HWND GetHandle() const { return _hWnd; }
	int GetId() const { return _id; }

private:
	HWND _hWnd = nullptr;
	HWND _parent = nullptr;
	HBRUSH _hBrush = nullptr;
	wstring _text;
	int _x, _y, _width, _height, _id;
	SColor _bgColor;
	SColor _textColor = SColor(0, 0, 0);
	HFONT _hFont = nullptr;
	bool _outLine = false;

private:
	void Redraw();
	void UpdateBrush();
	void CreateButton();
};