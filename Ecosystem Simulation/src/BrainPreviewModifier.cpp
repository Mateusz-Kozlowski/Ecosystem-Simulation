#include "BrainPreviewModifier.h"

gui::BrainPreviewModifier::BrainPreviewModifier(
	const sf::Vector2f& pos,
	const sf::Vector2f& size,
	const sf::Color& bgColor,
	const sf::Font& font,
	const sf::VideoMode& resolution)
	: m_previouslyModifiedBrainPreview(nullptr)
	, m_modifiedNeuronIdx(-1)
	, m_pos(pos)
	, m_size(size)
	, m_closeBtn(nullptr)
	, m_brainParameterInfo(nullptr)
	, m_isBrainParameterInfoRendered(false)
{
	initBg(pos, size, bgColor);
	initCloseBtn();
	initBrainParameterInfo(font, resolution);
}

void gui::BrainPreviewModifier::update(
	gui::BrainPreview& brainPreview,
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events,
	const sf::VideoMode& resolution)
{
	if (&brainPreview != m_previouslyModifiedBrainPreview)
	{
		m_previouslyModifiedBrainPreview = &brainPreview;
		m_modifiedNeuronIdx = -1;
	}
	
	updateNeuronsPos(brainPreview, mousePos, events);
	updateBrainParameterInfo(brainPreview, mousePos, resolution);
	updateCloseBtn(mousePos, events);
}

void gui::BrainPreviewModifier::render(
	gui::BrainPreview& brainPreview,
	sf::RenderTarget& target)
{
	target.draw(m_bg);
	
	renderResizedBrainPreview(brainPreview, target);
	
	if (m_isBrainParameterInfoRendered)
	{
		m_brainParameterInfo->render(target);
	}

	m_closeBtn->render(target);
}

// accessors:

const sf::RectangleShape& gui::BrainPreviewModifier::getBg() const
{
	return m_bg;
}

sf::FloatRect gui::BrainPreviewModifier::getBgBounds() const
{
	return sf::FloatRect(m_pos, m_size);
}

const std::unique_ptr<gui::ImageButton>& gui::BrainPreviewModifier::getCloseBtn() const
{
	return m_closeBtn;
}

// mutators:

void gui::BrainPreviewModifier::setBgColor(const sf::Color& bgColor)
{
	m_bg.setFillColor(bgColor);
}

// private methods:

// initialization:

void gui::BrainPreviewModifier::initBg(
	const sf::Vector2f& pos, 
	const sf::Vector2f& size, 
	const sf::Color& bgColor)
{
	m_bg.setPosition(pos);
	m_bg.setSize(size);
	m_bg.setFillColor(bgColor);
}

void gui::BrainPreviewModifier::initCloseBtn()
{	
	std::vector<std::pair<std::string, std::string>> texturesKeysAndPaths = {
		{ "IDLE", "resources/textures/GUI/God tools/remove previous version/remove.png" },
		{ "HOVERED", "resources/textures/GUI/God tools/remove previous version/remove light.png" },
		{ "PRESSED", "resources/textures/GUI/God tools/remove previous version/remove dark.png" }
	};

	const sf::Vector2f size{
		m_size.x / 20.0f,
		m_size.x / 20.0f
	};
	
	m_closeBtn = std::make_unique<gui::ImageButton>(
		texturesKeysAndPaths,
		"IDLE",
		sf::Vector2f(
			m_pos.x + m_size.x - size.x,
			m_pos.y
		),
		size
	);
}

void gui::BrainPreviewModifier::initBrainParameterInfo(
	const sf::Font& font,
	const sf::VideoMode& resolution)
{
	m_brainParameterInfo = std::make_unique<gui::BrainParameterInfo>(
		sf::Vector2f(256.0f, 256.0f),
		sf::Color::Black,
		font,
		resolution
	);
}

// utils:

void gui::BrainPreviewModifier::updateNeuronsPos(
	gui::BrainPreview& brainPreview,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	const sf::Vector2f prevPos = brainPreview.getPos();
	const sf::Vector2f prevSize = brainPreview.getSize();

	brainPreview.setPosition(m_pos);
	brainPreview.setSize(m_size);

	handleEvents(brainPreview, mousePos, events);

	if (m_modifiedNeuronIdx != -1)
	{
		brainPreview.setNeuronPos(
			m_modifiedNeuronIdx,
			sf::Vector2f(
				mousePos.x - brainPreview.calcNeuronsRadius(),
				mousePos.y - brainPreview.calcNeuronsRadius()
			)
		);
	}

	brainPreview.setSize(prevSize);
	brainPreview.setPosition(prevPos);
}

void gui::BrainPreviewModifier::handleEvents(
	gui::BrainPreview& brainPreview,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	if (aNeuronIsBeingModified())
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			brainPreview.setNeuronPos(m_modifiedNeuronIdx, mousePos);
			return;
		}

		m_modifiedNeuronIdx = -1;
		return;
	}

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) return;

	for (int i = 0; i < brainPreview.getNeurons().size(); i++)
	{
		if (neuronIsHovered(brainPreview.getNeurons()[i], mousePos))
		{
			m_modifiedNeuronIdx = i;
			return;
		}
	}
}

bool gui::BrainPreviewModifier::aNeuronIsBeingModified() const
{
	return m_modifiedNeuronIdx != -1;
}

bool gui::BrainPreviewModifier::neuronIsHovered(
	const sf::CircleShape& neuron,
	const sf::Vector2f& mousePos)
{
	float a = neuron.getPosition().x + neuron.getRadius() - mousePos.x;
	float b = neuron.getPosition().y + neuron.getRadius() - mousePos.y;

	return sqrt(pow(a, 2.0f) + pow(b, 2.0f)) < neuron.getRadius();
}

void gui::BrainPreviewModifier::updateBrainParameterInfo(
	gui::BrainPreview& brainPreview,
	const sf::Vector2f& mousePos,
	const sf::VideoMode& resolution)
{
	m_isBrainParameterInfoRendered = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
	{
		const sf::Vector2f prevPos = brainPreview.getPos();
		const sf::Vector2f prevSize = brainPreview.getSize();

		brainPreview.setPosition(m_pos);
		brainPreview.setSize(m_size);
		
		for (int s = 0; s < brainPreview.getSynapses().size(); s++)
		{
			if (brainPreview.getBrain().getSynapses()[s].disabled() == false)
			{
				bool synapseIsCoveredByMouse = mouseCoversSynapse(
					mousePos,
					brainPreview.getSynapses()[s],
					resolution
				);

				if (synapseIsCoveredByMouse)
				{
					m_brainParameterInfo->setSynapseInfo(
						brainPreview.getBrain().getSynapses()[s]
					);

					m_isBrainParameterInfoRendered = true;

					m_brainParameterInfo->setPos(mousePos);
				}
			}
		}

		for (int n = 0; n < brainPreview.getNeurons().size(); n++)
		{
			if (brainPreview.getBrain().getNeurons()[n].disabled() == false)
			{
				if (neuronIsHovered(brainPreview.getNeurons()[n], mousePos))
				{
					m_brainParameterInfo->setNeuronInfo(
						brainPreview.getBrain().getNeurons()[n]
					);

					m_isBrainParameterInfoRendered = true;

					m_brainParameterInfo->setPos(mousePos);
				}
			}
		}

		brainPreview.setSize(prevSize);
		brainPreview.setPosition(prevPos);
	}
}

bool gui::BrainPreviewModifier::mouseCoversSynapse(
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Vertex>& synapse,
	const sf::VideoMode& resolution)
{
	sf::Vertex leftVertex = synapse[0];
	sf::Vertex rightVertex = synapse[1];

	if (leftVertex.position.x > rightVertex.position.x)
	{
		std::swap(leftVertex, rightVertex);
	}
	
	if (mousePos.x < leftVertex.position.x || mousePos.x > rightVertex.position.x)
	{
		return false;
	}

	float a = (rightVertex.position.y - leftVertex.position.y) / (rightVertex.position.x - leftVertex.position.y);
	float b = leftVertex.position.y - a * leftVertex.position.x; // y = ax+b => b = y-ax

	float y = a * mousePos.x + b;

	const float EPSILON = gui::p2pY(2.0f, resolution);

	return abs(y - mousePos.y) < EPSILON;
}

void gui::BrainPreviewModifier::updateCloseBtn(
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	m_closeBtn->update(mousePos, events);
	getUpdatesFromImgBtn();
}

void gui::BrainPreviewModifier::getUpdatesFromImgBtn()
{
	if (m_closeBtn->isPressed())
	{
		m_closeBtn->setTexture("PRESSED");
		return;
	}
	if (m_closeBtn->isHovered())
	{
		m_closeBtn->setTexture("HOVERED");
		return;
	}
	if (m_closeBtn->getCurrentTextureKey() != "IDLE")
	{
		m_closeBtn->setTexture("IDLE");
		return;
	}
}

void gui::BrainPreviewModifier::renderResizedBrainPreview(
	gui::BrainPreview& brainPreview,
	sf::RenderTarget& target)
{
	const sf::Vector2f prevPos = brainPreview.getPos();
	const sf::Vector2f prevSize = brainPreview.getSize();

	brainPreview.setPosition(m_pos);
	brainPreview.setSize(m_size);

	brainPreview.render(target);

	brainPreview.setSize(prevSize);
	brainPreview.setPosition(prevPos);
}
