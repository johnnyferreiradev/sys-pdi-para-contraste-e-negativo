#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#define TAMVETIMGS 5 // Constante que define o tamanho do array de imagens de entrada

using namespace std;
using namespace cv;

int status=0; // Contador que verifica quantas imagens restam para o fim da compilação

// dirSaida: Diretorio da pasta onde as imagens compiladas serão gravadas
String dirSaida = "/home/johnny/Documentos/Sistemas de Informacao/S4/PDI/PDI_Experimento_N1/Saidas/";

// Diretorios de cada imagem a ser processada
String imgs[] = {
                    "/home/johnny/Documentos/Sistemas de Informacao/S4/PDI/PDI_Experimento_N1/Entradas/img1.jpeg",
                    "/home/johnny/Documentos/Sistemas de Informacao/S4/PDI/PDI_Experimento_N1/Entradas/img2.jpeg",
                    "/home/johnny/Documentos/Sistemas de Informacao/S4/PDI/PDI_Experimento_N1/Entradas/img3.jpeg",
                    "/home/johnny/Documentos/Sistemas de Informacao/S4/PDI/PDI_Experimento_N1/Entradas/img4.jpeg",
                    "/home/johnny/Documentos/Sistemas de Informacao/S4/PDI/PDI_Experimento_N1/Entradas/img5.jpeg",
                };

// Valores experimentais de cada algoritmo. Cada imagem é compilada com algum desses valores
float valoresLinearA[] = {0.2, 0.5, 1, 1.5, 2};
float valoresLinearB[] = {0, 20, 40, 80, 100};
float outrosValores[] = {0.2, 0.5, 1, 3, 6};

// Função que disponibiliza uma interface ao usuario, de forma que é possivel observar o progresso da execução e o seu termino
void alterarStatus(){
    status++;
    system("clear"); // So limpa a tela no Linux, no Windows o paramatro deve ser 'cls'
    cout<<"Progresso: "<<status<<"/80\n";
    if(status==80){
        cout<<"\nCompilado com sucesso!\n\n";
    }
}

// Função utilizada somente durante o desenvolvimento. Serve para visualizar uma imagem antes e depois do processamento
void exibirImagens(Mat img, Mat imgProcess){
    namedWindow("imagemOriginal");
    namedWindow("imagemProcessada");

    imshow("imagemOriginal",img);
    imshow("imagemProcessada",imgProcess);
    waitKey(0);
}

// Gera uma imagem baseado no diretorio de destino das imagens de entrada
Mat criarImagem(int i){
    Mat img = imread(imgs[i], 0);
    return img;
}

// Resposável por evidar o efeito de overflow e underflow das imagens
float overUnder(float n){
    if(n > 255){
        return 255;
    }
    if(n < 0){
        return 0;
    }
    return n;
}

// Recebe uma imagem e retorna uma nova imagem negativa
Mat negativo(Mat img){
    Mat imgProcess = img.clone();
    float  s;
    for(int i=0; i<img.rows; i++){
        for(int j=0; j<img.cols; j++){
            s = 255 - img.at<uchar>(i,j);
            imgProcess.at<uchar>(i,j) = s;
        }
    }
    return imgProcess;
}

// Recebe uma imagem, uma constante e um valor para brilho, onde contraste e o brilho vem dos valores de 'valoresLinearA[]' e 'valoresLinearB[]'.
// Retorna uma nova imagem com ajuste de brilho e contraste linear.
Mat linear(Mat img, float constante, int brilho){
    Mat imgProcess = img.clone();
    float s;
    for(int i=0; i<img.rows; i++){
        for(int j=0; j<img.cols; j++){
            s = constante * img.at<uchar>(i,j) + brilho;
            imgProcess.at<uchar>(i,j) = overUnder(s);
        }
    }
    return imgProcess;
}

// Recebe uma imageme e um contraste onde o contraste vem dos valores de 'outrosValores[]'.
// Retorna uma nova imagem com ajuste de contraste logaritimico.
Mat logaritimica(Mat img, float constante){
    Mat imgProcess = img.clone();
    float s;
    for(int i=0; i<img.rows; i++){
        for(int j=0; j<img.cols; j++){
            s = constante * log(1 + img.at<uchar>(i,j));
            imgProcess.at<uchar>(i,j) = overUnder(s);
        }
    }
    return imgProcess;
}

// Recebe uma imagem, uma constante e um expoente, onde o valor do contraste e expoente vem dos valores de 'outrosValores[]'.
// Retorna uma nova imagem com ajuste de contraste com potencia y.
Mat potencia(Mat img, float constante, float y){
    Mat imgProcess = img.clone();
    float s;
    for(int i=0; i<img.rows; i++){
        for(int j=0; j<img.cols; j++){
            s = constante * pow((img.at<uchar>(i,j) + 0.1), y);
            imgProcess.at<uchar>(i,j) = overUnder(s);
        }
    }
    return imgProcess;
}

// Funcao responsavel por estruturar a ordem em que as imagens irão ser processadas
// Define a forma que as imagens processadas são armazenadas no disco, distrinuindo diferentes imagens e formulas para diretorios apropriados
void compilarImagens(){
    int tam = TAMVETIMGS; // Define o numero de imagens de entrada e limite dos loop for

    Mat img[tam];
    for (int i=0; i<tam; i++) {
        img[i] = criarImagem(i); // Armazena cada imagem em um indice do vetor
    }

    Mat imgProNegativo, imgProLinear, imgProLog, imgProPotencia; // Variaveis utilizadas para armazenar o estado de uma imagem
    for(int i=0; i<tam; i++){

        String dirAtual = "Img" + to_string(i+1); // Define a qual diretorio uma imagem pertence. Exs: Img1/ , Img2/
        String imgValor;

        imgProNegativo = negativo(img[i]); // Chama a funcao que cria uma nova imagem negativa
        imwrite(dirSaida + dirAtual + "/Negativo/img.jpeg", imgProNegativo); // Define o nome da imagem de acordo com o valor testado em um dado momento. Ex: Img1/Linear/img1.jpeg. Salva a imagem processada em seu diretorio apropriado
        alterarStatus(); // exibe o estado atual do processamento

        for (int j=0; j<tam; j++) {
            imgValor = "img" + to_string(j+1);
            imgProLinear = linear(img[i], valoresLinearA[j], valoresLinearB[j]); // Descrição na linha 139
            imwrite(dirSaida + dirAtual + "/Linear/" + imgValor + ".jpeg", imgProLinear); // Define o nome da imagem de acordo com o valor testado em um dado momento. Ex: Img1/Linear/img1.jpeg. Salva a imagem processada em seu diretorio apropriado
            alterarStatus(); // Descrição na linha 141
        }

        /* O restante do codigo é semelhante ao anteriormente comentado, alterando-se
         * somente o algoritmo de criar uma nova imagem */

        for (int j=0; j<tam; j++) {
            imgValor = "img" + to_string(j+1);
            imgProLog = logaritimica(img[i], outrosValores[j]);
            imwrite(dirSaida + dirAtual + "/Logaritimico/" + imgValor + ".jpeg", imgProLog);
            alterarStatus();
        }

        for (int j=0; j<tam; j++) {
            imgValor = "img" + to_string(j+1);
            imgProPotencia = potencia(img[i], outrosValores[j], outrosValores[j]);
            imwrite(dirSaida + dirAtual + "/Potencia/" + imgValor + ".jpeg", imgProPotencia);
            alterarStatus();
        }
    }
}

// Inicio do programa
int main(){

    compilarImagens(); //Funcao resposável por startar o processamento das imagens

    return 1;
}
