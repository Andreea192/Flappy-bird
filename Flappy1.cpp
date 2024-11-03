#include <iostream>
#include <string>

using namespace std;

class Pasare {
    int viata;
public:
    Pasare() {
        viata = 100; // viata initiala a caracterului
    }
    Pasare(int viata_noua) {
        viata = viata_noua;
    }
    Pasare(const Pasare &p) {
        viata = p.viata; // Constructor de copiere
    }
    Pasare& operator=(const Pasare &p) {
        viata = p.viata;
        return *this;
    }
    ~Pasare() {
        cout << "Pasarea destructor" << endl;
    }
    void scade_viata(int valoare) {
        viata -= valoare;
        if (viata < 0) {
            viata = 0; // Viața nu poate scădea sub 0
        }
    }
    bool este_in_viata() const {
        return viata > 0;
    }
    int get_viata() const {
        return viata;
    }
    friend ostream& operator<<(ostream &os, const Pasare &p) {
        os << "viata: " << p.viata;
        return os;
    }
};

class Obstacole {
    string obstacol;
    int dauna;
public:
    Obstacole() {
        obstacol = "Tub";
        dauna = 100;
    }
    Obstacole(const string &tip_pericol, int dauna_noua) {
        obstacol = tip_pericol;
        dauna = dauna_noua;
    }
    Obstacole(const Obstacole &pericol) {
        obstacol = pericol.obstacol;
        dauna = pericol.dauna;
    }
    Obstacole& operator=(const Obstacole &pericol) {
        if (this != &pericol) {
            obstacol = pericol.obstacol;
            dauna = pericol.dauna;
        }
        return *this;
    }
    ~Obstacole() {
        // Destructor
    }
    string getObstacol() const {
        return obstacol; // Tipul obstacolului
    }
    int get_dauna() const {
        return dauna; // Dauna cauzată de obstacol
    }
    void interactiune(Pasare &pasare, const string& impact) {
        if (obstacol == "Tub") {
            pasare.scade_viata(dauna);
            cout << "Pasarea s-a lovit de un " << obstacol << ". Dauna: " << dauna << endl;
        } else if (obstacol == "Grindina") {
            if (impact == "piatra_mare") {
                pasare.scade_viata(90);
                cout << "Pasarea a fost lovita de o piatra mare" << endl;
            } else if (impact == "piatra_mica") {
                pasare.scade_viata(40);
                cout << "Pasarea a fost lovita de o piatra mica" << endl;
            }
        }
    }
    friend ostream& operator<<(ostream &os, const Obstacole &pericol) {
        os << "Obstacol: " << pericol.obstacol << ", dauna: " << pericol.dauna << endl;
        return os;
    }
};

class Nivel {
    int dificultate;
    Obstacole *obstacol;
    int nr_obstacole;
public:
    Nivel() {
        dificultate = 1;
        nr_obstacole = 0;
        obstacol = new Obstacole[10];
    }
    Nivel(int dificultate_noua) {
        dificultate = dificultate_noua;
        nr_obstacole = 0;
        obstacol = new Obstacole[20]; // Alocare mai mare pentru obstacole
        genereaza_obstacol(); // Generează obstacole la crearea nivelului
    }
    ~Nivel() {
        delete[] obstacol; // Destructor
    }
    void genereaza_obstacol() {
        if (dificultate == 1) {
            nr_obstacole = 4;
            for (int i = 0; i < nr_obstacole; i++) {
                obstacol[i] = Obstacole("Tub", 100);
            }
        } else if (dificultate == 2) {
            nr_obstacole = 10;
            for (int i = 0; i < nr_obstacole; i++) {
                obstacol[i] = Obstacole("Tub", 100);
            }
        } else if (dificultate == 3) {
            nr_obstacole = 5;
            for (int i = 0; i < nr_obstacole; i++) {
                obstacol[i] = Obstacole("Tub", 100);
            }
            obstacol[nr_obstacole++] = Obstacole("Grindina", 90); // Piatra mare
            obstacol[nr_obstacole++] = Obstacole("Grindina", 90); // Piatra mare
            obstacol[nr_obstacole++] = Obstacole("Grindina", 40); // Piatra mica
            obstacol[nr_obstacole++] = Obstacole("Grindina", 40); // Piatra mica
            obstacol[nr_obstacole++] = Obstacole("Grindina", 40); // Piatra mica
            for (int i = 0; i < 4; i++) {
                obstacol[nr_obstacole++] = Obstacole("Tub", 100);
            }
        } else if (dificultate == 4) {
            nr_obstacole = 2 * 10; // Dublăm obstacolele de la nivelul 3
            for (int i = 0; i < nr_obstacole; i++) {
                obstacol[i] = Obstacole("Tub", 100);
            }
        } else if (dificultate == 5) {
            nr_obstacole = 2 * 10; // Dublăm obstacolele de la nivelul 2
            for (int i = 0; i < nr_obstacole; i++) {
                obstacol[i] = Obstacole("Tub", 100);
            }
        }
    }
    int get_dificultate() const {
        return dificultate;
    }
    int get_nr_obstacole() const {
        return nr_obstacole;
    }
    void afiseaza_obstacol() {
        for (int i = 0; i < nr_obstacole; i++) {
            cout << obstacol[i].getObstacol() << " cu dauna " << obstacol[i].get_dauna() << endl;
        }
    }
    void interactiune(Pasare &pasare) {
        for (int i = 0; i < nr_obstacole; i++) {
            obstacol[i].interactiune(pasare, "piatra_mare"); // Exemplu de impact
            if (!pasare.este_in_viata()) {
                cout << "Pasărea nu mai este în viață!" << endl;
                break; // Ieșim din buclă dacă pasărea a murit
            }
        }
    }
};

class Meniu {
    string culori[4];
    int nivel_curent;
    int viteza;
public:
    Meniu() {
        culori[0] = "Rosu";
        culori[1] = "Verde";
        culori[2] = "Albastru";
        culori[3] = "Galben";
        nivel_curent = 1;
        viteza = 5;
    }
    void afiseaza_meniu() {
        cout << "Selecteaza o culoare pentru pasare: " << endl;
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ". " << culori[i] << endl;
        }
        cout << "Selecteaza nivelul (1 - " << nivel_curent << "): ";
        int nivel_selectat;
        cin >> nivel_selectat;
        if (nivel_selectat > 0 && nivel_selectat <= nivel_curent) {
            nivel_curent = nivel_selectat;
        } else {
            cout << "Nivel invalid! Se va folosi nivelul curent: " << nivel_curent << endl;
        }

        cout << "Setează viteza păsării (1-10): ";
        cin >> viteza;
        if (viteza < 1 || viteza > 10) {
            cout << "Viteza invalidă! Se va folosi viteza implicită: 1" << endl;
            viteza = 1;
        }
    }

    int get_dificultate() const { // Adăugat metoda
        return nivel_curent;
    }
};

int main() {
    Meniu meniu;
    meniu.afiseaza_meniu();

    Pasare pasare;
    int dificultate_selectata = meniu.get_dificultate(); // Acum va funcționa corect
    Nivel nivel(dificultate_selectata);

    cout << "Obstacolele generate pentru nivelul " << nivel.get_dificultate() << ":" << endl;
    nivel.afiseaza_obstacol();

    nivel.interactiune(pasare);

    cout << "Viața rămasă a păsării: " << pasare << endl;

    return 0;
}
