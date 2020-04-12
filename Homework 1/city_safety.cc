// Copyright 2020 <Aashir Shukla>
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <chrono> // NOLINT (build/c++11)
#include <string>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::ofstream;
using std::ifstream;

struct data {
    int inhabitants;
    int cases;
    int recoveries;
    int safety;
};

class measure_time {
 private:
    double elapsed_us;
    std::chrono::high_resolution_clock::time_point start;

 public:
    measure_time() {
        start = std::chrono::high_resolution_clock::now();
        elapsed_us = 0.0;
    }
    void Reset() {
        start = std::chrono::high_resolution_clock::now();
    }
    void end() {
        auto end = std::chrono::high_resolution_clock::now();
        elapsed_us =
        std::chrono::duration<double, std::micro>(end - start).count();
    }
    double current_time() {
        return elapsed_us;
    }
};

vector<data> city;

bool cmp(data A, data B) {
    return A.safety < B.safety;
}

int Safety(double a, double b, double c) {
    double z = 2.0 * (1.0 - b/a);
    double d = 0.5 * c/b;
    return 10000.0 * (z + d);
}

int linear_search(int x) {
    int cnt = 0;
    for (data i : city) {
        if (i.safety == x) 
            return 1; 
    }
    return cnt;
}

int low_binary_search(int x) {
    int ans = -1, low = 0, high = city.size()-1;
    while (low <= high) {
        int mid = (low+high) >> 1;
        if (city[mid].safety < x) {
            low = mid + 1;
        } else if (city[mid].safety > x) {
            high = mid-1;
        } else {
            ans = mid;
            high = mid-1;
        }
    }

    return ans;
}

int high_binary_search(int x) {
    int ans = -1, low = 0, high = city.size()-1;
    while (low <= high) {
        int mid = (low+high) >> 1;
        if (city[mid].safety < x) {
            low = mid + 1;
        } else if (city[mid].safety > x) {
            high = mid-1;
        } else {
            ans = mid;
            low = mid+1;
        }
    }

    return ans;
}

int main(int argc, char *argv[]) {
    measure_time tm;

    if (argc != 4) {
        cerr <<
            "Usage: " << argv[0] <<
            " <city_file.dat> <safety_file.dat> <result_file.dat>" << endl;
        return 0;
    }

    ifstream cities;
    ifstream safety_file;
    ofstream output_file;

    int arr[4];

    cities.open(argv[1]);
    safety_file.open(argv[2]);

    if (!cities.is_open()) {
        cerr << "Error: cannot open file " << argv[1] << endl;
        return 0;
    }

    if (!safety_file.is_open()) {
        cerr << "Error: cannot open file " << argv[2] << endl;
        return 0;
    }

    vector<int> safety;

    while (cities >> arr[0] >> arr[1] >> arr[2]) {
        data tmp;

        tmp.inhabitants = arr[0];
        tmp.cases = arr[1];
        tmp.recoveries = arr[2];
        tmp.safety = Safety(arr[0], arr[1], arr[2]);

        city.push_back(tmp);
    }


    while (safety_file >> arr[0]) {
        safety.push_back(arr[0]);
    }

    cout << "Choice of search method ([l]inear, [b]inary)?" << endl;
    char c;

    int ans = 0;

    while (cin >> c) {
        if(c == 'l' || c == 'b')
            break;
        cout << "Incorrect choice\n";
    }

    switch (c) {
        case 'l':
            for (int i : safety) {
                ans += linear_search(i);
            }
            break;

        case 'b':
            sort(city.begin(), city.end(), cmp);

            for (int i : safety) {
                int A = high_binary_search(i);
                if (A != -1)
                    ans++;
            }
            break;
    }
    
    tm.end();

    output_file.open(argv[3]);
    output_file << ans << endl;
    cout << "CPU time: " << tm.current_time() << " microseconds";

    return 0;
}
