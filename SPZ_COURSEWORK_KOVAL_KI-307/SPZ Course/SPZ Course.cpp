#include <iostream>
#include <sqlite3.h>
#include <sstream> // Для std::stringstream
#include <cstdlib> // Для функцій rand() та srand()
#include <ctime>   // Для функції time()
#include <Windows.h> // UA LOCALIZATION

void createTable(sqlite3* db, const std::string& nameOfTable, const std::string& nameOrText, const std::string& scoreOrIntg, const std::string& passedOrText) {
    char* errorMessage = nullptr;
    std::string createTableSQL = "CREATE TABLE IF NOT EXISTS " + nameOfTable + " ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        + nameOrText + " TEXT NOT NULL, "
        + scoreOrIntg + " INTEGER NOT NULL, "
        + passedOrText + " TEXT NOT NULL"
        ");";

    int rc = sqlite3_exec(db, createTableSQL.c_str(), nullptr, nullptr, &errorMessage);

    if (rc != SQLITE_OK) {
        std::cout << "Помилка при створенні таблиці: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << "Таблиця " << nameOfTable << " створена успішно." << std::endl;
    }
}


void insertRandomData(sqlite3* db, const std::string& name) {
    int Score = rand() % 20; // Випадковий бал від 0 до 100
    std::string passed = (Score >= 18) ? "Yes" : "No";

    std::stringstream ss;
    ss << Score;

    std::string insertSQL = "INSERT INTO MREO_Theory (name, scoreTest, passedTest) "
        "VALUES ('" + name + "', " + ss.str() + ", '" + passed + "');";

    char* errorMessage = nullptr;
    int rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &errorMessage);

    if (rc != SQLITE_OK) {
        std::cout << "Помилка при вставці даних: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << "Дані успішно вставлено " << name << std::endl;
    }
}

void deleteAllData(sqlite3* db, const std::string& nameOfTable) {
    char* errorMessage = nullptr;
    std::string deleteSQL = "DELETE FROM " + nameOfTable + ";";

    int rc = sqlite3_exec(db, deleteSQL.c_str(), nullptr, nullptr, &errorMessage);

    if (rc != SQLITE_OK) {
        std::cout << "Помилка при видаленні даних: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
    else {
        std::cout << "Всі дані успішно видалено з таблиці " << nameOfTable << std::endl;
    }
}
void displayAllData(sqlite3* db, const std::string& nameOfTable) {
    char* errorMessage = nullptr;
    std::string selectSQL = "SELECT * FROM " + nameOfTable + ";";

    int rc = sqlite3_exec(db, selectSQL.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
        std::cout << "-----------------------------------------\n";
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        return 0;
        }, nullptr, &errorMessage);

    if (rc != SQLITE_OK) {
        std::cout << "Помилка при виведенні даних: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}
void displaySortedData(sqlite3* db, const std::string& nameOfTable, const std::string& orderByColumn) {
    char* errorMessage = nullptr;
    std::string selectSQL = "SELECT * FROM " + nameOfTable + " ORDER BY " + orderByColumn + " ASC;";

    int rc = sqlite3_exec(db, selectSQL.c_str(), [](void* data, int argc, char** argv, char** azColName) -> int {
        std::cout << "-----------------------------------------\n";
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\n";
        }
        return 0;
        }, nullptr, &errorMessage);

    if (rc != SQLITE_OK) {
        std::cout << "Помилка при виведенні даних: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}
void InsertByUser(sqlite3* db, const std::string& tableName, const std::string& nameColumn, const std::string& scoreColumn, const std::string& passedColumn) {
    std::string name, passed;
    int Score;
    std::string insertSQL;

    for (int i = 0; i < 3; i++) {
        std::cout << "Введіть дані рядка " << i + 1 << ":" << std::endl;

        std::cout << "Введіть значення для стовбця '" << nameColumn << "' (TEXT): ";
        std::cin.ignore(); // Очищення буфера вводу перед викликом getline()
        std::getline(std::cin, name);

        std::cout << "Введіть значення для стовбця '" << scoreColumn << "' (INTEGER): ";
        std::cin >> Score;

        std::stringstream ss;
        ss << Score;

        std::cout << "Введіть значення для стовбця '" << passedColumn << "' (Yes або No): ";
        std::cin >> passed;

        insertSQL = "INSERT INTO " + tableName + " (" + nameColumn + ", " + scoreColumn + ", " + passedColumn + ") "
            "VALUES ('" + name + "', " + ss.str() + ", '" + passed + "');";

        char* errorMessage = nullptr;
        int rc = sqlite3_exec(db, insertSQL.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK) {
            std::cout << "Помилка при вставці даних: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
        }
        else {
            std::cout << "Дані успішно вставлено для рядка " << i + 1 << std::endl;
        }
    }
}


int main() {
    SetConsoleOutputCP(1251); // UA LOCALIZATION
    setlocale(LC_CTYPE, "ukr"); // UA LOCALIZATION
    static std::string s;
    static std::string n;
    static std::string c;
    static std::string p;
    sqlite3* db;
    char* errorMessage = nullptr;
    int rc;
    int answer;
    std::string nameOfTableToShow;
    rc = sqlite3_open("DB.db", &db);
    if (rc != SQLITE_OK) {
        std::cout << "Помилка! Не вдалося відкрити файл бази даних.\n" << std::endl;
        return 1;
    }
    std::cout << "Файл бази даних успішно відкрито.\n" << std::endl;

    do {
    do {
        std::cout << "Що бажаєте зробити?:\n 0 - Меню заповнення бази даних (Рекомендовано) (Оптимальне рішення) \n 1 - Видалити всі дані з бази даних. \n 2 - Сортування бази даних за зростанням \n 3 - Вивід всіх даних з бази даних \n 4 - Використовувати задані алгоритми бази даних (Тестування) \n 5 - Вихід з програми \n";
        std::cin >> answer;
    }while(answer != 0 && answer != 1 && answer != 2 && answer != 3 && answer != 4 && answer != 5);



    switch (answer) {
    case 0:
        std::cout << "Enter name of table: ";
        std::cin >> s;
        std::cout << "Enter name of column 2 (TEXT): ";
        std::cin >> n;
        std::cout << "Enter name of column 3 (INTEGER): ";
        std::cin >> c;
        std::cout << "Enter name of column 4 (TEXT): ";
        std::cin >> p;
        createTable(db, s, n, c, p);
        InsertByUser(db, s, n, c, p);
    break;
    case 1:
        std::cout << "Enter name of table: \n";
        std::cin >> nameOfTableToShow;
        deleteAllData(db, nameOfTableToShow); break;
    case 2:      
        std::cout << "Enter name of table: \n";
        std::cin >> nameOfTableToShow;
        displaySortedData(db, nameOfTableToShow, c); break;
    case 3: std::cout << "Enter name of table: \n";
        
        std::cin >> nameOfTableToShow;
        displayAllData(db, nameOfTableToShow); break;
    case 4:
        srand(time(nullptr)); // Ініціалізуємо генератор випадкових чисел за часом
        s = "MREO_Theory";
        n = "name";
        c = "scoreTest";
        p = "passedTest";
        deleteAllData(db, s);
        createTable(db, s, n, c, p);
        insertRandomData(db, "Kate Koval"); 
        insertRandomData(db, "Lana Oliveira");
        insertRandomData(db, "Participant3");
        insertRandomData(db, "Participant4");
        insertRandomData(db, "Participant5");
        displayAllData(db, s);
        break;
    case 5: sqlite3_close(db); return 0; break;
    }

    } while (answer != 5);
    sqlite3_close(db);

   
}