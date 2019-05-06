#ifndef check_box_hpp
#define check_box_hpp

#include <stdio.h>
#include <Siv3D.hpp> // OpenSiv3D v0.3.1

class CheckBox {
    
public:
    CheckBox() = default;
    
    CheckBox(const RectF& argRect);
    
    void draw(const ColorF& argColorBase, const ColorF& argColorBaseUnderCursor, const ColorF& argColorFrame, const Font& argFont);
    
    RectF& getRect();
    
    bool getStatus();
    
private:
    bool mStatus = false;
    RectF mRect;
};

#endif /* check_box_hpp */

