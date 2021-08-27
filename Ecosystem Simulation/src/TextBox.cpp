#include "pch.h"
#include "TextBox.h"

gui::TextBox::TextBox(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Font& font, 
    const std::string& defaultStr, 
    float charSize,
    const sf::Color& idleColor, 
    const sf::Color& hoveredColor,
    const sf::Color& pressedColor,
    const sf::Color& outlineIdleColor, 
    const sf::Color& outlineHoveredColor, 
    const sf::Color& outlinePressedColor,
    const sf::Color& textIdleColor, 
    const sf::Color& textHoveredColor, 
    const sf::Color& textPressedColor,
    float outlineThickness, 
    float cursorWidth, 
    float cursorFrequency,
    bool turnedOn, 
    int id)
    : m_rect()
    , m_text(defaultStr, font, charSize)
    , m_input(defaultStr)
    , m_cursor(sf::Vector2f(cursorWidth, charSize))
    , m_stopwatch(0.0f)
    , m_cursorFrequency(cursorFrequency)
    , m_cursorIsRendered(false)
    , m_colors()
    , m_turnedOnBlockade(false)
    , m_turnedOn(turnedOn)
    , m_hasBeenTurnedOn(false)
    , m_id(id)
    , m_state("IDLE")
    , m_textHasChanged(false)
    , m_stateHasChanged(false)
{
    initRect(position, size, idleColor, outlineIdleColor, outlineThickness);
    initText(textIdleColor);
    initCursor(textIdleColor);
    initColors(
        idleColor, hoveredColor, pressedColor,
        outlineIdleColor, outlineHoveredColor, outlinePressedColor,
        textIdleColor, textHoveredColor, textPressedColor
    );
}

void gui::TextBox::update(
    float dt,
    const std::vector<sf::Event>& events,
    const sf::Vector2i& mousePosWindow)
{
    updateState(events, mousePosWindow);

    if (m_stateHasChanged)
    {
        updateColors();
    }

    if (!m_turnedOn) return;

    handleTextEntering(events);

    if (m_textHasChanged)
    {
        updateTextPosition();
        updateCursorPosition();
    }

    updateCursorVisibility(dt);
}

void gui::TextBox::render(sf::RenderTarget& target)
{
    target.draw(m_rect);
    target.draw(m_text);

    if (m_cursorIsRendered)
    {
        target.draw(m_cursor);
    }
}

// accessors:

int gui::TextBox::getId() const
{
    return m_id;
}

const std::string& gui::TextBox::getInput() const
{
    return m_input;
}

bool gui::TextBox::isTurnedOn() const
{
    return m_turnedOn;
}

bool gui::TextBox::hasBeenTurnedOn() const
{
    return m_hasBeenTurnedOn;
}

// mutators:

void gui::TextBox::turnOn()
{
    m_turnedOn = true;
}

void gui::TextBox::turnOff()
{
    m_turnedOn = false;

    m_cursor.setFillColor(sf::Color::Transparent);
}

void gui::TextBox::setString(const std::string& string)
{
    m_text.setString(string);
    updateTextPosition();
    updateCursorPosition();
    m_input = string;
}

// private methods:

// initialization:

void gui::TextBox::initRect(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Color& idleColor,
    const sf::Color& outlineIdleColor,
    float outlineThickness)
{
    m_rect.setPosition(
        sf::Vector2f(
            position.x + outlineThickness,
            position.y + outlineThickness
        )
    );
    m_rect.setSize(
        sf::Vector2f(
            size.x - 2.0f * outlineThickness,
            size.y - 2.0f * outlineThickness
        )
    );
    m_rect.setFillColor(idleColor);

    m_rect.setOutlineThickness(outlineThickness);
    m_rect.setOutlineColor(outlineIdleColor);
}

void gui::TextBox::initText(const sf::Color& textIdleColor)
{
    m_text.setFillColor(textIdleColor);

    sf::FloatRect textGlobalBounds = m_text.getGlobalBounds();
    sf::FloatRect textLocalBounds = m_text.getLocalBounds();

    m_text.setOrigin(
        textGlobalBounds.width / 2.f,
        textLocalBounds.top + textLocalBounds.height / 2.f
    );

    sf::FloatRect rectGlobalBounds = m_rect.getGlobalBounds();

    m_text.setPosition(
        rectGlobalBounds.left + rectGlobalBounds.width / 2.f,
        rectGlobalBounds.top +  rectGlobalBounds.height / 2.f
    );
}

void gui::TextBox::initCursor(const sf::Color& textIdleColor)
{
    m_cursor.setOrigin(
        0.0f, 
        m_text.getCharacterSize() / 2.0f
    );

    m_cursor.setFillColor(textIdleColor);
    
    sf::FloatRect textBounds = m_text.getGlobalBounds();
    sf::FloatRect rectBounds = m_rect.getGlobalBounds();

    m_cursor.setPosition(
        textBounds.left + textBounds.width + m_cursor.getSize().x,
        rectBounds.top + rectBounds.height / 2.f
    );
}

void gui::TextBox::initColors(
    const sf::Color& idleColor, 
    const sf::Color& hoveredColor, 
    const sf::Color& pressedColor,
    const sf::Color& outlineIdleColor, 
    const sf::Color& outlineHoveredColor, 
    const sf::Color& outlinePressedColor,
    const sf::Color& textIdleColor, 
    const sf::Color& textHoveredColor, 
    const sf::Color& textPressedColor)
{
    m_colors["INPUT FIELD"]["IDLE"] = idleColor;
    m_colors["INPUT FIELD"]["HOVERED"] = hoveredColor;
    m_colors["INPUT FIELD"]["PRESSED"] = pressedColor;

    m_colors["OUTLINE"]["IDLE"] = outlineIdleColor;
    m_colors["OUTLINE"]["HOVERED"] = outlineHoveredColor;
    m_colors["OUTLINE"]["PRESSED"] = outlinePressedColor;

    m_colors["TEXT"]["IDLE"] = textIdleColor;
    m_colors["TEXT"]["HOVERED"] = textHoveredColor;
    m_colors["TEXT"]["PRESSED"] = textPressedColor;
}

// utils:

void gui::TextBox::updateState(
    const std::vector<sf::Event>& events, 
    const sf::Vector2i& mousePosWindow)
{
    m_hasBeenTurnedOn = false;
    m_stateHasChanged = false;

    auto mousePosWindowF = static_cast<sf::Vector2f>(mousePosWindow);

    if (m_rect.getGlobalBounds().contains(mousePosWindowF))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!m_turnedOnBlockade)
            {
                if (m_state != "PRESSED")
                {
                    m_stateHasChanged = true;
                    m_state = "PRESSED";
                }

                m_turnedOnBlockade = true;

                m_turnedOn = !m_turnedOn;

                if (m_turnedOn)
                {
                    m_hasBeenTurnedOn = true;
                }
                else
                {
                    m_cursorIsRendered = false;
                }
            }
        }
        else
        {
            m_turnedOnBlockade = false;

            if (m_state != "HOVERED")
            {
                m_stateHasChanged = true;
                m_state = "HOVERED";
            }
        }
    }
    else
    {
        if (m_state != "IDLE")
        {
            m_stateHasChanged = true;
            m_state = "IDLE";
        }
    }
}

void gui::TextBox::updateColors()
{
    m_rect.setFillColor(m_colors["INPUT FIELD"][m_state]);

    m_rect.setOutlineColor(m_colors["OUTLINE"][m_state]);

    m_text.setFillColor(m_colors["TEXT"][m_state]);

    m_cursor.setFillColor(m_colors["TEXT"][m_state]);
}

void gui::TextBox::handleTextEntering(const std::vector<sf::Event>& events)
{
    m_textHasChanged = false;

    for (const auto& event : events)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == 8) // backspace
            {
                if (!m_input.empty())
                {
                    m_textHasChanged = true;
                    m_input.pop_back();
                    m_text.setString(m_input);
                }
            }
            if (event.text.unicode > 31)
            {
                m_textHasChanged = true;
                m_input.push_back(event.text.unicode);
                m_text.setString(m_input);
            }
        }
    }
}

void gui::TextBox::updateTextPosition()
{
    sf::FloatRect textGlobalBounds = m_text.getGlobalBounds();
    sf::FloatRect textLocalBounds = m_text.getLocalBounds();
    
    m_text.setOrigin(
        textGlobalBounds.width / 2.f,
        textLocalBounds.top + textLocalBounds.height / 2.f
    );

    sf::FloatRect rectGlobalBounds = m_rect.getGlobalBounds();
    
    m_text.setPosition(
        rectGlobalBounds.left + rectGlobalBounds.width / 2.f,
        rectGlobalBounds.top  + rectGlobalBounds.height / 2.f
    );
}

void gui::TextBox::updateCursorPosition()
{
    sf::FloatRect textGlobalBounds = m_text.getGlobalBounds();
    sf::FloatRect rectGlobalBounds = m_rect.getGlobalBounds();

    m_cursor.setPosition(
        textGlobalBounds.left + textGlobalBounds.width + m_cursor.getSize().x,
        rectGlobalBounds.top + rectGlobalBounds.height / 2.f
    );
}

void gui::TextBox::updateCursorVisibility(float dt)
{
    m_stopwatch += dt;

    if (m_stopwatch > m_cursorFrequency)
    {
        m_cursorIsRendered = !m_cursorIsRendered;
        m_stopwatch = 0.f;
    }
}
