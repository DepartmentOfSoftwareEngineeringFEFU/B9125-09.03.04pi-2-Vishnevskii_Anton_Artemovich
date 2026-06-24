#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<limits>
#include<unordered_set>
#include<vector>
#include<set>
#include<unordered_map>

std::unordered_set<std::string> validCurrencies = {"USD", "EUR", "CNY", "JPY"};
enum currency {USD, EUR, CNY, JPY};
int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 30, 31, 30, 31};

struct BuySell{
    double buy;
    double sell;
};

struct dateTime{
    int year;
    int month;
    int day;

    bool operator==(const dateTime& other) {
        return year == other.year && month == other.month && day == other.day;
    }
};
 
struct exchangeRate{
    dateTime date;
    std::string bankName;
    BuySell buySellUsd;
    BuySell buySellEur;
    BuySell buySellCny;
    BuySell buySellJpy;
};

void trim(std::string& s){
    int start = 0;
    while (start < s.size() && isspace(s[start])){
        start++;
    }
    int end = s.size() - 1;
    while(end >= start && isspace(s[end])){
        end--;
    }
    s = s.substr(start, end - start + 1);
}

bool isValidSymbol(char c){
    static std::unordered_set<char> validSymbols;
    if (validSymbols.empty()){
        for (char ch = 'a'; ch <= 'z'; ch++) validSymbols.insert(ch);
        for (char ch = 'A'; ch <= 'Z'; ch++) validSymbols.insert(ch);
        validSymbols.insert('-');
        validSymbols.insert(' ');
        validSymbols.insert('.');
    }
    return validSymbols.count(c);
}

dateTime parseDate(std::string& s) {
    dateTime date;
    trim(s);

    if (s.size() != 10 || s[4] != '.' || s[7] != '.') {
        throw std::invalid_argument("Cannot parse date");
    }

    int y = stoi(s.substr(0, 4));
    int m = stoi(s.substr(5, 2));
    int d = stoi(s.substr(8, 2));

    if (y < 2000 || y > 2026){
        throw std::invalid_argument("Year is incorrect");
    }
    if (m < 1 || m > 12){
        throw std::invalid_argument("Month is incorrect");
    }
    int maxDay = daysInMonth[m];
    if (m == 2 && y%4 == 0){
        maxDay = 29;
    }
    if (d < 1 || d > maxDay){
        throw std::invalid_argument("Day is incorrect");
    }

    date.day = d;
    date.month = m;
    date.year = y;
    return date;
}

std::string getField(std::stringstream& ss, char separator = '|'){
    std::string s;
    std::getline(ss, s, separator);
    trim(s);
    return s;
}

exchangeRate parseLine(const std::string& s){
    if(s.empty()) throw std::invalid_argument("Line is empty");
    exchangeRate record;
    std::stringstream ss(s);
    std::string dateStr = getField(ss);
    std::string bankName = getField(ss);
    if (bankName.empty() || bankName.size()>50) throw std::invalid_argument("Bank has broken name");
    std::string usdBuy = getField(ss);
    std::string usdSell = getField(ss);
    std::string eurBuy = getField(ss);
    std::string eurSell = getField(ss);
    std::string cnyBuy = getField(ss);
    std::string cnySell = getField(ss);
    std::string jpyBuy = getField(ss);
    std::string jpySell = getField(ss);

    double ub, us, eb, es, cb, cs, jb, js;
    ub = std::stod(usdBuy); us = std::stod(usdSell);
    eb = std::stod(eurBuy); es = std::stod(eurSell);
    cb = std::stod(cnyBuy); cs = std::stod(cnySell);
    jb = std::stod(jpyBuy); js = std::stod(jpySell);
    if (ub <=0 || us <= 0 || eb <= 0 || es <= 0 ||
    cb <= 0 || cs <= 0 || jb <= 0 || js <= 0) {
        throw std::invalid_argument("argument < 0");
    }

    record.date = parseDate(dateStr);
    record.bankName = bankName;
    record.buySellUsd = {ub, us};
    record.buySellEur =  {eb, es};
    record.buySellCny = {cb, cs};
    record.buySellJpy = {jb, js};

    return record;
}

BuySell& getCurrencyRate(exchangeRate& r, currency cur) {
    switch(cur) {
        case USD: return r.buySellUsd;
        case EUR: return r.buySellEur;
        case CNY: return r.buySellCny;
        case JPY: return r.buySellJpy;
    }
    return r.buySellUsd;
}

int main(){

    std::ifstream in("input.txt");
    if (!in.is_open()){
        std::cout<< "Error: Cant open input file \n"; 
        return -1;
    }

    std::string currencyStr;
    do{
        std::cout<<"Enter currency (USD, EUR, CNY, JPY): ";
        std::cin>>currencyStr;
        trim(currencyStr);
        if (validCurrencies.find(currencyStr) == validCurrencies.end()) {
            std::cout << "Unknown currency. Try again: \n";
        } 
    } while (validCurrencies.find(currencyStr) == validCurrencies.end());

    currency cur = USD;
    if (currencyStr == "EUR") cur = EUR;
    if (currencyStr == "CNY") cur = CNY;
    if (currencyStr == "JPY") cur = JPY;


    std::string dateStr;
    std::cin.ignore();
    std::cout<<"Enter date (YYYY.MM.DD): ";
    std::getline(std::cin, dateStr);
    dateTime focusedDay;
    bool valid = false;
    do {
        try{
            focusedDay = parseDate(dateStr);
            valid = true;
        } catch (std::exception& e) {
            std::cout<<"Invalid format: "<<e.what()<<". Try again: ";
            std::getline(std::cin, dateStr);
        }
    } while (!valid);

    //Task 1
    double bestBuy = std::numeric_limits<double>::max();
    std::set<std::string> bestBuyBanks;

    //Task 2
    std::vector<std::string> banksOrder;
    std::unordered_map<std::string, std::pair<int, double>> banksAverage;

    std::string line;
    int linesCount = 0;
    while(getline(in, line)){
        linesCount++;
        try {
            exchangeRate record = parseLine(line);
            BuySell rate = getCurrencyRate(record, cur);
            
            //Task 1
            if(record.date == focusedDay){
                if(rate.buy < bestBuy){
                    bestBuy = rate.buy;
                    bestBuyBanks.clear();
                    bestBuyBanks.insert(record.bankName);
                } else if (rate.buy == bestBuy){
                    bestBuyBanks.insert(record.bankName);
                }
            }

            
            //Task 2
            if (banksAverage.find(record.bankName) == banksAverage.end()){
                banksOrder.push_back(record.bankName);
            }
            banksAverage[record.bankName].first++;
            banksAverage[record.bankName].second += rate.sell;

        } catch (std::exception& e){
            std::cout<< "Line "<<linesCount<<" has invalid data: "<<e.what()<<'\n';
        }
    }
    if (linesCount == 0){
        std::cout<<"Error: input file is empty\n";
        return -1;
    }

    in.close();

    //Task 1 print
    if (bestBuyBanks.empty()){
        std::cout<<"1) No data for "<<currencyStr<< " in "<< dateStr<<'\n';
    } else {
    std::cout<<"1) Best banks to buy: "<<currencyStr<<" on "<<dateStr<<": ";
    for (auto& el : bestBuyBanks) std::cout<<el<<", ";
    std::cout<<'\n';
    }

    //Task 2 calculate and print
    std::string bankName;
    double bestAverageSell = std::numeric_limits<double>::min();
    for (auto& name : banksOrder) {
        auto& el = banksAverage[name];
        if (el.second / el.first > bestAverageSell) {
            bestAverageSell = el.second / el.first;
            bankName = name;
        }
    }
    std::cout<<"2) Best bank by average sell of "<<currencyStr<<": "<<bankName<<'\n';

    std::string outName;
    bool validOutName = true;
    do{
        if (!validOutName){
            std::cout<<"Invalid format for output file. Enter another: ";
        }
    validOutName = true;
    std::cout<<"Enter output file name: ";
    std::cin>>outName;
    for (char c : outName) if (!isValidSymbol(c)) validOutName = false;
    } while(outName.size() > 50 || !validOutName);

    std::ofstream out;
    out.open(outName);
    if (out.is_open()) {
    for(auto& el : bestBuyBanks) {out<<el<<", ";}
    if (!bestBuyBanks.empty()) out<<'\n';
    out<<bankName;
    } else {
        std::cout<<"Error: cant open output file\n";
        return -1;
    }
    out.close();
    std::cout<<"File has been written";

    return 0;
}