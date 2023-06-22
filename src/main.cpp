#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

// FUNCIONES AUXILIARES
vector<vector<int> > readDataFromFile(const string& filename) {
    vector<vector<int> > data;
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
int heuristica_vmc(int depositos, int vendedores, vector<vector<int> >& sol_inicial_h_vmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades){

    // me creo el archivo de salida
    string solucion = "solucion_heuristica_vmc";
    ofstream output_file(solucion);

    // me creo el vector solucion
    vector<vector<int> > vec_sol(depositos, vector<int>(vendedores, -1));

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
            vec_sol[deposito_mas_cerca].push_back(a);
        }
        else{
            // busco la distancia mas grande del vendedor a que no pudo ser asignado
            int max = 0;
            for(int c = 0; c < depositos; c++){
                if (distancias[c][a] > max){
                    max = distancias[c][a];
            }
            dist_total = dist_total + (3 * max);
            cout << "Se agrego una penalizacion de" << 3 * max << " por el vendedor " << a << " que no fue asignado." << endl;
            }
        }
    
    }
    // escribo las asignaciones en el archivo de salida
    for (int i = 0; i < vec_sol.size(); i++) {
        for(int s = 0; s < vec_sol[i].size(); s++){
            if (vec_sol[i][s] != -1){
                sol_inicial_h_vmc[i].push_back(vec_sol[i][s]);
                output_file << vec_sol[i][s] << " ";
            }
        }
        output_file << endl;
    }

    output_file.close();
    return dist_total;
}

int heuristica_dmc(int depositos, int vendedores, vector<vector<int> > sol_inicial_h_dmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades ){

    vector<int> capacidades_restantes = capacidades;

    // me creo vectores solucion 
    vector<vector<int> > rta(depositos, std::vector<int>());

    // me creo una variable para guardar la distancia total recorrida
    int dist_total = 0;

    // me creo una lista de booleanos para representar que vendedores estan asignados a un deposito
    vector<bool> asignado(vendedores, false);

    // iterar por cada deposito
    for(int d = 0; d < depositos; d++){
        // itero por cada vendedor 
        int vendedor_mas_cerca;
        bool condicion =  true;
        //mientras ese deposito tenga espacio y pueda asignar el vendedor mas cercano, sigo iterando
        while (condicion){
            int min = 99999999;
            for(int e = 0; e < vendedores; e++){
                // busco el vendedor mas cercano al deposito
                if ((distancias[d][e] < min) && (capacidades_restantes[d] >= demandas[d][e]) && (asignado[e] == false)){
                    min = distancias[d][e];
                    vendedor_mas_cerca = e;
                }
            }
            // escribo la asignacion en el archivo solucion
            if(min != 99999999){
                //le resto a la capacidad del deposito la demanda del vendedor asignado
                capacidades_restantes[d] = capacidades_restantes[d] - demandas[d][vendedor_mas_cerca];
                // marco este vendedor como ya asignado
                asignado[vendedor_mas_cerca] = true;
                dist_total = dist_total + min;
                // agrego la asignacion al vector respuesta
                rta[d].push_back(vendedor_mas_cerca);
                sol_inicial_h_dmc[d].push_back(vendedor_mas_cerca);
            }
            else{
                condicion = false;
            }
        }
    }
    for(int x = 0; x < asignado.size(); x++){
        if (asignado[x] == false){
            // busco la distancia mas grande del vendedor a que no pudo ser asignado
            int max = 0;
            for(int p = 0; p < depositos; p++){
                if (distancias[p][x] > max){
                    max = distancias[p][x];
                }
            }

            dist_total = dist_total + (3 * max);
            cout << "Se agrego una penalizacion de" << 3 * max << " por el vendedor " << x << " que no fue asignado." << endl;
        }
    }
    // escribir en el txt
    // me creo el archivo de salida
    string solucion = "solucion_heuristica_deposito_mc";
    ofstream output_file(solucion);

    if (output_file.is_open()) {
        for (const auto& vector : rta) {
            for (int elemento : vector) {
                output_file << elemento << " ";
            }
            output_file << "\n"; // Nueva línea después de cada vector interno
        }
        output_file.close();
    } else {
        std::cout << "No se pudo abrir el archivo." << std::endl;
    }
    return dist_total;
}


// BUSQUEDA LOCAL
int relocate(vector<vector<int> > solucion_inicial, int depositos, vector<int> capacidades, vector<vector<int> > demandas, vector<vector<int> > distancias) {

    // calculamos la capacidad disponible de cada deposito en la solucion original
    vector<int> capacidad_disponible(depositos);
    for (size_t i = 0; i < solucion_inicial.size(); ++i) {
        capacidad_disponible[i] = capacidades[i];
        for (int pp = 0; pp < solucion_inicial[i].size(); pp++) {
            capacidad_disponible[i] = capacidad_disponible[i] - demandas[i][solucion_inicial[i][pp]];
        }
    }

    // distancia total original
    int dist_total = distancia_total(solucion_inicial, distancias);

    // iteramos por los depositos
    for (int z = 0; z < solucion_inicial.size(); z++){
        // iteramos por los vendedores
        for(int k = 0; k < solucion_inicial[z].size(); k++){
            // cada vendedor lo cambiamos de dposito
            for(int q = 0; q < solucion_inicial.size(); q++){
                // calcular la distancia total de la nueva solucion
                if (q != z){
                    if( capacidad_disponible[q] >= demandas[z][solucion_inicial[z][k]]){
                        int dist_parcial = dist_total - distancias[z][solucion_inicial[z][k]] + distancias[q][solucion_inicial[q][k]];
                        if (dist_parcial < dist_total){
                            dist_total = dist_parcial;
                            // escribimos la nueva solucion parcial en nuestro archivo salida
                            solucion_inicial[q].push_back(solucion_inicial[z][k]);
                            solucion_inicial[z][k] = -1;
                        }
                    }

                }
            }
        }

    }
    cout << "8" << endl;

    // escribir en el txt
    // nos creamos el archivo de salida
    string solucion_rel = "solucion_busqueda_local_relocate";
    ofstream output_file_rel(solucion_rel);

    if (output_file_rel.is_open()) {
        for (int u = 0; u < solucion_inicial.size(); u++){
            for (int y = 0; y < solucion_inicial[u].size(); y++){
                if (solucion_inicial[u][y] != -1){
                    output_file_rel << solucion_inicial[u][y] << " ";
                }
            }
            output_file_rel << "\n"; // Nueva línea después de cada vector interno
        }
        output_file_rel.close();
        std::cout << "Vector de vectores de enteros escrito en el archivo correctamente." << std::endl;
    } else {
        std::cout << "No se pudo abrir el archivo." << std::endl;
    }
    return dist_total;
    
}
/*

int swap(string filenamee, vector<int> capacidades, vector<vector<int> > demandas, vector<vector<int> > distancias) {
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

    // recorremos cada deposito
    for (int w = 0; w < lines_int.size(); w++){
        // recorremos cada vendedor
        for (int t = 0; t < lines_int[w].size(); t++){
            // para cada vendedor queremos iterar por los depositos
            for(int h = 0; h < lines_int.size(); h++){
                // hacemos una especie de poda para no repetir casos
                if (h > w){
                    // iteramos por todos los vendedores de los depositos distintos a w
                    for(int g = 0; g > lines_int[h].size(); g++){
                        // verificamos que haya espacio para el swap en ambos depositos
                        if((capacidad_disponible[w] + demandas[w][lines_int[w][t]] >= demandas[h][lines_int[h][g]]) && (capacidad_disponible[h] + demandas[h][lines_int[h][g]] >= demandas[w][lines_int[w][t]])){
                            // ver si esta solucion factible es una mejor solucion que la original
                            if(dist_total - distancias[w][lines_int[w][t]] - distancias[h][lines_int[h][g]] + distancias[w][lines_int[h][g]] + distancias[h][lines_int[w][t]] > dist_total){

                            }
                        }
                    }
                }
            }
        }
    }
}
*/


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

    //definimos nuestras variables
    int depositos;
    int vendedores;

    input_file >> depositos >> vendedores;

    vector<vector<int> > distancias(depositos, vector<int>(vendedores, 0));

    for(int deposito = 0; deposito < depositos; deposito++){
        for (int vendedor = 0; vendedor < vendedores; vendedor++){
        input_file >> line;
        distancias[deposito][vendedor] = stoi(line);
        }
    }

    vector<vector<int> >demandas(depositos, vector<int>(vendedores, 0));

    for(int deposito = 0; deposito < depositos; deposito++){
        for (int vendedor = 0; vendedor < vendedores; vendedor++){
        input_file >> line;
        demandas[deposito][vendedor] = stoi(line);
        }
    }

    vector<int> capacidades (depositos, 0);

    for(int deposito = 0; deposito < depositos; deposito++){
        string line;
        input_file >> line;
        capacidades[deposito] = stoi(line);
    }
    cout <<  endl << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo heurisstica del vecino mas cercano..." << endl;
    vector<vector<int> > sol_inicial_h_vmc(depositos);
    int resultado_h_vmc = heuristica_vmc(depositos, vendedores, sol_inicial_h_vmc, distancias, demandas, capacidades);
    cout << "La heurisstica del vecino mas cercano recorre una distancia total de: " << resultado_h_vmc << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo heurisstica del depocito mas cercano..." << endl;
    vector<vector<int> > sol_inicial_h_dmc(depositos);
    int resultado_h_dmc = heuristica_dmc(depositos, vendedores, sol_inicial_h_dmc, distancias, demandas, capacidades);
    cout << "La heurisstica del depocito mas cercano recorre una distancia total de: " << resultado_h_dmc << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo busqueda local relocate..." << endl;
    int resultado_relocate = relocate(sol_inicial_h_vmc, depositos, capacidades, demandas, distancias);
    cout << "Busqueda local relocate recorre una distancia total de: " << resultado_relocate << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;


    return 0;
}