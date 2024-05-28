#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

    // Gobal initalization 

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Breakout"); // width = 1000 , height = 600 
    int width = 1000, height = 600;

    sf::RectangleShape paddle;
    sf::CircleShape ball;

    sf::Vector2f speed;
    float def_speed = 2.5f;
    double pi = 3.14159265358979323846;
    float angle;

    sf::RectangleShape block(sf::Vector2f(50, 20));


    int total_blocks = ((width / block.getSize().x) * (height / block.getSize().y));
    //The calculation of blocks_per_row and blocks_per_column based on the screen's width and block_size.x and height and block_size.y is performed at runtime. This allows your program to adapt to different screen sizes.
    int* isblock = new int[total_blocks];

    sf::Font font;
    sf::Text score;

    //score 
    int scoreint{};

    // check mouse just press just one time while player is inside the game 
    bool firstmouse = true;

// Start and Initalization function 
void start()
{
    // Adding font and text for score 
    font.loadFromFile("Lato-Black.ttf");
    score.setFont(font);
    score.setFillColor(sf::Color::Cyan);
    score.setCharacterSize(20);

    paddle.setSize(sf::Vector2f(90, 15));
    paddle.setFillColor(sf::Color(153, 76, 0));
    paddle.setOutlineThickness(2);
    paddle.setOutlineColor(sf::Color(255, 153, 51));

    ball.setRadius(8);
    ball.setFillColor(sf::Color(0, 102, 51));
    ball.setOutlineThickness(1.0);
    ball.setOutlineColor(sf::Color(0, 152, 0));

    block.setOutlineThickness(2.0f);

    for (int i = 0; i < total_blocks; i++) // all block set as false ! 
    {
        isblock[i] = -1;
    }

    for (int i = 0; i < 200; i++) // all block set as false ! 
    {
        isblock[i] = i % 3;
    }

}

// Event Handling 
void eventHandling()
{
    sf::Event event;

    while (window.pollEvent(event))
    {
      
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::MouseMoved)
        {
            paddle.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x - paddle.getSize().x / 2, window.getSize().y - paddle.getSize().y));
        }
        else if (event.type == sf::Event::MouseButtonPressed && firstmouse ) // firstmouse is check that this loop just run one time then game started are reset 
        {
            angle = (-20 - (rand() % 140)) * (pi / 180.0);
            speed.x = cos(angle) * def_speed;
            speed.y = sin(angle) * def_speed;
           
            // ablove line : when we pressed mouse then ball set on paddle 
            ball.setPosition(sf::Vector2f(paddle.getPosition().x + paddle.getSize().x / 2 - ball.getRadius(), paddle.getPosition().y - paddle.getSize().y - ball.getOutlineThickness()));
            
            // firstmouse === false insure that then player is in game and press mouse then it can't run (loop not run )
            firstmouse = false;
            
        }
    }
}

//reset 
void reset()
{
    // Showing again blocks 
    for (int i = 0; i < 200; i++) 
    {
        isblock[i] = i % 3;
    }

    // ball set to paddle mid 
    speed.x = 0, speed.y = 0;

    // set score 
    scoreint = 1;

    // show scoer 0 then reset
    score.setString("0");

    // now we can press the mouse then ball move 
     firstmouse = true;


}

void update()
{

    ball.setPosition(sf::Vector2f(ball.getPosition().x + speed.x, ball.getPosition().y + speed.y));

    // ball with window border Collisions  
    // X-axis 
    if (ball.getPosition().x <= 0.0f)
    {
        speed.x = abs(speed.x); // in left of x axis there is x is negative and when it hits window boundry then we make it +ve means jsut change the direction towards +ve x axis 
    }

    else if (ball.getPosition().x >= width) {
        ball.setPosition(width - (ball.getRadius() * 2.f), ball.getPosition().y);// we use this becuse ball goes outside the window
        speed.x = -abs(speed.x);
    }

    // Y-axis 
    if (ball.getPosition().y <= 0.0f)
    {
        speed.y = abs(speed.y);
    }

    else if (ball.getPosition().y >= height)
    {
        reset();
    }

    // ball with paddle Collisions 

    if (ball.getPosition().y + ball.getRadius() >= paddle.getPosition().y &&
        ball.getPosition().x + ball.getRadius() >= paddle.getPosition().x &&
        ball.getPosition().x < paddle.getPosition().x + paddle.getSize().x &&
        ball.getPosition().y < paddle.getPosition().y + paddle.getSize().y)
    {
        speed.y += 0.0001f; // increasing  speed so it can  increas in difficulty 

        // it can change angle while collsion with paddle 
        float paddlecenter = paddle.getPosition().x + (paddle.getSize().x / 2); // this exution find the mid point of paddle 300(paddle.x) + (60)(paddle.size.x)/2 = 360 
        float ballcenter = ball.getPosition().x + ball.getRadius(); // midpoint of ball ball.x + radius 
        speed.x += (ballcenter - paddlecenter) * 0.05f;             // change in speed and angle 

        // Collision happened, adjust ball's position and velocity
        ball.setPosition(ball.getPosition().x, paddle.getPosition().y - (ball.getRadius() * 2.0f));
        speed.y = -abs(speed.y); // Change vertical speed to bounce the ball

    }

    // Ball with Block collision
    for (int y = 0; y < height / block.getSize().y; y++)
    {
        for (int x = 0; x < width / block.getSize().x; x++)
        {
            if (isblock[(int)(x + (y * (width / block.getSize().x)))] != -1)
            {

                sf::FloatRect blockBounds(
                    x * block.getSize().x, y * block.getSize().y,
                    block.getSize().x, block.getSize().y
                );

                sf::FloatRect ballBounds(
                    ball.getPosition().x, ball.getPosition().y,
                    ball.getRadius() * 2.0f, ball.getRadius() * 2.0f
                );

                if (ballBounds.intersects(blockBounds))
                {
                    isblock[(int)(x + (y * (width / block.getSize().x)))] = -1;
                    speed.y = -speed.y; // Reflect the ball vertically on collision

                    // increasing score 
                    score.setString(std::to_string(scoreint));
                    scoreint++;


                }
            }
        }
    }
    // Chat Gpt Gernated code for block collision

}

// Draw 
void Draw()
{
    window.clear();

    for (int y = 0; y < height; y += block.getSize().y)  // outer loop increse coloums 
    {
        for (int x = 0; x < width; x += block.getSize().x)  // inc rows 
        {
            switch (isblock[(int)((x / block.getSize().x) + (y / block.getSize().y * width / block.getSize().x))])
            {
            case 0:
                block.setFillColor(sf::Color(220, 20, 60));  // Ruby red
                block.setOutlineColor(sf::Color(0, 0, 0));
                break;

            case 1:
                block.setFillColor(sf::Color(25, 25, 112));  // Midnight blue
                block.setOutlineColor(sf::Color(0, 0, 0));
                break;

            case 2:
                block.setFillColor(sf::Color(192, 192, 192));  // Silver
                block.setOutlineColor(sf::Color(0, 0, 0));
                break;

            case 3:
                block.setFillColor(sf::Color(220, 20, 60));  // Ruby red (Same as case 0)
                block.setOutlineColor(sf::Color(0, 0, 0));
                break;


            default:
                block.setFillColor(sf::Color(rand() % 30, rand() % 30, rand() % 30));
                block.setOutlineColor(sf::Color(40, 40, 40));

            }
            /*if (isblock[(int)((x / block.getSize().x) + (y / block.getSize().y * width / block.getSize().x))])
                    {     //             1.2.3...                 1.2.3 ..              25. 26 ....
                            block.setFillColor(sf::Color(153, 0, 0));
                            block.setOutlineColor(sf::Color(0, 0, 0));

           // Here we use if and also isblock arry (bool) it means we set index it draw when bool of specific idenx is true .. so we use if .
         // x/block.getSize().x --> this line use for row index to increse
         // y/block.getSize().y * width/block.getSize().x  --> this is use to shift to new row !
         // std::cout << "y ->" << y << std::endl << "x-->" << x << std::endl << " block.getSize().y -> " << block.getSize().y << std::endl << " block.getSize().x --> " << block.getSize().x << std::endl;
            */

            block.setPosition(sf::Vector2f(x, y));
            window.draw(block);

        }
    }
    window.draw(paddle);
    window.draw(ball);
    window.draw(score);

    window.display();

}

int main()
{
    start();
    // window.setFramerateLimit(500);
     //sf::Clock clock; // Imeasure the time between frames (delta time).
    // float deltaTimeInSeconds{};

    // game loop 
    while (window.isOpen())
    {
        eventHandling();
        update();
        Draw();

    }

    delete[] isblock;
    return 0;
}
/*
*** H0w ball move in every iteration :

Initial Position: If the ball's initial x-coordinate (getPosition().x) is 1 and the initial speed (speed.x) is 1, then after the first iteration,
the new x-coordinate becomes 1 + 1 = 2.

Second Iteration: Now, with the updated x-coordinate being 2 and the same speed of 1, after the second iteration, the new x-coordinate becomes 2 + 1 = 3.

So yes, your understanding is spot on! In each iteration,

the ball's position is updated by adding the speed value to its current position, resulting in a gradual increase in
the position of the ball. This continuous updating creates the animation effect, making the ball move smoothly across the screen.


*** Why we set   42 - 45 inside the elseif becuse :
If you were to move this segment of code to the outer loop,
it might result in the ball's direction being recalculated continuously during every iteration of the game loop,
which could lead to unexpected and undesirable behavior.

*** how sin , angle , speed work

 Random Angle Calculation:
(rand() % 140): rand() generates a random number, and % 140 limits the result to a range between 0 and 139. This part generates a random number in the range [0, 139].
(-20 - (rand() % 140)): This subtracts the random number from -20, effectively shifting the range to [-20, 119]. This random value represents an angle offset.

Angle Conversion:
(-20 - (rand() % 140)) * (pi / 180.0): This calculates the angle in radians using the offset value from the previous step. (pi / 180.0) converts degrees to radians. sin and cos function work in radiinan here

Horizontal and Vertical Speed Components:
speed.x = cos(angle) * def_speed: The cos(angle) calculates the cosine of the angle calculated above. This represents the horizontal component of the ball's speed. Multiplying by def_speed scales this component.
speed.y = sin(angle) * def_speed: The sin(angle) calculates the sine of the angle, representing the vertical component of the ball's speed. Again, multiplying by def_speed scales this component.


***

*/
