#include "stdafx.h"
#include "ScaleSlider.h"

using namespace gui;

// constructor:
ScaleSlider::ScaleSlider(
	const sf::Vector2f& position,
	float textures_scale,
	const std::pair<float, float>& range,
	const std::pair<float, float>& not_snapping_to_edges_range,
	float default_value,
	const std::string& axis_idle_path, const std::string& handle_idle_path,
	const std::string& axis_hover_path, const std::string& handle_hover_path,
	const std::string& axis_pressed_path, const std::string& handle_pressed_path,
	const std::string& scale_function)
	: state("IDLE"), range(range), notSnappingToEdgesRange(not_snapping_to_edges_range),
	value(default_value), scaleFunction(scale_function)
{
	this->initTextures(
		axis_idle_path, handle_idle_path,
		axis_hover_path, handle_hover_path,
		axis_pressed_path, handle_pressed_path
	);
	this->initSprites(position, textures_scale);

	this->setValue(default_value);
}

// accessors:
float ScaleSlider::getCurrentValue() const
{
	return this->value;
}

const sf::Vector2f& ScaleSlider::getPosition() const
{
	return this->axis.at(this->state).getPosition();
}

// mutators:
void ScaleSlider::setValue(float value)
{
	if (value < this->range.first || value > this->range.second)
	{
		std::cerr << "ERROR::ScaleSlider::setValue::ARGUMENT IS NOT IN RANGE ("
			<< value << "DOESN'T BELONG TO <" << this->range.first << "; " << this->range.second << ">)\n";
		exit(-1);
	}

	this->value = value;

	float left = this->axis["IDLE"].getGlobalBounds().left;

	// set x positions:
	for (auto& it : this->handle)
	{
		float offsetX = (this->inverseScaleFunction(this->value) / (this->range.second - this->range.first));

		offsetX *= this->axis["IDLE"].getGlobalBounds().width;

		offsetX -= this->handle["IDLE"].getGlobalBounds().width / 2.f;

		it.second.setPosition(
			left + offsetX,
			it.second.getPosition().y
		);
	}
}

void ScaleSlider::setPosition(const sf::Vector2f& new_pos)
{
	for (auto& axis : this->axis) axis.second.setPosition(new_pos);

	// set handle position taking the current scale slider value into account:
	this->setValue(this->value);
}

// other public methods:
void ScaleSlider::update(sf::Vector2i mousePosWindow)
{
	if (this->state == "IDLE")
	{
		if (this->axis[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)) ||
			this->handle[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			this->state = "HOVERED";
			this->axis[state].setTexture(this->axisTextures["HOVERED"]);
			this->handle[state].setTexture(this->handleTextures["HOVERED"]);
		}

		return;
	}

	if (!this->axis[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)) &&
		!this->handle[this->state].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
	{
		this->state = "IDLE";
		this->axis[this->state].setTexture(this->axisTextures["IDLE"]);
		this->handle[this->state].setTexture(this->handleTextures["IDLE"]);

		return;
	}

	if (this->state == "HOVERED" && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		this->state = "PRESSED";
		this->axis[this->state].setTexture(this->axisTextures["PRESSED"]);
		this->handle[this->state].setTexture(this->handleTextures["PRESSED"]);
	}

	if (this->state == "PRESSED")
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->state = "HOVERED";
			this->axis[this->state].setTexture(this->axisTextures["HOVERED"]);
			this->handle[this->state].setTexture(this->handleTextures["HOVERED"]);
		}
		else
		{
			float left = this->axis["IDLE"].getGlobalBounds().left;
			float right = left + this->axis["IDLE"].getGlobalBounds().width;

			if (mousePosWindow.x >= left && mousePosWindow.x <= right)
				for (auto& it : this->handle)
					it.second.setPosition(
						sf::Vector2f(
							mousePosWindow.x - this->handle[this->state].getGlobalBounds().width / 2.f,
							this->handle[this->state].getPosition().y
						)
					);
		}
	}

	this->updateCurrentValue();
	this->snapToEdges();
}

void ScaleSlider::render(sf::RenderTarget& target)
{
	target.draw(this->axis[this->state]);
	target.draw(this->handle[this->state]);
}

// initialization:
void ScaleSlider::initTextures(
	const std::string& axis_idle_path, const std::string& handle_idle_path,
	const std::string& axis_hover_path, const std::string& handle_hover_path,
	const std::string& axis_pressed_path, const std::string& handle_pressed_path)
{
	// loading axis textures:
	if (!this->axisTextures["IDLE"].loadFromFile(axis_idle_path) ||
		!this->axisTextures["HOVERED"].loadFromFile(axis_hover_path) ||
		!this->axisTextures["PRESSED"].loadFromFile(axis_pressed_path))
	{
		throw "ERROR::ScaleSlider::initTextures::CANNOT LOAD AN AXIS TEXTURE FROM A FILE";
	}

	// loading handle textures:
	if (!this->handleTextures["IDLE"].loadFromFile(handle_idle_path) ||
		!this->handleTextures["HOVERED"].loadFromFile(handle_hover_path) ||
		!this->handleTextures["PRESSED"].loadFromFile(handle_pressed_path))
	{
		throw "ERROR::ScaleSlider::initTextres::CANNOT LOAD A HANDLE TEXTURE FROM A FILE";
	}
}

void ScaleSlider::initSprites(const sf::Vector2f& position, float textures_scale)
{
	// creating the axis:		
	for (auto& it : this->axisTextures)
	{
		sf::Sprite temp;

		temp.setTexture(it.second);
		temp.setScale(textures_scale, textures_scale);
		temp.setPosition(
			position.x - temp.getGlobalBounds().width / 2.f,
			position.y - temp.getGlobalBounds().height / 2.f
		);

		this->axis[it.first] = temp;
	}

	// creating the handle:
	for (auto& it : this->handleTextures)
	{
		sf::Sprite temp;

		temp.setTexture(it.second);
		temp.setScale(textures_scale, textures_scale);
		temp.scale(
			sf::Vector2f(
				this->axis["IDLE"].getGlobalBounds().height / temp.getGlobalBounds().height,
				this->axis["IDLE"].getGlobalBounds().height / temp.getGlobalBounds().height
			)
		);
		temp.setOrigin(0.f, 0.f);

		// set position:
		float left = this->axis["IDLE"].getGlobalBounds().left;

		float posX = left + this->axis["IDLE"].getGlobalBounds().width / 2.f - temp.getGlobalBounds().width / 2.f;
		float posY = this->axis["IDLE"].getGlobalBounds().top;

		temp.setPosition(posX, posY);

		this->handle[it.first] = temp;
	}
}

float ScaleSlider::inverseScaleFunction(float y)
{
	if (this->scaleFunction == "linear") return y;

	else if (this->scaleFunction == "quadratic") return sqrt(y);

	std::cerr << "ERROR::ScaleSlider::inverseScaleFunction::THERE IS NO: " << this->scaleFunction << " ACTIVATION FUNCTION\n";
	exit(-1);
}

float ScaleSlider::scaleFunctionValue(float x)
{
	if (this->scaleFunction == "linear") return x;

	else if (this->scaleFunction == "quadratic") return pow(x, 2);

	std::cerr << "ERROR::ScaleSlider::scaleFunctionValue::THERE IS NO: " << this->scaleFunction << " ACTIVATION FUNCTION\n";
	exit(-1);
}

// private utilities:
void ScaleSlider::updateCurrentValue()
{
	float x = this->handle[this->state].getPosition().x + this->handle[this->state].getGlobalBounds().width / 2.f;

	float left = this->axis["IDLE"].getGlobalBounds().left;
	float right = left + this->axis["IDLE"].getGlobalBounds().width;

	this->value = this->scaleFunctionValue(this->range.first + (this->range.second - this->range.first) * (x - left) / (right - left));
}

void ScaleSlider::snapToEdges()
{
	if (this->value < this->notSnappingToEdgesRange.first) this->setValue(this->range.first);

	else if (this->value > this->notSnappingToEdgesRange.second) this->setValue(this->range.second);
}
