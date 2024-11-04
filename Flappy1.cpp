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
    Pasare pasare1; // Crearea unui obiect Pasare
    Meniu meniu; // Crearea meniului
    meniu.afiseaza_meniu(); // Afișează opțiunile de meniu

    int nivel_curent = meniu.get_nivel();
    int pierderi = 0; // Contor pentru pierderi

    while (true) {
        cout << "Nivelul " << nivel_curent << ": " << endl;

        // Resetează viața pentru nivelul curent
        pasare1.reset();

        while (pasare1.este_in_viata()) {
            // Simularea jocului
            int nr_tuburi = 2 + (nivel_curent / 2); // Numărul de tuburi per nivel
            for (int i = 0; i < nr_tuburi; ++i) {
                int apasari_necesare = numar_apasari_necesare(nivel_curent, i);
                cout << "Apasa Enter de " << apasari_necesare << " ori pentru a trece prin tubul " << i + 1 << "." << endl;

                int apasari_realizate = 0;
                TimePoint last_enter_time = Clock::now();
                TimePoint start_time = last_enter_time;

                while (apasari_realizate < apasari_necesare) {
                    if (_kbhit()) {
                        char c = _getch();
                        if (c == '\r') { // Verificăm dacă s-a apăsat Enter
                            TimePoint current_time = Clock::now();
                            if (duration_cast<Seconds>(current_time - last_enter_time).count() >= 3) {
                                cout << "A trecut prea mult timp intre apasari! Pasarea a murit!" << endl;
                                pasare1.scade_viata(100); // Scade viața păsării
                                pierderi++; // Incrementăm contorul pierderilor
                                break; // Ieșim din bucla
                            }

                            apasari_realizate++;
                            last_enter_time = current_time;
                            cout << "Enter apasat! (număr apasari: " << apasari_realizate << ")" << endl;
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
                        apasari_realizate--; // Scade un Enter
                        last_enter_time = current_time; // Resetează timpul ultimei apăsări
                        cout << "Au trecut 2 secunde! Numarul de apasari scade: " << apasari_realizate << endl;

                        // Verifică dacă apasari_realizate a devenit negativ
                        if (apasari_realizate < 0) {
                            cout << "Pasarea a murit din cauza lipsei de apasari!" << endl;
                            pasare1.scade_viata(100);
                            pierderi++;
                            break; // Ieșim din bucla
                        }
                    }
                }

                if (!pasare1.este_in_viata()) {
                    cout << "Ai pierdut acest nivel!" << endl;
                    break; // Ieșim din bucla dacă pasărea a murit
                }
            }

            if (pasare1.este_in_viata()) {
                cout << "Nivel completat!" << endl;
                meniu.next_level();// trece la nivelul următor
                nivel_curent = meniu.get_nivel();
            }
        }

        // Verifică dacă pasărea a murit
        if (!pasare1.este_in_viata()) {
            cout << "Nivel pierdut! Viata ramane 0." << endl;
            pierderi++;
            asteapta_tasta_pentru_continuare();

            // Logica pentru reluarea nivelului
            if (pierderi < 3) {
                cout << "Se reia nivelul " << nivel_curent << endl;
                // viața se resetează automat la reluarea nivelului
            } else {
                cout << "Ai pierdut de 3 ori. Se revine la nivelul 1." << endl;
                meniu.reset_nivel(); // Resetează nivelul la 1
                pierderi = 0; // Resetare contor pierderi
            }
        }
    }

    return 0;
}
