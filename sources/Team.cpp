#include "Team.hpp"
#include <iostream>
#include <climits>
#include <stdexcept>

using namespace ariel;
using namespace std;

// Constructor
Team::Team(Character *leader)
{
    if (leader->isLeader())
    {
        throw runtime_error("Character is already a leader");
        return;
    }
    leader->setLeader();
    this->leader = leader;
    add(leader);
};
// Destructor
Team::~Team()
{

    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        delete characters[i];
        characters[i] = NULL;
    }
    leader = NULL;
};

SmartTeam::~SmartTeam()
{
    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        delete characters[i];
        characters[i] = NULL;
    }

    leader = NULL;
};

void Team::add(Character *newCharacter)
{
    validateTeamSize();
    validateCharacterNotInTeam(newCharacter);
    validateCharacterNotAddedToOtherTeam(newCharacter);
    allowCharacterInTeam(newCharacter);
    addCharacterToTeam(newCharacter);
    incrementCount();
}

void Team::validateTeamSize()
{
    if (count == TEAM_SIZE)
    {
        throw std::runtime_error("\033[1;31mError:\033[0m There is no place in the team");
    }
}

void Team::validateCharacterNotInTeam(Character *character)
{
    if (inTeam(character))
    {
        throw std::runtime_error("\033[1;31mError:\033[0m This character is already in the team");
    }
}

void Team::validateCharacterNotAddedToOtherTeam(Character *character)
{
    if (character->isInTeam())
    {
        throw std::runtime_error("\033[1;31mError:\033[0m This character has already been added to a team");
    }
}

void Team::allowCharacterInTeam(Character *character)
{
    character->allowTeam();
}

void Team::addCharacterToTeam(Character *newCharacter)
{
    Cowboy *c = dynamic_cast<Cowboy *>(newCharacter);
    Ninja *n = dynamic_cast<Ninja *>(newCharacter);

    if (c != nullptr && cowboyCount < TEAM_SIZE) // Cowboy
    {
        characters[cowboyCount++] = newCharacter;
    }
    else if (n != nullptr && (TEAM_SIZE - 1 - (count - cowboyCount)) >= 0) // Ninja
    {
        characters[TEAM_SIZE - 1 - (count - cowboyCount)] = newCharacter;
    }
    else
    {
        throw std::runtime_error("\033[1;31mError:\033[0m Invalid character type or team is full");
    }
}

void Team::incrementCount()
{
    count++;
}

bool Team::inTeam(Character *character)
{
    for (unsigned int i = 0; i < count; i++)
    {
        if (characters[i] == character)
        {
            return true;
        }
    }
    return false;
}

int Team::stillAlive() const
{
    int alive = 0;
    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        if (characters[i] && characters[i]->isAlive())
        {
            alive++;
        }
    }
    return alive;
};

void Team::print() const
{
    const int width = 35;
    std::cout << "\033[1;34m" << std::string(width, '-') << '\n';
    std::cout << "\033[1;32m" << std::setw(width / 2) << std::right << "Team Members" << '\n';
    std::cout << "\033[1;34m" << std::string(width, '-') << '\n';

    // Cowboys
    for (unsigned int i = 0; i < cowboyCount; i++)
    {
        if (characters[i])
        {
            std::cout << "\033[1;35m" << std::left << std::setw(width / 2) << (characters[i] == leader ? "LEADER" : "MEMBER")
                      << "\033[0m" << characters[i]->print() << '\n';
        }
    }
    // Ninjas
    unsigned int ninjaCount = count - cowboyCount;
    for (unsigned int i = TEAM_SIZE - 1; i >= TEAM_SIZE - ninjaCount && i < TEAM_SIZE; i--)
    {
        if (characters[i])
        {
            std::cout << "\033[1;36m" << std::left << std::setw(width / 2) << (characters[i] == leader ? "LEADER" : "MEMBER")
                      << "\033[0m" << characters[i]->print() << '\n';
        }
    }
    std::cout << "\033[1;34m" << std::string(width, '-') << "\033[0m" << '\n';
}

void Team::newLeader()
{
    leader = CloseCharacter(leader, this);
    leader->setLeader();
};

void Team::attack(Team *otherTeam)
{
    validateOtherTeamNotNull(otherTeam);
    validateNotAttackingItself(otherTeam);
    validateSelfNotEmpty();
    validateOtherTeamNotEmpty(otherTeam);

    ensureLeaderIsAlive();
    Character *target = findClosestTarget(otherTeam);
    if (!target)
        return;

    performCowboyAttacks(target, otherTeam);
    performNinjaAttacks(target, otherTeam);
}

void Team::validateOtherTeamNotNull(Team *otherTeam)
{
    if (otherTeam == nullptr)
    {
        throw std::invalid_argument("Can't attack NULL team");
    }
}

void Team::validateNotAttackingItself(Team *otherTeam)
{
    if (this == otherTeam)
    {
        throw std::invalid_argument("Can't attack itself");
    }
}

void Team::validateSelfNotEmpty()
{
    if (stillAlive() == 0)
    {
        throw std::runtime_error("Dead/empty team can't attack");
    }
}

void Team::validateOtherTeamNotEmpty(Team *otherTeam)
{
    if (otherTeam->stillAlive() == 0)
    {
        throw std::runtime_error("Can't attack dead/empty team");
    }
}

void Team::ensureLeaderIsAlive()
{
    if (!leader->isAlive())
    {
        newLeader();
    }
}

Character *Team::findClosestTarget(Team *otherTeam)
{
    return CloseCharacter(leader, otherTeam);
}

void Team::performCowboyAttacks(Character *target, Team *otherTeam)
{
    for (unsigned int i = 0; i < cowboyCount; i++)
    {
        if (characters[i]->isAlive())
        {
            Cowboy *cowboy = dynamic_cast<Cowboy *>(characters[i]);
            if (cowboy != nullptr && cowboy->hasboolets())
            {
                cowboy->shoot(target);
            }
            else
            {
                cowboy->reload();
            }
            target = isTarget(target, otherTeam);
            if (!target)
                return;
        }
    }
}
void Team::performNinjaAttacks(Character *target, Team *otherTeam)
{
    unsigned int ninjaCount = count - cowboyCount;
    for (unsigned int i = TEAM_SIZE - 1; i >= TEAM_SIZE - ninjaCount; i--)
    {
        performNinjaAction(i, target, otherTeam);
        target = isTarget(target, otherTeam);
        if (!target)
            break; // Exit the loop if no target found
    }
}

void Team::performNinjaAction(unsigned int index, Character *target, Team *otherTeam)
{
    if (characters[index]->isAlive())
    {
        Ninja *ninja = dynamic_cast<Ninja *>(characters[index]);
        if (ninja != nullptr && ninja->distance(target) <= 1)
        {
            if (target->isAlive())
            {
                ninja->slash(target);
            }
        }
        else
        {
            ninja->move(target);
        }
    }
}

Character *Team::isTarget(Character *target, Team *otherTeam)
{
    if (!target->isAlive())
    {
        target = CloseCharacter(leader, otherTeam);
    }
    return target;
}

// Team2

Team2::~Team2()
{
    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        delete characters[i];
        characters[i] = NULL;
    }

    leader = NULL;
};

Team2::Team2(Character *leader)
{
    if (leader->isLeader())
    {
        throw runtime_error("Already a leader");
        return;
    }
    leader->setLeader();
    this->leader = leader;
    this->add(leader);
};

void Team2::add(Character *newCharacter)
{
    if (count == TEAM_SIZE)
    {
        errormsg("Team is full");
        throw runtime_error("Team is full");
        return;
    }
    else if (inTeam(newCharacter))
    {
        throw runtime_error("Character already in team");
        return;
    }
    else if (newCharacter->isInTeam())
    {
        throw runtime_error("Character is already in a team");
        return;
    }
    Cowboy *c = dynamic_cast<Cowboy *>(newCharacter);
    Ninja *n = dynamic_cast<Ninja *>(newCharacter);
    if (c != NULL || n != NULL)
    {
        characters[count++] = newCharacter;
    }
    else
    {
        errormsg("Invalid character type (not Cowboy or Ninja))");
        throw runtime_error("Invalid character type (not Cowboy or Ninja))");
        return;
    }
}

void Team2::print() const
{
    std::cout << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::left << std::setw(15) << "Team Details" << std::endl;

    if (count == 0)
    {
        errormsg("No team members added yet.");
        std::cout << "No team members added yet." << std::endl;
    }
    else
    {
        std::cout << std::setw(15) << "Team Members: " << std::endl;
        // Cowboys
        for (unsigned int i = 0; i < count; i++)
        {
            std::cout << std::setw(5) << i + 1 << ". ";
            if (characters[i] == leader)
            {
                std::cout << "(Leader) ";
            }
            std::cout << characters[i]->print() << std::endl;
        }
    }

    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::endl;
}

void Team2::validateAttack(Team *enemies)
{
    validateEnemiesNotNull(enemies);
    validateNotAttackingSelf(enemies);
    validateAttackerAlive();
    validateEnemiesAlive(enemies);
}

void Team2::validateEnemiesNotNull(Team *enemies)
{
    if (enemies == NULL)
    {
        errormsg("Can't attack NULL team");
        throw invalid_argument("Can't attack NULL team");
    }
}

void Team2::validateNotAttackingSelf(Team *enemies)
{
    if (this == enemies)
    {
        errormsg("Can't attack itself");
        throw invalid_argument("Can't attack itself");
    }
}

void Team2::validateAttackerAlive()
{
    if (stillAlive() == 0)
    {
        errormsg("Dead/empty team can't attack");
        throw runtime_error("Dead/empty team can't attack");
    }
}

void Team2::validateEnemiesAlive(Team *enemies)
{
    if (enemies->stillAlive() == 0)
    {
        errormsg("Can't attack dead/empty team");
        throw invalid_argument("Can't attack dead/empty team");
    }
}

Character *Team2::prepareAttack(Team *enemies)
{
    if (!leader->isAlive())
    {
        newLeader();
    }
    return CloseCharacter(leader, enemies); // find target that is close to the leader
}

void Team2::executeAttack(Team *enemies, Character *target)
{
    for (unsigned int i = 0; i < count; i++)
    {
        if (characters[i]->isAlive())
        {
            Cowboy *c = dynamic_cast<Cowboy *>(characters[i]);
            Ninja *n = dynamic_cast<Ninja *>(characters[i]);
            if (c != NULL)
            {
                if (c->hasboolets())
                {
                    c->shoot(target);
                }
                else
                {
                    c->reload();
                }
            }
            else if (n != NULL)
            {
                if (n->distance(target) <= 1)
                {
                    n->slash(target);
                }
                else
                {
                    n->move(target);
                }
            }
            target = isTarget(target, enemies); // check if target is still alive or find a new target
            if (!target)
            {
                break;
            }
        }
    }
}

void Team2::attack(Team *enemies)
{
    validateAttack(enemies);
    Character *target = prepareAttack(enemies);
    if (target)
    {
        executeAttack(enemies, target);
    }
}

// Smart Team

SmartTeam::SmartTeam(Character *leader) : Team(leader){};

void SmartTeam::validateAttack(Team *otherTeam)
{
    if (otherTeam == NULL)
    {
        errormsg("Cant attack NULL team");
        throw std::invalid_argument("Cant attack NULL team");
    }
    if (this == otherTeam)
    {
        errormsg("Cant attack itself");
        throw std::invalid_argument("Cant attack itself");
    }
    if (stillAlive() == 0)
    {
        errormsg("Dead/empty team cant attack");
        throw std::runtime_error("Dead/empty team cant attack");
    }
    if (otherTeam->stillAlive() == 0)
    {
        errormsg("Cant attack dead/empty team");
        throw std::invalid_argument("Cant attack dead/empty team");
    }
}

void SmartTeam::ninjasAttack(Team *otherTeam)
{
    unsigned int ninjaCount = count - cowboyCount;
    for (unsigned int i = 9; i > 9 - ninjaCount; i--)
    {
        if (characters[i]->isAlive())
        {
            Ninja &ninja = dynamic_cast<Ninja &>(*(characters[i]));
            Character *closestEnemy = CloseCharacter(characters[i], otherTeam);
            if (ninja.distance(closestEnemy) <= 1)
            {
                ninja.slash(closestEnemy);
            }
            else
            {
                ninja.move(closestEnemy);
            }
        }
    }
}

void SmartTeam::cowboysAttack(Team *otherTeam)
{
    for (unsigned int i = 0; i < cowboyCount; i++)
    {
        if (characters[i]->isAlive())
        {
            Cowboy &cowboy = dynamic_cast<Cowboy &>(*(characters[i]));
            Character *target = findTarget(cowboy, otherTeam);
            cowboyAction(cowboy, target);
        }
    }
}

Character *SmartTeam::findTarget(Cowboy &cowboy, Team *otherTeam)
{
    return findWeakestEnemy(otherTeam);
}

void SmartTeam::cowboyAction(Cowboy &cowboy, Character *target)
{
    if (cowboy.hasboolets())
    {
        cowboy.shoot(target);
    }
    else
    {
        cowboy.reload();
    }
}

void SmartTeam::attack(Team *otherTeam)
{
    validateAttack(otherTeam);
    ninjasAttack(otherTeam);
    cowboysAttack(otherTeam);
}

Character *Team::CloseCharacter(Character *character, Team *team)
{
    Character *closest = NULL;
    int minDistance = INT_MAX;
    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        if (team->characters[i] && team->characters[i]->isAlive())
        {
            int distance = character->distance(team->characters[i]);
            if (distance < minDistance)
            {
                minDistance = distance;
                closest = team->characters[i];
            }
        }
    }
    return closest;
}

int SmartTeam::findMinHealthEnemy(Team *otherTeam)
{
    int minHealth = INT_MAX;
    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        if (otherTeam->characters[i] && otherTeam->characters[i]->isAlive())
        {
            int health = otherTeam->characters[i]->whatHealth();
            if (health < minHealth)
            {
                minHealth = health;
            }
        }
    }
    return minHealth;
}

Character *SmartTeam::findWeakestEnemy(Team *otherTeam)
{
    int minHealth = findMinHealthEnemy(otherTeam);
    Character *weakestEnemy = NULL;
    for (unsigned int i = 0; i < TEAM_SIZE; i++)
    {
        if (otherTeam->characters[i] && otherTeam->characters[i]->isAlive() && otherTeam->characters[i]->whatHealth() == minHealth)
        {
            weakestEnemy = otherTeam->characters[i];
            break;
        }
    }
    return weakestEnemy;
}

// error handling
void Team::errormsg(std::string msg) const
{
    std::cerr << "Error: " << msg << std::endl;
}
