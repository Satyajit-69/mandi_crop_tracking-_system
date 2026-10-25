#include "Crop.h"
#include <iostream>
#include <iomanip>
#include <cstring>
using namespace std;

int Crop::nextID = 1001;
Crop::Crop(const string &n, const string &m, double p, Date d)
    : CropID(nextID++), name(n), mandi(m), price(p), cropDate(d) {}

Crop::Crop(int id, const string &n, const string &m, double p, Date d)
    : CropID(id), name(n), mandi(m), price(p), cropDate(d)
{
    updateNextID(id);
}
Crop::Crop(const Crop &c)
    : CropID(c.CropID), name(c.name), mandi(c.mandi), price(c.price), cropDate(c.cropDate) {}

Crop::~Crop() = default;

int Crop::getCropID() const
{
    return CropID;
}

string Crop::getName() const
{
    return name;
}

string Crop::getMandi() const
{
    return mandi;
}

double Crop::getPrice() const
{
    return price;
}

Date Crop::getDate() const
{
    return cropDate;
}

void Crop::updatePrice(double p)
{
    price = p;
}

bool Crop::operator>(const Crop &other) const
{
    return price > other.price;
}

void Crop::updateNextID(int id)
{
    if (nextID <= id)
        nextID = id + 1;
}

static void printCrop(const Crop &c)
{
    Date d = c.getDate();
    cout << "ID: " << c.getCropID() << " | Type: " << c.getType()
         << " | Name: " << c.getName() << " | Mandi: " << c.getMandi()
         << " | Price: Rs. " << fixed << setprecision(2) << c.getPrice()
         << " | Date: " << d.date << '/' << d.month << '/' << d.year << '\n';
}
Grain::Grain(const string &n, const string &m, double p, Date d) : Crop(n, m, p, d) {}
Grain::Grain(int id, const string &n, const string &m, double p, Date d) : Crop(id, n, m, p, d) {}
string Grain::getType() const { return "Grain"; }
void Grain::display() const { printCrop(*this); }
Vegetable::Vegetable(const string &n, const string &m, double p, Date d) : Crop(n, m, p, d) {}
Vegetable::Vegetable(int id, const string &n, const string &m, double p, Date d) : Crop(id, n, m, p, d) {}
string Vegetable::getType() const { return "Vegetable"; }
void Vegetable::display() const { printCrop(*this); }
Fruit::Fruit(const string &n, const string &m, double p, Date d) : Crop(n, m, p, d) {}
Fruit::Fruit(int id, const string &n, const string &m, double p, Date d) : Crop(id, n, m, p, d) {}
string Fruit::getType() const { return "Fruit"; }
void Fruit::display() const { printCrop(*this); }

Cropdata convertToRecord(const Crop *c)
{
    Cropdata r{};
    r.cropId = c->getCropID();
    strncpy(r.type, c->getType().c_str(), sizeof(r.type) - 1);
    strncpy(r.name, c->getName().c_str(), sizeof(r.name) - 1);
    strncpy(r.mandi, c->getMandi().c_str(), sizeof(r.mandi) - 1);
    r.price = c->getPrice();
    Date d = c->getDate();
    r.date = d.date;
    r.month = d.month;
    r.year = d.year;
    return r;
}
Crop *createCropFromRecord(const Cropdata &r)
{
    Date d{r.date, r.month, r.year};
    string type(r.type);
    if (type == "Grain")
        return new Grain(r.cropId, r.name, r.mandi, r.price, d);
    if (type == "Vegetable")
        return new Vegetable(r.cropId, r.name, r.mandi, r.price, d);
    if (type == "Fruit")
        return new Fruit(r.cropId, r.name, r.mandi, r.price, d);
    return nullptr;
}
