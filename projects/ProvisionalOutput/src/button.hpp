#ifndef button_hpp
#define button_hpp

#include <stdio.h>
#include <Siv3D.hpp> // OpenSiv3D v0.3.1

class Button {
    
public:
    Button() = default;
    
    Button(const String& argLabel, const RectF& argRect);
    
    void draw(const ColorF& argColorBase, const ColorF& argColorBaseUnderCursor, const ColorF& argColorFrame, const Font& argFont);
    
    RectF& getRect();
    
    String& getLabel();
    
    bool getStatus();
    
private:
    bool mStatus = false;
    String mLabel;
    RectF mRect;
};

#endif /* button_hpp */
