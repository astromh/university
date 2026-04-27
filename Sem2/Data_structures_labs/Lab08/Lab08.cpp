#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Dish {
    string name;
    vector<string> components;
    string property;
};

void AddDishToHashTable(const Dish& dish, unordered_map<size_t, vector<Dish>>& hashTable);
void SearchDish(const unordered_map<size_t, vector<Dish>>& hashTable, vector<string> Components, 
                string property);
string BuildKey(vector<string> components, string property);
size_t GetHash(const string& key);

int main() {
    vector<Dish> menu;

    Dish dish1;
    dish1.name = "Chef Burger";
    dish1.components = {"fillet", "salad", "tomatoe"};
    dish1.property = "lactose_free";

    Dish dish2;
    dish2.name = "cheese Burger";
    dish2.components = {"cheese", "onion", "cutlet"};
    dish2.property = "not_vegetarian";

    Dish dish3;
    dish3.name = "Chef Roll";
    dish3.components = {"stripes", "salad", "tomatoe"};
    dish3.property = "halal";

    Dish dish4;
    dish4.name = "Roast Master";
    dish4.components = {"fillet", "cucumber", "cheese", "potatoes"};
    dish4.property = "not_kosher";

    menu.push_back(dish1);
    menu.push_back(dish2);
    menu.push_back(dish3);
    menu.push_back(dish4);

    unordered_map<size_t, vector<Dish>> hashTable;

    for (const Dish& dish : menu) {
        AddDishToHashTable(dish, hashTable);
    }

    vector<string> SearchComponents;
    string component;
    string SearchProperty;

    cout << "\n[?] Enter 3 components for search:" << endl;
    for (int i=0; i<3; ++i) {
        cout << i+1 << ") Component: ";
        cin >> component;
        SearchComponents.push_back(component);
    }

    cout << "\n[?] Enter a property: ";
    cin >> SearchProperty;

    SearchDish(hashTable, SearchComponents, SearchProperty);

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

size_t GetHash(const string& key) {
    return hash<string>{}(key);
}

void AddDishToHashTable(const Dish& dish, unordered_map<size_t, vector<Dish>>& hashTable) {
    string key = BuildKey(dish.components, dish.property);
    size_t hashValue = GetHash(key);

    hashTable[hashValue].push_back(dish);

    cout << "Dish: " << dish.name << endl;
    cout << "Key: " << key << endl;
    cout << "Hash: " << hashValue << endl;
    cout << "----------------------" << endl;

}

void SearchDish(const unordered_map<size_t, vector<Dish>>& hashTable, 
    vector<string> searchComponents, string searchProperty) {

    string SearchKey = BuildKey(searchComponents, searchProperty);
    size_t SearchHash = GetHash(SearchKey);

    cout << "\nSearch Key: " << SearchKey << endl;
    cout << "Search Hash: " << SearchHash << endl;

    auto it = hashTable.find(SearchHash);

    if (it == hashTable.end()) {
        cout << "[-] No dishes found!" << endl;
        return;
    }

    bool found = false;

    for (const Dish& dish: it->second) {
        string dishKey = BuildKey(dish.components, dish.property);

        if (dishKey == SearchKey) {
            cout << "\n[+] Dish Found: " << dish.name << endl;

            cout << "Components: ";
            for (const string& component: dish.components) {
                cout << component << " ";
            }
            cout << endl;

            cout << "Property: " << dish.property << endl;

            found = true;
        }
    }
    if (!found) {
        cout << "No exact dish found. Hash collision happened." << endl;
    }
 
}