# Inventory Management System (Clothing Store)

A professional **C++** command-line application designed for retail owners to manage clothing inventory, track sales, and analyze business profitability.

## 🚀 Key Features
- **Inventory Control:** Add, update, search, and delete products (Dresses, T-Shirts, Hoodies, etc.).
- **Smart Validation:** Strict input handling for Sizes (XS-XXL) and Colors to ensure data integrity.
- **Sales Tracking:** Process transactions, auto-update stock levels, and generate profit reports.
- **Low Stock Alerts:** Automatically identifies items that need restocking.
- **Data Persistence:** Saves all records to `inventory.txt`, `sales.txt`, and `daily_sales.txt`.

## 🛠️ Tech Stack
- **Language:** C++
- **Storage:** Flat-file database (.txt)
- **Features:** Structs, File I/O, Algorithm-based sorting, and Time-stamped reporting.

## 📦 How to Run
1. Compile the source code:
   ```bash
   g++ main.cpp -o InventorySystem
