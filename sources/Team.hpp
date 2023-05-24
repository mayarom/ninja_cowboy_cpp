#pragma once

#include "Character.hpp"
#include <array>
#include <stdexcept>
#include <iomanip>

namespace ariel
{
    const unsigned int TEAM_SIZE = 10;

    class Team
    {
    public:
        // Constructors and destructor

        // Constructor with leader character
        Team(Character *leader);

        // Default constructor
        Team() = default;

        // Copy constructor
        Team(const Team &) = default;

        // Copy assignment operator
        Team &operator=(const Team &) = default;

        // Move constructor
        Team(Team &&) noexcept = default;

        // Move assignment operator
        Team &operator=(Team &&) noexcept = default;

        // Virtual destructor
        virtual ~Team();

        // Member functions

        // Check if a character is in the team
        bool inTeam(Character *character);

        // Set a new leader for the team
        void newLeader();

        // Check if a character is a target in the enemy team
        Character *isTarget(Character *target, Team *otherTeam);

        // Find the closest character to a given character in a team
        Character *CloseCharacter(Character *character, Team *team);

        // Add a character to the team
        virtual void add(Character *character);

        // Perform an attack on the enemy team
        virtual void attack(Team *enemies);

        // Get the number of characters still alive in the team
        int stillAlive() const;

        // Print the team's information
        virtual void print() const;

        // Data members

        // Array of characters in the team
        std::array<Character *, TEAM_SIZE> characters = {nullptr};

        // Number of characters in the team
        unsigned int count = 0;

        // Pointer to the leader character
        Character *leader = nullptr;

        // Number of cowboy characters in the team
        unsigned int cowboyCount = 0;

        void errormsg(std::string msg) const;

    private:
        void validateTeamSize();
        void validateCharacterNotInTeam(Character *character);
        void validateCharacterNotAddedToOtherTeam(Character *character);
        void allowCharacterInTeam(Character *character);
        void addCharacterToTeam(Character *newCharacter);
        void incrementCount();
        void validateOtherTeamNotNull(Team *otherTeam);
        void validateNotAttackingItself(Team *otherTeam);
        void validateSelfNotEmpty();
        void validateOtherTeamNotEmpty(Team *otherTeam);
        void ensureLeaderIsAlive();
        Character *findClosestTarget(Team *otherTeam);
        void performCowboyAttacks(Character *target, Team *otherTeam);
        void performNinjaAttacks(Character *target, Team *otherTeam);
        void performNinjaAction(unsigned int index, Character *target, Team *otherTeam);
    };

    class Team2 : public Team
    {
    public:
        // Constructors and destructor

        // Constructor with leader character
        Team2(Character *leader);

        // Default constructor
        Team2() = default;

        // Copy constructor
        Team2(const Team2 &) = default;

        // Copy assignment operator
        Team2 &operator=(const Team2 &) = default;

        // Move constructor
        Team2(Team2 &&) noexcept = default;

        // Move assignment operator
        Team2 &operator=(Team2 &&) noexcept = default;

        // Virtual destructor
        ~Team2() override;

        // Member functions

        // Add a character to the team
        void add(Character *character) override;

        // Perform an attack on the enemy team
        void attack(Team *enemies) override;

        // Print the team's information
        void print() const override;

    private:
        // Validate the attack on the enemy team
        void validateAttack(Team *enemies);

        // Prepare the attack by selecting a target
        Character *prepareAttack(Team *enemies);

        // Execute the attack on the enemy team
        void executeAttack(Team *enemies, Character *target);

        void validateEnemiesNotNull(Team *enemies);
        void validateNotAttackingSelf(Team *enemies);
        void validateAttackerAlive();
        void validateEnemiesAlive(Team *enemies);
    };

    class SmartTeam : public Team
    {
    public:
        // Constructors and destructor

        // Constructor with leader character
        SmartTeam(Character *leader);

        // Default constructor
        SmartTeam() = default;

        // Copy constructor
        SmartTeam(const SmartTeam &) = default;

        // Copy assignment operator
        SmartTeam &operator=(const SmartTeam &) = default;

        // Move constructor
        SmartTeam(SmartTeam &&) noexcept = default;

        // Move assignment operator
        SmartTeam &operator=(SmartTeam &&) noexcept = default;

        // Virtual destructor
        ~SmartTeam() override;

        // Member functions

        // Perform an attack on the enemy team
        void attack(Team *enemies) override;

        // Find the weakest enemy character in the enemy team
        Character *findWeakestEnemy(Team *enemies);

        // Validate the attack on the enemy team
        void validateAttack(Team *otherTeam);

        // Perform the attack of the ninjas in the team on the enemy team
        void ninjasAttack(Team *otherTeam);

        // Perform the attack of the cowboys in the team on the enemy team
        void cowboysAttack(Team *otherTeam);

        // Find a suitable target for a cowboy in the enemy team
        Character *findTarget(Cowboy &cowboy, Team *otherTeam);

        // Perform the action of a cowboy on a target
        void cowboyAction(Cowboy &cowboy, Character *target);

    private:
        // Helper functions

        // Find the enemy character with the minimum health in the enemy team
        int findMinHealthEnemy(Team *otherTeam);
    };
}
