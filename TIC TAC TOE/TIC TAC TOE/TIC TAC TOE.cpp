// TIC TAC TOE.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <vector>
#include"SFML/Graphics.hpp"

sf::RenderWindow window(sf::VideoMode(470, 600), " TIC TAC TOE ");

// Adding background 
sf::Texture texture;
sf::Sprite backgound;

//Circle for player drwa 
sf::CircleShape cc(13);

// Adding Cross
sf::Font font;

sf::Text cross , start_screen_Text;

sf::RectangleShape rectangles[9]; // this box for cross and circle (Grid)

int  selected_Rectangles  = -1; // check index for cirlce and cross 

bool player_turn{};				// after one click player turn 

// i created vector for store index that player click on and after click vector store it and draw 
std::vector<int> vector_circle_index;
std::vector<int> vector_cross_index;

void window_init()
{
	texture.loadFromFile("back.jpg");
	backgound.setTexture(texture);
	backgound.setScale(10, 20);
	font.loadFromFile("Lato-Black.ttf");

}
void ractangles()
{
	// storing all positions in vector2D array 
	sf::Vector2f positions[] = {
		sf::Vector2f(70, 60),
		sf::Vector2f(180, 60),
		sf::Vector2f(290, 60),
		sf::Vector2f(70, 170),
		sf::Vector2f(180, 170),
		sf::Vector2f(290, 170),
		sf::Vector2f(70, 280),
		sf::Vector2f(180, 280),
		sf::Vector2f(290, 280)
	};
	// here we initalize all boxes . 
	for (int i = 0; i < 9; i++) {
		rectangles[i].setSize(sf::Vector2f(100, 100));
		rectangles[i].setPosition(positions[i]);
		rectangles[i].setFillColor(sf::Color(0,153,153));
		rectangles[i].setOutlineThickness(5.0f);
		rectangles[i].setOutlineColor(sf::Color::Black);
	}
}

void circle_shape(int index)
{
	// This function accept one argumnt to draw a cricle on box 
	sf::Vector2f rectanglePosition = rectangles[index].getPosition();
	sf::Vector2f rectangleSize = rectangles[index].getSize();

	cc.setFillColor(sf::Color(255, 255, 255, 100));
	cc.setOutlineThickness(15);
	cc.setOutlineColor(sf::Color::White);
	cc.setPointCount(500); // more clear circle 

	// make circle in centre so follwoing 2 lines have same conecpt 2 one easy to understand 
	//cc.setPosition(rectanglePosition + (rectangleSize / 2.0f) - sf::Vector2f(cc.getRadius(), cc.getRadius()));
	cc.setPosition(sf::Vector2f((rectanglePosition.x + (rectangleSize.x / 2.0f) - cc.getRadius()), (rectanglePosition.y + (rectangleSize.y / 2.0f) - cc.getRadius())));
}

void cross_shape(int index)
{
	sf::Vector2f rectanglePosition = rectangles[index].getPosition(); // here we get rectange positon to where we draw a cross 

	cross.setFont(font);
	cross.setFillColor(sf::Color::White);
	cross.setCharacterSize(70);
	cross.setString("X");
	cross.setPosition(sf::Vector2f(rectanglePosition.x+25, rectanglePosition.y+10)); // set the cross on mid of grid 
}

void start_Screen()
{
		start_screen_Text.setFont(font);
		start_screen_Text.setString("X- - - - - - - - - - - -O\nClick To Continue\nO- - - - - - - - - - - -X");
		start_screen_Text.setCharacterSize(40);

		// Center the title text on the screen CHAT GPT
		sf::FloatRect textRect = start_screen_Text.getLocalBounds();
		start_screen_Text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		start_screen_Text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
						return; // Exit the start screen when the left mouse button is clicked
				}
			}
			start_screen_Text.setFillColor(sf::Color(rand() % 60, rand() % 135, rand() % 245));

			window.clear();
			window.draw(backgound);
			window.draw(start_screen_Text);
			window.display();
		}
}
void event_handling()
{
	sf::Event event;

	while (window.pollEvent(event))
	{
		if (event.type == event.Closed)
			window.close();

		// if (event.type == event.MouseMoved) {
		//	std::cout << "X : " << event.mouseMove.x << std::endl;
		//	std::cout << "Y : " << event.mouseMove.y << std::endl;
		//}

		else if (event.type == event.MouseButtonPressed) 
		{
			if(event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i mousePosition = sf::Mouse::getPosition(window);// it retuern postion of mouse (same as event.mouseMove.x/y )

				if (player_turn == 0 )
				{
					for (int i = 0; i < 9; i++)
					{
						if (std::find(vector_cross_index.begin(), vector_cross_index.end(), i) == vector_cross_index.end() && std::find(vector_circle_index.begin(), vector_circle_index.end(), i) == vector_circle_index.end() && rectangles[i].getGlobalBounds().contains((sf::Vector2f)(mousePosition)) )
							//getGlobalBounds() function use for to get boundries for sprtie or box etc..
							//contains() function checks whether a given point (in this case, the mouse position) is inside the bounding box of an object.
							// sf::Vector2f)(mousePosition) : here i can explict type cast mousepoition(vector_int) to vector_float
						{
							selected_Rectangles = i;
							//circle_shape(selectedRectangle);
							// 
							 // Store the selected rectangle index in the vector
							vector_circle_index.push_back(selected_Rectangles);

							player_turn = 1;
							break;
						}
					}
					
				}
				else if (player_turn == 1 )
				{
					for (int i = 0; i < 9; i++)
					{
						if (std::find(vector_circle_index.begin(), vector_circle_index.end(), i) == vector_circle_index.end() && std::find(vector_cross_index.begin(), vector_cross_index.end(), i) == vector_cross_index.end() &&
							rectangles[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition)))
							/* 
							getGlobalBounds() function use for to get boundries for sprtie or box etc..
							contains() function checks whether a given point (in this case, the mouse position) is inside the bounding box of an object.
							 sf::Vector2f)(mousePosition) : here i can explict type cast mousepoition(vector_int) to vector_float
							
							find(vector_circle_index.begin(), vector_circle_index.end(), i) == vector_circle_index.end() use to find i is present in vector or not if it is present in vector then it give false .
							.begin() use to start vector  
							.end() find(vector_circle_index.begin(), vector_circle_index.end(), i) == vector_circle_index.end()
							and we == to .end() becuse if we find i not in vector then it gives .end() pointer which is == .end() so i is not present in vector 
							*/
					{

							selected_Rectangles = i;

							vector_cross_index.push_back(selected_Rectangles);

							player_turn = 0;
							break;

						}
					}
					
				}
			}
		}
	}
}
bool checkWin(std::vector<int>& moves) {
	// Define all winning combinations
	std::vector<std::vector<int>> winCombinations = {
		{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
		{0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
		{0, 4, 8}, {2, 4, 6}             // Diagonals
	};

	for (const auto& combination : winCombinations) {
		bool isWin = true;

		for (int position : combination) {
			if (std::find(moves.begin(), moves.end(), position) == moves.end()) {
				isWin = false;
				break;
			}
		}

		if (isWin) {
			return true;
		}
	}

	return false;
}

void win() {
	if (checkWin(vector_circle_index)) {
		std::cout << "Circle Wins!\n";
	}
	else if (checkWin(vector_cross_index)) {
		std::cout << "Cross Wins!\n";
	}
}

void draw() {
	window.draw(backgound);

	for (int i = 0; i < 9; i++) 
	{
		window.draw(rectangles[i]);
	}

	for (int i = 0 ; i <  vector_circle_index.size();  i++)
	{
		if(selected_Rectangles >= 0)
		{
		 
			circle_shape(vector_circle_index[i]);
			window.draw(cc);
		}
	}
		
	for (int i = 0; i < vector_cross_index.size(); i++)
	{
		if (selected_Rectangles >= 0)
		{
			cross_shape(vector_cross_index[i]);
			window.draw(cross);
		}
	}	


}

int main()
{
	window_init();
	ractangles();
	circle_shape(-1);// -1 not on grid or hidden 
	cross_shape(-1);
	start_Screen();

	
	while (window.isOpen())
	{
		event_handling();
		win();
		window.clear();
		draw();
		window.display();
	}
}