#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <time.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define pi 3.14159

int main()
{
    srand(time(0));

    // Create window
    sf::ContextSettings setting;
    setting.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "The impossible Pong!", sf::Style::Default, setting);
    sf::RectangleShape r1(sf::Vector2f(100.f, 100.f));
    window.setFramerateLimit(120);
    window.setVerticalSyncEnabled(false);

    // Border
    sf::RectangleShape border(sf::Vector2f(SCREEN_WIDTH - 10, SCREEN_HEIGHT - 10));
    border.setFillColor(sf::Color(0, 0, 0));
    border.setOutlineColor(sf::Color(255, 0, 0));
    border.setOutlineThickness(2);
    border.setOrigin(border.getSize().x / 2, border.getSize().y / 2);
    border.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Top paddle
    sf::RectangleShape topPaddle(sf::Vector2f(100.f, 20.f));
    topPaddle.setFillColor(sf::Color(0, 0, 0));
    topPaddle.setOutlineColor(sf::Color(250, 250, 250));
    topPaddle.setOutlineThickness(2);
    topPaddle.setOrigin(topPaddle.getSize().x / 2, topPaddle.getSize().y / 2);
    topPaddle.setPosition(SCREEN_WIDTH / 2, 20);

    // Bottom paddle
    sf::RectangleShape botPaddle(sf::Vector2f(100.f, 20.f));
    botPaddle.setFillColor(sf::Color(0, 0, 0));
    botPaddle.setOutlineColor(sf::Color(250, 250, 250));
    botPaddle.setOutlineThickness(2);
    botPaddle.setOrigin(botPaddle.getSize().x / 2, botPaddle.getSize().y / 2);
    botPaddle.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);

    // Ball
    sf::CircleShape ball(5, 2000);
    ball.setFillColor(sf::Color(0, 0, 0));
    ball.setOutlineColor(sf::Color(250, 250, 250));
    ball.setOutlineThickness(1);
    ball.setOrigin(ball.getRadius(), ball.getRadius());
    ball.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    // Collision sound
    sf::SoundBuffer ballColSoundBuffer;
    ballColSoundBuffer.loadFromFile("../Resource/Sound/Ball_Collision.wav");
    sf::Sound ballColSound;
    ballColSound.setBuffer(ballColSoundBuffer);

    // Wall properties
    bool wallLeft = true, wallRight = true;

    // Paddle properties
    float paddleSpeed = 250.f;
    bool topPadStatus = true, botPadStatus = true;

    // Ball properties
    float ballSpeed = 250.f;
    float moveAngle = 80.f; // with x-axis
    float angleToRadian = moveAngle * pi * 1.f / 180;

    // Bot properties
    bool predictStatus = false;
    float predictPos = SCREEN_WIDTH/2 ;
    
    sf::Clock clock;        // Time each loop
    sf::Clock playTime;     // Time each turn

    bool start = false;     // Game start
    float deltaTime = 0.f;  // 1/deltaTime = FPS

    // Player properties
    int botPoint = 0, humanPoint = 0;
    float curTimer=0;
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        curTimer += deltaTime;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed()) {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !start) {
            start = true;
        }
        if (!start) {
            window.clear(sf::Color::Black);
            window.draw(border);
            topPaddle.setPosition(SCREEN_WIDTH / 2, 20);
            window.draw(topPaddle);
            botPaddle.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 20);
            window.draw(botPaddle);
            ball.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
            window.draw(ball);
            window.display();
        }
        if (start) {
            // Ball speeds up after each 20s
            if (playTime.getElapsedTime().asSeconds() >= 15) {
                ballSpeed += 100.f;
                playTime.restart();
            }

            // Top paddle movement 
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && topPaddle.getPosition().x >= (topPaddle.getSize().x / 2 + border.getOutlineThickness() + SCREEN_WIDTH / 2 - border.getSize().x / 2))
                topPaddle.move(-paddleSpeed * deltaTime, 0);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && topPaddle.getPosition().x <= (SCREEN_WIDTH - topPaddle.getSize().x / 2 - border.getOutlineThickness() - (SCREEN_WIDTH / 2 - border.getSize().x / 2)))
                topPaddle.move(paddleSpeed * deltaTime, 0);

            // Bottom paddle movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && botPaddle.getPosition().x >= (botPaddle.getSize().x / 2 + border.getOutlineThickness() + SCREEN_WIDTH / 2 - border.getSize().x / 2))
                botPaddle.move(-paddleSpeed * deltaTime, 0);

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && botPaddle.getPosition().x <= (SCREEN_WIDTH - botPaddle.getSize().x / 2 - border.getOutlineThickness() - (SCREEN_WIDTH / 2 - border.getSize().x / 2)))
                botPaddle.move(paddleSpeed * deltaTime, 0);

            // Ball behaviour
            ball.move(cos(angleToRadian) * ballSpeed * deltaTime, sin(angleToRadian) * ballSpeed * deltaTime);

            // y-axis collision
            if (abs(moveAngle) > 90) {
                wallRight = false;
                wallLeft = true;
            }
            else {
                wallRight = true;
                wallLeft = false;
            }
            if ( (ball.getPosition().x >= (SCREEN_WIDTH / 2 + border.getSize().x / 2 - ball.getRadius()) && wallRight)
                ||
                (ball.getPosition().x <= (SCREEN_WIDTH / 2 - border.getSize().x / 2 + ball.getRadius()) && wallLeft) )
            {
                ballColSound.play();
                if (moveAngle > 0) moveAngle = 180.f - moveAngle;
                else moveAngle = -180.f - moveAngle;
            }
            angleToRadian = moveAngle * pi * 1.f / 180;
            
            // Paddle colision
            if ((ball.getPosition().y >= (botPaddle.getPosition().y - botPaddle.getSize().y / 2 - botPaddle.getOutlineThickness() - ball.getRadius() - 4)
                    &&
                ball.getPosition().x <= (botPaddle.getPosition().x + 50.f) && ball.getPosition().x >= (botPaddle.getPosition().x - 50.f)
                    &&
                botPadStatus)
                ||
                (ball.getPosition().y <= (topPaddle.getPosition().y + topPaddle.getSize().y / 2 + topPaddle.getOutlineThickness() + ball.getRadius() + 4)
                    &&
                ball.getPosition().x <= (topPaddle.getPosition().x + 50.f) && ball.getPosition().x >= (topPaddle.getPosition().x - 50.f)
                    &&
                topPadStatus))
            {
                ballColSound.play();
                if (ball.getPosition().y > SCREEN_HEIGHT / 2) {
                    botPadStatus = false;
                    topPadStatus = true;
                }
                else {
                    botPadStatus = true;
                    topPadStatus = false;
                }
                float curPosBallX = ball.getPosition().x;
                float curPosPaddleX;
                if (ball.getPosition().y > SCREEN_HEIGHT / 2) curPosPaddleX = botPaddle.getPosition().x;
                else curPosPaddleX = topPaddle.getPosition().x;
                std::cout << "Ball: " << curPosBallX << " Paddle: " << curPosPaddleX << std::endl;
                float dif = curPosBallX - curPosPaddleX;
                float changeOfAngle = 1 - abs(dif) * 2 / botPaddle.getSize().x;
                if (changeOfAngle <= 0.3) changeOfAngle += 0.1f;
                if (dif >= 0) moveAngle = -moveAngle / abs(moveAngle) * changeOfAngle * 90;
                else {
                    if (ball.getPosition().y > SCREEN_HEIGHT/2) moveAngle = -180 - -moveAngle / abs(moveAngle) * changeOfAngle * 90;
                    else moveAngle = 180 - -moveAngle / abs(moveAngle) * changeOfAngle * 90;
                } 
                std::cout << "Angle: " << moveAngle << "\nBallPos: " << ball.getPosition().x << " " << ball.getPosition().y << std::endl;
            }
            angleToRadian = moveAngle * pi * 1.f / 180;

            // Lose
            if (ball.getPosition().y >= botPaddle.getPosition().y - botPaddle.getSize().y / 2 - ball.getRadius()
                ||
                ball.getPosition().y <= topPaddle.getPosition().y + topPaddle.getSize().y / 2 + ball.getRadius()) 
            {
                if (ball.getPosition().y >= botPaddle.getPosition().y - botPaddle.getSize().y / 2 - ball.getRadius()) {
                    botPoint++;
                }
                else if (ball.getPosition().y <= topPaddle.getPosition().y + topPaddle.getSize().y / 2 + ball.getRadius()) {
                    humanPoint++;
                }
                start = false;
                ballSpeed = 250.f;
                moveAngle = 80.f;
                botPadStatus = true;
                topPadStatus = true;
                predictStatus = false;
            }
            
            // Bot
            if (moveAngle > 0) {
                predictStatus = false;
                float curBotPosX = topPaddle.getPosition().x, dir;
                if (curBotPosX >= SCREEN_WIDTH / 2) dir = -1;
                else dir = 1;
                if (curBotPosX != SCREEN_WIDTH / 2) topPaddle.move(dir * paddleSpeed * deltaTime, 0);
            }
            else if (!predictStatus){
                predictStatus = true;
                float totalBallXMovement = abs((ball.getPosition().y - topPaddle.getPosition().y - topPaddle.getSize().y/2 - ball.getRadius() - topPaddle.getOutlineThickness() - 4) / tan(angleToRadian));
                if (abs(moveAngle) < 90) {
                    float posBallToLeft = SCREEN_WIDTH / 2 + border.getSize().x / 2 - ball.getPosition().x - ball.getRadius();
                    float fnWidth = (totalBallXMovement - posBallToLeft) / (border.getSize().x - 2*ball.getRadius());
                    if (fnWidth > 0) {
                        int nnWidth = fnWidth;
                        float remainPath = (fnWidth - nnWidth) * (border.getSize().x - 2 * ball.getRadius()) + ball.getRadius();
                        if (nnWidth % 2 == 0) {
                            predictPos = border.getSize().x - remainPath + SCREEN_WIDTH / 2 - border.getSize().x / 2; 
                        }
                        else {
                            predictPos = remainPath + SCREEN_WIDTH / 2 - border.getSize().x / 2; 
                        }
                    }
                    else { 
                        predictPos = ball.getPosition().x + totalBallXMovement;   
                    }
                    std::cout << "Predicted position: " << predictPos << std::endl;
                    predictPos += ((rand() % 70) + -35); // make bot randomly chooses the collision pos
                }
                else {
                    float posBallToRight = ball.getPosition().x - ball.getRadius() - (SCREEN_WIDTH/2 - border.getSize().x/2);
                    float fnWidth = (totalBallXMovement - posBallToRight) / (border.getSize().x - 2 * ball.getRadius());
                    if (fnWidth > 0) {
                        int nnWidth = fnWidth;
                        float remainPath = (fnWidth - nnWidth) * (border.getSize().x - 2 * ball.getRadius()) + ball.getRadius();
                        if (nnWidth % 2 == 0) {
                            predictPos = remainPath + SCREEN_WIDTH / 2 - border.getSize().x / 2;
                        }
                        else {
                            predictPos = border.getSize().x - remainPath + SCREEN_WIDTH / 2 - border.getSize().x / 2;
                        }
                    }
                    else {
                        predictPos = ball.getPosition().x - totalBallXMovement;
                    }
                    std::cout << "Predicted position: " << predictPos << std::endl;
                    predictPos += ((rand() % 70) + -35);
                }
            }
            else {
                if ((predictPos > (SCREEN_WIDTH / 2 - border.getSize().x / 2 + topPaddle.getSize().x / 2 + topPaddle.getOutlineThickness())) && (predictPos < SCREEN_WIDTH / 2)) {
                    if (topPaddle.getPosition().x > (predictPos)) topPaddle.move(-paddleSpeed * deltaTime, 0);
                }
                else if (predictPos <= (SCREEN_WIDTH / 2 - border.getSize().x / 2 + topPaddle.getSize().x / 2 + topPaddle.getOutlineThickness())) {
                    if(topPaddle.getPosition().x > topPaddle.getSize().x / 2 + border.getOutlineThickness() + SCREEN_WIDTH / 2 - border.getSize().x / 2) topPaddle.move(-paddleSpeed * deltaTime, 0);
                }
                else if ((predictPos < (SCREEN_WIDTH / 2 + border.getSize().x / 2 - topPaddle.getSize().x / 2 - topPaddle.getOutlineThickness())) && (predictPos > SCREEN_WIDTH / 2)) {
                    if (topPaddle.getPosition().x < (predictPos)) topPaddle.move(paddleSpeed * deltaTime, 0);
                }
                else {
                    if (topPaddle.getPosition().x < border.getSize().x / 2 - topPaddle.getSize().x / 2 - border.getOutlineThickness() + SCREEN_WIDTH / 2) topPaddle.move(paddleSpeed * deltaTime, 0);
                }
            }
            
            window.clear(sf::Color::Black);
            window.draw(border);
            window.draw(topPaddle);
            window.draw(botPaddle);
            window.draw(ball);
            window.display();
        }
    }


    return 0;
}