#include "stdafx.h"
#include "InputField.h"

using namespace gui;

// constructor:
gui::InputField::InputField(
    float pos_x, float pos_y, 
    float width, float height, 
    const sf::Font& font, const std::string& default_text, float char_size, 
    sf::Color color, sf::Color text_color, sf::Color outline_color, 
    float outline_thickness, 
    int id)
    : posX(pos_x), posY(pos_y), 
      width(width), height(height), 
      font(font), input(default_text), charSize(char_size),
      color(color), textColor(text_color), outlineColor(outline_color),
      outlineThickness(outline_thickness),
      id(id),
      stopwatch(0.f)
{
    // outline:
    this->outline.setPosition(pos_x, pos_y);
    this->outline.setSize(sf::Vector2f(width, height));
    this->outline.setFillColor(outline_color);

    // field body:
    this->field.setPosition(pos_x + outline_thickness, pos_y + outline_thickness);
    this->field.setSize(sf::Vector2f(width - 2.f * outline_thickness, height - 2.f * outline_thickness));
    this->field.setFillColor(color);

    // input text and cursor:
    this->text.setString(default_text);
    this->text.setCharacterSize(char_size);
    this->text.setFillColor(textColor);
    this->text.setFont(font);
    this->updateTextAndCursorPositions();
    this->textCursor.setOrigin(0.f, this->charSize / 2.f);
    this->textCursor.setSize(sf::Vector2f(outline_thickness, char_size));
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

// other public methods:
void gui::InputField::update(float dt, const std::vector<sf::Event>& events)
{
    this->change = false;

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
    target.draw(this->outline);
    target.draw(this->field);
    target.draw(this->text);
    target.draw(this->textCursor);
}

// private methods:
void gui::InputField::updateTextAndCursorPositions()
{
    // text:
    this->text.setOrigin(
        this->text.getLocalBounds().left + this->text.getLocalBounds().width / 2.f,
        this->text.getLocalBounds().top + this->text.getLocalBounds().height / 2.f
    );
    this->text.setPosition(
        this->posX + 2.f * this->outlineThickness + this->text.getGlobalBounds().width / 2.f, 
        this->posY + height / 2.f
    );

    // cursor:
    this->textCursor.setPosition(
        this->posX + 3.f * this->outlineThickness + this->text.getGlobalBounds().width,
        this->posY + height / 2.f
    );
}
