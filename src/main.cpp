#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;



int heuristica_vmc(int m){

    // iterar por cada vecino
    //for(int a, a < )
}


int relocate(string filenamee, vector<int> capacidades) {
    string filename(filenamee);
    vector<string> lines;
    string line;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cout << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    while (getline(input_file, line)) {
        cout << line;
        lines.push_back(line);
    }

    vector<int> capacidad_disponible;
    for (size_t i = 0; i < lines.size(); ++i) {
    const string& line = lines[i];
    int sum = 0;
    string number_str;
    for (char c : line) {
        if (isdigit(c)) {
            number_str += c;
        } else if (!number_str.empty()) {
            sum += stoi(number_str);
            number_str.clear();
        }
    }
    if (!number_str.empty()) {
        sum += stoi(number_str);
    }
    capacidad_disponible[i] = capacidades[i] - sum;
    cout << "Sum of numbers in line " << i << ": " << sum << "capacidad disponible: " << capacidad_disponible[i] << endl;
    }


    input_file.close();
    return EXIT_SUCCESS;
}


int main(int argc, char** argv) {
    std::string filename = "instances/gap/gap_a/a05100";
    std::cout << "Reading file " << filename << std::endl;

    vector<int> cap = {30,7,20};
    relocate("resultados.txt", cap);

    return 0;
}