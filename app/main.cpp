#define KEY_UP 72
#define KEY_DOWN 80
#define ENTER 13
#define N_MAIN_MENU_OPTIONS 5

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <windows.h>
#include <conio.h>

#include "ArrayList.h"
#include "Trie.h"
#include "MaxHeap.h"

using std::string;
using std::count;
using std::istreambuf_iterator;
using std::ifstream;
using std::cerr;
using std::transform;
using std::tolower;
using std::cin;

bool greaterThanMin(int min, int num){
    if(num >= min){
        return true;
    }
    cout << "\nEl numero debe ser mayor que: " << min << endl;
    return false;
}

bool isInteger(string input){
    if(input[0] == '-'){
        input[0] = '0';
    }

    for(char c : input){
        if(!isdigit(c)){
            cout << "\nEl número debe ser entero" << endl;
            return false;
        }
    }

    return true;                        //si la entrada es entera retorna true
}

//solicita un número entero al usuario
string enterIntNumber(string message, int min){
    string input;

    do {
        cout << "\n" << message;
        getline(cin, input);

    }while((input == "") || (!isInteger(input)) || (!greaterThanMin(min, stoi(input))));

    return input;
}

string enterString(string message){
    string input;

    do{
        cout << "\n" << message;
        getline(cin, input);

    }while((input == ""));

    return input;
}

//coloca el cursor en las coordenadas x,y, según los valores recibidos
void gotoxy(short x, short y){
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // obtiene el identificador de la consola
   COORD pos = {x, y}; // crea una estructura COORD con las coordenadas
   SetConsoleCursorPosition(hConsole, pos); // establece la posición del cursor
}

//imprime el título y las opciones recibidas, contrala el cursor y devuelve la opción seleccionada
int selectOption(const char *title, string options[], int nOptions){
    int selectedOption = 1;     //guarda la opción seleccionada
    int key;                    //obtiene la tecla seleccionada
    bool repeat = true;         //maneja si se debe salir del do while o no

    do{                                     //se repite hasta que el usuario de un enter en la opción deseada
        system("cls");                      //limpia la pantalla

        gotoxy(5,1 + selectedOption);       //coloca el cursor
        cout << "==>";                      //se imprime el cursor que indica en que opción se encuentra

        gotoxy(10,0);                       //se coloca el cursor
        cout << title;                      //se imprime el título recibido

        for(int i = 0; i < nOptions; i++){              //se recorren el número de las opciones recibidas
            gotoxy(10,2 + i);                           //se coloca el cursor
            cout << i + 1 << ") " << options[i];        //se imprime el número y la opción correspondiente
        }

        do{
            key = getch();                                          //captura la tecla seleccionada

        }while(key != KEY_UP && key != KEY_DOWN && key != ENTER);   //se repite hasta que se seleccione enter, flecha arriba o abajo

        if(key == KEY_UP){                      //verifica si la tecla seleccionada es la tecla arriba
            selectedOption--;                   //de serlo, decrementa el número de la opción

            if(selectedOption < 1){             //verifica si el cursor está en la primera opción
                selectedOption = nOptions;      //en caso de estarlo, mueve el cursor de la primera opción hasta la última opción
            }
        }

        else if(key == KEY_DOWN){               //verifica si la tecla seleccionada es la flecha abajo
            selectedOption++;                   //incrementa el número de la opción en la que se este

            if(selectedOption > nOptions){      //verifica si el cursor está en la última opción
                selectedOption = 1;             //mueve el cursor de la última opción hasta la primera
            }
        }

        else if(key == ENTER){                  //verifica si la tecla seleccionada es un enter
            repeat = false;                     //de serlo, entonces pone el repeat en false, para salirse del do while
        }

    }while(repeat);                             //mientras repeat sea true, se repite el ciclo

    return selectedOption;                      //se retorna la opción seleccionada
}

bool isWordToIgnore(string word){
    if(word == ""){
        return true;
    }

    ifstream file("ignorar.txt");

    if(!file){
        cerr << "No se pudo abrir" << endl;
    }

    string line;

    while(getline(file, line)){
        if(line == word){
            return true;
        }
    }
    file.close();
    return false;

}

void queryByPrefix(Trie &words, List<string> *originalLines){
    string message = "Ingrese el prefijo a buscar: ";
    string prefix = enterString(message);

    List<string> *matches = words.getMatches(prefix);
    matches->goToStart();

    if(matches->getSize() > 0){
        cout << "\nCantidad de veces que aparece cada palabra en el archivo original, con el prefijo: " << prefix << endl;
        for(int i=0; i < matches->getSize(); i++){
            string word = matches->getElement();
            TrieNode *foundNode = words.findNode(word);
            cout << word << ": " << foundNode->indexList->getSize() << endl;
            matches->next();
        }
    }
    else{
        cout << "\nNo hay palabras, en el archivo, con el prefijo ingresado" << endl;
    }
}

void searchWord(Trie &words, List<string> *originalLines){
    string message = "Ingrese una palabra a buscar: ";
    string word = enterString(message);

    TrieNode *foundNode = words.findNode(word);
    if(foundNode != nullptr){
        List<int> *indexList = foundNode->indexList;

        indexList->goToStart();
        cout << "\nIndices y lineas originales de la palabra: " << word << endl;
        for(int i = 0; i < indexList->getSize(); i++){
            int index = indexList->getElement();
            originalLines->goToPos(index);
            string originalLine = originalLines->getElement();
            cout << "Índice: " << index << " - Línea original: " << originalLine << endl;
            //cout << "Línea original: " << originalLine << endl;
            indexList->next();
        }
    }
    else{
        cout << "\nLa palabra ingresada no existe en el archivo" << endl;
    }
}

void searchPerNLetters(Trie &words, List<string> *originalLines){
    string message = "Ingrese la cantidad de letras a buscar: ";
    string input = enterIntNumber(message, 1);
    int nLetters = stoi(input);
    List<string> *wordsPerNLetters = words.getWordsPerNLetters(nLetters);

    wordsPerNLetters->goToStart();
    cout << "\nCantidad de veces que aparece cada palabra con " << nLetters << " letras" << endl;
    for(int i = 0; i < wordsPerNLetters->getSize(); i++){
        string word = wordsPerNLetters->getElement();
        TrieNode *foundNode = words.findNode(word);
        cout << word << ": " << foundNode->indexList->getSize() << endl;

        wordsPerNLetters->next();
    }

}

void printMostUsedWords(Trie &words){
    string message = "Ingrese la cantidad de palabras a desplegar: ";
    string input = enterIntNumber(message, 1);
    int nTop = stoi(input);

    MaxHeap<KVPair<int,string>> *mostUsedWords = new MaxHeap<KVPair<int,string>>(words.getSize());
    List<string> *listWords = words.getMatches("");
    listWords->goToStart();

    for(int i = 0; i < listWords->getSize(); i++){
        string word = listWords->getElement();
        TrieNode *foundNode = words.findNode(word);

        if(!isWordToIgnore(word)){
            KVPair<int,string> kvpair;
            kvpair.key = foundNode->indexList->getSize();
            kvpair.value = word;
            mostUsedWords->insert(kvpair);
        }
        listWords->next();
    }

    cout << "\nTop " << nTop << " palabras más usadas " << endl;
    for(int i = 0; i < nTop; i++){
        KVPair<int,string> kvpair = mostUsedWords->removeFirst();
        cout << kvpair.value << ": " << kvpair.key << endl;
    }

    delete mostUsedWords;
}

void mainMenu(Trie &words, List<string> *originalLines){
    int option;
    const char *title = "Menu principal"; // título del menú de opciones
    string options[] = {"Consulta por prefijo", "Buscar palabra", "Buscar por cantidad de letras", "Palabras más usadas", "Salir"};

    do{
        try{
            option = selectOption(title, options, N_MAIN_MENU_OPTIONS );
            system("cls");

            if(option == 1){
                queryByPrefix(words, originalLines);
                cout << "\n\nPresione cualquier letra para continuar" << endl;
                getch();
            }

            else if(option == 2){
                searchWord(words, originalLines);
                cout << "\n\nPresione cualquier letra para continuar" << endl;
                getch();
            }

            else if(option == 3){
                searchPerNLetters(words, originalLines);
                cout << "\n\nPresione cualquier letra para continuar" << endl;
                getch();
            }

            else if(option == 4){
                printMostUsedWords(words);
                cout << "\n\nPresione cualquier letra para continuar" << endl;
                getch();
            }

            else{
                cout << "\nSaliendo del sistema" << endl;
            }

        }
        catch(const runtime_error &e){
            cerr << "\nUps, algo salió mal \n" << "Error: " << e.what() <<"\nPresione cualquier tecla para continuar" << endl;   //se manda el error correspondiente
            getch();
        }
    }while(option != 5);

}

void readFile(string fileName, Trie &words, List<string> *&originalLines){
    ifstream file(fileName);

    string line, word = "";
    int cont = 0;

    while(getline(file, line)){
        //cout << line << endl;
        originalLines->append(line);
        transform(line.begin(), line.end(), line.begin(), [](unsigned char c) {
            return tolower(c);
        });
        for(unsigned int i = 0; i < line.size(); i++){
            if (!isalpha(line[i])) {
                line[i] = ' ';
            }
            if(line[i] != ' '){
                word += line[i];
            }
            else{
                if(word != " "){
                    words.insert(word);
                    TrieNode *foundNode = words.findNode(word);
                    foundNode->indexList->append(cont);
                    word = "";
                }
            }
        }
        cont ++;
        words.insert(word);
        TrieNode *foundNode = words.findNode(word);
        foundNode->indexList->append(cont);
        word = "";
    }

    file.close();
}

int main(){
    setlocale(LC_ALL, "spanish");
    cout << "Bienvenido. Este programa se encarga de abrir y analizar archivos de texto proporcionados por el usuario." << endl;
    cout << "Se podrá consultar por prefijo, buscar una palabra, buscar por cantidad de letras y ver las palabras mas usadas." << endl;
    cout << "Todo eso, gracias a la estructura Trie, que nos permite guardar palabras, con el uso de árboles y diccionarios." << endl;
    cout << "\nPresione cualquier tecla para continuar " << endl;
    getch();
    system("cls");

    string fileName;
    string message = "Ingrese el nombre del archivo a analizar: ";
    fileName = enterString(message);

    ifstream file(fileName);

    if(!file){
        cerr << "\nNo es posible abrir el archivo.\nAsegúrese de que exista y el nombre este bien escrito" << endl;
        return 0;
    }

    int nLines = count(istreambuf_iterator<char>(file),istreambuf_iterator<char>(),'\n') + 1;
    List<string> *originalLines = new ArrayList<string>(nLines);
    Trie words;
    file.close();

    readFile(fileName, words, originalLines);

    mainMenu(words, originalLines);

    delete originalLines;

    return 0;
}
