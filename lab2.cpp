#include <iostream>
#include <fstream>  
#include <string> 

using namespace std;

struct truba {
    string name;
    float dlina;
    int diametr;
    bool remont;
};

struct ks {
    string nameks;
    int vsego_cehov;
    int rabotaet_cehov;
    int klass;
};

int proverka_chisla() {
    int chislo;
    while (true) {
        cin >> chislo;
        if (cin.fail() || chislo <= 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ne to. Vvedite normalno: ";
        }
        else {
            return chislo;
        }
    }
}

float proverka_drobi() {
    float chislo;
    while (true) {
        cin >> chislo;
        if (cin.fail() || chislo < 0) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Ne to. Vvedite normalno: ";
        }
        else {
            return chislo;
        }
    }
}

truba dobavit_trubu() {
    truba t;
    
    cout << "Vvedite nazvanie truby: ";
    getline(cin >> ws, t.name);
    
    cout << "Vvedite dlinu: ";
    t.dlina = proverka_drobi();
    
    cout << "Vvedite diametr: ";
    t.diametr = proverka_chisla();
    
    t.remont = false;
    
    return t;
}

ks dobavit_ks() {
    ks k;
    
    cout << "Vvedite nazvanie KS: ";
    getline(cin >> ws, k.nameks);
    
    cout << "Vvedite vsego cehov: ";
    k.vsego_cehov = proverka_chisla();
    
    cout << "Vvedite skolko rabotaet: ";
    k.rabotaet_cehov = proverka_chisla();
    
    while (k.rabotaet_cehov > k.vsego_cehov) {
        cout << "Ne mozhet rabotat bolshe chem est! Vvedite snova: ";
        k.rabotaet_cehov = proverka_chisla();
    }
    
    cout << "Vvedite klass: ";
    k.klass = proverka_chisla();
    
    return k;
}

void pokazat_trubu(const truba& t) {
    cout << "Truba: " << t.name << endl;
    cout << "Dlina: " << t.dlina << endl;
    cout << "Diametr: " << t.diametr << endl;
    if (t.remont) {
        cout << "V remonte: DA" << endl;
    }
    else {
        cout << "V remonte: NET" << endl;
    }
}

void pokazat_ks(const ks& k) {
    cout << "KS: " << k.nameks << endl;
    cout << "Cehov vsego: " << k.vsego_cehov << endl;
    cout << "Cehov rabotaet: " << k.rabotaet_cehov << endl;
    cout << "Klass: " << k.klass << endl;
}

void smena_remonta(truba& t) {
    t.remont = !t.remont;
    cout << "Sdelaem remont: " << (t.remont ? "DA" : "NET") << endl;
}

void zapustit_ceh(ks& k) {
    if (k.rabotaet_cehov < k.vsego_cehov) {
        k.rabotaet_cehov++;
        cout << "Zapustili ceh. Teper rabotaet: " << k.rabotaet_cehov << endl;
    }
    else {
        cout << "Uzhe vse rabotayut!" << endl;
    }
}

void ostanovit_ceh(ks& k) {
    if (k.rabotaet_cehov > 0) {
        k.rabotaet_cehov--;
        cout << "Ostanovili ceh. Teper rabotaet: " << k.rabotaet_cehov << endl;
    }
    else {
        cout << "Uzhe nichego ne rabotaet!" << endl;
    }
}

void sohranit_vse(const truba& t, const ks& k) {
    ofstream file("data.txt");
    
    if (file.is_open()) {
        file << "TRUBA" << endl;
        file << t.name << endl;
        file << t.dlina << " " << t.diametr << " " << t.remont << endl;
        
        file << "KS" << endl;
        file << k.nameks << endl;
        file << k.vsego_cehov << " " << k.rabotaet_cehov << " " << k.klass << endl;
        
        cout << "Vse sohraneno!" << endl;
    }
    else {
        cout << "Oshibka s failom!" << endl;
    }
    file.close();
}

void zagruzit_vse(truba& t, ks& k) {
    ifstream file("data.txt");
    
    if (!file.is_open()) {
        cout << "Net faila!" << endl;
        return;
    }
    
    string metka;
    while (file >> metka) {
        if (metka == "TRUBA") {
            getline(file >> ws, t.name);
            file >> t.dlina >> t.diametr >> t.remont;
        }
        else if (metka == "KS") {
            getline(file >> ws, k.nameks);
            file >> k.vsego_cehov >> k.rabotaet_cehov >> k.klass;
        }
    }
    
    cout << "Vse zagruzeno!" << endl;
    file.close();
}

int main() {
    truba my_truba;
    ks my_ks;
    
    bool est_truba = false;
    bool est_ks = false;
    
    while (true) {
        cout << "\n1. Dobavit trubu" << endl;
        cout << "2. Dobavit KS" << endl;
        cout << "3. Posmotret vse" << endl;
        cout << "4. Pomenyat remont truby" << endl;
        cout << "5. Upravlyat cehami KS" << endl;
        cout << "6. Sohranit" << endl;
        cout << "7. Zagruzit" << endl;
        cout << "0. Vihod" << endl;
        cout << "Vash vibor: ";
        
        int vibor;
        cin >> vibor;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Eto ne chislo!" << endl;
            continue;
        }
        
        switch (vibor) {
            case 1:
                my_truba = dobavit_trubu();
                est_truba = true;
                break;
                
            case 2:
                my_ks = dobavit_ks();
                est_ks = true;
                break;
                
            case 3:
                if (est_truba) {
                    pokazat_trubu(my_truba);
                    cout << "---" << endl;
                }
                else {
                    cout << "Net truby!" << endl;
                }
                
                if (est_ks) {
                    pokazat_ks(my_ks);
                }
                else {
                    cout << "Net KS!" << endl;
                }
                break;
                
            case 4:
                if (est_truba) {
                    smena_remonta(my_truba);
                }
                else {
                    cout << "Snachala dobavte trubu!" << endl;
                }
                break;
                
            case 5:
                if (est_ks) {
                    cout << "1 - zapustit, 2 - ostanovit: ";
                    int deistvie;
                    cin >> deistvie;
                    
                    if (deistvie == 1) {
                        zapustit_ceh(my_ks);
                    }
                    else if (deistvie == 2) {
                        ostanovit_ceh(my_ks);
                    }
                    else {
                        cout << "Ne ponyl!" << endl;
                    }
                }
                else {
                    cout << "Snachala dobavte KS!" << endl;
                }
                break;
                
            case 6:
                if (est_truba || est_ks) {
                    sohranit_vse(my_truba, my_ks);
                }
                else {
                    cout << "Net cego sohranyat!" << endl;
                }
                break;
                
            case 7:
                zagruzit_vse(my_truba, my_ks);
                if (my_truba.dlina > 0) est_truba = true;
                if (my_ks.vsego_cehov > 0) est_ks = true;
                break;
                
            case 0:
                cout << "Poka!" << endl;
                return 0;
                
            default:
                cout << "Net takogo punkta!" << endl;
                break;
        }
    }
    
    return 0;
}