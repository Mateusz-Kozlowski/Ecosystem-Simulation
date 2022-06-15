#include "BrainPreviewModifier.h"

gui::BrainPreviewModifier::BrainPreviewModifier(
	const sf::Vector2f& pos,
	const sf::Vector2f& size)
	: m_previouslyModifiedBrainPreview(nullptr)
	, m_modifiedNeuronIdx(-1)
	, m_pos(pos)
	, m_size(size)
	, m_closeBtn(nullptr)
{
	initCloseBtn();
}

void gui::BrainPreviewModifier::update(
	gui::BrainPreview& brainPreview,
	const sf::Vector2f& mousePos, 
	const std::vector<sf::Event>& events)
{
	if (&brainPreview != m_previouslyModifiedBrainPreview)
	{
		m_previouslyModifiedBrainPreview = &brainPreview;
		m_modifiedNeuronIdx = -1;
	}
	
	updateNeuronsPos(brainPreview, mousePos, events);
	updateCloseBtn(mousePos, events);
}

void gui::BrainPreviewModifier::render(
	gui::BrainPreview& brainPreview,
	sf::RenderTarget& target)
{
	const sf::Vector2f prevPos = brainPreview.getPosition();
	const sf::Vector2f prevSize = brainPreview.getSize();

	brainPreview.setPosition(m_pos);
	brainPreview.setSize(m_size);

	brainPreview.render(target, false);

	brainPreview.setSize(prevSize);
	brainPreview.setPosition(prevPos);

	m_closeBtn->render(target);
}

sf::FloatRect gui::BrainPreviewModifier::getBgBounds() const
{
	return sf::FloatRect(m_pos, m_size);
}

const std::unique_ptr<gui::ImageButton>& gui::BrainPreviewModifier::getCloseBtn() const
{
	return m_closeBtn;
}

// private methods:

// initialization:

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

// utils:

void gui::BrainPreviewModifier::updateNeuronsPos(
	gui::BrainPreview& brainPreview,
	const sf::Vector2f& mousePos,
	const std::vector<sf::Event>& events)
{
	const sf::Vector2f prevPos = brainPreview.getPosition();
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
	const sf::Vector2f& mousePos) const
{
	float a = neuron.getPosition().x + neuron.getRadius() - mousePos.x;
	float b = neuron.getPosition().y + neuron.getRadius() - mousePos.y;

	return sqrt(pow(a, 2.0f) + pow(b, 2.0f)) < neuron.getRadius();
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
