#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

std::vector<std::vector<int>> gerarMatriz(int rows, int cols) {
    std::vector<std::vector<int>> matriz(rows, std::vector<int>(cols));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matriz[i][j] = rand() % 10;
        }
    }
    return matriz;
}

void salvarMatriz(const std::vector<std::vector<int>>& matriz, const std::string& nomeArquivo) {
    std::ofstream arquivo(nomeArquivo);

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    int rows = matriz.size();
    int cols = matriz[0].size();

    arquivo << rows << " " << cols << std::endl;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arquivo << matriz[i][j] << " ";
        }
        arquivo << std::endl;
    }

    arquivo.close();
    std::cout << "Matriz salva com sucesso em " << nomeArquivo << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <n1> <m1> <n2> <m2>" << std::endl;
        return 1;
    }

    int n1 = std::atoi(argv[1]);
    int m1 = std::atoi(argv[2]);
    int n2 = std::atoi(argv[3]);
    int m2 = std::atoi(argv[4]);

    if (m1 != n2) {
        std::cerr << "Erro: Para multiplicar matrizes, o numero de colunas da primeira (m1=" << m1
                  << ") deve ser igual ao numero de linhas da segunda (n2=" << n2 << ")." << std::endl;
        return 1;
    }

    srand(time(0));

    std::cout << "Gerando Matriz 1 (" << n1 << "x" << m1 << ")..." << std::endl;
    std::vector<std::vector<int>> matriz1 = gerarMatriz(n1, m1);

    std::cout << "Gerando Matriz 2 (" << n2 << "x" << m2 << ")..." << std::endl;
    std::vector<std::vector<int>> matriz2 = gerarMatriz(n2, m2);

    salvarMatriz(matriz1, "matriz1.txt");
    salvarMatriz(matriz2, "matriz2.txt");

    return 0;
}