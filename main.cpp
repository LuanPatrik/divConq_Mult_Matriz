#include <iostream>
#include <math.h>
#include <time.h>

using namespace std;
 
struct Matriz{
    int *matrizA, *matrizB, *matrizC;
};

//Construct para iniciar as matrizes
Matriz construct(int dimensao){
    Matriz matriz;
    matriz.matrizA = new int[dimensao * dimensao];
    matriz.matrizB = new int[dimensao * dimensao];
    matriz.matrizC = new int[dimensao * dimensao];

    for (int i = 0; i < dimensao * dimensao; i++)
    { 
        cout << "Valor Matriz A: ";
        cin >> matriz.matrizA[i];
    }

    for (int i = 0; i < dimensao * dimensao; i++)
    {
        cout << "Valor Matriz B: ";
        cin >> matriz.matrizB[i];
    }

    return matriz;
}

//Função para dividir as matrizes em submatrizes (quadrantes)
void divideMatrizes(int A11[], int A12[], int A21[], int A22[], int B11[], int B12[], int B21[], int B22[], int matrizA[], int matrizB[], int meio, int dimensao){
    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio; j++)
        {
            A11[i * meio + j] = matrizA[i * dimensao + j];
            A12[i * meio + j] = matrizA[i * dimensao + j + meio];
            A21[i * meio + j] = matrizA[(i + meio) * dimensao + j];
            A22[i * meio + j] = matrizA[(i + meio) * dimensao + j + meio];

            B11[i * meio + j] = matrizB[i * dimensao + j];
            B12[i * meio + j] = matrizB[i * dimensao + j + meio];
            B21[i * meio + j] = matrizB[(i + meio) * dimensao + j];
            B22[i * meio + j] = matrizB[(i + meio) * dimensao + j + meio];
        }
    }
}

//Função para combinar as submatrizes em uma única matriz resultante
void combinaMatrizes(int matrizC[], int meio, int dimensao, int C11[], int C12[], int C21[], int C22[]){
    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio; j++)
        {
            matrizC[i * dimensao + j] = C11[i * meio + j];
            matrizC[i * dimensao + j + meio] = C12[i * meio + j];
            matrizC[(i + meio) * dimensao + j] = C21[i * meio + j];
            matrizC[(i + meio) * dimensao + j + meio] = C22[i * meio + j];
        }
    }
}

//Função para realizar a operação de multiplicação de duas matrizes
void multiplicaMatrizes(int matrizA[], int matrizB[], int matrizC[], int dimensao){
    //Caso base: Se as matrizes forem de tamanho 1
    if (dimensao == 1){
        matrizC[0] = matrizA[0] * matrizB[0];
        return;
    }

    //DIVISÃO: Dividindo as matrizes em submatrizes (quadrantes)
    int meio = dimensao / 2;
    int A11 [meio * meio], A12 [meio * meio], A21 [meio * meio], A22 [meio * meio];
    int B11 [meio * meio], B12 [meio * meio], B21 [meio * meio], B22 [meio * meio];
    int C11 [meio * meio], C12 [meio * meio], C21 [meio * meio], C22 [meio * meio];

    divideMatrizes(A11, A12, A21, A22, B11, B12, B21, B22, matrizA, matrizB, meio, dimensao);

    // CONQUISTA: calculando recursivamente os produtos das submatrizes
    int aux1[meio * meio], aux2[meio * meio];
    
    multiplicaMatrizes(A11, B11, aux1, meio);
    multiplicaMatrizes(A12, B21, aux2, meio);

    for (int i = 0; i < meio; ++i)
    {
        for (int j = 0; j < meio; ++j)
        {
            C11[i * meio + j] = aux1[i * meio + j] + aux2[i * meio + j];
        }
    }

    multiplicaMatrizes(A11, B12, aux1, meio);
    multiplicaMatrizes(A12, B22, aux2, meio);

    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio; j++)
        {
            C12[i * meio + j] = aux1[i * meio + j] + aux2[i * meio + j];
        }
    }

    multiplicaMatrizes(A21, B11, aux1, meio);
    multiplicaMatrizes(A22, B21, aux2, meio);

    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio; j++)
        {
            C21[i * meio + j] = aux1[i * meio + j] + aux2[i * meio + j];
        }
    }

    multiplicaMatrizes(A21, B12, aux1, meio);
    multiplicaMatrizes(A22, B22, aux2, meio);

    for (int i = 0; i < meio; i++)
    {
        for (int j = 0; j < meio; j++)
        {
            C22[i * meio + j] = aux1[i * meio + j] + aux2[i * meio + j];
        }
    }

    // COMBINAÇÃO: combinando as submatrizes em uma única matriz resultante
    combinaMatrizes(matrizC, meio, dimensao, C11, C12, C21, C22);

}

//Imprime matriz
void imprimeMatriz(int matriz[], int dimensao)
{
    for (int i = 0; i < dimensao; ++i)
    {
        for (int j = 0; j < dimensao; ++j)
        {
            cout << matriz[i * dimensao + j] << " ";
        }
        cout << endl;
    }
}

int main(){
    int dimensao = 0;
    clock_t tempo;

    cout << "Digite o valor das dimensões da matriz: ";
    cin >> dimensao;

    if (dimensao < 0)
    {
        cout << "Valor da dimensão inválido!" << endl;
        return 0;
    }

    dimensao = pow(2, dimensao);

    Matriz matriz = construct(dimensao);

    tempo = clock();

    multiplicaMatrizes(matriz.matrizA, matriz.matrizB, matriz.matrizC, dimensao);

    tempo = clock() - tempo;

    //Calcula o tempo do processador, subtraímos o tempo incial e final e dividi por CLOCKS_PER_SEC (o número de tiques do relógio por segundo)
    double tempo_final = (tempo) / CLOCKS_PER_SEC;

    cout << "--- Matriz A ---" << endl;
    imprimeMatriz(matriz.matrizA, dimensao);
    cout << endl;

    cout << "--- Matriz B ---" << endl;
    imprimeMatriz(matriz.matrizB, dimensao);
    cout << endl;
    cout << "--- Matriz C ---" << endl;
    imprimeMatriz(matriz.matrizC, dimensao);
    cout << endl;

    cout << "Tempo de Uso da CPU: " << tempo_final << endl;

    return 0;
}