#include "player.h"
#include "include/player.h"
#include "include/vector2d.h"
#include <iostream>

Player::Player() {}

Player::Player(Vector2D pos, int stamina, int staminaMax, int fov)
    : position(pos), stamina(stamina), staminaMax(staminaMax), fieldofvision(fov) {}

// Getters
Vector2D Player::getPosition() const { return position; }
int Player::getStamina() const { return stamina; }
int Player::getStaminaMax() const { return staminaMax; }
int Player::getfieldofvision() const { return fieldofvision; }

// Setters
void Player::setPosition(const Vector2D& pos) { position = pos; }
void Player::setStamina(int s) { stamina = s; }
void Player::setStaminaMax(int sMax) { staminaMax = sMax; }
void Player::setfieldofvision(int fov) { fieldofvision = fov; }
