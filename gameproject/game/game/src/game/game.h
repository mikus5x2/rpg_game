#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// klasa initializujaca przedmioty
class Przedmiot {
public:
    string nazwa;
    string typ;
    int efekt;
    bool zalozony;
    Przedmiot(string n, string t, int e);
};

// klasa initializujaca gracza
class Gracz {
public:
    int zdrowie;
    int obrazenia;
    bool luk_wyposazony;
    vector<Przedmiot> ekwipunek;

    Gracz();
    void pokaz_ekwipunek();
    void uzyj_przedmiotu(int index);
};

// klasa initializujaca przeciwnikow
class Przeciwnik {
public:
    string nazwa;
    int zdrowie;
    int obrazenia_min;
    int obrazenia_max;
    Przeciwnik(string n, int z, int o_min, int o_max);
    int atak();
};

// animacja tekstu
void tekst_char(const string& tekst);
// walka z przeciwnikiem
bool walka(Gracz& gracz, Przeciwnik& przeciwnik);
// ruch gracza
void eksploruj(Gracz& gracz, vector<Przeciwnik>& przeciwnicy, int poziom_bossa);
// rozpoczecie nowej gry
void nowa_gra();
// quit
void wyjdz();

#endif