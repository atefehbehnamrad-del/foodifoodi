#ifndef USER_H
#define USER_H

#include <string>

enum class UserRole {
    CUSTOMER,
    RESTAURANT_MANAGER,
    SYSTEM_ADMIN
};

class User {
protected:
    int id;
    std::string username;
    std::string password;
    std::string name;
    UserRole role;

public:
    User(int id,
         const std::string& username,
         const std::string& password,
         const std::string& name,
         UserRole role);

    virtual ~User() = default;

    int getId() const { return id; }

    const std::string& getUsername() const {
        return username;
    }

    const std::string& getName() const {
        return name;
    }

    UserRole getRole() const {
        return role;
    }

    const std::string& getPassword() const {
        return password;
    }

    virtual void showMenu() = 0;

    virtual void printInfo() const;

protected:
    bool checkPassword(const std::string& input) const;
};

#endif