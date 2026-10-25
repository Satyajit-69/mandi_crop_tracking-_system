Mandi Crop Price Tracker
A menu-driven C++ application for managing agricultural crop and mandi price records. The system supports Grain, Vegetable, and Fruit records and demonstrates object-oriented programming, file handling, STL algorithms, templates, and exception handling.
Features
- Add crop records with type, name, mandi, price, and date
- Display all crop records
- Search for a crop by ID
- Update a crop's price
- Delete a crop record
- Compare the prices of two crops
- Generate a summary report, including average and highest/lowest prices where implemented
- Save and load records using the binary file crops.dat
- Use inheritance and runtime polymorphism through the Crop, Grain, Vegetable, and Fruit classes
- Compare crop prices using the overloaded > operator
- Use STL containers and algorithms such as vector, find_if(), and sort() in the vector-based version
- Use the bestPrice<T>() function template to identify the highest-priced crop
- Handle invalid input and operational errors using exceptions where implemented
Keep this feature list aligned with the final code you submit. Use the vector-based implementation consistently across CropManager.h, CropManager.cpp, and main.cpp.

Project Structure
Mandi-Crop-Price-Tracker/
├── main.cpp          # Main menu and program control
├── Crop.h            # Date/Cropdata structures and class declarations
├── Crop.cpp          # Crop class implementations and record conversion
├── CropManager.h     # Crop-management function declarations and bestPrice<T>()
├── CropManager.cpp   # CRUD operations, search, sorting, summary, and file handling
└── crops.dat         # Binary data file created/used by the application
crops.dat is generated or updated when the program saves records. Keep it in the program's working directory so the application can find it when it starts.
Requirements
- A C++ compiler that supports C++11 or later (for example, GCC/MinGW)
- A terminal or IDE such as VS Code, Code::Blocks, or Visual Studio
Build
Compile all three .cpp files together from the project directory:
g++ -std=c++11 main.cpp Crop.cpp CropManager.cpp -o mandi
Run
Windows (PowerShell)
.\mandi.exe
Linux / macOS
./mandi
Menu Options
--- MANDI CROP PRICE TRACKER ---
1. Add Crop
2. Display All
3. Search by ID
4. Summary
5. Update Price
6. Delete Crop
7. Compare Prices
8. Exit
Choose an option by entering its number and follow the prompts.
Data Persistence
The application stores crop records in the binary file crops.dat. On startup, it attempts to load existing records. Changes made through create, update, and delete operations are written back to the file, depending on the final implementation.

To reset saved data, close the program and remove crops.dat. The file may be recreated when the application initializes storage or saves records.
Core C++ Concepts
- Classes and objects: organize crop data and behaviour
- Inheritance: Grain, Vegetable, and Fruit derive from Crop
- Abstraction: pure virtual functions define the interface for crop types
- Runtime polymorphism: virtual methods allow derived behaviour through Crop*
- Virtual destructor: supports safe deletion through a base-class pointer
- Operator overloading: operator> compares crop prices
- Binary file handling: saves and restores crop records
- STL algorithms: find_if(), sort(), and max_element() where used
- Function template: bestPrice<T>()
- Exception handling: try, catch, throw, and re-throw where implemented
Example Workflow
1. Run the program.
2. Select 1. Add Crop and enter crop type, name, mandi, price, and date.
3. Select 2. Display All to view records.
4. Use 3. Search by ID to find a crop.
5. Use 5. Update Price or 6. Delete Crop to modify records.
6. Use 7. Compare Prices to compare two crop IDs.
7. Use 4. Summary to view the summary report.
8. Select 8. Exit to close the program.
Limitations
- Console-based interface
- Local binary-file storage rather than a database
- No multi-user access
Future Improvements
- Add a graphical user interface
- Integrate MySQL or SQLite
- Add authentication and user roles
- Track price history and generate charts
- Export reports to CSV or PDF
- Improve calendar-date validation and input handling
Author
Name: [Your Name]
Course / Programme: [Your Course]
Institution: [Your Institution]
License
Created for academic and learning purposes. Add a license if you intend to distribute the project publicly.
