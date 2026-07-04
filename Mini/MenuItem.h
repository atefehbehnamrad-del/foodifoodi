#ifndef MENUITEM_H
#define MENUITEM_H

#include <string>

enum class ItemType {
    FOOD,
    DRINK,
    OTHER
};

class MenuItem {
protected:
    int id;
    std::string name;
    std::string description;
    double price;
    bool available;
    ItemType type;

public:
    MenuItem(int id, const std::string& name, const std::string& description, double price, bool available,
             ItemType type);

    virtual ~MenuItem() = default;

    int getId() const;
    const std::string& getName() const;
    const std::string& getDescription() const;
    double getPrice() const;
    ItemType getType() const;

    bool isAvailable() const;

    void setDescription(const std::string& description);
    void setPrice(double price);
    void setAvailability(bool available);

    virtual void display() const;
};

class Food : public MenuItem {
private:
    int readyTime;

public:
    Food(int readyTime,
         int id,
         const std::string& name,
         const std::string& description,
         double price,
         bool available);

    int getReadyTime() const;
    void setReadyTime(int readyTime);

    void display() const override;
};

class Drink : public MenuItem {
private:
    int capacity;

public:
    Drink(int capacity,  int id,  const std::string& name,  const std::string& description,  double price,
          bool available);

    int getCapacity() const;
    void setCapacity(int capacity);

    void display() const override;
};

class Other : public MenuItem {
private:
    std::string group;

public:
    Other(const std::string& group,  int id,  const std::string& name,  const std::string& description,
          double price,  bool available);

    const std::string& getGroup() const;
    void setGroup(const std::string& Group);

    void display() const override;
};

#endif