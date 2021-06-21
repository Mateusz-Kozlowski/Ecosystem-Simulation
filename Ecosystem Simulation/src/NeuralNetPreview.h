#pragma once

#include "Crappy Neural Nets/NeuralNet.h"

class NeuralNetPreview
{
public:
	// constructor:
	NeuralNetPreview(
		const CrappyNeuralNets::NeuralNet& brain, 
		const sf::Vector2f& position, 
		const sf::Vector2f& size,
		const sf::Color& background_color
	);

	// other public methods:
	void update(const sf::Vector2f& new_pos);
	void render(sf::RenderTarget& target) const;

private:
	const CrappyNeuralNets::NeuralNet& brain;
	
	sf::RectangleShape background;

	std::vector<std::vector<sf::CircleShape>> neurons;

	std::vector<std::vector<std::vector<std::vector<sf::Vertex>>>> synapses;
	//std::vector<std::vector<std::vector<sf::RectangleShape>>> synapses;

	// initialization:
	void initBackground(const sf::Vector2f& preview_pos, const sf::Vector2f& size, const sf::Color& background_color);
	void initNeurons(const sf::Vector2f& size);
	void initSynapses(const sf::Vector2f& size);

	// private utilities:
	unsigned biggestLayerSize() const;

	void updateNeurons(const sf::Vector2f& new_preview_pos);
	void updateSynapses(const sf::Vector2f& new_preview_pos);

	void setNeuronsPositions(const sf::Vector2f& preview_pos);
	void setSynapsesPositions(const sf::Vector2f& preview_pos);

	void setNeuronsColors();
	void setSynapsesColors();

	CrappyNeuralNets::Scalar getTheBiggestActivatedValue(unsigned layer_index);
	CrappyNeuralNets::Scalar getTheSmallestActivatedValue(unsigned layer_index);

	CrappyNeuralNets::Scalar getTheBiggestWeight();
	CrappyNeuralNets::Scalar getTheSmallestWeight();

	void renderNeurons(sf::RenderTarget& target) const;
	void renderSynapses(sf::RenderTarget& target) const;
};
