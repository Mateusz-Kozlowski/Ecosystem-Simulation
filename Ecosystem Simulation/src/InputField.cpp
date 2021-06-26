#include "stdafx.h"
#include "InputField.h"

using namespace gui;

// constructor:
gui::InputField::InputField(
    float pos_x, float pos_y, 
    float width, float height, 
    const sf::Font& font, const std::string& default_text, float char_size, 
    sf::Color color, sf::Color text_color, sf::Color outline_color, 
    float outline_thickness, float cursor_width,
    bool active,
    int id)
    : font(font), input(default_text), char_size(char_size),
      color(color), textColor(text_color), outlineColor(outline_color),
      active(active),
      id(id),
      stopwatch(0.f)
{
    // field:
    this->rect.setPosition(pos_x, pos_y);
    this->rect.setSize(sf::Vector2f(width, height));
    this->rect.setFillColor(color);

    this->rect.setOutlineThickness(outline_thickness);
    this->rect.setOutlineColor(outline_color);

    // input text and cursor:
    this->text.setString(default_text);
    this->text.setCharacterSize(char_size);
    this->text.setFillColor(textColor);
    this->text.setFont(font);
    this->updateTextAndCursorPositions();
    this->textCursor.setOrigin(0.f, this->char_size / 2.f);
    this->textCursor.setSize(sf::Vector2f(cursor_width, char_size));
    this->textCursor.setFillColor(this->textColor);
}

// accessors:
int gui::InputField::getId() const
{
    return this->id;
}

const std::string& gui::InputField::getInput() const
{
    return this->input;
}

bool gui::InputField::isActive() const
{
    return this->active;
}

bool gui::InputField::hasBeenClicked(const sf::Vector2f& mouse_pos_window, const std::vector<sf::Event>& events) const
{
    if (!this->rect.getGlobalBounds().contains(mouse_pos_window)) return false;

    if (!EventsAccessor::hasEventOccured(sf::Event::MouseButtonPressed, events)) return false;

    return true;
}

// mutators:
void gui::InputField::setActive(bool active)
{
    this->active = active;

    if (!active) this->textCursor.setFillColor(sf::Color::Transparent);
}

// other public methods:
void gui::InputField::update(float dt, const std::vector<sf::Event>& events)
{
    this->change = false;

    if (!this->active) return;

    for (const auto& event : events)
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode < 128)
            {
                if (event.text.unicode != 8) 
                {
                    this->change = true;
                    this->input.push_back(event.text.unicode);
                    this->text.setString(this->input);
                }
                else if (this->input.size())
                {
                    this->change = true;
                    this->input.pop_back();
                    this->text.setString(this->input);
                }
            }
        }

    if (this->change) this->updateTextAndCursorPositions();

    // update cursor visibility:
    this->stopwatch += dt;

    if (this->stopwatch > 0.5f)
    {
        if (!this->textCursor.getFillColor().a) this->textCursor.setFillColor(this->textColor);

        else this->textCursor.setFillColor(sf::Color::Transparent);

        this->stopwatch = 0.f;
    }
}

void gui::InputField::render(sf::RenderTarget& target)
{
    target.draw(this->rect);
    target.draw(this->text);
    target.draw(this->textCursor);
}

// private methods:
void gui::InputField::updateTextAndCursorPositions()
{
    // text:
    this->text.setOrigin(
        this->text.getGlobalBounds().width / 2.f,
        this->text.getLocalBounds().top + this->text.getLocalBounds().height / 2.f
    );
    this->text.setPosition(
        this->rect.getGlobalBounds().left + this->rect.getGlobalBounds().width / 2.f, 
        this->rect.getGlobalBounds().top + this->rect.getGlobalBounds().height / 2.f
    );

    // cursor:
    this->textCursor.setPosition(
        this->text.getGlobalBounds().left + this->text.getGlobalBounds().width,
        this->rect.getGlobalBounds().top + this->rect.getGlobalBounds().height / 2.f
    );
}
