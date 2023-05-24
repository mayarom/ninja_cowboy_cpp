#include "Character.hpp"
#include <string>
#include <iostream>
#include <stdexcept>

using namespace std;
using namespace ariel;

// Constructors
Character::Character(string name, int health, Point position) : name(name), health(health), position(position)
{
    // Validate health value
    if (health < 0)
    {
        throw invalid_argument("Health cannot be negative");
    }
}

Cowboy::Cowboy(string name, Point position) : Character(name, 110, position), bullets(6) {}

Ninja::Ninja(string name, int health, Point position, int speed) : Character(name, health, position), speed(speed)
{
    // Validate health and speed values
    if (health < 0)
    {
        throw invalid_argument("Health cannot be negative");
    }
    if (speed < 0)
    {
        throw invalid_argument("Speed cannot be negative");
    }
}

YoungNinja::YoungNinja(string name, Point position) : Ninja(name, 100, position, 14) {}

OldNinja::OldNinja(string name, Point position) : Ninja(name, 150, position, 8) {}

TrainedNinja::TrainedNinja(string name, Point position) : Ninja(name, 120, position, 12) {}
void Character::allowTeam()
{
    if (inTeam)
    {
        throw std::runtime_error("Character is already in a team");
    }
    inTeam = true;
}

void Character::setLeader()
{
    validateLeader();   // Validate if the character can be set as a leader
    performSetLeader(); // Set the character as the leader
}

void Character::validateLeader()
{
    if (leader)
    {
        throw std::runtime_error("Character is already a leader");
    }
}

void Character::performSetLeader()
{
    leader = true;
}

bool Character::isInTeam() const
{
    return inTeam;
}

bool Character::isLeader() const
{
    return leader;
}

int Character::whatHealth() const
{
    return health;
}

bool Character::isAlive() const
{
    return health > 0;
}

void Character::validateDamage(int damage)
{
    if (damage < 0)
    {
        throw invalid_argument("Damage cannot be negative");
    }
}

void Character::applyDamage(int damage)
{
    if (isAlive() && damage > 0)
    {
        health -= damage;
        if (health < 0)
        {
            health = 0;
        }
    }
}

void Character::hit(int damage)
{
    validateDamage(damage);
    applyDamage(damage);
}

string Character::getName() const
{
    return name;
}
Point Character::getLocation() const
{
    return position;
}
std::string Character::print() const
{
    std::string data;

    if (isAlive())
    {
        data += "Name: " + name + "\n";
        data += "Health: " + std::to_string(health) + " ❤️\n";
        data += "Position: " + position.print() + "\n";
    }
    else
    {
        data += "☠️☠️☠️ " + name + " ☠️☠️☠️\n";
        data += "Position: " + position.print() + "\n";
    }

    return data;
}

double Character::distance(const Character *character) const
{
    validateCharacter(character);        // Validate if the character is valid
    return calculateDistance(character); // Calculate the distance between the characters
}

void Character::validateCharacter(const Character *character) const
{
    if (character == nullptr)
    {
        throw std::invalid_argument("NULL character");
    }
}

double Character::calculateDistance(const Character *character) const
{
    return position.distance(character->getLocation());
}

void Character::addLocation(Point point)
{
    position = point;
}

void Cowboy::shoot(Character *enemy)
{
    validateShootTarget(enemy);
    performShootAction(enemy);
}
void Cowboy::reload()
{
    validateReload();
    performReload();
}

void Cowboy::validateReload()
{
    if (!isAlive())
    {
        throw std::runtime_error("Dead cowboys can't reload");
    }
}

void Cowboy::performReload()
{
    int bulletsToAdd = 6 - bullets;
    if (bulletsToAdd > 0)
    {
        bullets += bulletsToAdd;
    }
}

void Cowboy::validateShootTarget(Character *enemy)
{
    validateEnemyNotNull(enemy);
    validateNotShootingSelf(enemy);
    validateAlive();
    validateEnemyAlive(enemy);
}

void Cowboy::validateEnemyNotNull(Character *enemy)
{
    if (enemy == nullptr)
    {
        errormsg("NULL enemy");
        throw std::invalid_argument("NULL enemy");
    }
}

void Cowboy::validateNotShootingSelf(Character *enemy)
{
    if (this == enemy)
    {
        errormsg("Cowboys can't shoot themselves");
        throw std::runtime_error("Cowboys can't shoot themselves");
    }
}

void Cowboy::validateAlive()
{
    if (!isAlive())
    {
        errormsg("Dead cowboys can't shoot");
        throw std::runtime_error("Dead cowboys can't shoot");
    }
}

void Cowboy::validateEnemyAlive(Character *enemy)
{
    if (!enemy->isAlive())
    {
        throw std::runtime_error("Cowboys can't shoot dead enemies");
    }
}

void Cowboy::performShootAction(Character *enemy)
{
    if (hasboolets())
    {
        bullets--;
        enemy->hit(10);
    }
}

bool Cowboy::hasboolets() const
{
    return bullets > 0;
}

string Cowboy::print() const
{
    stringstream out;
    // break line
    out << endl;
    out << "    ╔═════════════════════════════╗" << endl;
    out << "    ║         Cowboy Stats        ║" << endl;
    out << "    ╟─────────────────────────────╢" << endl;
    out << "    ║   Name:     " << setw(16) << left << getName() << "║" << endl;
    out << "    ║   Health:   " << setw(16) << left << whatHealth() << "║" << endl;
    out << "    ║   Position: " << setw(16) << left << getLocation().print() << "║" << endl;
    out << "    ║   Ammo:     " << setw(16) << left << bullets << "║" << endl;
    out << "    ╚═════════════════════════════╝" << endl;
    return out.str();
}

void Ninja::slash(Character *enemy)
{
    validateEnemyNotNull(enemy);
    validateNotSlashingSelf(enemy);
    validateAlive();
    validateEnemyAlive(enemy);
    performSlash(enemy);
}

void Ninja::validateEnemyNotNull(Character *enemy)
{
    if (enemy == nullptr)
    {
        throw std::invalid_argument("NULL enemy");
    }
}

void Ninja::validateNotSlashingSelf(Character *enemy)
{
    if (this == enemy)
    {
        errormsg("Ninja, slash, Ninjas can't slash themselves");
        throw std::runtime_error("Ninjas can't slash themselves");
    }
}

void Ninja::validateAlive()
{
    if (!isAlive())
    {
        throw std::runtime_error("Dead ninjas can't slash");
    }
}

void Ninja::validateEnemyAlive(Character *enemy)
{
    if (!enemy->isAlive())
    {
        throw std::runtime_error("Ninjas can't slash dead enemies");
    }
}

void Ninja::performSlash(Character *enemy)
{
    if (distance(enemy) <= 1)
    {
        enemy->hit(40);
    }
}

string Ninja::print() const
{
    stringstream out;
    out << "\n   Members: " << endl;
    out << "    ╔═════════════════════════════╗" << endl;
    out << "    ║         Ninja Stats         ║" << endl;
    out << "    ╟─────────────────────────────╢" << endl;
    out << "    ║   Name:     " << setw(16) << left << getName() << "║" << endl;
    out << "    ║   Health:   " << setw(16) << left << whatHealth() << "║" << endl;
    out << "    ║   Position: " << setw(16) << left << getLocation().print() << "║" << endl;
    out << "    ║   Speed:    " << setw(16) << left << speed << "║" << endl;
    out << "    ╚═════════════════════errormsg════════╝" << endl;
    return out.str();
}

void Ninja::move(Character *enemy)
{
    validateMove(enemy);
    performMove(enemy);
}

void Ninja::validateMove(Character *enemy)
{
    if (enemy == nullptr)
    {
        errormsg("Invalid enemy: nullptr");
        throw std::invalid_argument("Invalid enemy: nullptr");
    }

    if (!isAlive())
    {
        errormsg("Dead ninjas cannot move");
        throw std::runtime_error("Dead ninjas cannot move");
    }

    if (this == enemy)
    {
        errormsg("Ninjas cannot move towards themselves");
        throw std::invalid_argument("Ninjas cannot move towards themselves");
    }
}

void Ninja::performMove(Character *enemy)
{
    Point enemyPos = enemy->getLocation();
    Point myPos = getLocation();

    if (!myPos.compare(enemyPos))
    {
        addLocation(Point::moveTowards(myPos, enemyPos, speed)); // set new position
    }
}
void Character::errormsg(std::string msg) const
{
    std::cerr << "Error: " << msg << std::endl;
}
