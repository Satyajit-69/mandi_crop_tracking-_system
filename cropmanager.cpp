
#include "CropManager.h"
#include <fstream>
#include <iomanip>
#include <limits>
#include <cmath>

using namespace std;

const char FILE_NAME[] = "crops.dat";

// Find a crop using its ID
vector<Crop*>::iterator findCrop(vector<Crop*>& crops, int id)
{
    return find_if(crops.begin(), crops.end(),
                   [id](Crop* c)
                   {
                       return c->getCropID() == id;
                   });
}

// Save records to file
void saveAll(const vector<Crop*>& crops)
{
    try
    {
        ofstream file(FILE_NAME, ios::binary | ios::trunc);

        if (!file)
            throw runtime_error("Cannot open file for saving!");

        for (Crop* c : crops)
        {
            Cropdata r = convertToRecord(c);
            file.write(reinterpret_cast<char*>(&r), sizeof(r));

            if (!file)
                throw runtime_error("Error writing crop record!");
        }

        file.close();

        if (!file)
            throw runtime_error("Error closing crop file!");
    }
    catch (const exception& e)
    {
        cerr << "Save error: " << e.what() << endl;
        throw;  // Re-throw to the caller
    }
}

// Load records from file
void loadPreviousData(vector<Crop*>& crops)
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

    Cropdata r{};

    while (file.read(reinterpret_cast<char*>(&r), sizeof(r)))
    {
        if (findCrop(crops, r.cropId) != crops.end())
            throw runtime_error("Duplicate crop ID in file!");

        Crop* p = createCropFromRecord(r);

        if (p == nullptr)
            throw runtime_error("Invalid crop type in file!");

        crops.push_back(p);
    }

    if (!file.eof())
        throw runtime_error("Error reading crop file!");

    cout << "Records loaded: " << crops.size() << endl;
}

// Create a crop
void createCrop(vector<Crop*>& crops)
{
    int type;

    cout << "Type: 1.Grain 2.Vegetable 3.Fruit: ";

    if (!(cin >> type))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Type must be a number!");
    }

    if (type < 1 || type > 3)
        throw invalid_argument("Invalid crop type!");

    string name, mandi;
    double price;
    Date d{};

    cout << "Crop name (max 29 chars): ";
    getline(cin >> ws, name);

    cout << "Mandi name (max 29 chars): ";
    getline(cin, mandi);

    if (name.empty() || mandi.empty() ||
        name.size() > 29 || mandi.size() > 29)
        throw invalid_argument("Names must contain 1-29 characters!");

    cout << "Price per quintal: ";

    if (!(cin >> price))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Price must be numeric!");
    }

    if (!isfinite(price) || price < 0)
        throw invalid_argument("Price cannot be negative or infinite!");

    cout << "Date (DD MM YYYY): ";

    if (!(cin >> d.date >> d.month >> d.year))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Date must contain three numbers!");
    }

    if (d.date < 1 || d.date > 31 ||
        d.month < 1 || d.month > 12 || d.year < 1)
        throw invalid_argument("Invalid date!");

    Crop* p = nullptr;

    if (type == 1)
        p = new Grain(name, mandi, price, d);
    else if (type == 2)
        p = new Vegetable(name, mandi, price, d);
    else
        p = new Fruit(name, mandi, price, d);

    // Check duplicate ID before adding
    if (findCrop(crops, p->getCropID()) != crops.end())
    {
        delete p;
        throw runtime_error("Duplicate crop ID!");
    }

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

// Read all crops, sorted by price
void readAll(vector<Crop*>& crops)
{
    if (crops.empty())
        throw runtime_error("No crop records available!");

    vector<Crop*> sorted = crops;

    sort(sorted.begin(), sorted.end(),
         [](Crop* a, Crop* b)
         {
             return a->getPrice() > b->getPrice();
         });

    for (Crop* c : sorted)
        c->display();
}

// Search by ID
void searchCrop(vector<Crop*>& crops)
{
    int id;

    cout << "Crop ID: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("ID must be a number!");
    }

    auto it = findCrop(crops, id);

    if (it == crops.end())
        throw runtime_error("Crop not found!");

    (*it)->display();
}

// Update price
void updateCrop(vector<Crop*>& crops)
{
    int id;
    double price;

    cout << "Crop ID to update: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("ID must be a number!");
    }

    auto it = findCrop(crops, id);

    if (it == crops.end())
        throw runtime_error("Crop not found!");

    cout << "New price: ";

    if (!(cin >> price))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("Price must be numeric!");
    }

    if (!isfinite(price) || price < 0)
        throw invalid_argument("Invalid price!");

    double oldPrice = (*it)->getPrice();
    (*it)->updatePrice(price);

    try
    {
        saveAll(crops);
    }
    catch (...)
    {
        (*it)->updatePrice(oldPrice);
        throw;
    }

    cout << "Price updated successfully!" << endl;
}

// Delete a crop
void deleteCrop(vector<Crop*>& crops)
{
    int id;

    cout << "Crop ID to delete: ";

    if (!(cin >> id))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("ID must be a number!");
    }

    auto it = findCrop(crops, id);

    if (it == crops.end())
        throw runtime_error("Crop not found!");

    Crop* removed = *it;
    size_t position = it - crops.begin();

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

    cout << "Crop deleted successfully!" << endl;
}

// Summary and best price
void summaryReport(vector<Crop*>& crops)
{
    if (crops.empty())
        throw runtime_error("No crop records available!");

    double total = 0;

    for (Crop* c : crops)
        total += c->getPrice();

    cout << fixed << setprecision(2);
    cout << "Records: " << crops.size() << endl;
    cout << "Average price: Rs. "
         << total / crops.size() << endl;

    Crop* best = bestPrice<Crop>(crops);

    cout << "Highest-priced crop: ";
    best->display();
}

// Compare two crops
void compareCrops(vector<Crop*>& crops)
{
    int aID, bID;

    cout << "First ID: ";

    if (!(cin >> aID))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("ID must be a number!");
    }

    cout << "Second ID: ";

    if (!(cin >> bID))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        throw invalid_argument("ID must be a number!");
    }

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
void clearCrops(vector<Crop*>& crops)
{
    for (Crop* c : crops)
        delete c;

    crops.clear();
}
