#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

using namespace std;
using HashType = uint32_t;

struct Dish {
    string name;
    vector<string> components;
    string property;
};

void AddDishToHashTable(const Dish& dish, unordered_map<HashType, vector<Dish>>& hashTable);
void SearchDish(const unordered_map<HashType, vector<Dish>>& hashTable, vector<string> Components, 
                string property);
void SearchByHash(const unordered_map<HashType, vector<Dish>>& hashTable, HashType searchHash);
string BuildKey(vector<string> components, string property);
HashType MurmurHash2(const void* key, size_t len, HashType seed);
HashType GetHash(const string& key);

int main() {
    vector<Dish> menu;

    Dish dish1;
    dish1.name = "Chef Burger";
    dish1.components = {"fillet", "salad", "tomato"};
    dish1.property = "lactose_free";

    Dish dish2;
    dish2.name = "cheese Burger";
    dish2.components = {"cheese", "onion", "cutlet"};
    dish2.property = "not_vegetarian";

    Dish dish3;
    dish3.name = "Chef Roll";
    dish3.components = {"stripes", "salad", "tomato"};
    dish3.property = "halal";

    Dish dish4;
    dish4.name = "Roast Master";
    dish4.components = {"fillet", "cucumber", "potatoes"};
    dish4.property = "not_kosher";

    Dish dish5;
    dish5.name = "Diet Roll";
    dish5.components = {"fillet", "salad", "tomato"};
    dish5.property = "lactose_free";

    Dish dish6;
    dish6.name = "Vegetable Soup";
    dish6.components = {"carrot", "dill", "potato"};
    dish6.property = "vegetarian";

    menu.push_back(dish1);
    menu.push_back(dish2);
    menu.push_back(dish3);
    menu.push_back(dish4);
    menu.push_back(dish5);
    menu.push_back(dish6);

    unordered_map<HashType, vector<Dish>> hashTable;

    for (const Dish& dish : menu) {
        AddDishToHashTable(dish, hashTable);
    }

    int choice;

    cout << "\nChoose search type:" << endl;
    cout << "1. Search by 3 components and property" << endl;
    cout << "2. Search by hash" << endl;
    cout << "Your choice: ";
    cin >> choice;

    if (choice == 1) {
        vector<string> searchComponents;
        string component;
        string searchProperty;

        cout << "\n[?] Enter 3 components for search:" << endl;

        for (int i = 0; i < 3; ++i) {
            cout << i + 1 << ") Component: ";
            cin >> component;
            searchComponents.push_back(component);
        }

        cout << "\n[?] Enter a property: ";
        cin >> searchProperty;

        SearchDish(hashTable, searchComponents, searchProperty);
    }
    else if (choice == 2) {
        HashType searchHash;

        cout << "\n[?] Enter hash value: ";
        cin >> searchHash;

        SearchByHash(hashTable, searchHash);
    }
    else {
        cout << "Invalid choice." << endl;
    }

    return 0;
}

string BuildKey(vector<string> components, string property) {
    sort(components.begin(), components.end());

    string key = components[0] + "#" + 
                 components[1] + "#" +
                 components[2] + "#" +
                 property;
    
    return key;
}

HashType MurmurHash2(const void* key, size_t len, HashType seed) {
    const HashType m = 0x5bd1e995;
    const int r = 24;

    HashType h = seed ^ static_cast<HashType>(len);

    const unsigned char* data = static_cast<const unsigned char*>(key);

    while (len >= 4) {
        HashType k = static_cast<HashType>(data[0])
                   | (static_cast<HashType>(data[1]) << 8)
                   | (static_cast<HashType>(data[2]) << 16)
                   | (static_cast<HashType>(data[3]) << 24);

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len) {
        case 3:
            h ^= static_cast<HashType>(data[2]) << 16;
        case 2:
            h ^= static_cast<HashType>(data[1]) << 8;
        case 1:
            h ^= static_cast<HashType>(data[0]);
            h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

HashType GetHash(const string& key) {
    const HashType seed = 0x9747b28c;
    return MurmurHash2(key.data(), key.size(), seed);
}

void AddDishToHashTable(const Dish& dish, unordered_map<HashType, vector<Dish>>& hashTable) {
    string key = BuildKey(dish.components, dish.property);
    HashType hashValue = GetHash(key);

    hashTable[hashValue].push_back(dish);

    cout << "Dish: " << dish.name << endl;
    cout << "Key: " << key << endl;
    cout << "MurmurHash2: " << hashValue << endl;
    cout << "----------------------" << endl;
}

void SearchDish(
    const unordered_map<HashType, vector<Dish>>& hashTable,
    vector<string> searchComponents,
    string searchProperty
) {
    string searchKey = BuildKey(searchComponents, searchProperty);
    HashType searchHash = GetHash(searchKey);

    cout << "\nSearch Key: " << searchKey << endl;
    cout << "Search MurmurHash2: " << searchHash << endl;

    auto it = hashTable.find(searchHash);

    if (it == hashTable.end()) {
        cout << "[-] No dishes found!" << endl;
        cout << "Count of matching dishes: 0" << endl;
        return;
    }

    int count = 0;

    for (const Dish& dish : it->second) {
        string dishKey = BuildKey(dish.components, dish.property);

        if (dishKey == searchKey) {
            cout << "\n[+] Dish Found: " << dish.name << endl;

            cout << "Components: ";
            for (const string& component : dish.components) {
                cout << component << " ";
            }
            cout << endl;

            cout << "Property: " << dish.property << endl;

            count++;
        }
    }

    if (count == 0) {
        cout << "No exact dish found. Hash collision happened." << endl;
    }

    cout << "\nCount of matching dishes: " << count << endl;
}

void SearchByHash(const unordered_map<HashType, vector<Dish>>& hashTable, HashType searchHash) {
    cout << "\nSearch by hash: " << searchHash << endl;

    auto it = hashTable.find(searchHash);

    if (it == hashTable.end()) {
        cout << "[-] No dishes found with this hash." << endl;
        cout << "Count of dishes with this hash: 0" << endl;
        return;
    }

    int count = 0;

    for (const Dish& dish : it->second) {
        string key = BuildKey(dish.components, dish.property);

        cout << "\n[+] Dish Found: " << dish.name << endl;
        cout << "Key: " << key << endl;

        cout << "Components: ";
        for (const string& component : dish.components) {
            cout << component << " ";
        }
        cout << endl;

        cout << "Property: " << dish.property << endl;

        count++;
    }

    cout << "\nCount of dishes with this hash: " << count << endl;
}