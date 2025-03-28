#include "player.h"
#include "include/player.h"
#include "include/vector2d.h"
#include <iostream>

Player::Player() {}

Player::Player(Vector2D pos, int stamina, int staminaMax, int fov) {

}

#pragma region Getters
Vector2D Player::getPos() const { return this->pos; }
int Player::getStamina() const { return this->stamina; }
int Player::getStaminaMax() const { return this->staminaMax; }
int Player::getFov() const { return this->fov; }
int Player::getRationStaminaBonus() const { return this->rationCapacityBonus; }
int Player::getStaminaMaxBonus() const { return this->staminaMaxBonus; }
int Player::getFovBonus() const { return this->fovBonus; }
int Player::getRationCapacityBonus() const { return this->rationCapacityBonus; }
int Player::getPickaxeCapacityBonus() const { return this->pickaxeCapacityBonus; }
#pragma endregion
#pragma region Setters
void Player::setPos(const Vector2D pos) { this->pos = pos; }
void Player::setStamina(const int value) { this->stamina = value; }
void Player::setStaminaMax(const int value) { this->staminaMax = value; }
void Player::setFov(const int value) { this->fov = value; }
void Player::setRationStaminaBonus(const int value) { this->rationStaminaBonus = value; }
void Player::setStaminaMaxBonus(const int value) { this->staminaMaxBonus = value; }
void Player::setFovBonus(const int value) { this->fovBonus = value; }
void Player::setRationCapacityBonus(const int value) { this->rationCapacityBonus = value; }
void Player::setPickaxeCapacityBonus(const int value) { this->pickaxeCapacityBonus = value; }
#pragma endregion
