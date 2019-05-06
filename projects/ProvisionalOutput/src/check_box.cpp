#include "check_box.hpp"

CheckBox::CheckBox(const RectF& argRect) {
    mRect = argRect;
}

void CheckBox::draw(const ColorF& argColorBase, const ColorF& argColorBaseUnderCursor, const ColorF& argColorFrame, const Font& argFont) {
    if (mRect.mouseOver()) {
        mRect.draw(argColorBaseUnderCursor).drawFrame(1, 0, argColorFrame);
    }
    else {
        mRect.draw(argColorBase).drawFrame(1, 0, argColorFrame);
    }
    
    if (mStatus) {
        argFont(U"✓").drawAt(mRect.center(), ColorF(Palette::Black));
    }
    
    if (mRect.leftClicked()) {
        if (!mStatus) {
            mStatus = true;
        }
        else {
            mStatus = false;
        }
    }
}

RectF& CheckBox::getRect() {
    return mRect;
}

bool CheckBox::getStatus() {
    return mStatus;
}

