#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include "CropManager.h"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

static const char FILE_NAME[] = "crops.dat";

// ---------- Input helpers ----------

static string readLineOrThrow()
{
    string line;
    if (!getline(cin, line))
        throw InputEnded();
    return line;
}

static string trim(const string &s)
{
    const char *ws = " \t\r\n";
    size_t b = s.find_first_not_of(ws);
    if (b == string::npos)
        return "";
    size_t e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

static string toLower(string s)
{
    for (char &ch : s)
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    return s;
}

bool readInt(int &value)
{
    string line = trim(readLineOrThrow());
    if (line.empty())
        return false;

    try
    {
        size_t pos = 0;
        long long v = stoll(line, &pos);
        if (pos != line.size() || v < INT_MIN || v > INT_MAX)
            return false;
        value = static_cast<int>(v);
        return true;
    }
    catch (const exception &)
    {
        return false;
    }
}

bool readDouble(double &value)
{
    string line = trim(readLineOrThrow());
    if (line.empty())
        return false;

    try
    {
        size_t pos = 0;
        double v = stod(line, &pos);
        if (pos != line.size())
            return false;
        value = v;
        return true;
    }
    catch (const exception &)
    {
        return false;
    }
}

static bool readDate(Date &d)
{
    istringstream iss(readLineOrThrow());
    if (!(iss >> d.date >> d.month >> d.year))
        return false;

    string extra;
    return !(iss >> extra);
}

// ---------- Core helpers ----------

// Find a crop using its ID
static vector<Crop *>::iterator findCrop(vector<Crop *> &crops, int id)
{
    return find_if(crops.begin(), crops.end(),
                   [id](Crop *c)
                   {
                       return c->getCropID() == id;
                   });
}

// Save records to a temporary file before replacing the original.
void saveAll(const vector<Crop *> &crops)
{
    const string tempFile = string(FILE_NAME) + ".tmp";

    try
    {
        ofstream file(tempFile, ios::binary | ios::trunc);

        if (!file)
            throw runtime_error("Cannot open temporary file!");

        for (Crop *c : crops)
        {
            Cropdata r = convertToRecord(c);
            file.write(reinterpret_cast<const char *>(&r), sizeof(r));

            if (!file)
                throw runtime_error("Error writing crop record!");
        }

        file.flush();

        if (!file)
            throw runtime_error("Error flushing temporary file!");

        file.close();

        if (file.fail())
            throw runtime_error("Error closing temporary file!");

#ifdef _WIN32
        if (!MoveFileExA(tempFile.c_str(), FILE_NAME,
                         MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
        {
            DWORD errorCode = GetLastError();
            throw runtime_error("Could not replace crop file. Windows error: " +
                                to_string(errorCode));
        }
#else
        if (std::rename(tempFile.c_str(), FILE_NAME) != 0)
            throw runtime_error("Could not replace crop file!");
#endif
    }
    catch (const exception &e)
    {
        std::remove(tempFile.c_str());
        cerr << "Save error: " << e.what() << endl;
        throw;
    }
}

// Load records from file
void loadPreviousData(vector<Crop *> &crops)
{
    ifstream file(FILE_NAME, ios::binary);

    if (!file)
    {
        // First run: create a new file
        ofstream create(FILE_NAME, ios::binary);

        if (!create)
            throw runtime_error("Cannot create crop file!");

        return;
    }

    // A valid file must contain complete fixed-size records.
    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();

    if (fileSize < 0 ||
        fileSize % static_cast<streamsize>(sizeof(Cropdata)) != 0)
    {
        throw runtime_error("Crop file contains an incomplete record!");
    }

    file.seekg(0, ios::beg);

    const size_t originalSize = crops.size();

    try
    {
        Cropdata r{};

        while (file.read(reinterpret_cast<char *>(&r), sizeof(r)))
        {
            if (findCrop(crops, r.cropId) != crops.end())
                throw runtime_error("Duplicate crop ID in file!");

            // Throws runtime_error if the record is invalid.
            Crop *p = createCropFromRecord(r);

            try
            {
                crops.push_back(p);
            }
            catch (...)
            {
                delete p;
                throw;
            }
        }

        if (!file.eof())
            throw runtime_error("Error reading crop file!");
    }
    catch (...)
    {
        // Delete only the objects added during this load attempt.
        while (crops.size() > originalSize)
        {
            delete crops.back();
            crops.pop_back();
        }

        throw;
    }

    cout << "Records loaded: " << crops.size() << endl;
}

// ---------- Operations ----------

// Create a crop
void createCrop(vector<Crop *> &crops)
{
    int type;

    cout << "Type: 1.Grain 2.Vegetable 3.Fruit: ";

    if (!readInt(type))
        throw invalid_argument("Type must be a number!");

    if (type < 1 || type > 3)
        throw invalid_argument("Invalid crop type!");

    cout << "Crop name (max 29 chars): ";
    string name = trim(readLineOrThrow());

    cout << "Mandi name (max 29 chars): ";
    string mandi = trim(readLineOrThrow());

    if (name.empty() || mandi.empty() ||
        name.size() > 29 || mandi.size() > 29)
    {
        throw invalid_argument("Names must contain 1-29 characters!");
    }

    double price;
    cout << "Price per quintal: ";

    if (!readDouble(price))
        throw invalid_argument("Price must be numeric!");

    if (!std::isfinite(price) || price < 0)
        throw invalid_argument("Price cannot be negative or infinite!");

    Date d{};
    cout << "Date (DD MM YYYY): ";

    if (!readDate(d))
        throw invalid_argument("Date must contain three numbers!");

    if (!isValidDate(d.date, d.month, d.year))
        throw invalid_argument("Please enter a valid date!");

    Crop *p = nullptr;

    if (type == 1)
        p = new Grain(name, mandi, price, d);
    else if (type == 2)
        p = new Vegetable(name, mandi, price, d);
    else
        p = new Fruit(name, mandi, price, d);

    crops.push_back(p);

    try
    {
        saveAll(crops);
    }
    catch (...)
    {
        crops.pop_back();
        delete p;
        throw;
    }

    cout << "Crop saved. ID: " << p->getCropID() << endl;
}

// Read all crops, sorted by price (highest first)
void readAll(vector<Crop *> &crops)
{
    if (crops.empty())
        throw runtime_error("No crop records available!");

    vector<Crop *> sorted = crops;

    stable_sort(sorted.begin(), sorted.end(),
                [](Crop *a, Crop *b)
                {
                    return a->getPrice() > b->getPrice();
                });

    for (Crop *c : sorted)
        c->display();
}

// Search by ID
void searchCrop(vector<Crop *> &crops)
{
    int id;

    cout << "Crop ID: ";

    if (!readInt(id))
        throw invalid_argument("ID must be a number!");

    auto it = findCrop(crops, id);

    if (it == crops.end())
        throw runtime_error("Crop not found!");

    (*it)->display();
}

// Update price (and the date of the new price)
void updateCrop(vector<Crop *> &crops)
{
    int id;
    double price;
    Date newDate{};

    cout << "Crop ID to update: ";

    if (!readInt(id))
        throw invalid_argument("ID must be a number!");

    auto it = findCrop(crops, id);

    if (it == crops.end())
        throw runtime_error("Crop not found!");

    cout << "New price: ";

    if (!readDouble(price))
        throw invalid_argument("Price must be numeric!");

    if (!std::isfinite(price) || price < 0)
        throw invalid_argument("Invalid price!");

    cout << "Date of new price (DD MM YYYY): ";

    if (!readDate(newDate))
        throw invalid_argument("Date must contain three numbers!");

    if (!isValidDate(newDate.date, newDate.month, newDate.year))
        throw invalid_argument("Please enter a valid date!");

    double oldPrice = (*it)->getPrice();
    Date oldDate = (*it)->getDate();

    (*it)->updatePrice(price);
    (*it)->updateDate(newDate);

    try
    {
        saveAll(crops);
    }
    catch (...)
    {
        (*it)->updatePrice(oldPrice);
        (*it)->updateDate(oldDate);
        throw;
    }

    cout << "Price updated successfully!" << endl;
}

// Delete a crop
void deleteCrop(vector<Crop *> &crops)
{
    int id;

    cout << "Crop ID to delete: ";

    if (!readInt(id))
        throw invalid_argument("ID must be a number!");

    auto it = findCrop(crops, id);

    if (it == crops.end())
        throw runtime_error("Crop not found!");

    cout << "Are you sure you want to delete this crop? (yes/no): ";
    string answer = toLower(trim(readLineOrThrow()));

    if (answer == "no" || answer == "n")
    {
        cout << "Deletion cancelled.\n";
        return;
    }

    if (answer != "yes" && answer != "y")
        throw invalid_argument("Please enter yes or no!");

    Crop *removed = *it;
    size_t position = static_cast<size_t>(it - crops.begin());

    crops.erase(it);

    try
    {
        saveAll(crops);
    }
    catch (...)
    {
        crops.insert(crops.begin() + position, removed);
        throw;
    }

    delete removed;

    cout << "Crop deleted successfully!\n";
}

// Summary and best price
void summaryReport(vector<Crop *> &crops)
{
    if (crops.empty())
        throw runtime_error("No crop records available!");

    double total = 0;

    for (Crop *c : crops)
        total += c->getPrice();

    cout << fixed << setprecision(2);
    cout << "Records: " << crops.size() << endl;
    cout << "Average price: Rs. " << total / crops.size() << endl;

    Crop *best = bestPrice<Crop>(crops);

    cout << "Highest-priced crop: ";
    best->display();
}

// Compare two crops
void compareCrops(vector<Crop *> &crops)
{
    int aID, bID;

    cout << "First ID: ";

    if (!readInt(aID))
        throw invalid_argument("ID must be a number!");

    cout << "Second ID: ";

    if (!readInt(bID))
        throw invalid_argument("ID must be a number!");

    auto a = findCrop(crops, aID);
    auto b = findCrop(crops, bID);

    if (a == crops.end() || b == crops.end())
        throw runtime_error("Crop ID not found!");

    if (**a > **b)
        cout << (*a)->getName() << " costs more." << endl;
    else if (**b > **a)
        cout << (*b)->getName() << " costs more." << endl;
    else
        cout << "Prices are equal." << endl;
}

// Free allocated memory
void clearCrops(vector<Crop *> &crops)
{
    for (Crop *c : crops)
        delete c;

    crops.clear();
}
