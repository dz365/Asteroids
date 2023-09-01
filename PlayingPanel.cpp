#include "PlayingPanel.h"
#include <random>
#include <string>
#include "GameOverPanel.h"

constexpr auto PI = 3.14159265358979323846;

std::random_device rd; // Create a random number generator engine
std::mt19937 gen(rd()); // Mersenne Twister engine

// TIMERS
constexpr UINT_PTR GENERATE_ASTEROID_TIMER_ID = 1;
constexpr UINT_PTR GENERATE_ALIEN_TIMER_ID = 2;
constexpr UINT_PTR PLAYER_FIRING_SPEED_TIMER_ID = 3;
constexpr UINT_PTR REFRESH_RATE_TIMER_ID = 4;
constexpr UINT_PTR KEYACTION_TIMER_ID = 5;
constexpr UINT_PTR UPDATE_GAMEOBJECTS_TIMER_ID = 6;

PlayingPanel::PlayingPanel(HWND hwnd) : 
    Panel(hwnd),
    player(std::make_shared<Player>()),
    asteroidObjects(),
    bulletObjects(),
    alienObjects(),
    alienBulletObjects(),
    alienTimers(),
    pressedKeys(),
    numAliensGenerated(0),
    score(0),
    allowedToFire(true) {

    // Start timers
    SetTimer(hwnd, GENERATE_ASTEROID_TIMER_ID, 2000, nullptr);
    SetTimer(hwnd, GENERATE_ALIEN_TIMER_ID, 10000, nullptr);
    SetTimer(hwnd, PLAYER_FIRING_SPEED_TIMER_ID, 300, nullptr);
    SetTimer(hwnd, REFRESH_RATE_TIMER_ID, 10, nullptr);
    SetTimer(hwnd, KEYACTION_TIMER_ID, 50, nullptr);
    SetTimer(hwnd, UPDATE_GAMEOBJECTS_TIMER_ID, 50, nullptr);
}

void PlayingPanel::handleKeyAction(KeyAction action, WPARAM key)
{
    if (action == KeyAction::KEYDOWN)
        pressedKeys.insert(key);
    else
        pressedKeys.erase(key);
}

void PlayingPanel::handlePressedKeys()
{
    if (pressedKeys.find('W') == pressedKeys.end()) {
        player->stopEngine();
    }
    else {
        player->startEngine();
    }

    if (pressedKeys.find('A') != pressedKeys.end())
        player->setRotation(player->getRotation() - PI / 12);

    if (pressedKeys.find('D') != pressedKeys.end())
        player->setRotation(player->getRotation() + PI / 12);

    if (allowedToFire &&
        pressedKeys.find(VK_SPACE) != pressedKeys.end() &&
        player->getBulletsAvailable() > 0) {
        POINT bulletPosition = player->getPosition();
        float playerRotation = player->getRotation();
        bulletPosition.x += static_cast<LONG>(sin(playerRotation) * 17);
        bulletPosition.y -= static_cast<LONG>(cos(playerRotation) * 17);
        bulletObjects.push_back(
            std::make_shared<Bullet>(bulletPosition, 15, player->getRotation(), 1));
        player->decreaseBullets();
        allowedToFire = false;
    }
}

// Handle collisions with asteroids and aliens
void PlayingPanel::handlePlayerCollision()
{
    RECT playerHitbox = player->getBoundingRect();
    for (auto& asteroid : asteroidObjects) {
        if (hasCollided(playerHitbox, asteroid->getBoundingRect())) {
            player->handleCollision();
            asteroid->handleCollision();
            asteroidObjects.remove(asteroid);
            break;
        }
    }
    for (auto& alien : alienObjects) {
        if (hasCollided(playerHitbox, alien->getBoundingRect())) {
            player->handleCollision();
            alien->handleCollision();
            KillTimer(hwnd, alien->getId());
            alienTimers.erase(alien->getId());
            alienObjects.remove(alien);
            break;
        }
    }
}

// Handle collisions with asteroids and aliens
void PlayingPanel::handlePlayerBulletCollision()
{
    std::list<std::shared_ptr<Bullet>> bulletsToRemove;
    std::list<std::shared_ptr<Asteroid>>asteroidsToRemove;
    std::list<std::shared_ptr<Alien>>aliensToRemove;

    for (auto& bullet : bulletObjects) {
        RECT bulletHitbox = bullet->getBoundingRect();
        for (auto& asteroid : asteroidObjects) {
            if (hasCollided(bulletHitbox, asteroid->getBoundingRect())) {
                bullet->handleCollision();
                asteroid->handleCollision();
                bulletsToRemove.push_back(bullet);
                asteroidsToRemove.push_back(asteroid);
                break;
            }
        }
        for (auto& alien : alienObjects) {
            if (hasCollided(bulletHitbox, alien->getBoundingRect())) {
                bullet->handleCollision();
                alien->handleCollision();
                bulletsToRemove.push_back(bullet);
                aliensToRemove.push_back(alien);
                break;
            }
        }

    }

    for (auto& alien : aliensToRemove) {
        KillTimer(hwnd, alien->getId());
        alienTimers.erase(alien->getId());
        alienObjects.remove(alien);
        score += 200;
    }

    for (auto& bullet : bulletsToRemove) {
        bulletObjects.remove(bullet);
        player->increaseBullets();
    }

    for (auto& asteroid : asteroidsToRemove) {
        asteroidObjects.remove(asteroid);
        handleAsteroidFragmentation(asteroid);
        score += 50 * (1 + static_cast<int>(asteroid->getSize()));
    }
}

void PlayingPanel::handleAsteroidFragmentation(std::shared_ptr<Asteroid> asteroid)
{
    POINT newAsteroidPosition = asteroid->getPosition();
    if (asteroid->getSize() != SMALL) {
        Size newAsteroidSize = static_cast<Size>(asteroid->getSize() - 1);
        asteroidObjects.push_back(std::make_shared<Asteroid>(
            newAsteroidPosition,
            asteroid->getVelocity(),
            asteroid->getRotation() - (PI / 2),
            newAsteroidSize)
        );
        asteroidObjects.push_back(std::make_shared<Asteroid>(
            newAsteroidPosition,
            asteroid->getVelocity(),
            asteroid->getRotation() + (PI / 2),
            newAsteroidSize)
        );
    }
}

void PlayingPanel::handleAlienBulletCollision() 
{
    RECT playerHitbox = player->getBoundingRect();
    std::list<std::shared_ptr<Bullet>> bulletsToRemove;

    for (auto& bullet : alienBulletObjects) {
        RECT bulletHitbox = bullet->getBoundingRect();
        // Check player collision
        if (hasCollided(bulletHitbox, playerHitbox)) {
            bullet->handleCollision();
            player->handleCollision();
            bulletsToRemove.push_back(bullet);
            break;
        }
    }
    for (auto& bullet : bulletsToRemove) {
        alienBulletObjects.remove(bullet);
    }
}

void PlayingPanel::handleOutOfBoundBullets()
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int screenWidth = clientRect.right - clientRect.left;
    int screenHeight = clientRect.bottom - clientRect.top;
    std::list<std::shared_ptr<Bullet>> bulletsToRemove;

    for (auto& bullet : bulletObjects) {
        POINT position = bullet->getPosition();
        if (position.x <= 0 || position.x > screenWidth - 1 || 
            position.y <= 0 || position.y > screenHeight - 1) {
            bulletsToRemove.push_back(bullet);
        }
    }

    for (auto& bullet : bulletsToRemove) {
        bulletObjects.remove(bullet);
        player->increaseBullets();
    }

    bulletsToRemove.clear();
    for (auto& bullet : alienBulletObjects) {
        POINT position = bullet->getPosition();
        if (position.x <= 0 || position.x > screenWidth - 1 ||
            position.y <= 0 || position.y > screenHeight - 1) {
            bulletsToRemove.push_back(bullet);
        }
    }
    for (auto& bullet : bulletsToRemove) {
        alienBulletObjects.remove(bullet);
    }
}

void PlayingPanel::render(Gdiplus::Graphics& graphics)
{
    
    graphics.Clear(Gdiplus::Color::Black);

    renderPlayerHealth(graphics);
    renderPlayerRemainingBullets(graphics);
    renderScore(graphics);

    renderGameObject(graphics, player);

    for (auto& obj : asteroidObjects) {
        renderGameObject(graphics, obj);
    }
    for (auto& obj : bulletObjects) {
        renderGameObject(graphics, obj);
    }
    for (auto& obj : alienObjects) {
        renderGameObject(graphics, obj);
    }
    for (auto& obj : alienBulletObjects) {
        renderGameObject(graphics, obj);
    }

    if (player->getHealth() <= 0) {
        gameOver();
        std::list<std::shared_ptr<GameObject>> gameObjects;
        gameObjects.push_back(player);
        gameObjects.insert(gameObjects.end(), asteroidObjects.begin(), asteroidObjects.end());
        gameObjects.insert(gameObjects.end(), bulletObjects.begin(), bulletObjects.end());
        gameObjects.insert(gameObjects.end(), alienObjects.begin(), alienObjects.end());
        gameObjects.insert(gameObjects.end(), alienBulletObjects.begin(), alienBulletObjects.end());
        std::shared_ptr<GameOverPanel> panel = std::make_shared<GameOverPanel>(hwnd, gameObjects, score);
        panel->setGamePanelContext(context);
        context->changePanel(panel);
        panel->render(graphics);
    }
}

void PlayingPanel::update()
{
    // Update positions
    player->update(hwnd);

    for (auto& obj : asteroidObjects) {
        obj->update(hwnd);
    }
    for (auto& obj : bulletObjects) {
        obj->update(hwnd);
    }
    for (auto& obj : alienObjects) {
        obj->update(hwnd);
    }
    for (auto& obj : alienBulletObjects) {
        obj->update(hwnd);
    }
    
    handlePlayerCollision();
    handlePlayerBulletCollision();        
    handleAlienBulletCollision();
    handleOutOfBoundBullets();
    
}

void PlayingPanel::handleTimerAction(UINT_PTR timerId)
{
    switch (timerId) {
    case GENERATE_ASTEROID_TIMER_ID:
        generateAsteroid();
        break;
    case GENERATE_ALIEN_TIMER_ID:
        generateAlien();
        break;
    case PLAYER_FIRING_SPEED_TIMER_ID:
        allowedToFire = true;
        break;
    case REFRESH_RATE_TIMER_ID:
        InvalidateRect(hwnd, nullptr, FALSE);
        break;
    case KEYACTION_TIMER_ID:
        handlePressedKeys();
        break;
    case UPDATE_GAMEOBJECTS_TIMER_ID:
        update();
        break;
    default:
        generateAlienBullet(timerId); // timer id is the alien id
    }
}

void PlayingPanel::generateAsteroid()
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int screenWidth = clientRect.right - clientRect.left;
    int screenHeight = clientRect.bottom - clientRect.top;

    std::uniform_int_distribution<int> distribution(0, screenWidth);

    // Generate a random number
    int randomX = distribution(gen);
    int randomY = distribution(gen) % 2 ? 0 : screenHeight;
    int randomRotation = distribution(gen);
    int velocity = 5 + distribution(gen) % 5;
    Size size = static_cast<Size>(distribution(gen) % 3);
    POINT position = { randomX, randomY };
    asteroidObjects.push_back(
        std::make_shared<Asteroid>(position, velocity, randomRotation, size));
}

void PlayingPanel::generateAlien()
{
    if (score < 1000) return;    // Calm before the storm
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int screenHeight = clientRect.bottom - clientRect.top;

    std::uniform_int_distribution<int> distribution(0, screenHeight - 10);
    POINT position = { 0, distribution(gen) };
    std::shared_ptr<Alien> newAlien =
        std::make_shared<Alien>(position, 10 + numAliensGenerated);
    alienObjects.push_back(newAlien);
    alienTimers[newAlien->getId()] = newAlien;
    SetTimer(hwnd, newAlien->getId(), 1000, nullptr);
    numAliensGenerated++;
}

void PlayingPanel::generateAlienBullet(int alienId)
{
    if (alienTimers.find(alienId) == alienTimers.end()) return;

    std::shared_ptr<Alien> alien = alienTimers[alienId];
    POINT bulletPosition = alien->getPosition();
    float alienRotation = alien->getRotation();
    alienBulletObjects.push_back(
        std::make_shared<Bullet>(bulletPosition, 15, alienRotation + PI / 2, 1));
}

void PlayingPanel::renderScore(Gdiplus::Graphics& graphics) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int screenWidth = clientRect.right - clientRect.left;
    Gdiplus::SolidBrush textBrush(Gdiplus::Color(255, 255, 255));
    Gdiplus::FontFamily fontFamily(L"Arial");
    Gdiplus::Font font(&fontFamily, 32, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::PointF point(screenWidth / 2 - 16, 10);
    std::wstring scoreStr = std::to_wstring(score);
    graphics.DrawString(scoreStr.c_str(), -1, &font, point, &textBrush);
}

void PlayingPanel::renderPlayerHealth(Gdiplus::Graphics& graphics) {
    Gdiplus::Image spaceship(L"spaceship.png"); // 24x24px
    for (int i = 0; i < player->getHealth(); i++) {
        graphics.DrawImage(&spaceship, 30 * i + 10, 10);
    }
}

void PlayingPanel::renderPlayerRemainingBullets(Gdiplus::Graphics& graphics) {
    Gdiplus::Image bullet(L"bullet.png"); // 24x24px
    for (int i = 0; i < player->getBulletsAvailable(); i++) {
        graphics.DrawImage(&bullet, 10 * i + 10, 40);
    }
}

void PlayingPanel::renderGameObject(Gdiplus::Graphics& graphics, std::shared_ptr<GameObject> obj) {
    Gdiplus::GraphicsState graphicsState = graphics.Save();
    obj->render(graphics);
    // Restore the original state of the Graphics object
    graphics.Restore(graphicsState);
}

bool PlayingPanel::hasCollided(RECT r1, RECT r2)
{
    return !(
        r1.left > r2.right || r2.left > r1.right ||     // horizontal collision
        r1.bottom < r2.top || r2.bottom < r1.top);      // vertical collision
}

void PlayingPanel::gameOver()
{
    for (const auto& pair : alienTimers)
        KillTimer(hwnd, pair.first);
    KillTimer(hwnd, GENERATE_ASTEROID_TIMER_ID);
    KillTimer(hwnd, GENERATE_ALIEN_TIMER_ID );
    KillTimer(hwnd, PLAYER_FIRING_SPEED_TIMER_ID);
    KillTimer(hwnd, REFRESH_RATE_TIMER_ID);
    KillTimer(hwnd, KEYACTION_TIMER_ID);
    KillTimer(hwnd, UPDATE_GAMEOBJECTS_TIMER_ID);
}

void PlayingPanel::handleButtonClickedAction(UINT_PTR buttonId)
{
}
