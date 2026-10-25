#ifndef CROP_H
#define CROP_H
#include <string>

struct Date
{
    int date;
    int month;
    int year;
};
struct Cropdata
{
    int cropId;
    char type[16];
    char name[30];
    char mandi[30];
    double price;
    int date, month, year;
};

class Crop
{
protected:
    int CropID;
    std::string name, mandi;
    double price;
    Date cropDate;
    static int nextID;

public:
    Crop(const std::string &, const std::string &, double, Date);
    Crop(int, const std::string &, const std::string &, double, Date);
    Crop(const Crop &);
    virtual ~Crop();
    int getCropID() const;
    std::string getName() const;
    std::string getMandi() const;
    double getPrice() const;
    Date getDate() const;
    void updatePrice(double);
    virtual std::string getType() const = 0;
    virtual void display() const = 0;
    bool operator>(const Crop &) const;
    static void updateNextID(int);
};

class Grain : public Crop
{
public:
    Grain(const std::string &, const std::string &, double, Date);
    Grain(int, const std::string &, const std::string &, double, Date);
    std::string getType() const override;
    void display() const override;
};
class Vegetable : public Crop
{
public:
    Vegetable(const std::string &, const std::string &, double, Date);
    Vegetable(int, const std::string &, const std::string &, double, Date);
    std::string getType() const override;
    void display() const override;
};
class Fruit : public Crop
{
public:
    Fruit(const std::string &, const std::string &, double, Date);
    Fruit(int, const std::string &, const std::string &, double, Date);
    std::string getType() const override;
    void display() const override;
};

Cropdata convertToRecord(const Crop *);
Crop *createCropFromRecord(const Cropdata &);
#endif
