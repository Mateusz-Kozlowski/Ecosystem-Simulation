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

    // input text:
    this->text.setString(default_text);
    this->text.setCharacterSize(char_size);
    this->text.setFillColor(textColor);
    this->text.setFont(font);
    this->text.setPosition(
        pos_x + 2.f * outline_thickness, 
        pos_y + outline_thickness + (height - 2.f * outline_thickness - this->text.getGlobalBounds().height) / 2.f
    );
    
    // cursor:
    this->textCursor.setSize(sf::Vector2f(outline_thickness, char_size));
    this->textCursor.setPosition(
        pos_x + 3.f * outline_thickness + this->text.getGlobalBounds().width, 
        pos_y + outline_thickness + (height - 2.f * outline_thickness - char_size) / 2.f
    );
    //this->textCursor.setFillColor(this->textColor);
    this->textCursor.setFillColor(sf::Color(255, 0, 0));
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
        if (event.type == sf::Event::TextEntered && event.text.unicode < 128)
        {
            this->input.push_back(event.text.unicode);
            this->change = true;
        }

    if (this->change)
    {
        this->text.setPosition(
            this->posX + 2.f * this->outlineThickness,
            this->posY + this->outlineThickness + (height - 2.f * this->outlineThickness - this->text.getGlobalBounds().height) / 2.f
        );

        this->textCursor.setPosition(
            this->posX + 3.f * this->outlineThickness + this->text.getGlobalBounds().width,
            this->posY + this->outlineThickness + (height - 2.f * this->outlineThickness - this->charSize) / 2.f
        );
    }

    this->stopwatch += dt;

    if (this->stopwatch > 1.f && this->stopwatch < 2.f) this->textCursor.setFillColor(sf::Color(0, 0, 0, 0));

    else if (this->stopwatch > 2.f)
    {
        this->stopwatch = 0.f;
        this->textCursor.setFillColor(this->textColor);
    }
}

void gui::InputField::render(sf::RenderTarget& target)
{
    target.draw(this->outline);
    target.draw(this->field);
    target.draw(this->text);
    target.draw(this->textCursor);
}
