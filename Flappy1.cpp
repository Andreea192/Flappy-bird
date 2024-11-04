#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <conio.h> // pentru _kbhit() și _getch()

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

    bool este_in_viata() const {
        return viata > 0; // verifică dacă pasărea este în viață
    }

    void reset() {
        viata = 100; // resetează viața păsării
    }

    friend ostream& operator<<(ostream& os, const Pasare& p) {
        os << "Pasare [viata: " << p.viata << "]";
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
        nivel_curent = 1; // setarea nivelului curent
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
            nivel_curent = nivel_selectat; // actualizează nivelul curent
        } else {
            cout << "Nivel invalid! Se va folosi nivelul curent: " << nivel_curent << endl;
        }
    }

    int get_nivel() const {
        return nivel_curent; // returnează nivelul curent
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
        this_thread::sleep_for(seconds(2)); // așteaptă 2 s
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
    Pasare pasare1; // Crearea unui obiect Pasare
    Meniu meniu; // Crearea meniului
    meniu.afiseaza_meniu(); // Afișează opțiunile de meniu

    int nivel_curent = meniu.get_nivel();
    int pierderi = 0; // Contor pentru pierderi

    while (pierderi < 2) { // Jucătorul poate pierde de maximum 2 ori
        cout << "Nivelul " << nivel_curent << " incepe!" << endl;
        asteapta_tasta_pentru_continuare(); // Așteaptă tasta 'p' pentru a începe

        bool nivel_completat = false; // Variabilă pentru a verifica completarea nivelului

        while (!nivel_completat && pasare1.este_in_viata()) {
            int nr_tuburi = 2 + (nivel_curent / 2); // Numărul de tuburi per nivel

            for (int i = 0; i < nr_tuburi; ++i) {
                int enter_necessar = numar_apasari_necesare(nivel_curent, i);
                cout << "Trecere prin tubul " << i + 1 << ". Apasa Enter de " << enter_necessar << " ori pentru a trece!" << endl;

                int enter_count = 0;
                TimePoint start_time = Clock::now();
                TimePoint last_enter_time = start_time;

                bool succes = false; // Variabilă pentru a verifica succesul trecerii

                while (enter_count < enter_necessar) {
                    if (_kbhit()) {
                        char c = _getch();
                        if (c == '\r') {
                            TimePoint current_time = Clock::now();
                            if (duration_cast<Seconds>(current_time - last_enter_time).count() > 3) {
                                cout << "A trecut prea mult timp intre apasari! Pasarea a murit!" << endl;
                                pasare1.scade_viata(100); // Scade viața păsării
                                pierderi++; // Incrementăm contorul pierderilor
                                break; // Ieșim din bucla
                            }

                            // Doar incrementăm dacă nu am atins numărul necesar de apăsări
                            if (enter_count < enter_necessar) {
                                enter_count++;
                                last_enter_time = current_time;
                                cout << "Enter apasat! (număr apasari: " << enter_count << ")" << endl;

                                if (enter_count == enter_necessar) {
                                    succes = true; // Trecerea a fost un succes
                                    break; // Ieșim din bucla
                                }
                            } else {
                                cout << "Ai apasat Enter de prea multe ori! Pasarea a murit!" << endl;
                                pasare1.scade_viata(100);
                                pierderi++;
                                break;
                            }
                        } else if (c == 'p') {
                            cout << "Jocul a fost pus pe pauza. Apasa 'p' pentru a continua..." << endl;
                            asteapta_tasta_pentru_continuare(); // Așteaptă tasta 'p' pentru a continua
                            start_time = Clock::now(); // Resetează timpul de început la continuare
                            last_enter_time = start_time; // Resetează timpul ultimei apăsări
                        }
                    }

                    // Verifică timpul de inactivitate
                    TimePoint current_time = Clock::now();
                    if (duration_cast<Seconds>(current_time - last_enter_time).count() >= 2) {
                        enter_count--; // Scade un Enter
                        last_enter_time = current_time; // Resetează timpul ultimei apăsări
                        cout << "Au trecut 2 secunde! Numarul de apasari scade: " << enter_count << endl;

                        // Verifică dacă enter_count a devenit negativ
                        if (enter_count < 0) {
                            cout << "Pasarea a murit din cauza lipsei de apasari!" << endl;
                            pasare1.scade_viata(100);
                            pierderi++;
                            break; // Ieșim din bucla
                        }
                    }
                }

                if (!succes) {
                    cout << "Nivelul a fost completat!" << endl;
                    nivel_completat = true; // Setăm nivelul ca completat
                }
            }
        }

        cout << pasare1; // Afișează starea păsării
    }

    cout << "Game Over!" << endl;

    return 0;
}
