#pragma once

#include "Animal.h"
#include "Fruit.h"
#include "EventsAccessor.h"
#include "GodTools.h"
#include "SideMenu.h"

class Ecosystem
{
public:
	Ecosystem(
		const std::string& name,
		const sf::Vector2f& world_size,
		unsigned borders_thickness,
		const sf::Color& background_color,
		const sf::Color& borders_color,
		unsigned animals_count,
		unsigned fruits_count,
		float animals_radius,
		float fruits_radius,
		float default_animals_hp,
		float default_fruits_energy,
		float mutation_percentage,
		const sf::Color& animals_color,
		const sf::Color& fruits_color,
		const sf::Color& tracked_animal_color,
		float simulation_speed_factor = 1.0f,
		bool simulation_is_paused = false,
		GodTool god_tool = GodTool::NONE,
		bool render_hp_bars_by_default = true,
		bool render_brains_by_default = true
	);
	Ecosystem(const std::string& folder_path);

	// public methods:
	void saveToFolder(const std::string& folder_path) const;
	void loadFromFolder(const std::string& folder_path);

	void useGodTools(
		const std::vector<sf::Event>& events,
		const sf::Vector2f& mouse_pos_view
	);

	void update(
		float dt,
		const std::vector<sf::Event>& events
	);
	void render(sf::RenderTarget& target) const;

	// accessors:
	const std::string& getName() const;

	sf::Vector2f getWorldSize() const;

	unsigned getBordersThickness() const;
	unsigned getAnimalsCount() const;

	const std::vector<std::shared_ptr<Animal>>& getAnimals() const;

	unsigned getFruitsCount() const;

	float getMutationPercentage() const;

	const sf::Color& getBackgroundColor() const;
	const sf::Color& getBordersColor() const;
	const sf::Color& getAnimalsColor() const;
	const sf::Color& getFruitsColor() const;
	const sf::Color& getTrackedAnimalColor() const;

	const Animal* getTrackedAnimal() const;

	float getSimulationSpeedFactor() const;

	bool isSimulationPaused() const;

	GodTool getCurrentGodTool() const;

	float getTotalTimeElapsed() const;

	float getTotalAnimalsHpEnergy() const;
	float getTotalAnimalsKineticEnergy() const;
	float getTotalFruitsEnergy() const;
	float getTotalEnergy() const;

	void printAllAnimalsPositions() const;

	// mutators:
	void setName(const std::string& name);

	void setMutationPercentage(float mutation_percentage);

	void setBackgroundColor(const sf::Color& background_color);

	void setBordersColor(const sf::Color& borders_color);
	void setAnimalsColor(const sf::Color& animals_color);
	void setFruitsColor(const sf::Color& fruits_color);
	void setTrackedAnimalColor(const sf::Color& tracked_animal_color);

	void setSimulationSpeedFactor(float simulation_speed_factor);

	void pauseSimulation();
	void unpauseSimulation();

	void setGodTool(GodTool god_tool);

private:
	std::string name;

	sf::RectangleShape background;

	// animals vector contains shared pointers 
	// (in contrast to fruits vector which contains unique pointers)
	// to make cloning animals possible
	// (cloning occurs by calling a copy constructor of Animal class)
	std::vector<std::shared_ptr<Animal>> animals;
	std::vector<std::unique_ptr<Fruit>> fruits;

	float mutationPercentage;
	
	// TODO: add a multiline comment that:
	// - explains why those variables are useful in those situations
	// - describes those situation (when they appear and how they disappear)
	
	// useful when there are temporarily no animals in an ecosystem:
	float animalsRadius; 
	
	// useful when there are temporarily no fruits in an ecosystem:
	float fruitsRadius;

	// useful when there are temporarily no animals in an ecosystem:
	sf::Color animalsColor;
	
	// useful when there are temporarily no fruits in an ecosystem:
	sf::Color fruitsColor;
	
	// useful when there is temporarily no tracked animal in an ecosystem
	sf::Color trackedAnimalColor;

	Animal* trackedAnimal;

	float simulationSpeedFactor;

	bool m_simulationIsPaused;

	GodTool godTool;

	std::unordered_map<Animal*, bool> hpBarsVisibility;
	std::unordered_map<Animal*, bool> brainsPreviewsVisibility;

	float totalTimeElapsed;

	// private methods:
	// initialization:
	void initBackgroundAndBorders(
		const sf::Vector2f& world_size,
		unsigned borders_thickness,
		const sf::Color& background_color,
		const sf::Color& borders_color
	);

	void createNewAnimals(
		unsigned animals_count,
		float default_animals_hp,
		float animals_radius,
		const sf::Color& animals_color,
		bool render_hp_bars_by_default,
		bool render_brains_by_default
	);
	void createNewAnimal(
		float default_animal_hp,
		float animal_radius,
		const sf::Color& animal_color,
		bool render_hp_bar_by_default,
		bool render_brain_by_default
	);

	void createNewFruits(
		unsigned fruits_count,
		float default_fruits_energy,
		float fruits_radius,
		const sf::Color& fruits_color
	);
	void createNewFruit(float energy, float radius, const sf::Color& fruit_color);

	// private utilities:
	void saveAnimals(const std::string& folder_path) const;
	void saveFruits(const std::string& folder_path) const;
	void saveEcosystem(const std::string& file_path) const;
	void saveHpBarsVisibility(const std::string& file_path) const;
	void saveBrainsPreviewsVisibility(const std::string& file_path) const;

	void loadAnimals(const std::string& folder_path);
	void loadAnimal(const std::string& folder_path);

	void loadFruits(const std::string& folder_path);
	void loadFruit(const std::string& file_path);

	void loadEcosystem(const std::string& file_path);
	void loadHpBarsVisibility(const std::string& file_path);
	void loadBrainsPreviewsVisibility(const std::string& file_path);

	int getTrackedAnimalIndex() const;

	// god tools:
	void trackingTool(const sf::Vector2f& mouse_pos_view);

	void killingTool(const sf::Vector2f& mouse_pos_view);
	void convertAnimalToFruit(std::shared_ptr<Animal>& animal, bool random_fruit_position);

	void replacingTool(const sf::Vector2f& mouse_pos_view);

	void brainTool(const sf::Vector2f& mouse_pos_view);

	void stoppingTool(const sf::Vector2f& mouse_pos_view);
	void convertKineticEnergyToFruit(Animal& animal, bool random_fruit_position);

	void infoTool(const sf::Vector2f& mouse_pos_view) const;
	void printInfoAboutAnimal(const Animal& animal) const;
	void printInfoAboutFruit(const Fruit& fruit) const;
	void printInfoAboutEcosystem() const;

	// other private utilities:
	void updateWorld(float dt);

	void updatingWorldLogs() const;

	void updateAnimals(float dt);
	const std::vector<CrappyNeuralNets::Scalar> getInputsForBrain(const Animal& animal) const;
	const Fruit* getTheNearestFruit(const Animal& animal) const;
	float calcDistance(const Animal& animal, const Fruit& fruit) const;

	void avoidTunneling();
	void avoidTunnelingByVerticalBorders(Animal& animal);
	void avoidTunnelingByHorizontalBorders(Animal& animal);

	void killAnimalsStickingToBorders();
	bool sticksToBorder(const Animal& animal);
	bool sticksToLeftBorder(const Animal& animal);
	bool sticksToRightBorder(const Animal& animal);
	bool sticksToTopBorder(const Animal& animal);
	bool sticksToBottomBorder(const Animal& animal);

	void removeDeadAnimals();
	void removeAnimal(std::shared_ptr<Animal>& animal);

	void feedAnimals();
	static bool compareAnimalsYPositions(
		std::shared_ptr<Animal> a1, 
		std::shared_ptr<Animal> a2
	);
	bool animalIsTooHigh(const Animal& animal, const Fruit& fruit) const;
	int tryToFindConsumer(Fruit& fruit, unsigned start_animal_index);
	bool animalReachesFruitInY(const Animal& animal, const Fruit& fruit) const;
	bool animalReachesFruit(const Animal& animal, const Fruit& fruit) const;
	void eat(Animal& animal, Fruit& fruit);

	void correctBrainPreviewsPositions();
	bool brainPreviewProtrudesWorldRightBorder(const gui::NeuralNetPreview& brain_preview);
	bool brainPreviewProtrudesWorldBottomBorder(const gui::NeuralNetPreview& brain_preview);

	void removeEatenFruits();
	void removeFruit(std::unique_ptr<Fruit>& fruit);
};
