#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

// FUNCIONES AUXILIARES
vector<vector<int>> readDataFromFile(const string& filename) {
    vector<vector<int>> data;
    ifstream input_file(filename);
    string line;
    
    if (!input_file.is_open()) {
        cout << "Could not open the file - '" << filename << "'" << endl;
        return data;
    }
    
    while (getline(input_file, line)) {
        vector<int> line_data;
        std::istringstream line_stream(line);
        int number;
        
        while (line_stream >> number) {
            line_data.push_back(number);
        }
        
        data.push_back(line_data);
    }
    
    input_file.close();
    return data;
}

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

int distancia_total(vector<vector<int> > vec, vector<vector<int> > distancias){
    int resultado = 0;
    for (int l = 0; l < vec.size(); l++){
        for (int m = 0; m < vec[l].size(); m++){
            resultado = resultado + distancias[l][vec[l][m]];
        }
    }
    return resultado;
} 

// HEURISTICAS CONSTRUCTIVAS
int heuristica_vmc(int depositos, int vendedores, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades){

    // me creo el archivo de salida
    string solucion = "solucion_heuristica_vmc";
    ofstream output_file(solucion);

    // me creo mi vector de capacidades restantes
    vector<int> capacidades_restantes = capacidades;

    // me creo una variable para guardar la distancia total recorrida
    int dist_total = 0;
    
    // iterar por cada vendedor
    for(int a = 0; a < vendedores; a++){
        // itero por cada deposito
        int min = 99999999;
        int deposito_mas_cerca;
        for(int b = 0; b < depositos; b++){
            if (distancias[b][a] < min && (capacidades_restantes[b] >= demandas[b][a])){
                min = distancias[b][a];
                deposito_mas_cerca = b;
            }
        }
        // escribo la asignacion en el archivo solucion
        if(min != 99999999){
            capacidades_restantes[deposito_mas_cerca] = capacidades_restantes[deposito_mas_cerca] - demandas[deposito_mas_cerca][a];
            dist_total = dist_total + min;
            for (int i = 0; i < deposito_mas_cerca; i++) {
            if (i == deposito_mas_cerca) {
                output_file << a;
            } else {
                output_file << std::endl;
            }
            }
        }
        else{
            // busco la distancia mas grande del vendedor a que no pudo ser asignado
            int max = 0;
            for(int c = 0; c < depositos; c++){
                if (distancias[c][a] > max){
                    max = distancias[c][a];
            }
            dist_total = dist_total + (3 * max);
            }
        }
    
    }
    output_file.close();
    std::cout << "File created and written successfully - '" << solucion << "'" << std::endl;
    return dist_total;
}

void heuristica_dmc(int depositos, int vendedores, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades ){
   
}


// BUSQUEDA LOCAL
int relocate(string filenamee, vector<int> capacidades, vector<vector<int> > demandas, vector<vector<int> > distancias) {
    // nos creamos el archivo de salida
    string solucion_rel = "solucion_busqueda_local_relocate";
    ofstream output_file_rel(solucion_rel);

    // leemos el archivo de entrada
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
        output_file_rel << line << endl;
        lines.push_back(line);
    }

    // calculamos la capacidad disponible de cada deposito en la solucion original
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
    

    // convertimos nuestras lines de string a int
    vector<vector<int> > lines_int;
    for (size_t j = 0; j < lines.size(); ++j){
        vector<std::string> depo;
        char delim = ' ';
        depo = split(line, delim);
        vector<int> depo_int;
        for (string numerito : depo){
            depo_int.push_back(stod(numerito));
        }
        lines_int.push_back(depo_int);
    }

    // distancia total original
    int dist_total = distancia_total(lines_int, distancias);

    // iteramos por los depositos
    for (int z = 0; z < lines_int.size(); z++){
        // iteramos por los vendedores
        for(int k = 0; k < lines_int[z].size(); k++){
            // cada vendedor lo cambiamos de dposito
            for(int q = 0; k < lines_int.size(); q++){
                // calcular la distancia total de la nueva solucion
                if (q != z){
                    if( capacidad_disponible[q] >= demandas[z][lines_int[z][k]]){
                        int dist_parcial = dist_total - distancias[z][lines_int[z][k]] + distancias[q][lines_int[z][k]];
                        if (dist_parcial < dist_total){
                            dist_total = dist_parcial;
                            // escribimos la nueva solucion parcial en nuestro archivo salida
                            lines_int[q].push_back(lines_int[z][k]);
                            lines_int[z].erase(k);

                            input_file.seekp(0); // Vuelve al inicio del archivo
                            input_file.truncate(0); // Borra el contenido existente



                            

                        }
                    }

                }
            }
        }

    }

    lines_int[0] = 2 9 8 5  
    lines_int[1] = 4 1
    lines_int[2] = 6 7
    

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

    heuristica_vmc(depositos, vendedores, distancias, demandas, capacidades);

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