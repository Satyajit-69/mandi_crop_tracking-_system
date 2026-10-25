
#include "CropManager.h"
#include <limits>

using namespace std;

int main()
{
    vector<Crop*> crops;
    int choice = 0;

    try
    {
        loadPreviousData(crops);
    }
    catch (const exception& e)
    {
        cout << "Loading error: " << e.what() << endl;
        clearCrops(crops);
        return 1;
    }

    do
    {
        cout << "\n--- MANDI CROP PRICE TRACKER ---" << endl;
        cout << "1. Add Crop" << endl;
        cout << "2. Display All" << endl;
        cout << "3. Search by ID" << endl;
        cout << "4. Summary" << endl;
        cout << "5. Update Price" << endl;
        cout << "6. Delete Crop" << endl;
        cout << "7. Compare Prices" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        try
        {
            switch (choice)
            {
            case 1:
                createCrop(crops);
                break;
            case 2:
                readAll(crops);
                break;
            case 3:
                searchCrop(crops);
                break;
            case 4:
                summaryReport(crops);
                break;
            case 5:
                updateCrop(crops);
                break;
            case 6:
                deleteCrop(crops);
                break;
            case 7:
                compareCrops(crops);
                break;
            case 8:
                cout << "Program terminated." << endl;
                break;
            default:
                throw invalid_argument("Invalid menu choice!");
            }
        }
        catch (const exception& e)
        {
            cout << "Error: " << e.what() << endl;
        }

    } while (choice != 8);

    clearCrops(crops);
    return 0;
}
