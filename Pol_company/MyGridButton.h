#pragma once
#include "MyWindow.h"
#include <vector>

class ScrollGridButtons {
private:
    HWND _hScrollWnd = nullptr;
    HWND _hParent = nullptr;
    MyButton* _background = nullptr;
    vector<vector<MyButton*>> _buttons; // Двумерный массив кнопок
    int _buttonWidth = 100;
    int _buttonHeight = 50;
    int _columns = 1;
    int _spacing = 10;
    int _scrollPos = 0;
    int _totalHeight = 0;
    int _xPos = 0;
    int _yPos = 0;
    int _scrolX, _scrolY;
    int _scrolHeight, _scrolWidth;

public:
    ScrollGridButtons(HWND parent, int x, int y, int width, int height, int buttonWidth = 100, int buttonHeight = 50, int columns = 1, int spacing = 10)
        : _hParent(parent), _buttonWidth(buttonWidth), _buttonHeight(buttonHeight), _columns(columns), 
        _spacing(spacing), _scrolX(x), _scrolY(y), _scrolHeight(height), _scrolWidth(width){
        // Создаем окно скролла
        _hScrollWnd = CreateWindowEx(
            0,
            L"SCROLLBAR",
            L"",
            WS_CHILD | WS_VISIBLE | SBS_VERT,
            x, y, width, height,
            parent,
            nullptr,
            (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
            nullptr);

        // Устанавливаем параметры скролла
        SCROLLINFO si = { sizeof(SCROLLINFO) };
        si.fMask = SIF_PAGE | SIF_RANGE;
        si.nMin = 0;
        si.nMax = 0;
        si.nPage = height;
        SetScrollInfo(_hScrollWnd, SB_CTL, &si, TRUE);
    }

    ~ScrollGridButtons() {
        ClearButtons();
        if (_hScrollWnd) DestroyWindow(_hScrollWnd);
    }

    // Новый метод для перемещения скролла и его содержимого
    void MoveTo(int newX, int newY) {
        _xPos = newX;
        _yPos = newY;

        if(_background)
        {
            DeleteObject(_background);
            _background = nullptr;
        }

        _background = MyButton::Create().
            parent(_hParent).
            text(L"").
            coordSize(_xPos, _yPos, _scrolX-_xPos, _scrolHeight).
            id(5000).
            backGroundColor(SColor::AdditionalColor()).
            textColor(SColor()).
            outline(false).
            Build();
        EnableWindow(_background->GetHandle(), false);


        RepositionButtons();
    }

    void HandleMouseWheel(WPARAM wParam) {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        int scrollSpeed = 20; // Скорость прокрутки (можно регулировать)

        // Направление прокрутки (обратите внимание на знак минус)
        int scrollDirection = (delta > 0) ? -1 : 1;

        // Рассчитываем смещение с учетом скорости
        int scrollOffset = scrollDirection * scrollSpeed;

        SCROLLINFO si = { sizeof(SCROLLINFO) };
        si.fMask = SIF_POS | SIF_RANGE | SIF_PAGE;
        GetScrollInfo(_hScrollWnd, SB_CTL, &si);

        int newPos = si.nPos + scrollOffset;
        newPos = max(si.nMin, min(newPos, si.nMax - (int)si.nPage));

        if (newPos != _scrollPos) {
            _scrollPos = newPos;
            SetScrollPos(_hScrollWnd, SB_CTL, _scrollPos, TRUE);
            RepositionButtons();
        }
    }

    void AddButton(MyButton* button, int row, int col) {
        // Увеличиваем массив при необходимости
        if (row >= _buttons.size()) {
            _buttons.resize(row + 1);
        }
        if (col >= _buttons[row].size()) {
            _buttons[row].resize(col + 1, nullptr);
        }

        _buttons[row][col] = button;
        UpdateLayout();
    }

    void ClearButtons() {
        for (auto& row : _buttons) {
            for (auto& button : row) {
                if (button) {
                    delete button;
                    button = nullptr;
                }
            }
        }
        _buttons.clear();
        _scrollPos = 0;
        _totalHeight = 0;
        UpdateScrollInfo();
    }

    void UpdateLayout() {
        if (_buttons.empty()) {
            _totalHeight = 0;
        }
        else {
            // Высота = количество строк * (высота кнопки + отступ) + дополнительный отступ сверху
            _totalHeight = static_cast<int>(_buttons.size()) * (_buttonHeight + _spacing) + _spacing;
        }

        UpdateScrollInfo();
        RepositionButtons();
    }

    void RepositionButtons() {
        int visibleHeight = _scrolHeight - _buttonHeight;

        for (size_t row = 0; row < _buttons.size(); ++row) {
            for (size_t col = 0; col < _buttons[row].size(); ++col) {
                if (_buttons[row][col]) {
                    int x = _xPos + col * (_buttonWidth + _spacing) + _spacing;
                    int y = _yPos + row * (_buttonHeight + _spacing) + _spacing - _scrollPos;

                    bool isVisible = (y + _buttonHeight > _yPos) && (y < _yPos + visibleHeight);
                    SetWindowPos(
                        _buttons[row][col]->GetHandle(),
                        nullptr,
                        x, y,
                        _buttonWidth, _buttonHeight,
                        SWP_NOZORDER | (isVisible ? SWP_SHOWWINDOW : SWP_HIDEWINDOW));
                }
            }
        }
    }

    void UpdateScrollInfo() {
        SCROLLINFO si = { sizeof(SCROLLINFO) };
        si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
        si.nMin = 0;
        si.nMax = _totalHeight;
        si.nPage = _buttonHeight * 5; // Примерная высота видимой области
        si.nPos = _scrollPos;
        SetScrollInfo(_hScrollWnd, SB_CTL, &si, TRUE);
    }

    void HandleScroll(WPARAM wParam) {
        SCROLLINFO si = { sizeof(SCROLLINFO) };
        si.fMask = SIF_ALL;
        GetScrollInfo(_hScrollWnd, SB_CTL, &si);

        int newPos = si.nPos;
        switch (LOWORD(wParam)) {
        case SB_LINEUP: newPos -= 10; break;
        case SB_LINEDOWN: newPos += 10; break;
        case SB_PAGEUP: newPos -= si.nPage; break;
        case SB_PAGEDOWN: newPos += si.nPage; break;
        case SB_THUMBTRACK: newPos = si.nTrackPos; break;
        default: break;
        }

        newPos = max(si.nMin, min(newPos, si.nMax - (int)si.nPage));
        if (newPos != _scrollPos) {
            _scrollPos = newPos;
            SetScrollPos(_hScrollWnd, SB_CTL, _scrollPos, TRUE);
            RepositionButtons();
        }
    }

    HWND GetScrollHandle() const { return _hScrollWnd; }
};
