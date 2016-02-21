/* 
 * File:   main.cpp
 * Author: Klara
 *
 * Created on 17 grudzień 2015, 20:17
 */

#include <iostream>

using namespace std;

class Prostokat {
    //reprezentuje pojedynczy prostokat
    int wierzcholek_x1, wierzcholek_x2, wierzcholek_y1, wierzcholek_y2;
public:

    Prostokat(int, int, int, int); //konstruktor

    int daj_wierzcholek_x1() const {
        return wierzcholek_x1;
    }

    int daj_wierzcholek_x2() const {
        return wierzcholek_x2;
    }

    int daj_wierzcholek_y1() const {
        return wierzcholek_y1;
    }

    int daj_wierzcholek_y2() const {
        return wierzcholek_y2;
    }

    bool porownaj(const Prostokat &) const;
    void wypisz() const;
};

Prostokat::Prostokat(int x1, int y1, int x2, int y2) {
    //konstruktor
    //zakladamy ze x1 < x2 i y1 < y2
    wierzcholek_x1 = x1;
    wierzcholek_x2 = x2;
    wierzcholek_y1 = y1;
    wierzcholek_y2 = y2;
}

bool Prostokat::porownaj(const Prostokat &p) const {
    return (wierzcholek_x1 == p.wierzcholek_x1 && wierzcholek_y1 == p.wierzcholek_y1 && wierzcholek_x2 == p.wierzcholek_x2 && wierzcholek_y2 == p.wierzcholek_y2);
}

void Prostokat::wypisz() const {
    cout << "Prostokat((" << wierzcholek_x1 << "," << wierzcholek_y1 << "),(" << wierzcholek_x2 << "," << wierzcholek_y2 << "))\n";
}

class Element_zbioru {
    //reprezentuje pojedynczy element zbioru prostokatow
    //musi zawierac wskaznik do nastepnego elementu aby moznabylo przechodzic po elementach zbioru
    Prostokat * prostokat;
    Element_zbioru * nastepny;
public:
    Element_zbioru(Prostokat *, Element_zbioru *); //konstruktor
    Element_zbioru * getNext() const;
    Prostokat * daj_prostokat() const;
    void wypisz() const;
    int daj_wierzcholek_x1() const;
    int daj_wierzcholek_x2() const;
    int daj_wierzcholek_y1() const;
    int daj_wierzcholek_y2() const;
};

Element_zbioru::Element_zbioru(Prostokat *p, Element_zbioru *n) {
    prostokat = p;
    nastepny = n;
}

Prostokat * Element_zbioru::daj_prostokat() const {
    return prostokat;
}

Element_zbioru * Element_zbioru::getNext() const {
    return nastepny;
}

void Element_zbioru::wypisz() const {
    prostokat->wypisz();
}

int Element_zbioru::daj_wierzcholek_x1() const{
    return prostokat->daj_wierzcholek_x1();
}

int Element_zbioru::daj_wierzcholek_x2() const{
    return prostokat->daj_wierzcholek_x2();
}

int Element_zbioru::daj_wierzcholek_y1() const{
    return prostokat->daj_wierzcholek_y1();
}

int Element_zbioru::daj_wierzcholek_y2() const{
    return prostokat->daj_wierzcholek_y2();
}

class Zbior {
    //jej obiekty sa zbiorami prostokatow
    Element_zbioru * pierwszy;
public:
    Zbior();
    ~Zbior();
    int rozmiar() const; //zwraca liczbe elementow w zbiorze
    bool contains(const Prostokat &) const;
    bool dodaj(const Prostokat &);
    bool dodaj(int, int, int, int);
    int dodaj(const Zbior &);
    void wypisz() const;
    Zbior * punkt(int, int) const;
    Zbior * wewnatrz(const Prostokat &p) const;
    void wyczysc();
};

Zbior::Zbior() {
    pierwszy = NULL;
}

Zbior::~Zbior(){
    wyczysc();
}

bool Zbior::contains(const Prostokat& p) const {
    Element_zbioru * tmp = pierwszy;
    Prostokat * tmp_p;
    while (tmp != NULL) {
        tmp_p = tmp->daj_prostokat();
        if (tmp_p->porownaj(p))
            return true;
        tmp = tmp->getNext();
    }
    return false;
}

bool Zbior::dodaj(const Prostokat &p) {
    //dodaje element zbioru jesli dany prostokat nie jest juz w zbiorze
    if (contains(p))
        return false;
    Prostokat * tmp = new Prostokat(p);
    pierwszy = new Element_zbioru(tmp, pierwszy);
    return true;

}

bool Zbior::dodaj(int x1, int y1, int x2, int y2) {
    //dodaje prostokat o wierzcholkach w punktach (x1,y1) i (x2,y2)
    Prostokat * p = new Prostokat(x1, y1, x2, y2);
    if (contains(*p))
        return false;
    pierwszy = new Element_zbioru(p, pierwszy);
}

int Zbior::dodaj(const Zbior& z) {
    if ((&z == NULL) || (&z == this))
        return 0;
    const Element_zbioru * tmp = z.pierwszy;
    int ile = 0;
    while (tmp != NULL) {
        if (dodaj(*(tmp->daj_prostokat())))
            ++ile;
        tmp = tmp->getNext();
    }
}

int Zbior::rozmiar() const {
    int ile;
    Element_zbioru * tmp = pierwszy;
    while (tmp != NULL) {
        ++ile;
        tmp = tmp->getNext();
    }
    return ile;
}

void Zbior::wypisz() const {
    cout << "Zbior = {";
    Element_zbioru * tmp = pierwszy;
    while (tmp != NULL) {
        tmp->wypisz();
        tmp = tmp->getNext();
    }
    cout << "}\n";
}

Zbior * Zbior::punkt(int x, int y) const{
    //tworzy nowy zbior zawierajacy wszystkie takieprostokaty z biezacej kolekcji, ktore zawieraja punkt (x,y)
    Zbior * result = new Zbior();
    Element_zbioru * tmp = pierwszy;
    while (tmp != NULL){
        if ((tmp->daj_wierzcholek_x1() >= x ) && (tmp->daj_wierzcholek_x2() <= x) && (tmp->daj_wierzcholek_y1() >= y) && (tmp->daj_wierzcholek_y2() <= y)){
            result->dodaj(*(tmp->daj_prostokat()));
            cout << "tu";
        }
        tmp = tmp->getNext();    
    }
    return result;
}

void Zbior::wyczysc(){
    while (pierwszy != NULL){
        Element_zbioru *tmp = pierwszy;
        pierwszy = pierwszy->getNext();
        delete tmp;
    }
}

/*
 * 
 */
int main(int argc, char** argv) {
    cout << "Test:\nZbior1\n";
    Prostokat p1 = Prostokat(1, 3, 2, 6);
    Prostokat p2 = Prostokat(1, 5, 4, 8);
    Zbior z1 = Zbior();
    z1.dodaj(p1);
    z1.dodaj(p2);
    z1.dodaj(2, 6, 4, 8);
    cout << z1.rozmiar() << endl;
    z1.wypisz();

    cout << "Test:\nZbior2\n";
    Zbior z2 = Zbior();
    z2.dodaj(1, 7, 4, 9);
    z2.dodaj(5, 7, 8, 10);
    cout << z2.rozmiar() << endl;
    z2.wypisz();

    z1.dodaj(z2);
    z1.wypisz();
    
    cout << "Test:\nZbior3\n";
    Zbior z3 = *(z1.punkt(1,4));
    z3.wypisz();
    return 0;
}

