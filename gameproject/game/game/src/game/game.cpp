#include <iostream>
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

class Przedmiot {
public:
    string nazwa;
    string typ;
    int efekt;
    bool zalozony;

    Przedmiot(string n, string t, int e) : nazwa(n), typ(t), efekt(e), zalozony(false) {}
};

class Gracz {
public:
    int zdrowie;
    int obrazenia;
    bool luk_wyposazony;
    vector<Przedmiot> ekwipunek;

    Gracz() : zdrowie(100), obrazenia(10), luk_wyposazony(false) {
        ekwipunek.push_back(Przedmiot("Mikstura zdrowia", "leczenie", 20));
    }

    void pokaz_ekwipunek() {
        cout << "\n=== Twoj Ekwipunek ===" << endl;
        for (size_t i = 0; i < ekwipunek.size(); ++i) {
            cout << i + 1 << ". " << ekwipunek[i].nazwa << " (" << ekwipunek[i].typ << ")";
            if (ekwipunek[i].zalozony) {
                cout << " (Zalozony)";
            }
            cout << endl;
        }
    }

    void uzyj_przedmiotu(int index) {
        if (index < 0 || index >= ekwipunek.size()) {
            cout << "Nieprawidlowa decyzja." << endl;
            return;
        }

        Przedmiot& przedmiot = ekwipunek[index];
        if (przedmiot.typ == "leczenie") {
            zdrowie = min(zdrowie + przedmiot.efekt, 100);
            cout << "Uzywasz " << przedmiot.nazwa << " i odzyskujesz " << przedmiot.efekt << " punktow zdrowia." << endl;
            ekwipunek.erase(ekwipunek.begin() + index);
        }
        else if (przedmiot.typ == "bron" || przedmiot.typ == "zbroja") {
            if (przedmiot.zalozony) {
                cout << "Już masz założony " << przedmiot.nazwa << "!" << endl;
                przedmiot.zalozony = false;
                if (przedmiot.typ == "bron") {
                    obrazenia -= przedmiot.efekt;
                }
                else if (przedmiot.typ == "zbroja") {
                    zdrowie -= przedmiot.efekt;
                }
            }
            else {
                for (auto& p : ekwipunek) {
                    if (p.zalozony && p.typ == przedmiot.typ) {
                        cout << "Już masz założony inny " << p.nazwa << "!" << endl;
                        return;
                    }
                }
                if (przedmiot.typ == "bron") {
                    obrazenia += przedmiot.efekt;
                }
                else if (przedmiot.typ == "zbroja") {
                    zdrowie += przedmiot.efekt;
                }
                cout << "Wyposazasz " << przedmiot.nazwa << " i zwiekszasz obrazenia o " << przedmiot.efekt << "!" << endl;
                przedmiot.zalozony = true;
            }
        }
        else if (przedmiot.nazwa == "Luk") {
            luk_wyposazony = true;
            cout << "Wyposazasz Luk!" << endl;
            przedmiot.zalozony = true;
        }
    }

    void zapisz_stan() {
        ofstream plik("zapisgry.txt");
        plik << zdrowie << endl;
        plik << obrazenia << endl;
        plik << luk_wyposazony << endl;
        plik << ekwipunek.size() << endl;
        for (const auto& przedmiot : ekwipunek) {
            plik << przedmiot.nazwa << " " << przedmiot.typ << " " << przedmiot.efekt << " " << przedmiot.zalozony << endl;
        }
        plik.close();
    }

    void wczytaj_stan() {
        ifstream plik("zapisgry.txt");
        if (plik.is_open()) {
            plik >> zdrowie;
            plik >> obrazenia;
            plik >> luk_wyposazony;
            int rozmiar;
            plik >> rozmiar;
            ekwipunek.clear();
            for (int i = 0; i < rozmiar; ++i) {
                string n, t;
                int e;
                bool z;
                plik >> n >> t >> e >> z;
                Przedmiot przedmiot(n, t, e);
                przedmiot.zalozony = z;
                ekwipunek.push_back(przedmiot);
            }
            plik.close();
        }
    }
};

class Przeciwnik {
public:
    string nazwa;
    int zdrowie;
    int obrazenia_min;
    int obrazenia_max;

    Przeciwnik(string n, int z, int o_min, int o_max) : nazwa(n), zdrowie(z), obrazenia_min(o_min), obrazenia_max(o_max) {}

    int atak() {
        return rand() % (obrazenia_max - obrazenia_min + 1) + obrazenia_min;
    }
};

void tekst_char(const string& tekst) {
    for (char c : tekst) {
        cout << c;
        Sleep(30);
    }
    cout << endl;
}

bool walka(Gracz& gracz, Przeciwnik& przeciwnik) {
    while (przeciwnik.zdrowie > 0 && gracz.zdrowie > 0) {
        system("cls");
        cout << "=== Walka ===" << endl;
        cout << "Twoje zdrowie: " << gracz.zdrowie << "\nZdrowie przeciwnika: " << przeciwnik.zdrowie << endl;
        cout << "Co chcesz zrobic? 1 - Atakuj, 2 - Uciekaj, 3 - Ekwipunek";
        if (gracz.luk_wyposazony) cout << ", 4 - Atakuj z luku";
        cout << endl;

        int akcja;
        cin >> akcja;

        if (akcja == 1) {
            int obrazenia = gracz.obrazenia;
            if (rand() % 100 < 20) {
                obrazenia *= 2;
                cout << "Trafienie krytyczne!" << endl;
            }
            cout << "Atakujesz " << przeciwnik.nazwa << " i zadajesz " << obrazenia << " obrazen!" << endl;
            przeciwnik.zdrowie -= obrazenia;
        }
        else if (akcja == 2) {
            if (rand() % 100 < 15) {
                cout << "Udalo Ci sie uciec!" << endl;
                return true;
            }
            else {
                cout << "Nie udalo sie uciec! Przeciwnik atakuje." << endl;
            }
        }
        else if (akcja == 3) {
            gracz.pokaz_ekwipunek();
            cout << "Wybierz przedmiot do uzycia (0, aby anulowac): ";
            int wybor;
            cin >> wybor;
            if (wybor != 0) {
                gracz.uzyj_przedmiotu(wybor - 1);
            }
            continue;
        }
        else if (akcja == 4 && gracz.luk_wyposazony) {
            cout << "Atakujesz z luku!" << endl;
            przeciwnik.zdrowie -= gracz.obrazenia;
            if (rand() % 100 < 40) {
                cout << "Unikasz ataku przeciwnika!" << endl;
                system("pause");
                continue;
            }
        }
        else {
            cout << "Nieprawidlowa decyzja, przeciwnik atakuje!" << endl;
        }

        if (przeciwnik.zdrowie > 0) {
            if (rand() % 100 >= 10) {
                int dmg = przeciwnik.atak();
                cout << przeciwnik.nazwa << " atakuje Ciebie i zadaje " << dmg << " obrazen!" << endl;
                gracz.zdrowie -= dmg;
            }
            else {
                cout << przeciwnik.nazwa << " pudluje!" << endl;
            }
        }

        system("pause");
    }

    if (gracz.zdrowie <= 0) {
        cout << "Zostales pokonany przez " << przeciwnik.nazwa << "!" << endl;
        return false;
    }

    cout << "Pokonales " << przeciwnik.nazwa << "!" << endl;
    return true;
}

void eksploruj(Gracz& gracz, vector<Przeciwnik>& przeciwnicy) {
    srand(time(nullptr));
    vector<string> historia;
    bool znaleziono_skrzynke = false;
    int poziom = 1;

    while (gracz.zdrowie > 0) {
        system("cls");
        cout << "=== Poziom Katakumb: " << poziom << " ===" << endl;
        cout << "Twoje zdrowie: " << gracz.zdrowie << endl;
        cout << "Historia ruchow: ";
        for (const auto& ruch : historia) {
            cout << ruch << " > ";
        }
        cout << endl;
        cout << "\nWybory:" << endl;
        cout << "1 - Prosto" << endl;
        cout << "2 - Lewo" << endl;
        cout << "3 - Prawo" << endl;
        cout << "4 - Cofnij" << endl;
        cout << "5 - Ekwipunek" << endl;
        cout << "6 - Zapisz gre" << endl;
        cout << "7 - Wczytaj gre" << endl;
        int wybor;
        cin >> wybor;

        if (wybor == 5) {
            gracz.pokaz_ekwipunek();
            cout << "Wybierz przedmiot do uzycia (0, aby anulowac): ";
            int ekw_wybor;
            cin >> ekw_wybor;
            if (ekw_wybor != 0) {
                gracz.uzyj_przedmiotu(ekw_wybor - 1);
            }
            continue;
        }
        else if (wybor == 6) {
            gracz.zapisz_stan();
            cout << "Gra zostala zapisana." << endl;
            system("pause");
            continue;
        }
        else if (wybor == 7) {
            gracz.wczytaj_stan();
            cout << "Gra zostala wczytana." << endl;
            system("pause");
            continue;
        }

        if (wybor < 1 || wybor > 4) {
            cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
            system("pause");
            continue;
        }

        if (wybor == 4) {
            if (!historia.empty()) {
                historia.pop_back();
            }
        }
        else {
            switch (wybor) {
            case 1: historia.push_back("Prosto"); break;
            case 2: historia.push_back("Lewo"); break;
            case 3: historia.push_back("Prawo"); break;
            }

            if (rand() % 100 < 15) {
                ++poziom;
                cout << "Odnajdujesz przejście do poziomu " << poziom << " katakumb!" << endl;
            }
            else {
                cout << "Nie udalo sie przejsc na wyzszy poziom." << endl;
            }
            system("pause");
        }

        if (!znaleziono_skrzynke && rand() % 10 == 0) {
            znaleziono_skrzynke = true;
            cout << "Znalazles skrzynke! Otwierasz ja i znajdujesz..." << endl;

            bool otrzymano_przedmiot = false;
            if (rand() % 100 < 20) {
                gracz.ekwipunek.push_back(Przedmiot("Miecz Mocy", "bron", 5));
                cout << "- Miecz Mocy" << endl;
                otrzymano_przedmiot = true;
            }

            if (rand() % 100 < 20) {
                gracz.ekwipunek.push_back(Przedmiot("Zbroja ze Smoczej Skory", "zbroja", 15));
                cout << "- Zbroja ze Smoczej Skory" << endl;
                otrzymano_przedmiot = true;
            }

            if (rand() % 100 < 20) {
                gracz.ekwipunek.push_back(Przedmiot("Luk", "bron", 0));
                cout << "- Luk" << endl;
                otrzymano_przedmiot = true;
            }

            if (!otrzymano_przedmiot) {
                gracz.ekwipunek.push_back(Przedmiot("Mikstura zdrowia", "leczenie", 20));
                cout << "- Mikstura zdrowia (pocieszenie)" << endl;
            }

            system("pause");
            continue;
        }

        if (poziom == 10) {
            Przeciwnik smok("Smok", 150, 15, 30);
            cout << "\n=== BOSS ===\n" << endl;
            cout << "Spotykasz " << smok.nazwa << "!" << endl;
            system("pause");
            if (!walka(gracz, smok)) break;
            cout << "Pokonales Smoka! Gra konczy sie." << endl;
            cout << "Koniec gry! Dziekuje za gre!" << endl;
            cout << "Kliknij Enter, aby przejsc do menu glownego." << endl;
            cin.ignore();
            cin.get();
            break;
        }

        if (rand() % 100 < 3) {
            Przeciwnik krolSzkieletow("Krol Szkieletow", 100, 10, 20);
            cout << "\n=== SEKRETNY BOSS ===\n" << endl;
            cout << "Spotykasz " << krolSzkieletow.nazwa << "!" << endl;
            system("pause");
            if (!walka(gracz, krolSzkieletow)) break;
            gracz.ekwipunek.push_back(Przedmiot("Miecz z Kosci", "bron", 12));
            gracz.zdrowie += 40;
            cout << "Otrzymujesz Miecz z Kosci i zwiekszasz zdrowie o 40!" << endl;
            system("pause");
        }

        if (rand() % 3 == 0) {
            Przeciwnik przeciwnik = przeciwnicy[rand() % przeciwnicy.size()];
            cout << "Spotykasz " << przeciwnik.nazwa << "!" << endl;
            system("pause");
            if (!walka(gracz, przeciwnik)) break;
        }
        else {
            cout << "Nie spotykasz nikogo na swojej drodze." << endl;
        }

        if (rand() % 6 == 0) {
            cout << "Znalazles miksture zdrowia! Dodano do ekwipunku." << endl;
            gracz.ekwipunek.push_back(Przedmiot("Mikstura zdrowia", "leczenie", 20));
        }

        system("pause");
    }

    if (gracz.zdrowie <= 0) {
        cout << "Koniec gry! Zostales pokonany." << endl;
    }
}

void nowa_gra() {
    system("cls");
    const string fabula =
        R"(
    Znajdujesz sie w podziemnych katakumbach. Twoim celem jest odnalezienie skarbu ukrytego gleboko w ich wnetrzu. 
    Musisz walczyc z przeciwnikami i unikac smiertelnych pulapek. Powodzenia, smialku!
    )";

    tekst_char(fabula);
    system("pause");

    Gracz gracz;
    vector<Przeciwnik> przeciwnicy = {
        Przeciwnik("Goblin", 30, 4, 10),
        Przeciwnik("Szkielet", 20, 3, 7),
        Przeciwnik("Zjawa", 25, 5, 12),
        Przeciwnik("Wampir", 35, 8, 15)
    };

    eksploruj(gracz, przeciwnicy);
}

void wyjdz() {
    exit(0);
}

int main() {
    while (true) {
        system("cls");
        cout << "=== Gra RPG - Podziemne Katakumby ===" << endl;
        cout << "1 - Nowa gra" << endl;
        cout << "2 - Wczytaj gre" << endl;
        cout << "3 - Wyjdz z gry" << endl;
        cout << "Wybierz opcje: ";

        int wybor;
        cin >> wybor;

        if (wybor == 1) {
            nowa_gra();
        }

        else if (wybor == 2) {
            Gracz gracz;
            gracz.wczytaj_stan();
            cout << "Gra zostala wczytana." << endl;

            vector<Przeciwnik> przeciwnicy = {
                Przeciwnik("Goblin", 30, 4, 10),
                Przeciwnik("Szkielet", 20, 3, 7),
                Przeciwnik("Zjawa", 25, 5, 12),
                Przeciwnik("Wampir", 35, 8, 15)
            };

            eksploruj(gracz, przeciwnicy);
        }

        else if (wybor == 3) {
            wyjdz();
        }
        else {
            cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
            system("pause");
        }
    }
}