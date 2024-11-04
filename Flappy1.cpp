#include <iostream>
#include <string>
#include <chrono> // asta imi masoara timpul, folosit pentru distanta dintre enter sau cand dau p pentru pauza
#include <thread> // pentru bucla de asteptare cand am timpul de executie pe pauza si sa nu execute
#include <conio.h> // pentru _kbhit() și _getch() care ma ajuta sa fac legatura cu consola

using namespace std;
using namespace std::chrono;

using Clock = steady_clock;
using TimePoint = Clock::time_point;
using Seconds = duration<int>;

class Pasare {
    int viata;
public:
    Pasare() {
        viata = 100; // viața inițială a păsării
    }

    void scade_viata(int valoare) {
        viata -= valoare;
        if (viata < 0) {
            viata = 0; // viața nu poate scădea sub 0
        }
    }

    void reset() {
        viata = 100; // resetează viața păsării
    }

    bool este_in_viata() const {
        return viata > 0; // verifică dacă pasărea este în viață
    }

    friend ostream& operator<<(ostream& os, const Pasare& p) {
        os << "Pasare [viata: " << p.viata << "]";
        return os;
    }
};

class Obstacol {
    string obstacol;
    int dauna;
public:
    Obstacol(const string &tip_pericol = "Tub", int dauna_noua = 100) { // dauna unui tub este 100
        obstacol = tip_pericol;
        dauna = dauna_noua;
    }

    void interactiune(Pasare &pasare, bool a_trecut) {
        if (!a_trecut) {
            pasare.scade_viata(dauna);
            cout << "Pasarea s-a lovit de un " << obstacol << ". Dauna: " << dauna << endl;
        } else {
            cout << "Pasarea a trecut cu succes prin " << obstacol << "!" << endl;
        }
    }

    friend ostream& operator<<(ostream &os, const Obstacol &o) {
        os << "Obstacol [Tip: " << o.obstacol << ", Dauna: " << o.dauna << "]";
        return os;
    }
};

class Nivel {
    int dificultate;
    Obstacol obstacol;
public:
    Nivel() { // Constructorul fără parametru
        dificultate = 1; // dificultatea pe care o am inițial
        obstacol = Obstacol("Tub", 100); // initializeaza obstacolul(tubul)
    }

    Nivel(int dificultate_noua) { // Constructor cu parametru
        dificultate = dificultate_noua; // dificultatea noua
        obstacol = Obstacol("Tub", 100); // la fel ca mai sus
    } // tubul da dauna de 100 deci moare:)

    void interactiune(Pasare &pasare, bool a_trecut) {
        obstacol.interactiune(pasare, a_trecut); // impact pasare-tub
    }

    friend ostream& operator<<(ostream &os, const Nivel &n) {
        os << "Nivel [Dificultate: " << n.dificultate << "]";
        return os;
    }
};

class Meniu {
    string culori[4];
    int nivel_curent;
public:
    Meniu() {
        culori[0] = "Rosu";
        culori[1] = "Verde";
        culori[2] = "Albastru";
        culori[3] = "Galben";
        nivel_curent = 1;
    }

    void afiseaza_meniu() {
        cout << "Selecteaza o culoare pentru pasare: " << endl;
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ". " << culori[i] << endl;
        }
        cout << "Selecteaza nivelul " << nivel_curent;
        int nivel_selectat;
        cin >> nivel_selectat;
        if (nivel_selectat > 0 && nivel_selectat <= nivel_curent) {
            nivel_curent = nivel_selectat; // actualizează nivelul curent
        } else {
            cout << "Nivel invalid! Se va folosi nivelul curent: " << nivel_curent << endl;
        }
    }

    int get_nivel() const {
        return nivel_curent; // returnează nivelul curent
    }

    void next_level() {
        nivel_curent++; // trece la nivelul următor
    }

    void reset_nivel() {
        nivel_curent = 1; // resetează nivelul la 1
    }

    // Operator friend pentru a putea utiliza cout
    friend ostream& operator<<(ostream& os, const Meniu& m) {
        os << "Meniu [nivel curent: " << m.nivel_curent << "]";
        return os;
    }
};

void asteapta_tasta_pentru_continuare() {
    cout << "Apasa 'p' pentru a continua..." << endl;
    while (true) {
        if (_kbhit()) {
            char c = _getch();
            if (c == 'p') {
                break; // continuă jocul
            }
        }
        this_thread::sleep_for(seconds(1)); // așteaptă 1 s
    }
}

int numar_apasari_necesare(int nivel, int index_tub) {
    if (nivel == 1) {
        return 3; // necesare 3 apăsări pentru nivelul 1
    } else if (nivel == 2) {
        return 2; // necesare 2 apăsări pentru nivelul 2
    } else if (nivel == 3) {
        return 4; // necesare 4 apăsări pentru nivelul 3
    } else {
        // Alternare pentru niveluri >= 4
        if (index_tub % 4 == 0) {
            return 3; // tub de 3 apăsări
        } else if (index_tub % 4 == 1 || index_tub % 4 == 2) {
            return 2; // tub de 2 apăsări
        } else {
            return 4; // tub de 4 apăsări
        }
    }
}

int main() {
    Pasare pasare1;
    Meniu meniu;
    meniu.afiseaza_meniu();

    int nivel_curent = meniu.get_nivel();
    int pierderi = 0; //numar de cate ori pierde pasarea pentru a stii cand reia jocul de la 1 si cand reia doar nivelul actual

    while (true) {
        cout << "Nivelul " << nivel_curent << ": " << endl;

        // are iar viata 100 pentru ca resetez pentru nivelul actual
        pasare1.reset();
        Nivel nivel(nivel_curent);

        while (pasare1.este_in_viata()) {
            int nr_tuburi = 2 + (nivel_curent / 2); // numar la intamplare de tuburi pt fiecare nivel-progresie
            for (int i = 0; i < nr_tuburi; ++i) {
                int apasari_necesare = numar_apasari_necesare(nivel_curent, i);
                cout << "Apasa Enter de " << apasari_necesare << " ori pentru a trece prin tubul " << i + 1 << "." << endl;

                int apasari_realizate = 0;
                TimePoint last_enter_time = Clock::now();
                TimePoint start_time = last_enter_time; // tine in evidenta apasarea tastei enter

                while (apasari_realizate < apasari_necesare) {
                    if (_kbhit()) {
                        char c = _getch();
                        if (c == '\r') { // Verificăm dacă s-a apăsat Enter
                            TimePoint current_time = Clock::now();
                            if (duration_cast<Seconds>(current_time - last_enter_time).count() >= 3) {
                                cout << "A trecut prea mult timp intre apasari! Pasarea a murit!" << endl;
                                pasare1.scade_viata(100); // scade viața păsării
                                pierderi++;
                                break; //se opreste pentru ca a pierdut si reia
                            }

                            apasari_realizate++;
                            last_enter_time = current_time; // reseteaza ultima apasare a tastei
                            cout << "Apasare realizată: " << apasari_realizate << endl;
                        } else if (c == 'p') {
                            cout << "Jocul a fost pus pe pauza. Apasa 'p' pentru a continua..." << endl;
                            asteapta_tasta_pentru_continuare();
                            start_time = Clock::now(); // reia timpul de unde a fost
                            last_enter_time = start_time; // reseteaza timpul de cand am apasat tasta ultima data
                        }
                    }

                    // verifica cat nu s-a apasat Enter pentru a numara apasarile mai bine
                    TimePoint current_time = Clock::now();
                    if (duration_cast<Seconds>(current_time - last_enter_time).count() >= 2) {
                        apasari_realizate--; // scade un Enter
                        last_enter_time = current_time; // reseteaza iar ultima apasare ca mai sus
                        cout << "Au trecut 2 secunde! Numarul de apasari scade: " << apasari_realizate << endl;

                        // daca e negativ automat moare
                        if (apasari_realizate < 0) {
                            cout << "Pasarea a murit din cauza lipsei de apasari!" << endl;
                            pasare1.scade_viata(100);
                            pierderi++;
                            break; // se opreste
                        }
                    }
                }

                bool a_trecut = (apasari_realizate >= apasari_necesare); // verifica daca a trecut de tub

                nivel.interactiune(pasare1, a_trecut);

                //încheie nivelul
                if (!pasare1.este_in_viata()) {
                    cout << "Ai pierdut acest nivel!" << endl;
                    break;
                }
            }

            // verific dacă pasărea a murit
            if (pasare1.este_in_viata()) {
                cout << "Nivel completat! Apasa 'p' pentru a continua la urmatorul nivel." << endl;
                asteapta_tasta_pentru_continuare();
                meniu.next_level(); // trece la nivelul următor
                nivel_curent = meniu.get_nivel();
            }
        }

        // verific iar dacă pasărea a murit
        if (!pasare1.este_in_viata()) {
            cout << "Nivel pierdut! Viata ramane 0." << endl;
            pierderi++;
            asteapta_tasta_pentru_continuare();

            // logica reluarii nivelului
            if (pierderi < 3) {
                cout << "Se reia nivelul " << nivel_curent << endl;
                // resetez viata automat la 100
            } else {
                cout << "Ai pierdut de 3 ori. Se revine la nivelul 1." << endl;
                meniu.reset_nivel(); // nivelul 1 iar
                pierderi = 0; //pierderi 0 iar, practic se reia jocul cu totul
            }
        }
    }
}