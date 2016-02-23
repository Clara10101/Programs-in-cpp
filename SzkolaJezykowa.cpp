/* 
 * File:   main.cpp
 * Author: Klara
 *
 * Created on 21 luty 2016, 17:58
 */

#include <iostream>

using namespace std;

//mozliwe poziomy zaawansowania

enum PoziomZaawansowania {
    A1, A2, B1, B2, C1, C2
};

int liczbaKursow = 10;
int liczbaGrup = 15;
int liczbaOsob = 20;

class Osoba;
class Nauczyciel;
class Chetny;

class KursJezykowy {
    string jezyk;
    PoziomZaawansowania poziom;
    int liczbaGodzin;
    int cena;
public:
    //konstruktor i destruktor
    KursJezykowy(); //konstruktor bezargumentowy
    KursJezykowy(string, PoziomZaawansowania, int, int);
    ~KursJezykowy() = default; //destruktor domyslny
    void opisKursu();
    //konstruktor kopiujacy domyslny
    KursJezykowy(const KursJezykowy&) = default;
    //domyslna operacja przypisania
    KursJezykowy& operator=(KursJezykowy const &) = default;
};

class Grupa {
    string nazwaGrupy;
    string terminDzien;
    int terminGodzina;
    int numerSali;
    int limitMiejsc;
    Nauczyciel * prowadzacy;
    Chetny ** listaZapisanych;
    int licznikZapisanych;
public:
    Grupa(string, string, int, int, int, Nauczyciel *); //konstruktor
    ~Grupa() = default; //destruktor - valgrid sprawdzenie wycieku pamieci
    Grupa(const Grupa&) = default; //konstruktor kopiujacy domyslny jest wystarczajacy
    Grupa& operator=(Grupa const &) = default;
    void opisGrupy();
    bool ZapiszChetnego(Chetny *);
    void WypiszZapisanych() const;
};

class Osoba {
    string imie;
    string nazwisko;
public:
    Osoba(string, string);//konstruktor
    virtual ~Osoba() = default;//nadklasa z wirtualnym konstruktorem
    Osoba(const Osoba &) = default;//konstruktor kopiujacy doyslny
    Osoba& operator=(Osoba const &) = default;//operacja przypisania domyslna jest wystarczajacy
    string nazwa();
};

class Nauczyciel : public Osoba {
public:
    Nauczyciel(string, string);
    Grupa * rozmowa(Chetny *);//zwraca liste grup do ktorych moze zapisac sie chetny
};

class Chetny : public Osoba {
public:
    Chetny(string, string);
    virtual ~Chetny() = default;
    bool zapisujeSie(Grupa *);
    virtual Grupa * wybierzGrupe(Grupa *) = 0; //wybiera grupe sposrod listy zaproponowanych
};

class NocnyMarek : public Chetny {
public:
    NocnyMarek(string, string);
    ~NocnyMarek() = default;
    Grupa * wybierzGrupe(Grupa *) override;
};

class Pasjonat : public Chetny {
public:
    Pasjonat(string, string);
    Grupa * wybierzGrupe(Grupa *) override;
};

class RannyPtaszek : public Chetny {
public:
    RannyPtaszek(string, string);
    Grupa * wybierzGrupe(Grupa *) override;
};

class Fan : public Chetny {
    Nauczyciel * ulubionyNauczyciel;
public:
    Fan(string, string);
    Grupa * wybierzGrupe(Grupa *) override;
};

class Szkola {
    KursJezykowy ** kursy;
    int licznikKursow;
    Grupa ** grupy;
    int licznikGrup;
    Nauczyciel ** nauczyciele;
    int licznikNauczycieli;
public:
    Szkola(); //konstruktor
    ~Szkola(); //destruktor
    Szkola(const Szkola&) = delete; //konstruktor kopiujacy
    Szkola& operator=(Szkola const &) = delete; //nie ma mozliwosci kopiowania szkoly
    bool zgloszenie(Chetny *);//czy udalo sie zapisac chetnego do jakiejs grupy
    Nauczyciel * przydzielNauczyciela(Chetny *); //przydziela nauczyciela dla chetnego
    Grupa * podajListeGrupDlaChetnego(Chetny *);
    void wypiszKursy();
    void wypiszGrupy();
    void wypiszOsobyWGrupie(const Grupa&) const;
    bool dodajKurs(KursJezykowy);
    bool dodajGrupe(Grupa);
    bool dodajNauczyciela(Nauczyciel);
};

KursJezykowy::KursJezykowy(string j, PoziomZaawansowania p, int g, int c) : jezyk(j), poziom(p), liczbaGodzin(g), cena(c) {
}

string printPoziomZaawansowania(PoziomZaawansowania p) {
    if (p == A1)
        return "A1";
    if (p == A2)
        return "A2";
    if (p == B1)
        return "B1";
    if (p == B2)
        return "B2";
    if (p == C1)
        return "C1";
    if (p == C2)
        return "C2";
}

void KursJezykowy::opisKursu() {
    cout << this->jezyk << " na poziomie " << printPoziomZaawansowania(this->poziom) << ", " << this->liczbaGodzin << " godzin, " << this->cena << " zl" << endl;
}

Osoba::Osoba(string i, string n) : imie(i), nazwisko(n) {
}

string Osoba::nazwa() {
    string nazwa = this->imie;
    nazwa.append(" ");
    nazwa.append(this->nazwisko);
    return nazwa;
}

Nauczyciel::Nauczyciel(string i, string n) : Osoba(i, n) {
}

Nauczyciel * Szkola::przydzielNauczyciela(Chetny* chetny){
    return this->nauczyciele[0];//zawsze przydzielany pierwszy nauczyciel
}

Chetny::Chetny(string i, string n) : Osoba(i, n){
}

bool Chetny::zapisujeSie(Grupa * g){
    return g->ZapiszChetnego(this);
}

NocnyMarek::NocnyMarek(string i, string n) : Chetny(i,n){
}

Grupa * NocnyMarek::wybierzGrupe(Grupa *){
    
}

Grupa::Grupa(string nazwa, string dzien, int godzina, int sala, int limit, Nauczyciel * n) : nazwaGrupy(nazwa), terminDzien(dzien), terminGodzina(godzina), numerSali(sala), limitMiejsc(limit), prowadzacy(n) {
    listaZapisanych = new Chetny * [liczbaOsob];
    licznikZapisanych = 0;
}

void Grupa::opisGrupy() {
    cout << "grupa " << this->nazwaGrupy << ", " << this->terminDzien << " o " << this->terminGodzina << ", w sali " << this->numerSali << ", limit miejsc " << this->limitMiejsc << " osob, prowadzacy " << this->prowadzacy->nazwa() << endl;
}

bool Grupa::ZapiszChetnego(Chetny* chetny){
    if (this->licznikZapisanych < liczbaOsob){
        this->listaZapisanych[licznikZapisanych] = chetny;
        this->licznikZapisanych += 1;
        return true;
    }
    return false;
}

void Grupa::WypiszZapisanych() const{
    cout << "Osoby zapisane do grupy: ";
    for (int i = 0; i < this->licznikZapisanych; i++){
        cout << this->listaZapisanych[i]->nazwa();
    }
    cout << endl;
}

Szkola::Szkola() {
    kursy = new KursJezykowy * [liczbaKursow];
    licznikKursow = 0;
    grupy = new Grupa * [liczbaGrup];
    licznikGrup = 0;
    nauczyciele = new Nauczyciel * [liczbaOsob];
    licznikNauczycieli = 0;
}

Szkola::~Szkola() {
    //usuniecie wszystkic tablic
    delete[] kursy;
    delete[] grupy;
    delete[] nauczyciele;
}

bool Szkola::dodajGrupe(Grupa g){
    if (this->licznikGrup < liczbaGrup){
        this->grupy[licznikGrup] = &g;
        this->licznikGrup += 1;
        return true;
    }
    //nie mozna wstawic, brak miejsca w tablicy
    return false;
}

bool Szkola::dodajKurs(KursJezykowy k){
    if (this->licznikKursow < liczbaKursow){
        this->kursy[licznikKursow] = &k;
        this->licznikKursow += 1;
        return true;
    }
    return false;
}

bool Szkola::dodajNauczyciela(Nauczyciel n){
    if (this->licznikNauczycieli < liczbaOsob){
        this->nauczyciele[licznikNauczycieli] = &n;
        this->licznikNauczycieli += 1;
        return true;
    }
    return false;
}

void Szkola::wypiszGrupy(){
    for (int i = 0; i < this->licznikGrup; i++){
        cout << "Grupa " << i+1 << ":" << endl;
        this->grupy[i]->opisGrupy();
    }
}

void Szkola::wypiszKursy(){
    for (int i = 0; i < this->licznikKursow; i++){
        cout << "Kurs " << i+1 << ":" << endl;
        this->kursy[i]->opisKursu();
    }
}

void Szkola::wypiszOsobyWGrupie(const Grupa& g) const{
    g.WypiszZapisanych();
}

bool Szkola::zgloszenie(Chetny* chetny){
    //przydziel nauczyciela do rozmowy kwalifikacyjnej
    Nauczyciel * n = this->przydzielNauczyciela(chetny);
    //wynik rozmowy z nauczycielem
    /*Grupa * zaproponowaneGrupy = n->rozmowa(chetny);
    Grupa * wybranaGrupa = chetny->wybierzGrupe(zaproponowaneGrupy);
    if (wybranaGrupa != nullptr){
        //chetny zapisuje sie do wybranej grupy
        return chetny->zapisujeSie(wybranaGrupa);
    }*/
    return false;
}


/*
 * 
 */
int main(int argc, char** argv) {
    KursJezykowy kurs1 = KursJezykowy("angielski", B2, 60, 1190);
    //kurs1.opisKursu();
    Nauczyciel n1 = Nauczyciel("Jan", "Kowalski");
    Grupa g1 = Grupa("G34", "sobota", 9, 6, 9, &n1);
    //g1.opisGrupy();
    Grupa g2 = Grupa("G21", "poniedzialek", 10, 8, 16, &n1);
    KursJezykowy kurs2 = KursJezykowy("niemiecki", A1, 90, 1250);
    //Szkola ma zdefiniowana w programie max liczbe kursow, grup, osob
    //Tworzy pusta liste wskaznikow takiej dlugosci
    Szkola * s = new Szkola();
    s->dodajNauczyciela(n1);
    s->dodajGrupe(g1);
    s->dodajGrupe(g2);
    s->wypiszGrupy();
    s->dodajKurs(kurs1);
    s->wypiszKursy();
    //Zapisanie chetnego do grupy
    NocnyMarek kursant1 = NocnyMarek("Joanna","Kowalska");
    kursant1.zapisujeSie(&g1);
    //g1.WypiszZapisanych();
    s->wypiszOsobyWGrupie(g1);
    return 0;
}

