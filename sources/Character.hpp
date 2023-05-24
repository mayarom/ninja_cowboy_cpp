#pragma once

#include <sstream>
#include <iomanip>
#include "Point.hpp"
#include <string>

namespace ariel
{

    class Character
    {
        Point position;
        int health;
        std::string name;
        bool inTeam = false, leader = false;

    public:
        Character(std::string name = "", int health = 0, Point position = Point(0, 0));
        bool isAlive() const;
        double distance(const Character *character) const;
        void hit(int damage);
        std::string getName() const;
        Point getLocation() const;
        virtual std::string print() const;
        void addLocation(Point point);
        int whatHealth() const;
        void allowTeam();
        void setLeader();
        bool isInTeam() const;
        bool isLeader() const;

        Character(const Character &) = default;
        Character &operator=(const Character &) = default;
        Character(Character &&) noexcept = default;
        Character &operator=(Character &&) noexcept = default;
        virtual ~Character() = default;
        // Error handling function
        void errormsg(std::string msg) const;

    protected:
        void validateDamage(int damage);
        void applyDamage(int damage);
        void validateLeader();
        void performSetLeader();
        void validateCharacter(const Character *character) const;
        double calculateDistance(const Character *character) const;
    };

    class Cowboy : public Character
    {
        int bullets = 0;

    public:
        Cowboy(std::string name, Point position);
        void shoot(Character *enemy);
        bool hasboolets() const;
        void reload();
        std::string print() const override;

        Cowboy() = default;
        Cowboy(const Cowboy &) = default;
        Cowboy &operator=(const Cowboy &) = default;
        Cowboy(Cowboy &&) noexcept = default;
        Cowboy &operator=(Cowboy &&) noexcept = default;
        ~Cowboy() override = default;

    private:
        void validateShootTarget(Character *enemy);
        void performShootAction(Character *enemy);
        void validateReload();
        void performReload();
        void validateEnemyNotNull(Character *enemy);
        void validateNotShootingSelf(Character *enemy);
        void validateAlive();
        void validateEnemyAlive(Character *enemy);
    };

    class Ninja : public Character
    {
        int speed = 0;

    public:
        Ninja(std::string name, int health, Point position, int speed = 0);
        void move(Character *enemy);
        void slash(Character *enemy);
        std::string print() const override;

        Ninja() = default;
        Ninja(const Ninja &) = default;
        Ninja &operator=(const Ninja &) = default;
        Ninja(Ninja &&) noexcept = default;
        Ninja &operator=(Ninja &&) noexcept = default;
        ~Ninja() override = default;

    private:
        void validateMove(Character *enemy);
        void performMove(Character *enemy);
        void validateEnemyNotNull(Character *enemy);
        void validateNotSlashingSelf(Character *enemy);
        void validateAlive();
        void validateEnemyAlive(Character *enemy);
        void performSlash(Character *enemy);
    };

    class YoungNinja : public Ninja
    {
    public:
        YoungNinja(std::string name, Point position);

        YoungNinja() = default;
        YoungNinja(const YoungNinja &) = default;
        YoungNinja &operator=(const YoungNinja &) = default;
        YoungNinja(YoungNinja &&) noexcept = default;
        YoungNinja &operator=(YoungNinja &&) noexcept = default;
        ~YoungNinja() override = default;
    };

    class OldNinja : public Ninja
    {
    public:
        OldNinja(std::string name, Point position);

        OldNinja() = default;
        OldNinja(const OldNinja &) = default;
        OldNinja &operator=(const OldNinja &) = default;
        OldNinja(OldNinja &&) noexcept = default;
        OldNinja &operator=(OldNinja &&) noexcept = default;
        ~OldNinja() override = default;
    };

    class TrainedNinja : public Ninja
    {
    public:
        TrainedNinja(std::string name, Point position);

        TrainedNinja() = default;
        TrainedNinja(const TrainedNinja &) = default;
        TrainedNinja &operator=(const TrainedNinja &) = default;
        TrainedNinja(TrainedNinja &&) noexcept = default;
        TrainedNinja &operator=(TrainedNinja &&) noexcept = default;
        ~TrainedNinja() override = default;
    };

} // namespace ariel
