#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

// definimos la funcion split
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    
    while (getline(tokenStream, token, delimiter)) {
        cout << "mmmmmm" << endl;
        tokens.push_back(token);
    }

    return tokens;
}

int heuristica_vmc(int m){

    // iterar por cada vecino
    //for(int a, a < )
}


int relocate(string filenamee, vector<int> capacidades, vector<vector<int> > demandas) {
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
                sum += demandas[i][stoi(number_str)];
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
    std::string filenamee = "instances/gap/gap_a/a05100";
    std::cout << "Reading file " << filenamee << std::endl;

    // leemos el archivo
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
        lines.push_back(line);
    }

    input_file >> line;

    char delimiter = ' ';
    vector<string> linea_0 = split(line, delimiter);

    cout << "1" << endl;

    //definimos nuestras variables
    cout << linea_0[0] << endl;
    cout << typeid(linea_0[0]).name() << endl;

    int depositos = stoi(linea_0[0]);
    int vendedores = stoi(linea_0[1]);

    vector<vector<int> >distancias(depositos, vector<int>(vendedores, 0));
    cout << "2" << endl;

    int deposito = 0;
    int vendedor = 0;
    while(deposito < depositos){
        string line;
        input_file >> line;
        cout << line << endl;
        vector<std::string> numeritos;
        char delim = ' ';
        numeritos = split(line, delim);
        for (string numerito : numeritos){
        distancias[deposito][vendedor] = stod(numerito);
        vendedor++;
        }
        if (vendedor >= vendedores){
            vendedor = 0;
            deposito++;
        }
    }
    cout << "3" << endl;

    vector<vector<int> >demandas(depositos, vector<int>(vendedores, 0));

    int deposito_2 = 0;
    int vendedor_2 = 0;
    while(deposito_2 < depositos){
        string line;
        input_file >> line;
        vector<std::string> numeritos;
        char delim = ' ';
        numeritos = split(line, delim);
        for (string numerito : numeritos){
        demandas[deposito_2][vendedor_2] = stod(numerito);
        vendedor_2++;
        }
        if (vendedor_2 >= vendedores){
            vendedor_2 = 0;
            deposito++;
        }
    }

    vector<int> capacidades (depositos, 0);

    int deposito_3 = 0;
    while (deposito_3 < depositos){
        string line;
        input_file >> line;
        vector<std::string> numeritos;
        char delim = ' ';
        numeritos = split(line, delim);
        for (string numerito : numeritos){
            capacidades[deposito_3] = stoi(numerito);
        }
        deposito_3++;
    }

    vector<int> cap;
    cap.push_back(30);
    cap.push_back(7);
    cap.push_back(20);

    vector<int> incv;
    incv.push_back(1);
    incv.push_back(2);
    incv.push_back(3);
    incv.push_back(4);
    incv.push_back(5);
    incv.push_back(6);
    incv.push_back(7);
    incv.push_back(8);
    incv.push_back(9);

    vector<vector<int> > demanda_prueba;
    demanda_prueba.push_back(incv);

    relocate("resultados.txt", cap, demanda_prueba);

    return 0;
}