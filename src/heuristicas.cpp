#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>
#include <set>
#include <random>

using namespace std;

// HEURISTICAS CONSTRUCTIVAS
int heuristica_vmc(double& tiempo, int depositos, int vendedores, vector<vector<int> >& sol_inicial_h_vmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades){
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
    tiempo = tiempo_transcurrido.count();

    return dist_total;
}

int heuristica_dmc(double& tiempo, int depositos, int vendedores, vector<vector<int> >& sol_inicial_h_dmc, vector<vector<int> > distancias,vector<vector<int> > demandas,vector<int> capacidades ){
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
    tiempo = tiempo_transcurrido.count();

    return dist_total;
}