#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cmath>

std::vector<std::vector<int>> lerMatriz(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return {};
    }
    int rows, cols;
    arquivo >> rows >> cols;
    std::vector<std::vector<int>> matriz(rows, std::vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arquivo >> matriz[i][j];
        }
    }
    arquivo.close();
    return matriz;
}

void calcular_parte(int id, const std::vector<std::vector<int>>& m1, const std::vector<std::vector<int>>& m2, int P, int inicio_idx) {
    
    auto inicio_thread = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<std::string, int>> resultado_parcial;

    int m1_cols = m1[0].size();
    int res_rows = m1.size();
    int res_cols = m2[0].size();
    
    for (int k = 0; k < P; ++k) {
        int idx_global = inicio_idx + k;
        
        if (idx_global >= res_rows * res_cols) {
            break;
        }

        int i = idx_global / res_cols;
        int j = idx_global % res_cols;

        int valor = 0;
        for (int m = 0; m < m1_cols; ++m) {
            valor += m1[i][m] * m2[m][j];
        }
        
        std::string coordenada = "c" + std::to_string(i + 1) + std::to_string(j + 1);
        resultado_parcial.push_back({coordenada, valor});
    }

    auto fim_thread = std::chrono::high_resolution_clock::now();
    long long duracao_thread = std::chrono::duration_cast<std::chrono::microseconds>(fim_thread - inicio_thread).count();

    std::string nomeArquivo = "resultado_thread_" + std::to_string(id) + ".txt";
    std::ofstream arquivo(nomeArquivo);

    arquivo << res_rows << " " << res_cols << std::endl;
    for (const auto& par : resultado_parcial) {
        arquivo << par.first << " " << par.second << std::endl;
    }
    arquivo << duracao_thread << std::endl;
    arquivo.close();
}


int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_matriz1> <arquivo_matriz2> <P>" << std::endl;
        return 1;
    }

    std::vector<std::vector<int>> m1 = lerMatriz(argv[1]);
    std::vector<std::vector<int>> m2 = lerMatriz(argv[2]);
    int P = std::atoi(argv[3]);

    if (m1.empty() || m2.empty() || P <= 0) {
        std::cerr << "Erro ao ler matrizes ou valor de P invalido." << std::endl;
        return 1;
    }

    int n1 = m1.size();
    int m2_cols = m2[0].size();
    int total_elementos = n1 * m2_cols;

    int num_threads = std::ceil(static_cast<double>(total_elementos) / P);
    std::cout << "Calculando com " << num_threads << " threads..." << std::endl;

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i) {
        int inicio_idx = i * P;
        threads.push_back(std::thread(calcular_parte, i, std::ref(m1), std::ref(m2), P, inicio_idx));
    }

    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Todas as threads terminaram. Os resultados parciais foram salvos." << std::endl;

    return 0;
}