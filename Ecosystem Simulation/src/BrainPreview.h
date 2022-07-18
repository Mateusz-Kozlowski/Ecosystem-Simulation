#pragma once

#include "ImageButton.h"
#include "Brain.h"

namespace gui
{
	class BrainPreview
	{
	public:
		BrainPreview(
			const Blueberry::Brain& brain,
			const sf::Vector2f& position,
			const sf::Vector2f& size,
			const sf::Color& backgroundColor
		);
		BrainPreview(const BrainPreview& rhs) = delete;
		BrainPreview& operator=(const BrainPreview& rhs) = delete;

		void update(
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events
		);
		void updateOnlyImgBtn(
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events
		);

		void render(sf::RenderTarget& target, bool renderImgBtn) const;

		// accessors:

		const Blueberry::Brain& getBrain() const;

		const sf::Vector2f& getPos() const;
		const sf::Vector2f& getSize() const;

		const sf::Color& getBackgroundColor() const;

		float calcNeuronsRadius() const;

		const std::vector<sf::CircleShape>& getNeurons() const;

		const std::unique_ptr<gui::ImageButton>& getImgBtn() const;

		// mutators:

		void setPosition(const sf::Vector2f& newPosition);
		void setPosition(float x, float y);

		void move(const sf::Vector2f& offset);
		void move(float xOffset, float yOffset);

		void setSize(const sf::Vector2f& size);
		void setSize(float x, float y);

		void scale(const sf::Vector2f& scaleFactor);
		void scale(float xScaleFactor, float yScaleFactor);

		void setBackgroundColor(const sf::Color& color);

		void setNeuronPos(unsigned neuronIndex, const sf::Vector2f& pos);

	private:
		// initialization:

		void initBackground(
			const sf::Vector2f& position, 
			const sf::Color& backgroundColor
		);
		void initNeurons();
		void initSynapses();
		void initImgBtn();

		// utils:

		void setNeuronsSizes();

		void setInputNeuronsPos();
		float calcInputLayerTopMargin() const;

		void setRandomHiddenNeuronsPos();
		
		void setOutputNeuronsPos();
		float calcOutputLayerTopMargin() const;

		void setNeuronsColors();

		Blueberry::Scalar getTheBiggestAbsActVal() const;
		Blueberry::Scalar getTheBiggestActVal() const;
		Blueberry::Scalar getTheSmallestActVal() const;

		void setColor(
			sf::CircleShape& neuronCircle,
			const Blueberry::Neuron& neuron,
			const Blueberry::Scalar& theBiggestAbsActVal
		);

		void setSynapsesPos();
		void setPos(
			std::vector<sf::Vertex>& synapseVertices,
			const Blueberry::Synapse& synapse
		);

		void setImgBtnSize();
		sf::Vector2f calcImgBtnSize() const;

		void setImgBtnPos();
		sf::Vector2f calcImgBtnPos() const;

		void setSynapsesColors();
		void setColor(
			std::vector<sf::Vertex>& synapseVertices,
			const Blueberry::Synapse& synapse,
			const Blueberry::Scalar& theBiggestAbsWeight
		);

		Blueberry::Scalar getTheBiggestAbsWeight() const;
		Blueberry::Scalar getTheBiggestWeight() const;
		Blueberry::Scalar getTheSmallestWeight() const;		

		void handleNewNeurons();
		void setRandomPos(sf::CircleShape& neuron);

		void handleNewSynapses();

		void updateImgBtn(
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events
		);
		void updateImgBtnTexture();

		void renderNeurons(sf::RenderTarget& target) const;
		void renderSynapses(sf::RenderTarget& target) const;

		void moveNeurons(float xOffset, float yOffset);

		void scaleHiddenNeuronsPos(
			float xScaleFactor, 
			float yScaleFactor
		);

	private:
		const Blueberry::Brain& m_brain;

		sf::RectangleShape m_background;

		std::vector<sf::CircleShape> m_neurons;
		std::vector<std::vector<sf::Vertex>> m_synapses;

		std::unique_ptr<gui::ImageButton> m_imgBtn;
	};
}
