#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <numeric>
#include <sstream>
#include <fstream>

using namespace std;

const char* TXT_PATH = "C:/prac/Frolov.txt";
const char* BIN_PATH = "C:/prac/Frolov.bin";

struct Date {
    unsigned short int day;
    unsigned short int month;
    unsigned short int year;
};

struct Record {
    char author[20];
    char name[100];
    unsigned short year;
    char group[5];
    Date date;
};

void drawBorder(int tableWidth) {
    cout << endl;
    cout.width(tableWidth);
    cout.fill('-');
    cout << '-' << endl;
}

void formatColumn(string str, int maxLen, int tableWidth, bool header = false) {
    cout << "| " << str;
    header ? cout.width(tableWidth - str.length() - 2) : cout.width(maxLen - str.length() - 2);
    cout.fill(' ');
    cout << " |";
}

void drawBottom(int tableWidth) {
    string bottom[2] = { "| Примечание: Х - художественная литература; У - учебная литература;", "| С - справочная литература;" };
    for (int i = 0; i < 2; i++) {
        cout << bottom[i];
        cout.width(tableWidth - size(bottom[i]));
        cout.fill(' ');
        cout << "|";
        if (!i) cout << endl;
    }
    drawBorder(tableWidth);
}

void drawLine(vector<string> columns, vector<int> params, int tableWidth) {
    for (int i = 0; i < columns.size(); i++) {
        formatColumn(columns[i], params[i], tableWidth);
    }
}

int getMaxLen(vector<string> names) {
    int max = 0;
    for (string str : names)
    {
        int len = str.length();
        max = len > max ? len : max;
    }
    return max + 4;
}

void drawTable(vector<vector<string>> lines, vector<string> columns, string title, bool shouldDrawBottom = false) {
    
    vector<int> params;
    for (int i = 0; i < columns.size(); i++) {
        vector<string> columnValues = { columns[i] };
        for (int j = 0; j < lines.size(); j++) {
            columnValues.push_back(lines[j][i]);
        }
        params.push_back(getMaxLen(columnValues));
    }

    int tableWidth = accumulate(params.begin(), params.end(), 0);
    drawBorder(tableWidth);
    if (title.length()) {
        formatColumn(title, title.length(), tableWidth, true);
        drawBorder(tableWidth);
    }

    for (int i = 0; i < lines.size() + 1; i++) {
        if (!i) {
            drawLine(columns, params, tableWidth);
            drawBorder(tableWidth);
            continue;
        }
     
        drawLine(lines[i - 1], params, tableWidth);
        drawBorder(tableWidth);
    }

    if(shouldDrawBottom) drawBottom(tableWidth);
}

vector<Record> modifyRecords(vector<Record> lines) {
    vector<int> average;
    for (auto &record : lines) {
        strcpy_s(record.name, to_string(((record.date.day + record.date.month + record.date.year + record.year) / 4)).c_str());
    }
    return lines;
}

string dateToString(Date date) {
    return to_string(date.day) + "." + to_string(date.month) + "." + to_string(date.year);
}

vector<Record> readDataBin() {
    ifstream file(BIN_PATH, ios::binary);
    Record record;

    vector<Record> records;

    for (int i = 0; file.read((char*)&record, sizeof(record)); i++) {
        records.push_back(record);
    }
    file.close();
    return records;
}

void addRecordBin(vector <Record> records) {
    ofstream file(BIN_PATH, ios::binary | ios::app);
    for (auto &record : records) {
        file.write((char*)&record, sizeof(record));
    }
    file.close();
}

vector<Record> readDataTxt() {
    ifstream file(TXT_PATH);
    Record record;

    vector<Record> records;

    for(int i = 0; i < 3; i++) {
        file >> record.date.day;
        file >> record.date.month;
        file >> record.date.year;
        file >> record.author;
        file >> record.name;
        file >> record.year;
        file >> record.group;
        records.push_back(record);
    }

    file.close();

    return records;
}

void addRecordTxt(vector<Record> records) {
    ofstream file(TXT_PATH, ios::app);
    for(int i = 0; i < records.size(); i++) {
        file << records[i].date.day << endl;
        file << records[i].date.month << endl;
        file << records[i].date.year << endl;
        file << records[i].author << endl;
        file << records[i].name << endl;
        file << records[i].year << endl;
        file << records[i].group << endl;
    }
    file.close();
}
int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<Record> lines = {
      { "Сенкевич", "Потоп", 1978, "Х", {11, 11, 2020} },
      { "Ландау", "Механика", 1989, "У", {11, 11, 2020} },
      { "Дойль", "Сумчатые", 1990, "С", {12, 12, 2010} }
    };

    auto changedRecords = modifyRecords(lines);
    vector<string> columnsMain = { "Автор книги", "Название", "Год выпуска", "Группа", "Дата подписания рукописи" };
    string title0 = "Каталог библиотеки (Практика 0, 3)";
    vector<vector<string>> records;
    vector<vector<string>> modifiedRecords;
    for (auto rec : lines) {
        records.push_back({ rec.author, rec.name, to_string(rec.year), rec.group, dateToString(rec.date) });
    }
    drawTable(records, columnsMain, title0, true);
    
    cout << "Задание: " << "изменить записи (элементы массива структур), содержащие вводимое с клавиатуры значение,\n";
    cout << "значение на среднеарифметическое значение в соответствующих числовых столбцах";
    string title1 = "Каталог библиотеки (Практика 1)";

    for (auto rec : changedRecords) {
        modifiedRecords.push_back({ rec.author, rec.name, to_string(rec.year), rec.group, dateToString(rec.date) });
    }
    drawTable(modifiedRecords, columnsMain, title1, true);

    Record* A;
    Record** B;
    B = (Record**) new Record * [10];
    A = (Record*)calloc(10, sizeof(Record));
    for (int i = 0; i < 10; i++) {
        A[i] = lines[i % 3];
        B[i] = (Record*) new Record;
        *(B[i]) = lines[i % 3];
    }

    A = (Record*)realloc(A, 10 * sizeof(Record));
    
    ostringstream firstItemAddressTable;
    ostringstream firstItemAddressA;
    ostringstream firstItemAddressB;

    firstItemAddressTable << &lines[0];
    firstItemAddressA << &A[0];
    firstItemAddressB << &B[0];

    vector<vector<string>> firstItemAddresses = { {firstItemAddressTable.str(), firstItemAddressA.str(), firstItemAddressB.str()} };
    vector<string> columnsFirstItem = { "Table", "A", "B" };
    string firstItemTableHeader = "Адреса первых элементов массива";

    cout << "Практика 2:";

    drawTable(firstItemAddresses, columnsFirstItem, firstItemTableHeader);

    vector<vector<string>> totalAddressesA;
    vector<vector<string>> totalAddressesB;


    for (int i = 0; i < 20; i++) {
        ostringstream address;
        if (i < 10) {
            address << &A[i];
            totalAddressesA.push_back({ to_string(i), address.str(), A[i].author, A[i].name, to_string(A[i].year), A[i].group, dateToString(A[i].date) });
            continue;
        }
        address << &B[i - 10];
        totalAddressesB.push_back({ to_string(i - 10), address.str(), B[i - 10]->author, B[i - 10]->name, to_string(B[i - 10]->year), B[i - 10]->group, dateToString(B[i - 10]->date)});
    }

    vector<string> adressesColumns = { "i", "Адрес", "Автор книги", "Название", "Год выпуска", "Группа", "Дата подписания рукописи" };
    string addressesHeaderA = "Адреса и значения массива А";
    string addressesHeaderB = "Адреса и значения массива B";
  
    drawTable(totalAddressesA, adressesColumns, addressesHeaderA);
    drawTable(totalAddressesB, adressesColumns, addressesHeaderB);

    free(A);

    for (int i = 0; i < 10; i++) {
        delete B[i];
    }

    delete[]B;

    cout << "Практика номер 4 (Вариант 5)\n";

    vector<Record> linesFromFile;
    vector<vector<string>> stringLines;

    addRecordTxt(lines);
    linesFromFile = readDataTxt();
    string titleTxt = "Каталог библиотеки (из текстового файла)";

    for(auto rec : linesFromFile) {
        stringLines.push_back({rec.author, rec.name, to_string(rec.year), rec.group, dateToString(rec.date)});
    }
    drawTable(stringLines, columnsMain, titleTxt, true);

    stringLines = {};

    addRecordBin(lines);
    linesFromFile = readDataBin();
    string titleBin = "Каталог библиотеки (из бинарного файла)";

    for(auto rec : linesFromFile) {
        stringLines.push_back({rec.author, rec.name, to_string(rec.year), rec.group, dateToString(rec.date)});
    }
    drawTable(stringLines, columnsMain, titleBin, true);

    cout << "Добавление четвертой записи:\n";

    Record newRecord = {};

    cout << "Введите автора книги\n";
    cin >> newRecord.author;

    cout << "Введите название книги\n";
    cin >> newRecord.name;

    cout << "Введите год выпуска книги\n";
    cin >> newRecord.year;

    cout << "Введите группу книги\n";
    cin >> newRecord.group;

    cout << "Введите день подписания рукописи\n";
    cin >> newRecord.date.day;

    cout << "Введите месяц подписания рукописи\n";
    cin >> newRecord.date.month;

    cout << "Введите год подписания рукописи\n";
    cin >> newRecord.date.year;

    lines.push_back(newRecord);

    stringLines = {};

    addRecordBin({lines[lines.size() - 1]});
    linesFromFile = readDataBin();
    string newTitleBin = "Каталог библиотеки (с добавленной записью)";

    for(auto rec : linesFromFile) {
        stringLines.push_back({rec.author, rec.name, to_string(rec.year), rec.group, dateToString(rec.date)});
    }
    drawTable(stringLines, columnsMain, newTitleBin, true);

    cout << "Задание: Изменить записи файла, содержащие вводимое с клавиатуры строковое значение, увеличив соответствующую дату на 1";

    for(int i = 0; i < linesFromFile.size(); i++) {
        linesFromFile[i].date.day++;
    }

    remove(BIN_PATH);

    addRecordBin(linesFromFile);

    stringLines = {};

    string changedTitleBin = "Каталог библиотеки (с измененной записью)";

    for(auto rec : linesFromFile) {
        stringLines.push_back({rec.author, rec.name, to_string(rec.year), rec.group, dateToString(rec.date)});
    }

    drawTable(stringLines, columnsMain, changedTitleBin, true);
}
