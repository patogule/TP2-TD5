#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <set>
#include <random>

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


// HEURISTICAS CONSTRUCTIVAS
int heuristica_vmc(int depositos, int vendedores, vector<vector<int> >& sol_inicial_h_vmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades){
    auto start = chrono::steady_clock::now();

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
            }
            dist_total = dist_total + (3 * max);
            cout << "Se agrego una penalizacion de" << 3 * max << " por el vendedor " << a << " que no fue asignado." << endl;
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

    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;

    return dist_total;
}

int heuristica_dmc(int depositos, int vendedores, vector<vector<int> >& sol_inicial_h_dmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades ){
    auto start = chrono::steady_clock::now();

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
    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;

    return dist_total;
}


// BUSQUEDA LOCAL
int relocate(int dist_original, vector<vector<int> > solucion_inicial, int depositos, vector<int> capacidades, vector<vector<int> > demandas, vector<vector<int> > distancias) {
    auto start = chrono::steady_clock::now();

    // calculamos la capacidad disponible de cada deposito en la solucion original
    vector<int> capacidad_disponible(depositos);
    for (size_t i = 0; i < solucion_inicial.size(); ++i) {
        capacidad_disponible[i] = capacidades[i];
        for (int pp = 0; pp < solucion_inicial[i].size(); pp++) {
            capacidad_disponible[i] = capacidad_disponible[i] - demandas[i][solucion_inicial[i][pp]];
        }
    }

    // distancia total original
    int dist_total = dist_original;
    int dist_parcial_aux = dist_total;

    vector<vector<int> > solucion_parcial = solucion_inicial;

    // iteramos por los depositos
    for (int z = 0; z < solucion_inicial.size(); z++){
        // iteramos por los vendedores
        for(int k = 0; k < solucion_inicial[z].size(); k++){
            // cada vendedor lo cambiamos de dposito
            for(int q = 0; q < solucion_inicial.size(); q++){
                // calcular la distancia total de la nueva solucion
                if (q != z){
                    if( capacidad_disponible[q] >= demandas[q][solucion_inicial[z][k]]){
                        int dist_parcial = dist_total - distancias[z][solucion_inicial[z][k]] + distancias[q][solucion_inicial[z][k]];
                        if (dist_parcial < dist_parcial_aux){
                            dist_parcial_aux = dist_parcial;
                            solucion_parcial = solucion_inicial;
                            // escribimos la nueva solucion parcial en nuestro vector de solucion parcial
                            solucion_parcial[q].push_back(solucion_parcial[z][k]);
                            solucion_parcial[z][k] = -1;
                        }
                    }
                }
            }
        }
    }

    // escribir en el txt
    // nos creamos el archivo de salida
    string solucion_rel = "solucion_busqueda_local_relocate";
    ofstream output_file_rel(solucion_rel);

    if (output_file_rel.is_open()) {
        for (int u = 0; u < solucion_parcial.size(); u++){
            for (int y = 0; y < solucion_parcial[u].size(); y++){
                if (solucion_parcial[u][y] != -1){
                    output_file_rel << solucion_parcial[u][y] << " ";
                }
            }
            output_file_rel << "\n"; // Nueva línea después de cada vector interno
        }
        output_file_rel.close();
    } else {
        std::cout << "No se pudo abrir el archivo." << std::endl;
    }
    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;

    return dist_parcial_aux;
    
}

int swap(int dist_original, vector<vector<int> >& sol_swap, vector<vector<int> > solucion_inicial, int depositos, vector<int> capacidades, vector<vector<int> > demandas, vector<vector<int> > distancias) {
    auto start = chrono::steady_clock::now();

    // nos creamos el archivo de salida
    string solucion_rel = "solucion_busqueda_local_relocate";
    ofstream output_file_rel(solucion_rel);

    // calculamos la capacidad disponible de cada deposito en la solucion original
    vector<int> capacidad_disponible(depositos);
    for (size_t i = 0; i < solucion_inicial.size(); ++i) {
        capacidad_disponible[i] = capacidades[i];
        for (int pp = 0; pp < solucion_inicial[i].size(); pp++) {
            capacidad_disponible[i] = capacidad_disponible[i] - demandas[i][solucion_inicial[i][pp]];
        }
    }

    // distancia total original
    int dist_total = dist_original;
    int dist_parcial_aux = dist_total;

    vector<vector<int> > solucion_parcial = solucion_inicial;

    // recorremos cada deposito
    for (int w = 0; w < solucion_inicial.size(); w++){
        // recorremos cada vendedor
        for (int t = 0; t < solucion_inicial[w].size(); t++){
            // para cada vendedor queremos iterar por los depositos
            for(int h = 0; h < solucion_inicial.size(); h++){
                // hacemos una especie de poda para no repetir casos
                if (h > w){
                    // iteramos por todos los vendedores de los depositos distintos a w
                    for(int g = 0; g < solucion_inicial[h].size(); g++){
                        // verificamos que haya espacio para el swap en ambos depositos
                        if((capacidad_disponible[w] + demandas[w][solucion_inicial[w][t]] >= demandas[h][solucion_inicial[h][g]]) && (capacidad_disponible[h] + demandas[h][solucion_inicial[h][g]] >= demandas[w][solucion_inicial[w][t]])){
                            // ver si esta solucion factible es una mejor solucion que la original
                            int dist_parcial = dist_total - distancias[w][solucion_inicial[w][t]] - distancias[h][solucion_inicial[h][g]] + distancias[w][solucion_inicial[h][g]] + distancias[h][solucion_inicial[w][t]];
                            if(dist_parcial < dist_parcial_aux){
                                dist_parcial_aux = dist_parcial;
                                solucion_parcial = solucion_inicial;
                                // escribimos la nueva solucion parcial en nuestro vector de solucion parcial
                                solucion_parcial[w].push_back(solucion_parcial[h][g]);
                                solucion_parcial[h][g] = -1;
                                solucion_parcial[h].push_back(solucion_parcial[w][t]);
                                solucion_parcial[w][t] = -1;
                            }
                        }
                    }
                }
            }
        }
    }
    // escribir en el txt
    // nos creamos el archivo de salida
    string solucion_swap = "solucion_busqueda_local_swap";
    ofstream output_file_swap(solucion_swap);

    if (output_file_swap.is_open()) {
        for (int u = 0; u < solucion_parcial.size(); u++){
            for (int y = 0; y < solucion_parcial[u].size(); y++){
                if (solucion_parcial[u][y] != -1){
                    output_file_swap << solucion_parcial[u][y] << " ";
                }
            }
            output_file_swap << "\n"; // Nueva línea después de cada vector interno
        }
        output_file_swap.close();
    } else {
        std::cout << "No se pudo abrir el archivo." << std::endl;
    }

    sol_swap = solucion_parcial;

    auto end = chrono::steady_clock::now();
    chrono::duration<double, std::milli> tiempo_transcurrido = end - start;

    return dist_parcial_aux;
}


// METAHEURISTICA

vector<vector<int> > greedy_randomized(int& dist_total_greedy, int depositos, int vendedores, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades, int rcl_size){

    // me creo el vector solucion
    vector<vector<int> > vec_sol(depositos);

    // me creo mi vector de capacidades restantes
    vector<int> capacidades_restantes = capacidades;

    // me creo una variable para guardar la distancia total recorrida
    int dist_total = 0;
    
    // iterar por cada vendedor
    for(int a = 0; a < vendedores; a++){
        // itero por cada deposito
        int min = 99999999;
        // buscamos la cantidad de depositos mas cercanos que nos pide rcl_size
        vector<int> depositos_mas_cercanos;
        set<int> depositos_mas_cercanos_set;
        vector<int> vector_min;
        for (int q = 0; q < rcl_size; q++){
            int min = 99999999;
            int indice;
            for(int b = 0; b < depositos; b++){
                if (distancias[b][a] < min && (capacidades_restantes[b] >= demandas[b][a]) && (depositos_mas_cercanos_set.count(b) == 0)){
                    min = distancias[b][a];
                    indice = b;
                }
            }
            vector_min.push_back(distancias[indice][a]);
            depositos_mas_cercanos.push_back(indice);
            depositos_mas_cercanos_set.insert(indice);
        }  

        // elegir un deposito random dentro de los rcl_zise depositos mas cercanos
        random_device rd;
        mt19937 generador(rd());
        uniform_int_distribution<> distribucion(0, depositos_mas_cercanos.size() - 1);
        int indiceAleatorio = distribucion(generador);
        int deposito_elegido = depositos_mas_cercanos[indiceAleatorio];
        min = vector_min[indiceAleatorio];

        // escribo la asignacion en el archivo solucion
        if(depositos_mas_cercanos.size() != 0){
            capacidades_restantes[deposito_elegido] = capacidades_restantes[deposito_elegido] - demandas[deposito_elegido][a];
            dist_total = dist_total + min;
            vec_sol[deposito_elegido].push_back(a);
        }

        else{
            // busco la distancia mas grande del vendedor a que no pudo ser asignado
            int max = 0;
            for(int c = 0; c < depositos; c++){
                if (distancias[c][a] > max){
                    max = distancias[c][a];
                }
            }
            dist_total = dist_total + (3 * max);
            cout << "Se agrego una penalizacion de" << 3 * max << " por el vendedor " << a << " que no fue asignado." << endl;
        }
    }
    dist_total_greedy = dist_total;
    return vec_sol;
}

int grasp(int depositos, int vendedores, vector<vector<int> >& sol_grasp, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades, int n_iters, int rcl_size){
    // inicializo el vector solucion y una distancia recorrida best en infinito
    vector<vector<int> > vector_best(depositos);
    int best = 999999999;

    // hago n_iters iteraciones
    for(int r = 0; r < n_iters; r++){
        // calculo un greedy randomizado
        int dist_total_greedy;
        vector<vector<int> > vector_current = greedy_randomized(dist_total_greedy, depositos, vendedores, distancias, demandas, capacidades, rcl_size);
        // aplico la busqueda local swap a nuestro greedy randomizado
        vector<vector<int> > sol_swap(depositos);
        int dist_total = swap(dist_total_greedy, sol_swap, vector_current, depositos, capacidades, demandas, distancias);

        // si esta nueva distancia total es menor a la best global esta va a ser mi nueva distancia best global
        //cout << "dist total best: " << best << endl << "dist de esta ronda: "  << dist_total << endl << endl;
        if (dist_total < best){
            best = dist_total;
            vector_best = sol_swap;
        }
    }
    sol_grasp = vector_best;
    return best;
}

int main(int argc, char** argv) {
    std::string filenamee = "instances/real/real_instance";
    std::cout << "Reading file " << filenamee << std::endl;

    string soluciones = "soluciones";
    ofstream output_file(soluciones);

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
    cout << "Corriendo busqueda local relocate de la heuristica vecino mas cercano..." << endl;
    int resultado_relocate_vmc = relocate(resultado_h_vmc, sol_inicial_h_vmc, depositos, capacidades, demandas, distancias);
    cout << "Busqueda local relocate recorre una distancia total de: " << resultado_relocate_vmc << endl;
    cout << "La distancia decrecio en " << resultado_h_vmc - resultado_relocate_vmc << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo busqueda local relocate de deposito mas cercano..." << endl;
    int resultado_relocate_dmc = relocate(resultado_h_dmc, sol_inicial_h_dmc, depositos, capacidades, demandas, distancias);
    cout << "Busqueda local relocate recorre una distancia total de: " << resultado_relocate_dmc << endl;
    cout << "La distancia decrecio en " << resultado_h_dmc - resultado_relocate_dmc << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo busqueda local swap de la heuristica vecino mas cercano..." << endl;
    vector<vector<int> > sol_swap_1(depositos);
    int resultado_swap_vmc = swap(resultado_h_vmc, sol_swap_1, sol_inicial_h_vmc, depositos, capacidades, demandas, distancias);
    cout << "Busqueda local swap recorre una distancia total de: " << resultado_swap_vmc << endl;
    cout << "La distancia decrecio en " << resultado_h_vmc - resultado_swap_vmc << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo busqueda local swap de la heuristica deposito mas cercano..." << endl;
    vector<vector<int> > sol_swap_2(depositos);
    int resultado_swap_dmc = swap(resultado_h_dmc, sol_swap_2, sol_inicial_h_dmc, depositos, capacidades, demandas, distancias);
    cout << "Busqueda local swap recorre una distancia total de: " << resultado_swap_dmc << endl;
    cout << "La distancia decrecio en " << resultado_h_dmc - resultado_swap_dmc << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    cout << "//////////////////////////////////////////////////" << endl;
    cout << "Corriendo metaheuristica grasp..." << endl;
    vector<vector<int> > sol_grasp(depositos);
    int resultado_grasp = grasp(depositos, vendedores, sol_grasp, distancias, demandas, capacidades, 30, 7);
    cout << "La metaheuristica grasp recorre una distancia total de: " << resultado_grasp << endl;
    cout << "La distancia decrecio en " << resultado_h_vmc - resultado_grasp << " de la heuristica del vendedor mas cercano original" << endl;
    cout << "//////////////////////////////////////////////////" << endl << endl;

    return 0;
}