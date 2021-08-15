#include "pch.h"
#include "Slider.h"

gui::Slider::Slider(
	const sf::Vector2f& position,
	float texturesScale,
	const std::pair<float, float>& range,
	const std::pair<float, float>& notSnappingToEdgesRange,
	float defaultValue,
	const std::string& axisIdlePath, const std::string& handleIdlePath,
	const std::string& axisHoverPath, const std::string& handleHoverPath,
	const std::string& axisPressedPath, const std::string& handlePressedPath,
	const std::string& scaleFunction)
	: m_axisTextures()
	, m_handleTextures()
	, m_axisSprites()
	, m_handleSprites()
	, m_state("IDLE")
	, m_range(range)
	, m_notSnappingToEdgesRange(notSnappingToEdgesRange)
	, m_value(defaultValue)
	, m_scaleFunction(scaleFunction)
{
	initTextures(
		axisIdlePath, handleIdlePath,
		axisHoverPath, handleHoverPath,
		axisPressedPath, handlePressedPath
	);
	initSprites(position, texturesScale);

	setValue(defaultValue);
}

void gui::Slider::update(sf::Vector2i mousePosWindow)
{
	sf::Sprite currentAxisSprite = m_axisSprites[m_state];
	sf::Sprite currentHandleSprite = m_handleSprites[m_state];

	auto mousePosWindowF = static_cast<sf::Vector2f>(mousePosWindow);

	if (m_state == "IDLE")
	{
		if (currentAxisSprite.getGlobalBounds().contains(mousePosWindowF) 
			|| currentHandleSprite.getGlobalBounds().contains(mousePosWindowF))
		{
			m_state = "HOVERED";
			currentAxisSprite.setTexture(m_axisTextures["HOVERED"]);
			currentHandleSprite.setTexture(m_handleTextures["HOVERED"]);
		}

		return;
	}

	if (!currentAxisSprite.getGlobalBounds().contains(mousePosWindowF)
		&& !currentHandleSprite.getGlobalBounds().contains(mousePosWindowF))
	{
		m_state = "IDLE";
		currentAxisSprite.setTexture(m_axisTextures["IDLE"]);
		currentHandleSprite.setTexture(m_handleTextures["IDLE"]);

		return;
	}

	if (m_state == "HOVERED" && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		m_state = "PRESSED";
		currentAxisSprite.setTexture(m_axisTextures["PRESSED"]);
		currentHandleSprite.setTexture(m_handleTextures["PRESSED"]);
	}

	if (m_state == "PRESSED")
	{
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_state = "HOVERED";
			currentAxisSprite.setTexture(m_axisTextures["HOVERED"]);
			currentHandleSprite.setTexture(m_handleTextures["HOVERED"]);
		}
		else
		{
			sf::FloatRect axisBounds = currentAxisSprite.getGlobalBounds();
			sf::FloatRect handleBounds = currentHandleSprite.getGlobalBounds();

			float leftBound = axisBounds.left;
			float rightBound = axisBounds.left + axisBounds.width;

			if (mousePosWindow.x >=  leftBound 
				&& mousePosWindow.x <= rightBound)
			{
				float xPos = mousePosWindow.x + handleBounds.width / 2.0f;
				float yPos = currentHandleSprite.getPosition().y;

				for (auto& handleSprites : m_handleSprites)
				{
					handleSprites.second.setPosition(
						sf::Vector2f(
							xPos,
							yPos
						)
					);
				}
			}
		}
	}

	updateCurrentValue();
	snapToEdges();
}

void gui::Slider::render(sf::RenderTarget& target)
{
	target.draw(m_axisSprites[m_state]);
	target.draw(m_handleSprites[m_state]);
}

// accessors:

float gui::Slider::getCurrentValue() const
{
	return m_value;
}

const sf::Vector2f& gui::Slider::getPosition() const
{
	return m_axisSprites.at(m_state).getPosition();
}

// mutators:

void gui::Slider::setValue(float value)
{
	if (value < m_range.first || value > m_range.second)
	{
		throw std::invalid_argument(
			Blueberry::Formatter()
			<< "Error::gui::Slider::setValue(float):: "
			<< value
			<< " doesn't belong to "
			<< '<' << m_range.first << "; " << m_range.second << ">\n"
		);
	}

	m_value = value;

	sf::FloatRect axisBounds = m_axisSprites["IDLE"].getGlobalBounds();
	sf::FloatRect handleBounds = m_handleSprites["IDLE"].getGlobalBounds();

	// set x positions:
	for (auto& handleSprite : m_handleSprites)
	{
		float rangeSize = m_range.second - m_range.first;
		
		float xPos = axisBounds.left;
		float yPos = handleSprite.second.getPosition().y;
		
		xPos += inverseScaleFunction(value) / rangeSize * axisBounds.width;
		xPos -= handleBounds.width / 2.f;

		handleSprite.second.setPosition(
			xPos,
			yPos
		);
	}
}

void gui::Slider::setPosition(const sf::Vector2f& new_pos)
{
	for (auto& axisSprite : m_axisSprites)
	{
		axisSprite.second.setPosition(new_pos);
	}

	// set handle position 
	// taking the current scale slider value into account:
	setValue(m_value);
}

// private methods:

void gui::Slider::initTextures(
	const std::string& axisIdlePath, const std::string& handleIdlePath,
	const std::string& axisHoverPath, const std::string& handleHoverPath,
	const std::string& axisPressedPath, const std::string& handlePressedPath)
{
	// loading axis textures:
	if (!m_axisTextures["IDLE"].loadFromFile(axisIdlePath) 
		|| !m_axisTextures["HOVERED"].loadFromFile(axisHoverPath)
		|| !m_axisTextures["PRESSED"].loadFromFile(axisPressedPath))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::gui::Slider::initTextures(\n"
			<< "\tconst std::string&, const std::string&\n"
			<< "\tconst std::string&, const std::string&\n"
			<< "\tconst std::string&, const std::string&\n"
			<< ")::cannot load an axis texture from a file\n"
		);
	}

	// loading handle textures:
	if (!m_handleTextures["IDLE"].loadFromFile(handleIdlePath) 
		|| !m_handleTextures["HOVERED"].loadFromFile(handleHoverPath)
		|| !m_handleTextures["PRESSED"].loadFromFile(handlePressedPath))
	{
		throw std::runtime_error(
			Blueberry::Formatter()
			<< "Error::gui::Slider::initTextures(\n"
			<< "\tconst std::string&, const std::string&\n"
			<< "\tconst std::string&, const std::string&\n"
			<< "\tconst std::string&, const std::string&\n"
			<< ")::cannot load a handle texture from a file\n"
		);
	}
}

void gui::Slider::initSprites(
	const sf::Vector2f& position, 
	float texturesScale)
{
	// creating axes:		
	for (auto& axisTexture : m_axisTextures)
	{
		m_axisSprites[axisTexture.first].setTexture(axisTexture.second);
		m_axisSprites[axisTexture.first].setScale(
			texturesScale, 
			texturesScale
		);

		const sf::Sprite& axisSprite = m_axisSprites[axisTexture.first];

		m_axisSprites[axisTexture.first].setScale(
			position.x - axisSprite.getGlobalBounds().width / 2.f,
			position.y - axisSprite.getGlobalBounds().height / 2.f
		);
	}

	// creating handles:
	for (auto& handleTexture : m_handleTextures)
	{
		sf::Sprite& axisSprite = m_axisSprites[handleTexture.first];
		sf::Sprite& handleSprite = m_handleSprites[handleTexture.first];

		const sf::FloatRect axisBounds = axisSprite.getGlobalBounds();
		const sf::FloatRect handleBounds = handleSprite.getGlobalBounds();

		handleSprite.setTexture(handleTexture.second);
		handleSprite.setScale(texturesScale, texturesScale);

		float scaleFactor = axisBounds.height / handleBounds.height;
		
		handleSprite.scale(
			sf::Vector2f(
				scaleFactor,
				scaleFactor
			)
		);
		handleSprite.setOrigin(0.f, 0.f);

		// set position:
		float axisLeftBound = axisBounds.left;
		float axisWidth = axisBounds.width;
		float handleWidth = handleBounds.width;

		float posX = axisLeftBound + axisWidth / 2.f - handleWidth / 2.f;
		float posY = axisBounds.top;

		handleSprite.setPosition(posX, posY);
	}
}

float gui::Slider::inverseScaleFunction(float y)
{
	if (m_scaleFunction == "linear") return y;

	else if (m_scaleFunction == "quadratic") return sqrt(y);

	std::cerr
		<< "Error::gui::Slider::inverseScaleFunction(float)::"
		<< "there is no such activation function as "
		<< m_scaleFunction << '\n';
	assert(false);
}

float gui::Slider::scaleFunctionValue(float x)
{
	if (m_scaleFunction == "linear") return x;

	else if (m_scaleFunction == "quadratic") return pow(x, 2);

	std::cerr
		<< "Error::gui::Slider::scaleFunctionValue(float)::"
		<< "there is no such activation function as "
		<< m_scaleFunction << '\n';
	assert(false);
}

void gui::Slider::updateCurrentValue()
{
	const sf::Sprite& handleSprite = m_handleSprites[m_state];
	const sf::FloatRect handleBounds = handleSprite.getGlobalBounds();

	float x = handleSprite.getPosition().x + handleBounds.width / 2.f;

	float left = m_axisSprites["IDLE"].getGlobalBounds().left;
	float right = left + m_axisSprites["IDLE"].getGlobalBounds().width;

	float rangeSize = m_range.second - m_range.first;

	float funcVal = m_range.first;
	funcVal =+ rangeSize * (x - left) / (right - left);

	m_value = scaleFunctionValue(funcVal);
}

void gui::Slider::snapToEdges()
{
	if (m_value < m_notSnappingToEdgesRange.first)
	{
		setValue(m_range.first);
	}

	else if (m_value > m_notSnappingToEdgesRange.second)
	{
		setValue(m_range.second);
	}
}
