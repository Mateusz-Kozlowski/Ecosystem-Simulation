#pragma once

namespace gui
{
	class NeuralNetPreview
	{
	public:
		NeuralNetPreview(
			const Blueberry::Brain& brain,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& background_color
		);
		
		void update();
		void render(sf::RenderTarget& target) const;

		// accessors:
		
		const Blueberry::Brain& getBrain() const;

		const sf::Vector2f& getPosition() const;
		const sf::Vector2f& getSize() const;

		const sf::Color& getBackgroundColor() const;

		// mutators:

		void setPosition(const sf::Vector2f& position);
		void setPosition(float x, float y);

		void setSize(const sf::Vector2f& size);

		void setBackgroundColor(const sf::Color& color);

	private:
		void initBackground(
			const sf::Vector2f& preview_pos, 
			const sf::Vector2f& size, 
			const sf::Color& background_color
		);
		void initNeurons();
		void initNeuronsVector();
		void initSynapses();
		void initSynapsesVector();
		
		void setNeuronsPositions();

		sf::Vector2f calcNeuronPosition(unsigned index1, unsigned index2) const;

		float calcTopMargin(unsigned index1) const;
		float calcGapBetweenLayers() const;
		float calcNeuronsRadius() const;

		unsigned getTheBiggestLayerSize() const;

		void setNeuronsSizes();
		void setNeuronsColors();

		void setInputNeuronsColors();
		void setHiddenNeuronsColors();
		void setOutputNeuronsColors();

		Blueberry::Scalar getTheBiggestActivatedValue(unsigned layer_index);
		Blueberry::Scalar getTheSmallestActivatedValue(unsigned layer_index);

		void setSynapsesPositions();
		void setSynapsesColors();

		Blueberry::Scalar getTheBiggestWeight();
		Blueberry::Scalar getTheSmallestWeight();

		void updateNeurons();

		void renderNeurons(sf::RenderTarget& target) const;
		void renderSynapses(sf::RenderTarget& target) const;

	private:
		// there are some errors in constructor while using here a reference:
		const Blueberry::Brain* brain;

		sf::RectangleShape background;

		std::vector<std::vector<sf::CircleShape>> neurons;

		std::vector<std::vector<std::vector<std::vector<sf::Vertex>>>> synapses;
	};
}
