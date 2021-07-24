#include "pch.h"
#include "InputField.h"

using namespace gui;

InputField::InputField(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Font& font, const std::string& default_str, float char_size,
    const sf::Color& idle_color, const sf::Color& hovered_color, const sf::Color& pressed_color,
    const sf::Color& outline_idle_color, const sf::Color& outline_hovered_color, const sf::Color& outline_pressed_color,
    const sf::Color& text_idle_color, const sf::Color& text_hovered_color, const sf::Color& text_pressed_color,
    float outline_thickness, float cursor_width, float cursor_frequency,
    bool turned_on, int id)
    : font(font), charSize(char_size), input(default_str),
      stopwatch(0.f), cursorFrequency(cursor_frequency), cursorIsRendered(false),
      turnedOnBlockade(false), turnedOn(turned_on), m_HasBeenTurnedOn(false), id(id),
      state("IDLE"),
      textHasChanged(false), stateHasChanged(false)
{
    this->initRect(position, size, idle_color, outline_idle_color, outline_thickness);
    this->initText(default_str, char_size, text_idle_color);
    this->initCursor(cursor_width, text_idle_color);
    this->initColors(idle_color, hovered_color, pressed_color,
        outline_idle_color, outline_hovered_color, outline_pressed_color,
        text_idle_color, text_hovered_color, text_pressed_color
    );
}

// public methods:
void InputField::update(
    float dt,
    const std::vector<sf::Event>& events,
    const sf::Vector2i& mouse_pos_window)
{
    this->updateState(events, mouse_pos_window);

    if (this->stateHasChanged)
        this->updateColors();

    if (!this->turnedOn)
        return;

    this->handleTextEntering(events);

    if (this->textHasChanged)
    {
        this->updateTextPosition();
        this->updateCursorPosition();
    }

    this->updateCursorVisibility(dt);
}

void InputField::render(sf::RenderTarget& target)
{
    target.draw(this->rect);
    target.draw(this->text);

    if (this->cursorIsRendered)
        target.draw(this->cursor);
}

// accessors:
int InputField::getId() const
{
    return this->id;
}

const std::string& InputField::getInput() const
{
    return this->input;
}

bool InputField::isTurnedOn() const
{
    return this->turnedOn;
}

bool InputField::hasBeenTurnedOn() const
{
    return this->m_HasBeenTurnedOn;
}

// mutators:
void InputField::turnOn()
{
    this->turnedOn = true;
}

void InputField::turnOff()
{
    this->turnedOn = false;

    this->cursor.setFillColor(sf::Color::Transparent);
}

void InputField::setString(const std::string& string)
{
    this->text.setString(string);
    this->updateTextPosition();
    this->updateCursorPosition();
    this->input = string;
}

// private methods:

// initialization:
void InputField::initRect(
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const sf::Color& idle_color,
    const sf::Color& outline_idle_color,
    float outline_thickness)
{
    this->rect.setPosition(
        sf::Vector2f(
            position.x + outline_thickness,
            position.y + outline_thickness
        )
    );
    this->rect.setSize(
        sf::Vector2f(
            size.x - 2.0f * outline_thickness,
            size.y - 2.0f * outline_thickness
        )
    );
    this->rect.setFillColor(idle_color);

    this->rect.setOutlineThickness(outline_thickness);
    this->rect.setOutlineColor(outline_idle_color);
}

void InputField::initText(
    const std::string& default_str,
    unsigned char_size,
    const sf::Color& text_idle_color)
{
    this->text.setString(default_str);
    this->text.setCharacterSize(char_size);
    this->text.setFillColor(text_idle_color);
    this->text.setFont(font);
    this->text.setOrigin(
        this->text.getGlobalBounds().width / 2.f,
        this->text.getLocalBounds().top + this->text.getLocalBounds().height / 2.f
    );
    this->text.setPosition(
        this->rect.getGlobalBounds().left + this->rect.getGlobalBounds().width / 2.f,
        this->rect.getGlobalBounds().top + this->rect.getGlobalBounds().height / 2.f
    );
}

void InputField::initCursor(float cursor_width, const sf::Color& text_idle_color)
{
    this->cursor.setOrigin(0.f, this->charSize / 2.f);
    this->cursor.setSize(sf::Vector2f(cursor_width, charSize));
    this->cursor.setFillColor(text_idle_color);
    this->cursor.setPosition(
        this->text.getGlobalBounds().left + this->text.getGlobalBounds().width + this->cursor.getSize().x,
        this->rect.getGlobalBounds().top + this->rect.getGlobalBounds().height / 2.f
    );
}

void InputField::initColors(
    const sf::Color& idle_color, const sf::Color& hovered_color, const sf::Color& pressed_color,
    const sf::Color& outline_idle_color, const sf::Color& outline_hovered_color, const sf::Color& outline_pressed_color,
    const sf::Color& text_idle_color, const sf::Color& text_hovered_color, const sf::Color& text_pressed_color)
{
    this->colors["INPUT FIELD"]["IDLE"] = idle_color;
    this->colors["INPUT FIELD"]["HOVERED"] = hovered_color;
    this->colors["INPUT FIELD"]["PRESSED"] = pressed_color;

    this->colors["OUTLINE"]["IDLE"] = outline_idle_color;
    this->colors["OUTLINE"]["HOVERED"] = outline_hovered_color;
    this->colors["OUTLINE"]["PRESSED"] = outline_pressed_color;

    this->colors["TEXT"]["IDLE"] = text_idle_color;
    this->colors["TEXT"]["HOVERED"] = text_hovered_color;
    this->colors["TEXT"]["PRESSED"] = text_pressed_color;
}

// other private methods:
void InputField::updateState(const std::vector<sf::Event>& events, const sf::Vector2i& mouse_pos_window)
{
    this->m_HasBeenTurnedOn = false;
    this->stateHasChanged = false;

    if (this->rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_pos_window)))
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if (!this->turnedOnBlockade)
            {
                if (this->state != "PRESSED")
                {
                    this->stateHasChanged = true;
                    this->state = "PRESSED";
                }

                this->turnedOnBlockade = true;

                this->turnedOn = !this->turnedOn;

                if (this->turnedOn) 
                    this->m_HasBeenTurnedOn;
                else 
                    this->cursorIsRendered = false;
            }
        }
        else
        {
            this->turnedOnBlockade = false;

            if (this->state != "HOVERED")
            {
                this->stateHasChanged = true;
                this->state = "HOVERED";
            }
        }
    }
    else
    {
        if (this->state != "IDLE")
        {
            this->stateHasChanged = true;
            this->state = "IDLE";
        }
    }
}

void InputField::updateColors()
{
    this->rect.setFillColor(this->colors["INPUT FIELD"][this->state]);

    this->rect.setOutlineColor(this->colors["OUTLINE"][this->state]);

    this->text.setFillColor(this->colors["TEXT"][this->state]);

    this->cursor.setFillColor(this->colors["TEXT"][this->state]);
}

void InputField::handleTextEntering(const std::vector<sf::Event>& events)
{
    this->textHasChanged = false;

    for (const auto& event : events)
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == 8) // backspace
            {
                if (!this->input.empty())
                {
                    this->textHasChanged = true;
                    this->input.pop_back();
                    this->text.setString(this->input);
                }
            }
            if (event.text.unicode > 31)
            {
                    this->textHasChanged = true;
                    this->input.push_back(event.text.unicode);
                    this->text.setString(this->input);
            }
        }
}

void InputField::updateTextPosition()
{
    this->text.setOrigin(
        this->text.getGlobalBounds().width / 2.f,
        this->text.getLocalBounds().top + this->text.getLocalBounds().height / 2.f
    );
    this->text.setPosition(
        this->rect.getGlobalBounds().left + this->rect.getGlobalBounds().width / 2.f,
        this->rect.getGlobalBounds().top + this->rect.getGlobalBounds().height / 2.f
    );
}

void InputField::updateCursorPosition()
{
    this->cursor.setPosition(
        this->text.getGlobalBounds().left + this->text.getGlobalBounds().width + this->cursor.getSize().x,
        this->rect.getGlobalBounds().top + this->rect.getGlobalBounds().height / 2.f
    );
}

void InputField::updateCursorVisibility(float dt)
{
    this->stopwatch += dt;

    if (this->stopwatch > this->cursorFrequency)
    {
        this->cursorIsRendered = !this->cursorIsRendered;
        this->stopwatch = 0.f;
    }
}
