#include "game.h" 

int main() {
    while (true) {
        system("cls");
        cout << "=== Gra RPG - Podziemne Katakumby ===" << endl;
        cout << "1 - Nowa gra" << endl;
        cout << "2 - Wyjdz z gry" << endl;
        cout << "Wybierz opcje: ";

        int wybor;
        cin >> wybor;

        if (wybor == 1) {
            nowa_gra();
        }
        else if (wybor == 2) {
            wyjdz();
        }
        else {
            cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
            system("pause");
        }
    }
}