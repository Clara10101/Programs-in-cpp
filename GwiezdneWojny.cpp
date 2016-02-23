/* 
 * File:   main.cpp
 * Author: Klara Trzcinska
 *
 * Created on 29 grudzien 2015, 17:37
 */

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;

enum FormyTerenu {
    teren_plaski, las, skaly, rzeka, ocean
};

class ObiektRuchomy {
protected:
    int szybkosc;
    int czas_karczowania_lasu = 0;
    int czas_zasypywania_rzek = 0;
    int czas_niwelowania_gor = 0;
    bool rozstawiaDD = false;
public:
    virtual ~ObiektRuchomy() = default;
    virtual string etykieta() const = 0;
    virtual char DlaKogoWalcze() const = 0;
    virtual bool CzyMozeWejscNaPole(FormyTerenu) const = 0;
    virtual int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *) = 0;
    int PodajSzybkosc() const;
    virtual bool CzyModyfikujePole(FormyTerenu) const;
    int CzasPotrzebnyNaModyfikacje(FormyTerenu) const;
    bool CzyMozeRozstawiacDD() const;
};

bool ObiektRuchomy::CzyMozeRozstawiacDD() const {
    return rozstawiaDD;
}

int ObiektRuchomy::PodajSzybkosc() const {
    return szybkosc;
}

int ObiektRuchomy::CzasPotrzebnyNaModyfikacje(FormyTerenu teren) const {
    if (teren == las) {
        return this->czas_karczowania_lasu;
    }
    if (teren == skaly) {
        return this->czas_niwelowania_gor;
    }
    if (teren == rzeka) {
        return this->czas_zasypywania_rzek;
    } else
        return 0;
}

bool ObiektRuchomy::CzyModyfikujePole(FormyTerenu teren) const {
    if (teren == las && this->czas_karczowania_lasu > 0)
        return true;
    if (teren == skaly && this->czas_niwelowania_gor > 0)
        return true;
    if (teren == rzeka && this->czas_zasypywania_rzek > 0)
        return true;
    return false;
}

class Rebelianci : public ObiektRuchomy {
public:
    char DlaKogoWalcze() const;
    virtual ~Rebelianci() = default;
};

char Rebelianci::DlaKogoWalcze() const {
    return 'R';
}

class Rebeliant : public Rebelianci {
    //R
    int numer_rebelianta;
public:
    ~Rebeliant() = default;
    string etykieta() const;
    Rebeliant(int, int, int, int, int);
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *);
};

/**
 * Okresla co stanie sie po wejsciu obiektu na pole zajmowane przez inny obiekt
 * @param obiekt obiekt znajdujacy sie na polu na ktore wchodzi
 * @return 0 - przegrywa i jest usuwany, 1 - wygrywa i zostaje na polu, 2 - ruch nie jest mozliwy, 
 * 3 - wymaga sprawdzenia okolicy
 */
int Rebeliant::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    if (obiekt->DlaKogoWalcze() == 'I') {
        //przegrywa i jest usuwany
        return 0;
    }
    //jesli pole zajmowane przez rebelianta to ruch nie jest mozliwy
    return 2;
}

/**
 * Metoda pomocnicza, zwraca etykiete obiektu. Potrzebna m. in. do rysowania obiektu na planszy.
 * @return Etykieta obiektu.
 */
string Rebeliant::etykieta() const {
    string etykieta;
    etykieta.append("R");
    ostringstream numer;
    numer << numer_rebelianta;
    etykieta.append(numer.str());
    return etykieta;

}

Rebeliant::Rebeliant(int i, int s, int las, int skaly, int rzeki) {
    szybkosc = s;
    numer_rebelianta = i;
    czas_karczowania_lasu = las;
    czas_zasypywania_rzek = rzeki;
    czas_niwelowania_gor = skaly;
    rozstawiaDD = true;
}

bool Rebeliant::CzyMozeWejscNaPole(FormyTerenu teren) const {
    if (teren == ocean) {
        return false;
    } else
        return true;
}

class Programista : public Rebelianci {
    //P
    int numer_programisty;
public:
    ~Programista() = default;
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    string etykieta() const;
    Programista(int, int);
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *);
};

Programista::Programista(int i, int s) {
    szybkosc = s;
    numer_programisty = i;
}

int Programista::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    if (obiekt->DlaKogoWalcze() == 'I') {
        //RobotZolnierz
        if (obiekt->etykieta()[0] == 'Z') {
            return 1; //wygrywa zostaje na polu
        }
        return 0;
    }
    return 2;
}

string Programista::etykieta() const {
    string etykieta;
    etykieta.append("P");
    ostringstream numer;
    numer << numer_programisty;
    etykieta.append(numer.str());
    return etykieta;
}

bool Programista::CzyMozeWejscNaPole(FormyTerenu teren) const {
    if (teren == teren_plaski)
        return true;
    return false;
}

class LukeSkywalker : public Rebelianci {
    //L
public:
    ~LukeSkywalker() = default;
    LukeSkywalker(int);
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *);

    string etykieta() const {
        return "L ";
    }
};

LukeSkywalker::LukeSkywalker(int s) {
    szybkosc = s;
}

int LukeSkywalker::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    //wygrywa z Lordem Vaderem, z pozostalymi przegrywa
    if (obiekt->DlaKogoWalcze() == 'I') {
        if (obiekt->etykieta()[0] == 'V') {
            return 1;
        }
        return 0;
    }
    return 2;
}

bool LukeSkywalker::CzyMozeWejscNaPole(FormyTerenu teren) const {
    return true;
}

class KsiezniczkaLeia : public Rebelianci {
    //K
public:
    KsiezniczkaLeia(int, int, int, int);
    ~KsiezniczkaLeia() = default;
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *);

    string etykieta() const {
        return "K ";
    }
};

KsiezniczkaLeia::KsiezniczkaLeia(int s, int las, int skaly, int rzeki) {
    szybkosc = s;
    czas_karczowania_lasu = las;
    czas_zasypywania_rzek = rzeki;
    czas_niwelowania_gor = skaly;
    rozstawiaDD = true;
}

int KsiezniczkaLeia::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    if (obiekt->DlaKogoWalcze() == 'I') {
        if (obiekt->etykieta()[0] == 'I') {
            return 3; //wymaga sprawdzenia czy w okolicy pola jest Luke lub Programista
        }
        return 0;
    }
    return 2;
}

bool KsiezniczkaLeia::CzyMozeWejscNaPole(FormyTerenu teren) const {
    if (teren == ocean)
        return false;
    else
        return true;
}

class WyslannicyImperium : public ObiektRuchomy {
public:
    virtual ~WyslannicyImperium() = default;
    char DlaKogoWalcze() const;
};

char WyslannicyImperium::DlaKogoWalcze() const {
    return 'I';
}

class RobotZolnierz : public WyslannicyImperium {
    //Z
    int numer_zolnierza;
public:
    ~RobotZolnierz() = default;
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *);

    string etykieta() const;
    RobotZolnierz(int, int);
};

RobotZolnierz::RobotZolnierz(int i, int s) {
    szybkosc = s;
    numer_zolnierza = i;
}

string RobotZolnierz::etykieta() const {
    string etykieta;
    etykieta.append("Z");
    ostringstream numer;
    numer << numer_zolnierza;
    etykieta.append(numer.str());
    return etykieta;
}

int RobotZolnierz::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    if (obiekt->DlaKogoWalcze() == 'R') {
        return 1;
    }
    return 2;
}

bool RobotZolnierz::CzyMozeWejscNaPole(FormyTerenu teren) const {
    if (teren == teren_plaski) {
        return true;
    }
    return false;
}

class LordVader : public WyslannicyImperium {
    //V
public:
    LordVader(int);
    ~LordVader() = default;
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy *);

    string etykieta() const {
        return "V ";
    }
};

LordVader::LordVader(int s) {
    szybkosc = s;
}

int LordVader::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    if (obiekt->DlaKogoWalcze() == 'R') {
        return 1;
    }
    return 2;
}

bool LordVader::CzyMozeWejscNaPole(FormyTerenu teren) const {
    return true;
}

class Kanclerz : public WyslannicyImperium {
    //I
    int szybkosc = 0;
public:
    bool CzyMozeWejscNaPole(FormyTerenu) const;
    int WchodziNaPoleZajmowanePrzez(ObiektRuchomy*);
    ~Kanclerz() = default;

    string etykieta() const {
        return "I ";
    }
};

int Kanclerz::WchodziNaPoleZajmowanePrzez(ObiektRuchomy* obiekt) {
    //ruch nie jest mozliwy
    return 2;
}

bool Kanclerz::CzyMozeWejscNaPole(FormyTerenu teren) const {
    return false;
}

class Pole {
    friend class Plansza;
    int indeks;
    bool widoczne;
    bool znany_teren;
    bool detektor;
    FormyTerenu teren;
    Pole(const Pole &) = delete;
    Pole& operator=(Pole const &) = delete;
    void UstalIndeks(int);
    void UstawDetektor();
    void UsunDetektor();
    bool CzyDetektor() const;
public:
    Pole();
    ~Pole();
    int ZwrocIndeks() const;
    void PrzypiszFormeTerenu(char);
    string StojacyObiekt();
    bool Wolne() const;
    bool Widoczne() const;
    bool PoznanyTeren() const;
    FormyTerenu Teren() const;
    bool SetObiektRuchomy(ObiektRuchomy *);
    ObiektRuchomy * stojacy_obiekt;
    void UwidocznijPole();
    void UkryjPole();
    void PoznajTeren();
    void UsunStojacyObiekt();
    bool CzyZajmowanePrzezRebelianta() const;
    void ZmienTerenNaPlaski();
    bool CzyObiektMozeWejsc(ObiektRuchomy *) const;
    char EtykietaStojacegoObiektu() const;
};

Pole::Pole() {
    stojacy_obiekt = NULL;
    widoczne = false;
    znany_teren = false;
}

Pole::~Pole(){
    if (stojacy_obiekt != NULL){
        delete stojacy_obiekt;
    }
}

/** Zwraca jednoliterowa etykiete stojacego na polu obiektu
 */
char Pole::EtykietaStojacegoObiektu() const {
    if (!this->Wolne()) {
        return this->stojacy_obiekt->etykieta()[0];
    }
    return ' ';
}

FormyTerenu Pole::Teren() const {
    return this->teren;
}

void Pole::ZmienTerenNaPlaski() {
    this->teren = teren_plaski;
}

bool Pole::Widoczne() const {
    if (this->CzyDetektor()) {
        return true;
    }
    return widoczne;
}

bool Pole::PoznanyTeren() const {
    return znany_teren;
}

void Pole::UwidocznijPole() {
    widoczne = true;
}

void Pole::UkryjPole() {
    widoczne = false;
}

void Pole::PoznajTeren() {
    znany_teren = true;
}

void Pole::UstalIndeks(int i) {
    indeks = i;
}

void Pole::UsunStojacyObiekt() {
    this->stojacy_obiekt = NULL;
}

int Pole::ZwrocIndeks() const {
    return this->indeks;
}

bool Pole::Wolne() const {
    if (this->stojacy_obiekt == NULL)
        return true;
    else
        return false;
}

bool Pole::CzyZajmowanePrzezRebelianta() const {
    if (this->stojacy_obiekt->DlaKogoWalcze() == 'R') {
        return true;
    } else
        return false;
}

/**
 * Zwraca string reprezentujacy obiekt stojacy na danym polu, jesli pole jest puste to zwraca '?', jesli pole jest niewidoczne to '*'.
 * Kolor odpowiada formie terenu.
 * Kolor jedynie w przypadku poznanych form terenu
 * @return Etykieta reprezentujaca obiekt stojacy na danym polu.
 */
string Pole::StojacyObiekt() {
    string etykieta;
    if (this->PoznanyTeren()) {
        switch (this->teren) {
            case teren_plaski: etykieta.append("\033[1;35m");
                break; //magnata
            case las: etykieta.append("\033[1;32m");
                break; //zielony
            case skaly: etykieta.append("\033[1;31m");
                break; //czerwony
            case rzeka: etykieta.append("\033[1;36m");
                break; //cyan
            case ocean: etykieta.append("\033[1;34m");
                break; //niebieski
        }
    }
    if (this->Widoczne()) {
        if (this->Wolne()) {
            etykieta.append("? ");
        } else
            etykieta.append(this->stojacy_obiekt->etykieta()); //etykieta obiektu zajmujacego pole
    } else
        etykieta.append("* ");
    if (this->PoznanyTeren())
        etykieta.append("\033[0m");
    return etykieta;
}

/**
 * Przypisuje forme terenu na podstawie char
 * @param teren_forma l - las, p - teren_plaski, s - skaly, r - rzeka, o - ocean
 */
void Pole::PrzypiszFormeTerenu(char teren_forma) {
    switch (teren_forma) {
        case 'p': this->teren = teren_plaski;
            break;
        case 'l': this->teren = las;
            break;
        case 's': this->teren = skaly;
            break;
        case 'r': this->teren = rzeka;
            break;
        case 'o': this->teren = ocean;
            break;
    }

}

bool Pole::CzyDetektor() const {
    return this->detektor;
}

void Pole::UstawDetektor() {
    if (!this->detektor)
        detektor = true;
}

void Pole::UsunDetektor() {
    if (this->detektor)
        detektor = false;
}

bool Pole::SetObiektRuchomy(ObiektRuchomy * obiekt) {
    if (this->Wolne()) {
        this->stojacy_obiekt = obiekt;
        return true;
    } else
        return false;
}

bool Pole::CzyObiektMozeWejsc(ObiektRuchomy* obiekt) const {
    FormyTerenu teren = this->teren;
    return obiekt->CzyMozeWejscNaPole(teren);
}

class Plansza {
    int wymiary_planszy_x;
    int wymiary_planszy_y;
    Pole * plansza;
    void AktualizujWidocznoscPolSasiednich(Pole *, int);
    void PoznajTerenAktualizujWidocznosc(Pole *, int, bool, bool, bool);
public:
    Plansza(int, int, string); //konstruktor
    void rysuj() const;
    ~Plansza();
    Plansza& operator=(Plansza const &) = delete;
    Pole * SetObiektRuchomy(int, ObiektRuchomy *);
    Pole * ZnajdzPoleWKierunku(char, const Pole *, int);
    void Uwidocznij(Pole *, int);
    void Ukryj(Pole *, int);
    void PoznajTeren(Pole *, int);
    void UstawDetektorNaPolu(Pole *);
    void UsunDetektorZPola(Pole *);
    bool SprawdzenieOkolicyPola(Pole *);
};

/**
 * Konstruktor planszy.
 * @param wymiar_x Liczba kolumn.
 * @param wymiar_y Liczba wierszy.
 * @param teren String w ktorym kazda litera odpowiada formie terenu, 
 * p - teren plaski, l - las, s - skaly, r - rzeka, o - ocean
 */
Plansza::Plansza(int wymiar_x, int wymiar_y, string teren) {
    wymiary_planszy_x = wymiar_x; //height
    wymiary_planszy_y = wymiar_y; //width

    plansza = new Pole[wymiary_planszy_x * wymiary_planszy_y]; //tablica elementow klasy Pole

    //przypisanie formy terenu
    int i = 0;
    for (int row = 0; row < this->wymiary_planszy_y; row++) {
        for (int col = 0; col < this->wymiary_planszy_y; col++) {
            plansza[row * wymiary_planszy_y + col].PrzypiszFormeTerenu(teren[i]);
            plansza[row * wymiary_planszy_y + col].UstalIndeks(row * wymiary_planszy_y + col);
            i++;
        }
    }
}

Plansza::~Plansza() {
    delete[] plansza;
}

Pole * Plansza::SetObiektRuchomy(int pozycja, ObiektRuchomy * obiekt) {
    plansza[pozycja].SetObiektRuchomy(obiekt);
    return &plansza[pozycja];
}

/**
 * Rysuje aktualna plansze.
 */
void Plansza::rysuj() const {
    char plansza_naglowek[] = "----+";

    for (int row = 0; row < this->wymiary_planszy_y; row++) {
        //linie przedzielejace
        cout << "-";
        for (int col = 0; col < this->wymiary_planszy_y; col++) {
            cout << plansza_naglowek;
        }
        cout << endl;
        //obiekty stojace na danym polu
        cout << "| ";
        for (int col = 0; col < this->wymiary_planszy_y; col++) {
            cout << this->plansza[row * wymiary_planszy_y + col].StojacyObiekt() << " | ";
        }
        cout << endl;
    }
    //zakonczenie planszy
    cout << "-";
    for (int col = 0; col < this->wymiary_planszy_y; col++) {
        cout << plansza_naglowek;
    }
    cout << endl;
}

/**
 * Znajduje pole na ktore nastepuje przejscie, jesli przejscie jest niemozliwe lub nie ma takiego pola zwraca null.
 * @param kierunek char oznaczajacy kierunek. 
 * Mozliwe kierunki to P - prawy, L - lewy, D - dol, G - gora, Q - lewy gorny rog, W - prawy gorny rog, E - lewy dolny rog, R - prawy dolny rog
 * @param p Wskaznik do aktualnie zajmowanego pola.
 * @param Szybkosc szybkosc przesuwanego obiektu.
 * @return Wskaznik do pola na ktore nastepuje przejscie obiektu, pod warunkiem ze przejscie jest mozliwe, wpp NULL.
 */
Pole * Plansza::ZnajdzPoleWKierunku(char kierunek, const Pole * p, int szybkosc) {
    int indeks_aktualnego_pola = p->indeks;
    int indeks_nowego_pola;
    int wiersz = indeks_aktualnego_pola / this->wymiary_planszy_x;
    int kolumna = indeks_aktualnego_pola % this->wymiary_planszy_x;
    int wiersz_nowe_pole = wiersz;
    int kolumna_nowe_pole = kolumna;
    switch (kierunek) {
        case 'G':
            for (int i = 0; i < szybkosc; i++) {
                //jesli nie jest to najwyzszy rzad - mozna przejsc do gory
                if (wiersz_nowe_pole > 0) {
                    wiersz_nowe_pole--;
                }
            }
            break;
        case 'D':
            for (int i = 0; i < szybkosc; i++) {
                //jesli nie jest to ostatni rzad - mozna przejsc do gory
                if (wiersz_nowe_pole < this->wymiary_planszy_x * (this->wymiary_planszy_y - 1)) {
                    wiersz_nowe_pole++;
                }
            }
            break;
        case 'L':
            for (int i = 0; i < szybkosc; i++) {
                if (kolumna_nowe_pole > 0) {
                    kolumna_nowe_pole--;
                }
            }
            break;
        case 'P':
            for (int i = 0; i < szybkosc; i++) {
                if (kolumna_nowe_pole < this->wymiary_planszy_x - 1) {
                    kolumna_nowe_pole++;
                }
            }
            break;
        case 'Q'://lewy ukos gorny
            for (int i = 0; i < szybkosc; i++) {
                //jesli nie jest pirwszy wiersz i pierwsza kolumna
                if (wiersz_nowe_pole > 0 && kolumna_nowe_pole > 0) {
                    kolumna_nowe_pole--;
                    wiersz_nowe_pole--;
                }
            }
            break;
        case 'W'://prawy ukos gorny
            for (int i = 0; i < szybkosc; i++) {
                if (wiersz_nowe_pole > 0 && kolumna_nowe_pole < this->wymiary_planszy_x - 1) {
                    kolumna_nowe_pole++;
                    wiersz_nowe_pole--;
                }
            }
            break;
        case 'E'://lewy ukos dolny
            for (int i = 0; i < szybkosc; i++) {
                if (wiersz_nowe_pole < this->wymiary_planszy_x * (this->wymiary_planszy_y - 1) && kolumna_nowe_pole > 0) {
                    kolumna_nowe_pole--;
                    wiersz_nowe_pole++;
                }
            }
            break;
        case 'R'://prawy ukos dolny
            for (int i = 0; i < szybkosc; i++) {
                if (wiersz_nowe_pole < this->wymiary_planszy_x * (this->wymiary_planszy_y - 1) && kolumna_nowe_pole < this->wymiary_planszy_x - 1) {
                    kolumna_nowe_pole++;
                    wiersz_nowe_pole++;
                }
            }
            break;
        default:
            return NULL;
            break;
    }
    if (wiersz != wiersz_nowe_pole || kolumna != kolumna_nowe_pole) {
        indeks_nowego_pola = wiersz_nowe_pole * this->wymiary_planszy_x + kolumna_nowe_pole;
    } else
        return NULL;
    return &plansza[indeks_nowego_pola];

}

/**
 * Aktualizuje zmienne zwiazane z widocznoscia pola i znajomoscia terenu dla pola i pol sasiednich.
 * @param pole Wskaznik do pola.
 * @param promien Promien poznawanego terenu.
 * @param teren True - uwidocznij teren.
 * @param widocznosc True - uwidocznij pole.
 * @param ukryj True - ukryj pole.
 */
void Plansza::PoznajTerenAktualizujWidocznosc(Pole* pole, int promien, bool teren, bool widocznosc, bool ukryj) {
    int indeks = pole->ZwrocIndeks();
    int wiersz = indeks / this->wymiary_planszy_x;
    int kolumna = indeks % this->wymiary_planszy_x;
    for (int i = -1 * promien; i <= promien; i++) {
        for (int j = -1 * promien; j <= promien; j++) {
            int w = wiersz - i;
            int k = kolumna - j;
            if (w >= 0 && w < this->wymiary_planszy_y && k >= 0 && k < this->wymiary_planszy_x) {
                int p = w * this->wymiary_planszy_x + k;
                //aktualizacja terenu
                if (teren) {
                    if (!this->plansza[p].PoznanyTeren()) {
                        this->plansza[p].PoznajTeren();
                    }
                }
                //ustalenie widocznosci
                if (widocznosc) {
                    if (!this->plansza[p].Widoczne()) {
                        this->plansza[p].UwidocznijPole();
                    }
                }
                //ukrycie pola
                if (ukryj) {
                    if (this->plansza[p].Widoczne()) {
                        this->plansza[p].UkryjPole();
                    }
                }
            }
        }
    }
}

/**
 * Znajduje wszystkie pola zajmowane przez rebeliantow w sasiedztwie pola i ustawia ich widocznosc.
 * Metoda pomocnicza przy aktualizacji widocznosci. Pola sasiednie sprawdzane w odleglosci 2*promien.
 * @param pole Pole dla ktorego znajdowane sa sasiednie pola.
 * @param promien Promien widocznosci.
 */
void Plansza::AktualizujWidocznoscPolSasiednich(Pole* pole, int promien) {
    int indeks = pole->ZwrocIndeks();
    int wiersz = indeks / this->wymiary_planszy_x;
    int kolumna = indeks % this->wymiary_planszy_x;
    for (int i = -2 * promien; i <= 2 * promien; i++) {
        for (int j = -2 * promien; j <= 2 * promien; j++) {
            int w = wiersz - i;
            int k = kolumna - j;
            if (w >= 0 && w < this->wymiary_planszy_y && k >= 0 && k < this->wymiary_planszy_x) {
                int p = w * this->wymiary_planszy_x + k;
                //sprawdzenie czy pole sasiednie zajmowane przez rebelianta
                if (!this->plansza[p].Wolne()) {
                    if (this->plansza[p].CzyZajmowanePrzezRebelianta()) {
                        PoznajTerenAktualizujWidocznosc(&this->plansza[p], promien, false, true, false);
                    }
                }
            }
        }
    }
}

void Plansza::PoznajTeren(Pole* pole, int promien) {
    this->PoznajTerenAktualizujWidocznosc(pole, promien, true, false, false);
}

void Plansza::Uwidocznij(Pole* pole, int promien) {
    this->PoznajTerenAktualizujWidocznosc(pole, promien, false, true, false);
}

void Plansza::Ukryj(Pole* pole, int promien) {
    this->PoznajTerenAktualizujWidocznosc(pole, promien, false, false, true);
    this->AktualizujWidocznoscPolSasiednich(pole, promien);
}

void Plansza::UstawDetektorNaPolu(Pole* pole) {
    if (!pole->CzyDetektor()) {
        pole->UstawDetektor();
    }
}

void Plansza::UsunDetektorZPola(Pole* pole) {
    if (pole->CzyDetektor()) {
        pole->UsunDetektor();
    }
}

/**
 * Sprawdza czy w sasiedztwie pola nie znajduje sie Luke lub Programista
 * @param pole Pole ktorego okolica jest sprawdzana.
 * @return True - w sasiedztwie znajduje sie Luke lub Programista, wpp False
 */
bool Plansza::SprawdzenieOkolicyPola(Pole* pole) {
    int indeks = pole->ZwrocIndeks();
    int wiersz = indeks / this->wymiary_planszy_x;
    int kolumna = indeks % this->wymiary_planszy_x;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int w = wiersz - i;
            int k = kolumna - j;
            if (w >= 0 && w < this->wymiary_planszy_y && k >= 0 && k < this->wymiary_planszy_x) {
                int p = w * this->wymiary_planszy_x + k;
                if (this->plansza[p].EtykietaStojacegoObiektu() == 'L' || this->plansza[p].EtykietaStojacegoObiektu() == 'P') {
                    return true;
                }
            }
        }
    }
    return false;
}

unordered_map<string, string> read_params(string file_name) {

    ifstream fin(file_name);
    if (!fin.is_open()) {
        cout << "Cannot open file " << file_name << endl;
        cout << "Terminate execution" << endl;
    }
    unordered_map<string, string> m;
    string par_name;
    string par_value;
    while (fin >> par_name >> par_value)
        m[par_name] = par_value;
    return m;
}

void print_params(unordered_map<string, Pole *> m) {
    for (auto par : m)
        cout << par.first << ": " << par.second << endl;
}

/**
 * Funkcja pomocnicza, do sprawdzenia czy szybkosc obiektow jest podana w parametrach dla wszystkich obiektow.
 * @param m Slownik parametrow z pliku konfiguracyjnego.
 * @return Zwraca slownik postaci <etykieta_str np 'R', int_szybkosc>.
 */
unordered_map<char, int> szybkosc_obiektow_ruchomych(unordered_map<string, string> m) {
    unordered_map<char, int> szybkosci;
    for (auto par : m) {
        if (par.first.substr(0, 9) == "szybkosc_") {
            int szybkosc_int;
            sscanf(par.second.c_str(), "%d", &szybkosc_int);
            szybkosci[par.first.substr(9, 1)[0]] = szybkosc_int;
        }
    }
    //sprawdzenie czy wszystkie szybkosci sa podane
    try {
        szybkosci['V'];
        szybkosci['Z'];
        szybkosci['K'];
        szybkosci['L'];
        szybkosci['P'];
        szybkosci['R'];
    } catch (int e) {
        cout << "An exception occurred. Exception Nr. " << e << '\n';
        cout << "Bledny plik z parametrami, brak ustalonej szybkosci dla wszystkich obiektow" << endl;
    }
    return szybkosci;
}

unordered_map<string, Pole *> rozmieszczenie_obiektow_ruchomych(unordered_map<string, string> m, Plansza * p, unordered_map<char, int> szybkosci_obiektow, int promien, int czas_karczowania_lasu, int czas_zasypywania_rzek, int czas_niwelowania_gor) {
    //tworzy obiekty ruchome, nadaje im szybkosc zgodna z plikiem wejsciowym i umieszcza na poczatkowych polach
    //w wyniku slownik postaci <str_etykieta, wskaznik do zajmowanego pola>
    int liczba_rebeliantow = 1;
    int liczba_programistow = 1;
    int liczba_zolnierzy = 1;
    unordered_map<string, Pole *> obiekty; //pola zajmowane przez obiekty
    for (auto par : m) {//tworzy obiekty ktore sa na polach i przypisuje do odpowiednich pol na planszy
        ObiektRuchomy * postac;
        string obiekt = par.first;
        if (obiekt.substr(0, 8) == "pozycja_") {
            char typ_obiektu = obiekt.substr(8, 1)[0];
            switch (typ_obiektu) {
                case 'I': postac = new Kanclerz;
                    break;
                case 'V': postac = new LordVader(szybkosci_obiektow['V']);
                    break;
                case 'Z': postac = new RobotZolnierz(liczba_zolnierzy, szybkosci_obiektow['Z']);
                    liczba_zolnierzy++;
                    break;
                case 'K': postac = new KsiezniczkaLeia(szybkosci_obiektow['K'], czas_karczowania_lasu, czas_niwelowania_gor, czas_zasypywania_rzek);
                    break;
                case 'L': postac = new LukeSkywalker(szybkosci_obiektow['L']);
                    break;
                case 'P': postac = new Programista(liczba_programistow, szybkosci_obiektow['P']);
                    liczba_programistow++;
                    break;
                case 'R': postac = new Rebeliant(liczba_rebeliantow, szybkosci_obiektow['R'], czas_karczowania_lasu, czas_niwelowania_gor, czas_zasypywania_rzek);
                    liczba_rebeliantow++;
                    break;
            }
            //przypisanie obiektu do odpowiadajacego mu pola
            string pozycja = par.second;
            int pozycja_int;
            sscanf(pozycja.c_str(), "%d", &pozycja_int);
            Pole * pole;
            pole = p->SetObiektRuchomy(pozycja_int, postac);
            if (postac->DlaKogoWalcze() == 'R') {
                //aktualizacja widocznosci
                p->Uwidocznij(pole, promien);
                //poznanie terenu
                p->PoznajTeren(pole, promien);
            }
            obiekty[postac->etykieta()] = pole;
        }
    }
    return obiekty;
}

/**Przesuwa obiekt o podanej etykiecie we wskazanym kierunku.
 * Dozwolone kierunki: P - prawy, L - lewy, D - dol, G - gora, Q - lewy gorny rog, W - prawy gorny rog, E - lewy dolny rog, R - prawy dolny rog
 * aktualne_pole Wskaznik do pola na ktorym znajduje sie obiekt
 * nowe pole Pole na ktore nastepuje przesuniecie
 */
bool PrzesunObiekt(string obiekt_etykieta, char kierunek, Pole * aktualne_pole, Plansza * plansza, int promien, int &czas_gry, unordered_map<string, Pole *> &obiekty_pola) {
    ObiektRuchomy * obiekt = aktualne_pole->stojacy_obiekt;
    Pole * nowe_pole = plansza->ZnajdzPoleWKierunku(kierunek, aktualne_pole, obiekt->PodajSzybkosc());
    char strona_obiektu = obiekt->DlaKogoWalcze();
    if (nowe_pole != NULL) {//jesli pole w tym kierunku istnieje
        if (obiekt->CzyMozeWejscNaPole(nowe_pole->Teren())) {//sprawdzenie czy obiekt moze wejsc na dane pole - ograniczenia zwiazane z terenem
            //jesli moze to sprawdzenie czy je modyfikuje i dodanie czasu potrzebneg na ta modyfikacje
            //modyfikacja tylko dla rebeliantow
            if (obiekt->CzyModyfikujePole(nowe_pole->Teren()) && obiekt->DlaKogoWalcze() == 'R') {
                czas_gry += obiekt->CzasPotrzebnyNaModyfikacje(nowe_pole->Teren());
                //po zmodyfikowaniu teren plaski
                nowe_pole->ZmienTerenNaPlaski();
            }
            //jesli nowe pole jest wolne to wstawiany jest na nie obiekt 
            if (nowe_pole->Wolne()) {
                aktualne_pole->UsunStojacyObiekt();
                nowe_pole->SetObiektRuchomy(obiekt);
                if (obiekt->DlaKogoWalcze() == 'R') {
                    plansza->Ukryj(aktualne_pole, promien);
                    plansza->Uwidocznij(nowe_pole, promien);
                    plansza->PoznajTeren(nowe_pole, promien);
                }
                obiekty_pola[obiekt_etykieta] = nowe_pole;
            } else {//jesli pole nie jest wolne
                int wynik_rozgrywki = obiekt->WchodziNaPoleZajmowanePrzez(nowe_pole->stojacy_obiekt);
                if (wynik_rozgrywki == 2) {
                    //ruch nie jest mozliwy
                    return false;
                }
                if (wynik_rozgrywki == 0) {//obiekt przegrywa i jest usuwany                   
                    aktualne_pole->UsunStojacyObiekt();
                    if (obiekt->DlaKogoWalcze() == 'R') {
                        plansza->Ukryj(aktualne_pole, promien);
                    }
                    delete obiekt;
                    obiekt = NULL;
                    obiekty_pola[obiekt_etykieta] = NULL;
                }
                if (wynik_rozgrywki == 1) {
                    //obiekt wygrywa i wchodzi na pole
                    aktualne_pole->UsunStojacyObiekt();
                    obiekty_pola[nowe_pole->stojacy_obiekt->etykieta()] = NULL;
                    delete nowe_pole->stojacy_obiekt;
                    nowe_pole->stojacy_obiekt = NULL;
                    nowe_pole->SetObiektRuchomy(obiekt);
                    if (obiekt->DlaKogoWalcze() == 'R') {
                        plansza->Ukryj(aktualne_pole, promien);
                        plansza->Uwidocznij(nowe_pole, promien);
                        plansza->PoznajTeren(nowe_pole, promien);
                    }
                    if (obiekt->DlaKogoWalcze() == 'I') {
                        plansza->UsunDetektorZPola(nowe_pole);
                    }
                    obiekty_pola[obiekt_etykieta] = nowe_pole;
                }
                if (wynik_rozgrywki == 3) {
                    if (plansza->SprawdzenieOkolicyPola(nowe_pole)) {
                        //kanclerz przechodzi na dobra strone mocy
                        //wygrywaja rebelianci
                        cout << "Wygrana Rebeliantow!\n" << "Kanclerz przeszedl na dobra strone mocy" << endl;
                    } else {
                        //ksiezniczka leia zostaje pojmana
                        cout << "Przegrana Rebeliantow!" << endl;
                    }
                    exit(0);
                }
            }
            return true;
        } else {
            return false; //nie moze wejsc na to pole ze wzgledu na teren
        }
    }
    return false; //ruch w tym kierunku nie jest mozliwy
}

/**
 * Losuje etykete obecnego na planszy wyslannika imperium (z wyjatkiem Kanclerza, ktory nie moze sie poruszac).
 * @param obiekty_pola Slownik z kluczem bedacym etykieta obiektu i wartosci bedaca wskaznikiem do zajowanego pola.
 * @return Etykiete losowego wyslannika imperium.
 */
string LosowyWyslannikImperium(unordered_map<string, Pole *> &obiekty_pola) {
    vector<string> etykiety;
    for (auto par : obiekty_pola) {
        if (par.second != NULL) {
            if (par.first[0] == 'Z' || par.first[0] == 'V') {
                etykiety.push_back(par.first);
            }
        }
    }
    srand(time(NULL));
    int randomIndex = rand() % etykiety.size();
    return etykiety[randomIndex];
}

/**
 * Losuje kierunek ruchu.
 * @return char reprezentujacy losowy kierunek ruchu.
 */
char LosowyKierunek() {
    char kierunki[] = {'Q', 'G', 'W', 'L', 'P', 'E', 'D', 'R'};
    srand(time(NULL));
    int randomIndex = rand() % 8;
    return kierunki[randomIndex];
}

/*
 * 
 */
int main(int argc, char** argv) {

    //Przebieg gry
    int czas_gry = 0;
    unordered_map<string, string> parametry = read_params("parametry.txt"); //slownik zawierajacy parametry gry

    //zmienne przechowujace parametry gry
    int wiersze;
    int kolumny;
    int promien_poznawanego_terenu;
    int czas_karczowania_lasu;
    int czas_zasypywania_rzek;
    int czas_niwelowania_gor;
    int liczba_kolejek;

    istringstream(parametry["wymiar_planszy_wiersze"]) >> wiersze;
    istringstream(parametry["wymiar_planszy_kolumny"]) >> kolumny;
    istringstream(parametry["promien_poznawanego_terenu"]) >> promien_poznawanego_terenu;
    istringstream(parametry["czas_karczowania_lasu"]) >> czas_karczowania_lasu;
    istringstream(parametry["czas_zasypywania_rzek"]) >> czas_zasypywania_rzek;
    istringstream(parametry["czas_niwelowania_gor"]) >> czas_niwelowania_gor;
    istringstream(parametry["liczba_kolejek"]) >> liczba_kolejek;

    //przypisanie formy terenu do kazdego pola
    Plansza p = Plansza(wiersze, kolumny, parametry["teren"]);
    unordered_map<char, int> szybkosci_obiektow = szybkosc_obiektow_ruchomych(parametry);
    unordered_map<string, Pole *> obiekty_pola = rozmieszczenie_obiektow_ruchomych(parametry, &p, szybkosci_obiektow, promien_poznawanego_terenu, czas_karczowania_lasu, czas_zasypywania_rzek, czas_niwelowania_gor);

    string rebeliant;
    char kierunek_rebelianta;
    string wyslannik_imperium;
    char kierunek_wyslannika_imperium;

    while (czas_gry < liczba_kolejek) {
        bool poprawny_ruch = false;
        cout << "Ruch rebelianta:\n\n";
        p.rysuj();
        cout << endl;
        while (!poprawny_ruch) {
            cout << "Podaj rebelianta, ktorego chcesz przesunac:" << endl;
            cin >> rebeliant;
            cout << "Podaj kierunek:\nMozliwe kierunki to P - prawy, L - lewy, D - dol, G - gora, Q - lewy gorny rog, W - prawy gorny rog, E - lewy dolny rog, R - prawy dolny rog" << endl;
            cin >> kierunek_rebelianta;
            if (rebeliant == "K" || rebeliant == "L") {
                rebeliant.append(" ");
            }
            auto search = obiekty_pola.find(rebeliant);
            //sprawdzenie czy poprawna etyketa i kierunek
            if ((kierunek_rebelianta == 'P' || kierunek_rebelianta == 'L' || kierunek_rebelianta == 'D' || kierunek_rebelianta == 'G' || kierunek_rebelianta == 'Q' || kierunek_rebelianta == 'W' || kierunek_rebelianta == 'E' || kierunek_rebelianta == 'R') && (search != obiekty_pola.end()) && (search->second != NULL)) {
                poprawny_ruch = PrzesunObiekt(rebeliant, kierunek_rebelianta, obiekty_pola[rebeliant], &p, promien_poznawanego_terenu, czas_gry, obiekty_pola);
            }
        }
        p.rysuj();
        cout << endl;
        czas_gry += 1;
        cout << "Ruch komputera:\n\n";
        bool prawidlowy_ruch_komputera = false;
        while (!prawidlowy_ruch_komputera) {
            wyslannik_imperium = LosowyWyslannikImperium(obiekty_pola);
            kierunek_wyslannika_imperium = LosowyKierunek();
            prawidlowy_ruch_komputera = PrzesunObiekt(wyslannik_imperium, kierunek_wyslannika_imperium, obiekty_pola[wyslannik_imperium], &p, promien_poznawanego_terenu, czas_gry, obiekty_pola);
        }
        p.rysuj();
        cout << endl;
    }
    cout << "Czas gry minal, nie udalo sie pokonac wyslannikow imeprium.\nRebelianci przegrywaja." << endl;
    return 0;
}