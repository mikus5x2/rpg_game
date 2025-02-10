#include "game.h"
#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include <algorithm>

Przedmiot::Przedmiot(string n, string t, int e) : nazwa(n), typ(t), efekt(e), zalozony(false) {}

// konstruktor inicjalizujący statystyki gracza
Gracz::Gracz() : zdrowie(100), obrazenia(10), luk_wyposazony(false) {
    ekwipunek.push_back(Przedmiot("Mikstura zdrowia", "leczenie", 20));
}

// wyświetl ekwipunek gracza
void Gracz::pokaz_ekwipunek() {
    cout << "\n=== Twoj Ekwipunek ===" << endl;
    for (size_t i = 0; i < ekwipunek.size(); ++i) {
        cout << i + 1 << ". " << ekwipunek[i].nazwa << " (" << ekwipunek[i].typ << ")";
        if (ekwipunek[i].zalozony) {
            cout << " (Zalozony)";
        }
        cout << endl;
    }
}

// użyj przedmiot 
void Gracz::uzyj_przedmiotu(int index) {
    if (index < 0 || index >= ekwipunek.size()) {
        cout << "Nieprawidlowa decyzja." << endl;
        return;
    }

    Przedmiot& przedmiot = ekwipunek[index];
    if (przedmiot.typ == "leczenie") {
        // leczenie
        zdrowie = min(zdrowie + przedmiot.efekt, 100);
        cout << "Uzywasz " << przedmiot.nazwa << " i odzyskujesz " << przedmiot.efekt << " punktow zdrowia." << endl;
        ekwipunek.erase(ekwipunek.begin() + index);
    }
    else if (przedmiot.typ == "bron" || przedmiot.typ == "zbroja") {
        // zakładanie i zdejmowanie przedmiotu
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
            // sprawdza czy inny przedmiot tego samego typu nie jest już założony
            for (auto& p : ekwipunek) {
                if (p.zalozony && p.typ == przedmiot.typ) {
                    cout << "Już masz założony inny " << p.nazwa << "!" << endl;
                    return;
                }
            }
            // wyposażenie przedmiotu
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

Przeciwnik::Przeciwnik(string n, int z, int o_min, int o_max) : nazwa(n), zdrowie(z), obrazenia_min(o_min), obrazenia_max(o_max) {}

int Przeciwnik::atak() {
    return rand() % (obrazenia_max - obrazenia_min + 1) + obrazenia_min;
}

// fajny wyświetlany tekst -- animacja
void tekst_char(const string& tekst) {
    for (char c : tekst) {
        cout << c;
        Sleep(30);
    }
    cout << endl;
}

// gameloop w walce między graczem a przeciwnikiem
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

        // gracz atakuje
        if (akcja == 1) {
            int obrazenia = gracz.obrazenia;
            if (rand() % 100 < 20) {
                obrazenia *= 2;
                cout << "Trafienie krytyczne!" << endl;
            }
            cout << "Atakujesz " << przeciwnik.nazwa << " i zadajesz " << obrazenia << " obrazen!" << endl;
            przeciwnik.zdrowie -= obrazenia;
        }
        // gracz ucieka - 15%
        else if (akcja == 2) {
            if (rand() % 100 < 15) {
                cout << "Udalo Ci sie uciec!" << endl;
                return true;
            }
            else {
                cout << "Nie udalo sie uciec! Przeciwnik atakuje." << endl;
            }
        }
        // gracz przegląda ekwipunek
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
        // atak z łuku (40% na uniknięcie)
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

        // kontraatak przeciwnika
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

    // sprawdza wynik walki
    if (gracz.zdrowie <= 0) {
        cout << "Zostales pokonany przez " << przeciwnik.nazwa << "!" << endl;
        return false;
    }

    cout << "Pokonales " << przeciwnik.nazwa << "!" << endl;
    return true;
}

// system ruchow gracza
void eksploruj(Gracz& gracz, vector<Przeciwnik>& przeciwnicy, int poziom_bossa) {
    srand(time(nullptr));
    vector<string> historia; // przechowuje historie ruchow gracza
    bool znaleziono_skrzynke = false; // boolean z znalezieniem skrzynki - true or false
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
        int wybor;
        cin >> wybor;

        // obsługa ekwipunku
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

        // sprawdzanie poprawności wyboru
        if (wybor < 1 || wybor > 4) {
            cout << "Nieprawidlowy wybor, sprobuj ponownie." << endl;
            system("pause");
            continue;
        }

        // cofnięcie poprzedniego ruchu
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

            // 15% na pewną śmierć. (przejście na wyższy poziom c: )
            if (rand() % 100 < 15) {
                ++poziom;
                cout << "Odnajdujesz przejście do poziomu " << poziom << " katakumb!" << endl;
            }
            else {
                cout << "Nie udalo sie przejsc na wyzszy poziom." << endl;
            }
            system("pause");
        }

        // szansa na znalezienie skrzynki -- (10%)
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

        // system spotkania glownego bossa - zalezy od poziomu trudnosci
        if (poziom == poziom_bossa) {
            Przeciwnik smok("Smok", 150, 15, 30);
            cout << "\n=== BOSS ===\n" << endl;
            cout << "Spotykasz " << smok.nazwa << "!" << endl;
            system("pause");
            if (!walka(gracz, smok)) break;
            cout << "Udalo ci sie pokonac glownego straznika katakumb oraz znalezc ostatni skarb lochow!" << endl;
            cout << "Kliknij enter, aby powrocic do menu glownego." << endl;
            cin.ignore();
            cin.get();
            break;
        }

        // 3% na sekretnego bossa
        if (rand() % 100 < 3) {
            Przeciwnik krolSzkieletow("Krol Szkieletow", 100, 10, 20);
            cout << "\n=== SEKRETNY BOSS ===\n" << endl;
            cout << "Spotykasz " << krolSzkieletow.nazwa << "!" << endl;
            system("pause");
            if (!walka(gracz, krolSzkieletow)) break;
            gracz.ekwipunek.push_back(Przedmiot("Miecz z Kosci", "bron", 12));
            gracz.zdrowie += 40;
            cout << "Otrzymujesz niecz z kosci i zwiekszasz zdrowie o 40!" << endl;
            system("pause");
        }

        // 33% na spotkanie przeciwnika
        if (rand() % 3 == 0) {
            Przeciwnik przeciwnik = przeciwnicy[rand() % przeciwnicy.size()];
            cout << "Spotykasz " << przeciwnik.nazwa << "!" << endl;
            system("pause");
            if (!walka(gracz, przeciwnik)) break;
        }
        else {
            cout << "Nie spotykasz nikogo na swojej drodze." << endl;
        }

        // 15% dropnięcie mikstury zdrowia
        if (rand() % 6 == 0) {
            cout << "Znalazles miksture zdrowia! Dodano do ekwipunku." << endl;
            gracz.ekwipunek.push_back(Przedmiot("Mikstura zdrowia", "leczenie", 20));
        }

        system("pause");
    }

    if (gracz.zdrowie <= 0) {
        cout << "Koniec gry! Zostales pokonany." << endl;
        system("pause");
    }
}

// rozpocznij nową gre
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

    // poziomy trudnosci
    int poziom_trudnosci;
    cout << "Wybierz poziom trudnosci: 1 - Latwy, 2 - Normalny, 3 - Trudny: ";
    cin >> poziom_trudnosci;

    int poziom_bossa = 0;
    if (poziom_trudnosci == 1) {
        poziom_bossa = 5;
    }
    else if (poziom_trudnosci == 2) {
        poziom_bossa = 10;
    }
    else if (poziom_trudnosci == 3) {
        poziom_bossa = 15;
    }

    eksploruj(gracz, przeciwnicy, poziom_bossa);
}

void wyjdz() {
    exit(0);
}