#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Visible.h"

class Interactive: virtual public Visible
{
public:

    enum class Direction
    {
        In,
        Out
    };
    enum class Event
    {
        Hover,
        Press,
        Release
    };
    
    Interactive();
    ~Interactive();
    virtual sf::FloatRect getGlobalBounds() const = 0;
    virtual void setHoverBehavior(void (*hoverBehavior)(Direction, void*));
    virtual void setPressBehavior(void (*pressBehavior)(Direction, void*));
    void setAction(void (*action)(void*));
    void setBehaviorContextData(void* behaviorContexData);
    void* getBehaviorContextData();
    void onHover(int x, int y);
    void onPress();
    void onRelease();
    bool isHovered() const;
    bool isPressed() const;
    static void updateInteractives(Event event, int mouseX = -1, int mouseY = -1);

protected:
    
    static std::vector<Interactive*> interactives;
    bool hovered;
    bool pressed;
    void (*onHoverBehavior)(Direction, void*);
    void (*onPressBehavior)(Direction, void*);
    void (*trigger)(void*);
    void* behaviorContextData;
};
