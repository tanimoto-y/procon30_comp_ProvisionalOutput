#include "button.hpp"
    
Button::Button(const String& argLabel, const RectF& argRect) {
    mRect = argRect;
    mLabel = argLabel;
}

void Button::draw(const ColorF& argColorBase, const ColorF& argColorBaseUnderCursor, const ColorF& argColorFrame, const Font& argFont) {
    if (mRect.mouseOver()) {
        mRect.draw(argColorBaseUnderCursor).drawFrame(1, 0, argColorFrame);
    }
    else {
        mRect.draw(argColorBase).drawFrame(1, 0, argColorFrame);
    }
    
    argFont(mLabel).drawAt(mRect.center(), ColorF(Palette::Black));
    
    if (mRect.leftClicked()) {
        mStatus = true;
    }
}
    
RectF& Button::getRect() {
    return mRect;
}
    
String& Button::getLabel() {
    return mLabel;
}

bool Button::getStatus() {
    if (mStatus) {
        mStatus = false;
        return true;
    }
    
    return mStatus;
}
