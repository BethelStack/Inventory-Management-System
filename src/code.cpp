#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <ctime>
using namespace std;

/* ===================== STRUCT ===================== */
struct Product {
    string id;
    string type;
    string size;
    string color;
    double price;
    double costPrice;
    int quantity;
};

struct DailySale {
    string date;
    double totalProfit;
    int totalQuantity;
};

/* ===================== FUNCTION DECLARATIONS ===================== */
void addProduct();
void viewProducts();
void deleteProduct();
void searchProduct();      
void updateProduct();      
void sortProducts();       
void calculateProfit();    
bool productExists(string type, string size, string color);
string generateID(string type);
void checkLowStock();
void processSale();
void salesReport();
void dailyProfitReport();
bool inventoryExists();
void initialProductUpload();
void resetSystem();
bool isValidSize(string size);      
bool isValidColor(string color);    
bool isValidType(string type);      
string getValidType();
string getValidSize();
string getValidColor();
double getValidPrice(string prompt);
double getValidCost(string prompt);
int getValidQuantity(string prompt);
string getCurrentDate();

// Common valid options for sample store
const string VALID_TYPES[] = {"DRESS", "T-SHIRT", "SHIRT", "HOODIE", "JEANS", "JACKET"};
const int NUM_TYPES = 6;

const string VALID_SIZES[] = {"XS", "S", "M", "L", "XL", "XXL"};
const int NUM_SIZES = 6;

const string VALID_COLORS[] = {"RED", "BLUE", "BLACK", "WHITE", "GREEN", "GRAY", "BROWN"};
const int NUM_COLORS = 7;

/* ===================== GET CURRENT DATE ===================== */
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    
    string year = to_string(1900 + ltm->tm_year);
    string month = to_string(1 + ltm->tm_mon);
    string day = to_string(ltm->tm_mday);
    
    if (month.length() == 1) month = "0" + month;
    if (day.length() == 1) day = "0" + day;
    
    return year + "-" + month + "-" + day;
}

/* ===================== RESET SYSTEM ===================== */
void resetSystem() {
    cout << "\n⚠ WARNING: This will delete ALL inventory and sales data!\n";
    cout << "Are you sure you want to reset? (y/n): ";
    char confirm;
    cin >> confirm;
    
    if (confirm == 'y' || confirm == 'Y') {
        remove("inventory.txt");
        remove("sales.txt");
        remove("daily_sales.txt");
        cout << "\n✓ System reset successfully!\n";
        cout << "Please restart the program to begin with initial upload.\n";
        exit(0);
    } else {
        cout << "Reset cancelled.\n";
    }
}

/* ===================== VALIDATION FUNCTIONS ===================== */
bool isValidType(string type) {
    string upperType = type;
    transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);
    
    for (int i = 0; i < NUM_TYPES; i++) {
        if (upperType == VALID_TYPES[i]) {
            return true;
        }
    }
    return false;
}

bool isValidSize(string size) {
    string upperSize = size;
    transform(upperSize.begin(), upperSize.end(), upperSize.begin(), ::toupper);
    
    for (int i = 0; i < NUM_SIZES; i++) {
        if (upperSize == VALID_SIZES[i]) {
            return true;
        }
    }
    return false;
}

bool isValidColor(string color) {
    string upperColor = color;
    transform(upperColor.begin(), upperColor.end(), upperColor.begin(), ::toupper);
    
    for (int i = 0; i < NUM_COLORS; i++) {
        if (upperColor == VALID_COLORS[i]) {
            return true;
        }
    }
    return false;
}

string getValidType() {
    string type;
    while (true) {
        cout << "\nAvailable types: Dress, T-Shirt, Shirt, Hoodie, Jeans, Jacket\n";
        cout << "Enter product type: ";
        cin >> type;
        
        if (isValidType(type)) {
            string formattedType = type;
            formattedType[0] = toupper(formattedType[0]);
            for (size_t i = 1; i < formattedType.length(); i++) {
                formattedType[i] = tolower(formattedType[i]);
            }
            if (formattedType == "T-shirt") formattedType = "T-Shirt";
            return formattedType;
        } else {
            cout << "Invalid type! Please choose from the list above.\n";
        }
    }
}

string getValidSize() {
    string size;
    while (true) {
        cout << "Available sizes: XS, S, M, L, XL, XXL\n";
        cout << "Enter size: ";
        cin >> size;
        
        if (isValidSize(size)) {
            transform(size.begin(), size.end(), size.begin(), ::toupper);
            return size;
        } else {
            cout << "Invalid size! Please choose from the list above.\n";
        }
    }
}

string getValidColor() {
    string color;
    while (true) {
        cout << "Available colors: Red, Blue, Black, White, Green, Gray, Brown\n";
        cout << "Enter color: ";
        cin >> color;
        
        if (isValidColor(color)) {
            string formattedColor = color;
            formattedColor[0] = toupper(formattedColor[0]);
            for (size_t i = 1; i < formattedColor.length(); i++) {
                formattedColor[i] = tolower(formattedColor[i]);
            }
            return formattedColor;
        } else {
            cout << "Invalid color! Please choose from the list above.\n";
        }
    }
}

double getValidPrice(string prompt) {
    double price;
    while (true) {
        cout << prompt;
        cin >> price;
        
        if (cin.fail() || price < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid price (Birr). please Try again.\n";
        } else {
            cin.ignore(1000, '\n');
            return price;
        }
    }
}

double getValidCost(string prompt) {
    double cost;
    while (true) {
        cout << prompt;
        cin >> cost;
        
        if (cin.fail() || cost < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid! Cost (Birr). please Try again.\n";
        } else {
            cin.ignore(1000, '\n');
            return cost;
        }
    }
}

int getValidQuantity(string prompt) {
    int quantity;
    while (true) {
        cout << prompt;
        cin >> quantity;
        
        if (cin.fail() || quantity < 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid Quantity!. please Try again.\n";
        } else {
            cin.ignore(1000, '\n');
            return quantity;
        }
    }
}

/* ===================== INITIAL PRODUCT UPLOAD ===================== */
void initialProductUpload() {
    cout << "\n=== INITIAL PRODUCT UPLOAD ===\n";
    cout << "Let's add your initial inventory.\n";
    cout << "You can add as many products as you need.\n";
    cout << "Press 'n' when you're done.\n";
    cout << "================================\n\n";
    
    addProduct();
}

/* ===================== MAIN ===================== */
int main() {
    cout << "\n============================================\n";
    cout << "   WELCOME TO CLOTHING STORE MANAGER!\n";
    cout << "============================================\n";
    cout << "This is your clothing store management system.\n";
    cout << "You can manage your inventory, track sales, and\n";
    cout << "analyze profits all in one place!\n";
    cout << "============================================\n";
    
    ifstream checkFile("inventory.txt");
    bool fileExists = checkFile.good();
    bool fileEmpty = true;
    
    if (fileExists) {
        fileEmpty = checkFile.peek() == ifstream::traits_type::eof();
    }
    checkFile.close();
    
    if (!fileExists || fileEmpty) {
        cout << "\nYour inventory is currently empty.\n";
        cout << "Let's add your products first.\n";
        initialProductUpload();
        
        cout << "\n============================================\n";
        cout << "Initial upload completed!\n";
        cout << "Now you can use all features of the system.\n";
        cout << "============================================\n";
    } else {
        cout << "\nInventory loaded successfully.\n";
    }
    
    int choice;
    do {
        cout << "\n  MAIN MENU   \n";
        cout << "1. Product Management\n";
        cout << "2. Sales & Transactions\n";
        cout << "3. Reports\n";
        cout << "4. Reset System (Start Fresh)\n";
        cout << "5. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            int pm;
            do {
                cout << "\n  PRODUCT MANAGEMENT    \n";
                cout << "1. Add Product\n";
                cout << "2. View Products\n";
                cout << "3. Search Product\n";
                cout << "4. Update Product\n";
                cout << "5. Delete Product\n";
                cout << "6. Sort Products\n";
                cout << "7. Calculate Profit\n";
                cout << "8. Back\n";
                cout << "Choose: ";
                cin >> pm;

                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                if (pm == 1) addProduct();
                else if (pm == 2) {
                    if (inventoryExists()) viewProducts();
                    else cout << "No products in inventory yet.\n";
                }
                else if (pm == 3) {
                    if (inventoryExists()) searchProduct();
                    else cout << "No products to search.\n";
                }
                else if (pm == 4) {
                    if (inventoryExists()) updateProduct();
                    else cout << "No products to update.\n";
                }
                else if (pm == 5) {
                    if (inventoryExists()) deleteProduct();
                    else cout << "No products to delete.\n";
                }
                else if (pm == 6) {
                    if (inventoryExists()) sortProducts();
                    else cout << "No products to sort.\n";
                }
                else if (pm == 7) {
                    if (inventoryExists()) calculateProfit();
                    else cout << "No products to calculate profit.\n";
                }
            } while (pm != 8);
        }
        else if (choice == 2) {
            if (inventoryExists()) processSale();
            else cout << "No products available to sell. Please add products first.\n";
        }
        else if (choice == 3) {
            if (inventoryExists()) {
                int reportChoice;
                cout << "\n--- REPORTS MENU ---\n";
                cout << "1. Sales Report\n";
                cout << "2. Daily Profit Report\n";
                cout << "3. Low Stock Alert\n";
                cout << "4. Back\n";
                cout << "Choose: ";
                cin >> reportChoice;
                
                if (reportChoice == 1) salesReport();
                else if (reportChoice == 2) dailyProfitReport();
                else if (reportChoice == 3) checkLowStock();
            } else cout << "No sales or inventory data available.\n";
        }
        else if (choice == 4) {
            resetSystem();
        }
        else if (choice == 5) {
            cout << "\nThank you for using Clothing Store Manager!\n";
            cout << "Have a wonderful day and happy selling!\n";
        }
        else {
            cout << "Invalid choice. Please select 1-5.\n";
        }
    } while (choice != 5);

    return 0;
}

/* ===================== CHECK IF INVENTORY FILE EXISTS ===================== */
bool inventoryExists() {
    ifstream file("inventory.txt");
    if (!file.good()) {
        return false;
    }
    return file.peek() != ifstream::traits_type::eof();
}

/* ===================== ADD PRODUCT ===================== */
/* ===================== ADD PRODUCT (Updated to handle existing products) ===================== */
void addProduct() {
    Product p;
    char more = 'y';
    
    while (more == 'y' || more == 'Y') {
        cout << "\n--- ADD NEW PRODUCT ---\n";
        
        p.type = getValidType();
        p.size = getValidSize();
        p.color = getValidColor();

        // Check if product exists
        if (productExists(p.type, p.size, p.color)) {
            cout << "\n⚠ This product already exists in inventory.\n";
            cout << "Do you want to add more quantity to it? (y/n): ";
            char addQty;
            cin >> addQty;
            cin.ignore(1000, '\n');

            if (addQty == 'y' || addQty == 'Y') {
                int additionalQty = getValidQuantity("Enter quantity to add: ");

                // Update inventory file
                Product temp;
                ifstream infile("inventory.txt");
                ofstream outfile("temp.txt");

                while (infile >> temp.id >> temp.type >> temp.size >> temp.color
                              >> temp.price >> temp.costPrice >> temp.quantity) {
                    if (temp.type == p.type && temp.size == p.size && temp.color == p.color) {
                        temp.quantity += additionalQty; // Add the new quantity
                        cout << "\n✓ Quantity updated. New total quantity: " << temp.quantity << "\n";
                    }
                    outfile << temp.id << " " << temp.type << " " << temp.size << " "
                            << temp.color << " " << temp.price << " " << temp.costPrice
                            << " " << temp.quantity << endl;
                }

                infile.close();
                outfile.close();

                remove("inventory.txt");
                rename("temp.txt", "inventory.txt");
            } else {
                cout << "No changes made for this product.\n";
            }
        } else {
            // Product does not exist, add new
            p.id = generateID(p.type);
            p.price = getValidPrice("Enter selling price (Birr): ");
            p.costPrice = getValidCost("Enter cost price (Birr): ");
            
            if (p.price < p.costPrice) {
                cout << "\n⚠ WARNING: Selling at a loss!\n";
                cout << "Do you want to continue? (y/n): ";
                char confirm;
                cin >> confirm;
                cin.ignore(1000, '\n');
                if (confirm != 'y' && confirm != 'Y') {
                    cout << "Product not added. Please enter prices again.\n";
                    continue;
                }
            }

            p.quantity = getValidQuantity("Enter quantity: ");

            ofstream file("inventory.txt", ios::app);
            file << p.id << " " << p.type << " " << p.size << " "
                 << p.color << " " << p.price << " " << p.costPrice
                 << " " << p.quantity << endl;
            file.close();

            cout << "\n✓ Product added successfully!\n";
            cout << "Product ID: " << p.id << "\n";
            cout << "Details: " << p.type << " | " << p.color << " | " << p.size << "\n";
            cout << "Price: " << p.price << " Birr | Cost: " << p.costPrice << " Birr\n";
            cout << "Quantity: " << p.quantity << "\n";
        }

        cout << "\nDo you want to add another product? (y/n): ";
        cin >> more;
        cin.ignore(1000, '\n');
    }
}


/* ===================== VIEW PRODUCTS ===================== */
void viewProducts() {
    ifstream infile("inventory.txt");
    if (!infile) {
        cout << "No products found.\n";
        return;
    }

    Product p;
    cout << "\n--- INVENTORY ---\n";
    cout << "ID\tType\tSize\tColor\tPrice\tCost\tQuantity\n";

    while (infile >> p.id >> p.type >> p.size >> p.color >> p.price >> p.costPrice >> p.quantity) {
        cout << p.id << "\t" << p.type << "\t" << p.size << "\t" << p.color
             << "\t" << p.price << "\t" << p.costPrice << "\t" << p.quantity << "\n";
    }

    infile.close();
    cout << "-----------------\n";
}


/* ===================== SEARCH PRODUCT ===================== */
void searchProduct() {
    int searchChoice;
    cout << "\n--- SEARCH PRODUCT ---\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Type\n";
    cout << "3. Search by Size\n";
    cout << "4. Search by Color\n";
    cout << "5. Search by Price Range\n";
    cout << "Choose: ";
    cin >> searchChoice;

    ifstream file("inventory.txt");
    if (!file) {
        cout << "No products found.\n";
        return;
    }

    Product p;
    bool found = false;
    int count = 0;

    cout << "\nSearch Results:\n";

    // ---------------- SEARCH BY ID ----------------
    if (searchChoice == 1) {
        string searchID;
        cout << "Enter product ID: ";
        cin >> searchID;

        transform(searchID.begin(), searchID.end(), searchID.begin(), ::toupper);

        while (file >> p.id >> p.type >> p.size >> p.color
                    >> p.price >> p.costPrice >> p.quantity) {

            string upperId = p.id;
            transform(upperId.begin(), upperId.end(), upperId.begin(), ::toupper);

            if (upperId == searchID) {
                found = true;
                cout << "✓ Found\n";
                cout << "ID: " << p.id << "\n";
                cout << "Type: " << p.type << "\n";
                cout << "Size: " << p.size << "\n";
                cout << "Color: " << p.color << "\n";
                cout << "Price: " << p.price << " Birr\n";
                cout << "Cost: " << p.costPrice << " Birr\n";
                cout << "Quantity: " << p.quantity << "\n";
                break;
            }
        }
    }

    // ---------------- SEARCH BY TYPE ----------------
    else if (searchChoice == 2) {
        string searchType;
        cout << "Enter product type: ";
        cin >> searchType;

        transform(searchType.begin(), searchType.end(), searchType.begin(), ::toupper);

        while (file >> p.id >> p.type >> p.size >> p.color
                    >> p.price >> p.costPrice >> p.quantity) {

            string upperType = p.type;
            transform(upperType.begin(), upperType.end(), upperType.begin(), ::toupper);

            if (upperType == searchType) {
                found = true;
                count++;
                cout << count << ". " << p.type << " (ID: " << p.id << ") "
                     << "| Size: " << p.size
                     << " | Color: " << p.color
                     << " | Price: " << p.price
                     << " | Qty: " << p.quantity << "\n";
            }
        }
    }

    // ---------------- SEARCH BY SIZE ----------------
    else if (searchChoice == 3) {
        string searchSize;
        cout << "Enter size: ";
        cin >> searchSize;

        transform(searchSize.begin(), searchSize.end(), searchSize.begin(), ::toupper);

        while (file >> p.id >> p.type >> p.size >> p.color
                    >> p.price >> p.costPrice >> p.quantity) {

            string upperSize = p.size;
            transform(upperSize.begin(), upperSize.end(), upperSize.begin(), ::toupper);

            if (upperSize == searchSize) {
                found = true;
                count++;
                cout << count << ". " << p.type << " (ID: " << p.id << ") "
                     << "| Size: " << p.size
                     << " | Color: " << p.color
                     << " | Price: " << p.price
                     << " | Qty: " << p.quantity << "\n";
            }
        }
    }

    // ---------------- SEARCH BY COLOR ----------------
    else if (searchChoice == 4) {
        string searchColor;
        cout << "Enter color: ";
        cin >> searchColor;

        transform(searchColor.begin(), searchColor.end(), searchColor.begin(), ::toupper);

        while (file >> p.id >> p.type >> p.size >> p.color
                    >> p.price >> p.costPrice >> p.quantity) {

            string upperColor = p.color;
            transform(upperColor.begin(), upperColor.end(), upperColor.begin(), ::toupper);

            if (upperColor == searchColor) {
                found = true;
                count++;
                cout << count << ". " << p.type << " (ID: " << p.id << ") "
                     << "| Size: " << p.size
                     << " | Color: " << p.color
                     << " | Price: " << p.price
                     << " | Qty: " << p.quantity << "\n";
            }
        }
    }

    // ---------------- SEARCH BY PRICE RANGE ----------------
    else if (searchChoice == 5) {
        double minPrice, maxPrice;
        cout << "Enter minimum price: ";
        cin >> minPrice;
        cout << "Enter maximum price: ";
        cin >> maxPrice;

        while (file >> p.id >> p.type >> p.size >> p.color
                    >> p.price >> p.costPrice >> p.quantity) {

            if (p.price >= minPrice && p.price <= maxPrice) {
                found = true;
                count++;
                cout << count << ". " << p.type << " (ID: " << p.id << ") "
                     << "| Size: " << p.size
                     << " | Color: " << p.color
                     << " | Price: " << p.price
                     << " | Qty: " << p.quantity << "\n";
            }
        }
    }

    else {
        cout << "Invalid choice.\n";
        file.close();
        return;
    }

    file.close();

    if (!found) {
        cout << "No products found.\n";
    } else if (count > 0) {
        cout << "\nTotal found: " << count << " product(s)\n";
    }
}


/* ===================== DELETE PRODUCT ===================== */
void deleteProduct() {
    // First show products with numbers
    viewProducts();
    
    int productCount = 0;
    Product products[100];
    ifstream file("inventory.txt");
    
    while (file >> products[productCount].id >> products[productCount].type 
                >> products[productCount].size >> products[productCount].color
                >> products[productCount].price >> products[productCount].costPrice 
                >> products[productCount].quantity) {
        productCount++;
    }
    file.close();
    
    if (productCount == 0) {
        cout << "No products to delete.\n";
        return;
    }
    
    int choice;
    cout << "\nEnter product NUMBER to delete (1-" << productCount << "): ";
    cin >> choice;
    
    if (choice < 1 || choice > productCount) {
        cout << "Invalid choice!\n";
        return;
    }
    
    // Get the ID from the selected product
    string delID = products[choice-1].id;
    Product p;
    
    ifstream infile("inventory.txt");
    ofstream temp("temp.txt");
    bool found = false;

    while (infile >> p.id >> p.type >> p.size >> p.color
                  >> p.price >> p.costPrice >> p.quantity) {
        if (p.id != delID) {
            temp << p.id << " " << p.type << " " << p.size << " "
                 << p.color << " " << p.price << " "
                 << p.costPrice << " " << p.quantity << endl;
        } else {
            found = true;
            cout << "Deleting: " << p.type << " " << p.color << " " << p.size << " (ID: " << p.id << ")\n";
        }
    }

    infile.close();
    temp.close();

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (found) cout << "✓ Product deleted successfully.\n";
    else cout << "✗ Product ID not found. Nothing deleted.\n";
}

/* ===================== UPDATE PRODUCT ===================== */
void updateProduct() {
    // First show products with numbers
    viewProducts();
    
    int productCount = 0;
    Product products[100];
    ifstream file("inventory.txt");
    
    while (file >> products[productCount].id >> products[productCount].type 
                >> products[productCount].size >> products[productCount].color
                >> products[productCount].price >> products[productCount].costPrice 
                >> products[productCount].quantity) {
        productCount++;
    }
    file.close();
    
    if (productCount == 0) {
        cout << "No products to update.\n";
        return;
    }
    
    int choice;
    cout << "\nEnter product NUMBER to update (1-" << productCount << "): ";
    cin >> choice;
    
    if (choice < 1 || choice > productCount) {
        cout << "Invalid choice!\n";
        return;
    }
    
    string updateID = products[choice-1].id;
    Product p;
    ifstream infile("inventory.txt");
    ofstream temp("temp.txt");
    bool found = false;

    if (!infile) {
        cout << "No products found.\n";
        return;
    }

    while (infile >> p.id >> p.type >> p.size >> p.color
                  >> p.price >> p.costPrice >> p.quantity) {
        if (p.id == updateID) {
            found = true;
            
            cout << "\n--- CURRENT DETAILS ---\n";
            cout << "Type: " << p.type << " | Size: " << p.size << " | Color: " << p.color << "\n";
            cout << "Price: " << p.price << " Birr | Cost: " << p.costPrice << " Birr | Qty: " << p.quantity << "\n";
            
            int updateChoice;
            cout << "\nWhat would you like to update?\n";
            cout << "1. Price only\n";
            cout << "2. Cost only\n";
            cout << "3. Quantity only\n";
            cout << "4. All details\n";
            cout << "Choose: ";
            cin >> updateChoice;

            if (updateChoice == 1) {
                p.price = getValidPrice("Enter new price (Birr): ");
                if (p.price < p.costPrice) {
                    cout << "\n⚠ WARNING: You're selling at a LOSS!\n";
                    cout << "Selling price (" << p.price << " Birr) is LESS than cost price (" << p.costPrice << " Birr)\n";
                    cout << "You will lose " << (p.costPrice - p.price) << " Birr per item!\n";
                    cout << "Do you want to continue? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore(1000, '\n');
                    if (confirm != 'y' && confirm != 'Y') {
                        cout << "Update cancelled.\n";
                        p.price = getValidPrice("Enter new price (Birr): ");
                    }
                }
            }
            else if (updateChoice == 2) {
                p.costPrice = getValidCost("Enter new cost price (Birr): ");
                if (p.price < p.costPrice) {
                    cout << "\n⚠ WARNING: You're selling at a LOSS!\n";
                    cout << "Selling price (" << p.price << " Birr) is LESS than cost price (" << p.costPrice << " Birr)\n";
                    cout << "You will lose " << (p.costPrice - p.price) << " Birr per item!\n";
                    cout << "Do you want to continue? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore(1000, '\n');
                    if (confirm != 'y' && confirm != 'Y') {
                        cout << "Update cancelled.\n";
                        p.costPrice = getValidCost("Enter new cost price (Birr): ");
                    }
                }
            }
            else if (updateChoice == 3) {
                p.quantity = getValidQuantity("Enter new quantity: ");
            }
            else if (updateChoice == 4) {
                cout << "\nEnter new details:\n";
                p.type = getValidType();
                p.size = getValidSize();
                p.color = getValidColor();
                p.price = getValidPrice("Enter new price (Birr): ");
                p.costPrice = getValidCost("Enter new cost price (Birr): ");
                if (p.price < p.costPrice) {
                    cout << "\n⚠ WARNING: You're selling at a LOSS!\n";
                    cout << "Selling price (" << p.price << " Birr) is LESS than cost price (" << p.costPrice << " Birr)\n";
                    cout << "You will lose " << (p.costPrice - p.price) << " Birr per item!\n";
                    cout << "Do you want to continue? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore(1000, '\n');
                    if (confirm != 'y' && confirm != 'Y') {
                        cout << "Update cancelled. Re-entering prices...\n";
                        p.price = getValidPrice("Enter new price (Birr): ");
                        p.costPrice = getValidCost("Enter new cost price (Birr): ");
                    }
                }
                p.quantity = getValidQuantity("Enter new quantity: ");
            }
            else {
                cout << "Invalid choice. No changes made.\n";
            }
        }
        temp << p.id << " " << p.type << " " << p.size << " "
             << p.color << " " << p.price << " "
             << p.costPrice << " " << p.quantity << endl;
    }

    infile.close();
    temp.close();

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (found) {
        cout << "\n Product updated successfully.\n";
    } else {
        cout << "\n Product ID not found.\n";
    }
}

/* ===================== SORT PRODUCTS ===================== */
void sortProducts() {
    int sortChoice;
    cout << "\n--- SORT PRODUCTS ---\n";
    cout << "1. Sort by Price (Low to High)\n";
    cout << "2. Sort by Price (High to Low)\n";
    cout << "3. Sort by Quantity (Low to High)\n";
    cout << "4. Sort by Quantity (High to Low)\n";
    cout << "5. Sort by Profit (High to Low)\n";
    cout << "Choose: ";
    cin >> sortChoice;

    Product products[100];
    int count = 0;
    ifstream file("inventory.txt");

    if (!file) {
        cout << "file not found.\n";
        return;
    }

    while (file >> products[count].id >> products[count].type >> products[count].size 
                >> products[count].color >> products[count].price 
                >> products[count].costPrice >> products[count].quantity) {
        count++;
    }
    file.close();

    if (count == 0) {
        cout << "No products to sort.\n";
        return;
    }

    if (sortChoice == 1) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (products[j].price > products[j + 1].price) {
                    Product temp = products[j];
                    products[j] = products[j + 1];
                    products[j + 1] = temp;
                }
            }
        }
        cout << "\n--- SORTED BY PRICE (LOW TO HIGH) ---\n";
    }
    else if (sortChoice == 2) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (products[j].price < products[j + 1].price) {
                    Product temp = products[j];
                    products[j] = products[j + 1];
                    products[j + 1] = temp;
                }
            }
        }
        cout << "\n--- SORTED BY PRICE (HIGH TO LOW) ---\n";
    }
    else if (sortChoice == 3) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (products[j].quantity > products[j + 1].quantity) {
                    Product temp = products[j];
                    products[j] = products[j + 1];
                    products[j + 1] = temp;
                }
            }
        }
        cout << "\n--- SORTED BY QUANTITY (LOW TO HIGH) ---\n";
    }
    else if (sortChoice == 4) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (products[j].quantity < products[j + 1].quantity) {
                    Product temp = products[j];
                    products[j] = products[j + 1];
                    products[j + 1] = temp;
                }
            }
        }
        cout << "\n--- SORTED BY QUANTITY (HIGH TO LOW) ---\n";
    }
    else if (sortChoice == 5) {
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                double profit1 = products[j].price - products[j].costPrice;
                double profit2 = products[j + 1].price - products[j + 1].costPrice;
                if (profit1 < profit2) {
                    Product temp = products[j];
                    products[j] = products[j + 1];
                    products[j + 1] = temp;
                }
            }
        }
        cout << "\n--- SORTED BY PROFIT (HIGH TO LOW) ---\n";
    }
    else {
        cout << "Invalid choice.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        double profitPerUnit = products[i].price - products[i].costPrice;
        cout << (i+1) << ". ID: " << products[i].id << " | " << products[i].type 
             << " | Size: " << products[i].size << " | Color: " << products[i].color
             << " | Price: " << products[i].price << " Birr | Qty: " << products[i].quantity
             << " | Profit: " << profitPerUnit << " Birr\n";
    }
    cout << "\nTotal products: " << count << "\n";
}

/* ===================== CALCULATE PROFIT ===================== */
void calculateProfit() {
    Product p;
    ifstream file("inventory.txt");
    
    if (!file) {
        cout << "No products found.\n";
        return;
    }

    double totalPotentialProfit = 0;
    double totalInventoryValue = 0;
    double totalCostValue = 0;
    int totalItems = 0;
    int lowStockCount = 0;
    int outOfStockCount = 0;
    
    cout << "\n--- PROFIT CALCULATION ---\n";
    
    while (file >> p.id >> p.type >> p.size >> p.color
                >> p.price >> p.costPrice >> p.quantity) {
        totalItems++;
        if (p.quantity == 0) outOfStockCount++;
        else if (p.quantity <= 2) lowStockCount++;
        
        double unitProfit = p.price - p.costPrice;
        double productProfit = unitProfit * p.quantity;
        double productValue = p.price * p.quantity;
        double productCost = p.costPrice * p.quantity;
        
        totalPotentialProfit += productProfit;
        totalInventoryValue += productValue;
        totalCostValue += productCost;
    }
    file.close();

    cout << "\n--- SUMMARY ---\n";
    cout << "Total Products: " << totalItems << "\n";
    cout << "Total Inventory Value: " << totalInventoryValue << " Birr\n";
    cout << "Total Cost Value: " << totalCostValue << " Birr\n";
    cout << "Total Potential Profit: " << totalPotentialProfit << " Birr\n";
    
    if (totalCostValue > 0) {
        double profitMargin = ((totalInventoryValue - totalCostValue) / totalCostValue) * 100;
        cout << "Average Profit Margin: " << profitMargin << "%\n";
    }
    
    cout << "\n--- STOCK STATUS ---\n";
    cout << "Low Stock Items (≤2): " << lowStockCount << "\n";
    cout << "Out of Stock Items: " << outOfStockCount << "\n";
    cout << "In Stock Items: " << (totalItems - lowStockCount - outOfStockCount) << "\n";
}

/* ===================== CHECK DUPLICATE ===================== */
bool productExists(string type, string size, string color) {
    Product p;
    ifstream file("inventory.txt");

    while (file >> p.id >> p.type >> p.size >> p.color
                >> p.price >> p.costPrice >> p.quantity) {
        if (p.type == type && p.size == size && p.color == color) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/* ===================== GENERATE ID ===================== */
string generateID(string type) {
    static int dressCount = 1, tshirtCount = 1, shirtCount = 1, 
                hoodieCount = 1, jeansCount = 1, jacketCount = 1;
    
    string lowerType = type;
    transform(lowerType.begin(), lowerType.end(), lowerType.begin(), ::tolower);
    
    if (lowerType == "dress") return "DR" + to_string(dressCount++);
    else if (lowerType == "t-shirt") return "TS" + to_string(tshirtCount++);
    else if (lowerType == "shirt") return "SH" + to_string(shirtCount++);
    else if (lowerType == "hoodie") return "HO" + to_string(hoodieCount++);
    else if (lowerType == "jeans") return "JE" + to_string(jeansCount++);
    else if (lowerType == "jacket") return "JA" + to_string(jacketCount++);
    else return "PR" + to_string(1);
}

/* ===================== PROCESS SALE ===================== */
void processSale() {
    string today = getCurrentDate();
    Product products[1000];
    int count = 0;

    // Open inventory file
    ifstream inventoryFile("inventory.txt");
    if (!inventoryFile) {
        cout << "No products available for sale (inventory file missing).\n";
        return;
    }

    // Read all products into array
    while (inventoryFile >> products[count].id >> products[count].type 
                        >> products[count].size >> products[count].color
                        >> products[count].price >> products[count].costPrice 
                        >> products[count].quantity) {
        count++;
    }
    inventoryFile.close();

    if (count == 0) {
        cout << "No products available for sale.\n";
        return;
    }

    char more = 'y';
    while (more == 'y' || more == 'Y') {
        // Display all products
        cout << "\n=== AVAILABLE PRODUCTS ===\n";
        cout << "Index\tType\t\tSize\tColor\tPrice\tQty\tStatus\n";
        cout << "--------------------------------------------------------\n";

        for (int i = 0; i < count; i++) {
            cout << i << "\t";
            if (products[i].type.length() < 8) cout << products[i].type << "\t\t";
            else cout << products[i].type << "\t";
            cout << products[i].size << "\t" 
                 << products[i].color << "\t" 
                 << products[i].price << "\t" 
                 << products[i].quantity << "\t";

            if (products[i].quantity == 0) cout << "OUT OF STOCK";
            else if (products[i].quantity <= 2) cout << "LOW STOCK";
            else cout << "AVAILABLE";

            cout << " (ID: " << products[i].id << ")";
            cout << endl;
        }

        // Ask user for product index
        int index;
        cout << "\nEnter product INDEX to sell (0-" << count - 1 << "): ";
        cin >> index;

        if (cin.fail() || index < 0 || index >= count) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid index.\n";
            continue;
        }

        Product& selectedProduct = products[index];

        // Check if product is in stock
        if (selectedProduct.quantity == 0) {
            cout << "This product is out of stock.\n";
            continue;
        }

        // Ask quantity to sell
        int qty;
        while (true) {
            cout << "\nSelected: " << selectedProduct.type << " " 
                 << selectedProduct.color << " " << selectedProduct.size << "\n";
            cout << "Price: " << selectedProduct.price << " Birr | Available: " 
                 << selectedProduct.quantity << "\n";
            cout << "Enter quantity to sell: ";
            cin >> qty;

            if (cin.fail() || qty <= 0 || qty > selectedProduct.quantity) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Invalid! Enter a number between 1 and " << selectedProduct.quantity << ".\n";
            } else {
                break;
            }
        }

        // Update quantity & calculate profit
        selectedProduct.quantity -= qty;
        double profit = (selectedProduct.price - selectedProduct.costPrice) * qty;

        // Rewrite inventory file
        ofstream outFile("inventory.txt");
        for (int i = 0; i < count; i++) {
            outFile << products[i].id << " " << products[i].type << " " 
                    << products[i].size << " " << products[i].color << " "
                    << products[i].price << " " << products[i].costPrice << " "
                    << products[i].quantity << endl;
        }
        outFile.close();

        // Record sale in sales files
        ofstream sales("sales.txt", ios::app);
        ofstream daily("daily_sales.txt", ios::app);

        sales << selectedProduct.id << " " << qty << " " << profit << " " << today << endl;
        daily << today << " " << selectedProduct.id << " " << qty << " " << profit << endl;

        sales.close();
        daily.close();

        // Display sale summary
        cout << "\n=== SALE COMPLETED ===\n";
        cout << "Date: " << today << "\n";
        cout << "Product: " << selectedProduct.type << " " 
             << selectedProduct.color << " " << selectedProduct.size << "\n";
        cout << "Quantity Sold: " << qty << "\n";
        cout << "Unit Price: " << selectedProduct.price << " Birr\n";
        cout << "Total Price: " << (selectedProduct.price * qty) << " Birr\n";
        cout << "Total Profit: " << profit << " Birr\n";
        cout << "Remaining Stock: " << selectedProduct.quantity << "\n";
        cout << "=======================\n";

        cout << "\nSell another product? (y/n): ";
        cin >> more;
        cin.ignore(1000, '\n');
    }
}


/* ===================== SALES REPORT ===================== */
void salesReport() {
    ifstream sales("sales.txt");
    string id, date;
    int qty;
    double profit, total = 0;
    int totalQty = 0;

    cout << "\n--- SALES REPORT (ALL TIME) ---\n";
    
    bool hasSales = false;
    while (sales >> id >> qty >> profit >> date) {
        hasSales = true;
        cout << "Date: " << date << " | ID: " << id << " | Sold: " << qty
             << " | Profit: " << profit << " Birr\n";
        total += profit;
        totalQty += qty;
    }

    if (!hasSales) {
        cout << "No sales recorded yet.\n";
        return;
    }

    cout << "----------------------------------------\n";
    cout << "TOTAL QUANTITY SOLD: " << totalQty << "\n";
    cout << "TOTAL PROFIT: " << total << " Birr\n";
    
    if (totalQty > 0) {
        cout << "AVERAGE PROFIT PER SALE: " << (total / totalQty) << " Birr\n";
    }
    sales.close();
}

/* ===================== DAILY PROFIT REPORT ===================== */
void dailyProfitReport() {
    ifstream daily("daily_sales.txt");
    
    if (!daily) {
        cout << "\nNo daily sales recorded yet.\n";
        return;
    }
    
    string date, id;
    int qty;
    double profit;
    
    DailySale dailySales[100];
    int dayCount = 0;
    
    cout << "\n--- DAILY PROFIT REPORT ---\n";
    cout << "Date\t\tTotal Profit\tTotal Quantity\n";
    cout << "----------------------------------------\n";
    
    while (daily >> date >> id >> qty >> profit) {
        bool found = false;
        
        for (int i = 0; i < dayCount; i++) {
            if (dailySales[i].date == date) {
                dailySales[i].totalProfit += profit;
                dailySales[i].totalQuantity += qty;
                found = true;
                break;
            }
        }
        
        if (!found) {
            dailySales[dayCount].date = date;
            dailySales[dayCount].totalProfit = profit;
            dailySales[dayCount].totalQuantity = qty;
            dayCount++;
        }
    }
    daily.close();
    
    if (dayCount == 0) {
        cout << "No daily sales recorded yet.\n";
        return;
    }
    
    double grandTotalProfit = 0;
    int grandTotalQty = 0;
    
    for (int i = 0; i < dayCount; i++) {
        cout << dailySales[i].date << "\t" 
             << dailySales[i].totalProfit << " Birr\t\t"
             << dailySales[i].totalQuantity << "\n";
        
        grandTotalProfit += dailySales[i].totalProfit;
        grandTotalQty += dailySales[i].totalQuantity;
    }
    
    cout << "----------------------------------------\n";
    cout << "GRAND TOTAL: " << grandTotalProfit << " Birr\t\t" << grandTotalQty << "\n";
    
    string today = getCurrentDate();
    double todayProfit = 0;
    int todayQty = 0;
    
    for (int i = 0; i < dayCount; i++) {
        if (dailySales[i].date == today) {
            todayProfit = dailySales[i].totalProfit;
            todayQty = dailySales[i].totalQuantity;
            break;
        }
    }
    
    cout << "\n--- TODAY'S SUMMARY (" << today << ") ---\n";
    if (todayProfit > 0) {
        cout << "Today's Profit: " << todayProfit << " Birr\n";
        cout << "Today's Quantity Sold: " << todayQty << "\n";
        cout << "Average per sale today: " << (todayQty > 0 ? todayProfit / todayQty : 0) << " Birr\n";
    } else {
        cout << "No sales recorded today yet.\n";
    }
    
    if (dayCount > 0) {
        int bestDayIndex = 0;
        for (int i = 1; i < dayCount; i++) {
            if (dailySales[i].totalProfit > dailySales[bestDayIndex].totalProfit) {
                bestDayIndex = i;
            }
        }
        
        cout << "\n--- BEST DAY ---\n";
        cout << "Date: " << dailySales[bestDayIndex].date << "\n";
        cout << "Profit: " << dailySales[bestDayIndex].totalProfit << " Birr\n";
        cout << "Quantity: " << dailySales[bestDayIndex].totalQuantity << "\n";
    }
}

/* ===================== LOW STOCK ALERT ===================== */
void checkLowStock() {
    Product p;
    ifstream file("inventory.txt");

    cout << "\n--- LOW STOCK ALERT ---\n";
    
    bool hasLowStock = false;
    bool hasOutOfStock = false;
    
    while (file >> p.id >> p.type >> p.size >> p.color
                >> p.price >> p.costPrice >> p.quantity) {
        if (p.quantity == 0) {
            hasOutOfStock = true;
            cout << "OUT OF STOCK: " << p.type << " " << p.color << " " << p.size 
                 << " (ID: " << p.id << ")\n";
        } else if (p.quantity <= 2) {
            hasLowStock = true;
            cout << " LOW STOCK: " << p.type << " " << p.color << " " << p.size 
                 << " (ID: " << p.id << ") - Only " << p.quantity << " left\n";
        }
    }
    file.close();
    
    if (!hasLowStock && !hasOutOfStock) {
        cout << "All products have sufficient stock. Good job!\n";
    }
}

