#pragma once

#include "ImageButton.h"
#include "BrainPreview.h"
#include "BrainParameterInfo.h"

namespace gui
{
	class BrainPreviewModifier
	{
	public:
		BrainPreviewModifier(
			const sf::Vector2f& pos,
			const sf::Vector2f& size,
			const sf::Color& bgColor,
			const sf::Font& font,
			const sf::VideoMode& resolution
		);

		void update(
			gui::BrainPreview& brainPreview,
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events,
			const sf::VideoMode& resolution
		);
		void render(
			gui::BrainPreview& brainPreview,
			sf::RenderTarget& target
		);

		// accessors:
		
		const sf::RectangleShape& getBg() const;

		sf::FloatRect getBgBounds() const;

		const std::unique_ptr<gui::ImageButton>& getCloseBtn() const;

		// mutators:

		void setBgColor(const sf::Color& bgColor);

	private:
		// initialization:

		void initBg(
			const sf::Vector2f& pos,
			const sf::Vector2f& size,
			const sf::Color& bgColor
		);
		void initCloseBtn();
		void initBrainParameterInfo(
			const sf::Font& font,
			const sf::VideoMode& resolution
		);

		// utils:

		void updateNeuronsPos(
			gui::BrainPreview& brainPreview,
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events
		);
		void handleEvents(
			gui::BrainPreview& brainPreview,
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events
		);
		bool aNeuronIsBeingModified() const;

		static bool neuronIsHovered(
			const sf::CircleShape& neuron,
			const sf::Vector2f& mousePos
		);

		void updateBrainParameterInfo(
			gui::BrainPreview& brainPreview,
			const sf::Vector2f& mousePos,
			const sf::VideoMode& resolution
		);

		static bool mouseCoversSynapse(
			const sf::Vector2f& mousePos,
			const std::vector<sf::Vertex>& synapse,
			const sf::VideoMode& resolution
		);

		void updateCloseBtn(
			const sf::Vector2f& mousePos,
			const std::vector<sf::Event>& events
		);
		void getUpdatesFromImgBtn();

		void renderResizedBrainPreview(
			gui::BrainPreview& brainPreview,
			sf::RenderTarget& target
		);

	private:
		sf::RectangleShape m_bg;

		gui::BrainPreview* m_previouslyModifiedBrainPreview;

		int m_modifiedNeuronIdx;

		const sf::Vector2f m_pos;
		const sf::Vector2f m_size;

		std::unique_ptr<gui::ImageButton> m_closeBtn;

		std::unique_ptr<gui::BrainParameterInfo> m_brainParameterInfo;
		bool m_isBrainParameterInfoRendered;
	};
}
