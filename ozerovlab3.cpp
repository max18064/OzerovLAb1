#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// ==================== Базовые функции ввода ====================
int get_int() {
    int x;
    while (!(cin >> x) || x < 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Ошибка! Введите целое число >= 0: ";
    }
    return x;
}

double get_double() {
    double x;
    while (!(cin >> x) || x < 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Ошибка! Введите число >= 0: ";
    }
    return x;
}

// ==================== Класс Труба ====================
class Pipe {
public:
    int id;
    string name;
    double length;
    int diameter;
    bool in_repair;
    
    static int next_id;
    
    Pipe() {
        id = next_id++;
        in_repair = false;
    }
    
    void input() {
        cout << "Название трубы: ";
        cin >> ws;
        getline(cin, name);
        cout << "Длина: ";
        length = get_double();
        cout << "Диаметр: ";
        diameter = get_int();
    }
    
    void show() {
        cout << "Труба ID:" << id << " \"" << name << "\" Д:" << diameter 
             << " L:" << length << " Ремонт:" << (in_repair ? "Да" : "Нет") << endl;
    }
    
    void edit() {
        in_repair = !in_repair;
        cout << "Статус ремонта изменен" << endl;
    }
    
    bool match_name(const string& filter) {
        return name.find(filter) != string::npos;
    }
    
    bool match_repair(bool status) {
        return in_repair == status;
    }
};

int Pipe::next_id = 1;

// ==================== Класс КС ====================
class KS {
public:
    int id;
    string name;
    int total_shops;
    int working_shops;
    
    static int next_id;
    
    KS() {
        id = next_id++;
    }
    
    void input() {
        cout << "Название КС: ";
        cin >> ws;
        getline(cin, name);
        cout << "Всего цехов: ";
        total_shops = get_int();
        cout << "Работает цехов: ";
        working_shops = get_int();
        while (working_shops > total_shops) {
            cout << "Не может работать больше чем всего! Введите снова: ";
            working_shops = get_int();
        }
    }
    
    void show() {
        double percent = total_shops > 0 ? (100.0 * (total_shops - working_shops) / total_shops) : 0;
        cout << "КС ID:" << id << " \"" << name << "\" Цехов:" << working_shops 
             << "/" << total_shops << " Простой:" << percent << "%" << endl;
    }
    
    bool match_name(const string& filter) {
        return name.find(filter) != string::npos;
    }
    
    bool match_unused(double min_percent) {
        if (total_shops == 0) return false;
        double percent = 100.0 * (total_shops - working_shops) / total_shops;
        return percent >= min_percent;
    }
    
    void start_shop() {
        if (working_shops < total_shops) {
            working_shops++;
            cout << "Запущен цех. Работает: " << working_shops << endl;
        } else {
            cout << "Все цехи уже работают!" << endl;
        }
    }
    
    void stop_shop() {
        if (working_shops > 0) {
            working_shops--;
            cout << "Остановлен цех. Работает: " << working_shops << endl;
        } else {
            cout << "Нет работающих цехов!" << endl;
        }
    }
};

int KS::next_id = 1;

// ==================== Глобальные данные ====================
vector<Pipe> pipes;
vector<KS> ks_list;
ofstream log_file("log.txt", ios::app);

// ==================== Функции логирования ====================
void log(const string& msg) {
    log_file << msg << endl;
}

// ==================== Функции работы с трубами ====================
void add_pipe() {
    Pipe p;
    p.input();
    pipes.push_back(p);
    log("Добавлена труба ID:" + to_string(p.id));
    cout << "Труба добавлена (ID:" << p.id << ")" << endl;
}

void show_pipes() {
    if (pipes.empty()) {
        cout << "Нет труб" << endl;
        return;
    }
    for (auto& p : pipes) p.show();
}

vector<int> find_pipes_by_name() {
    string filter;
    cout << "Поиск по названию: ";
    cin >> ws;
    getline(cin, filter);
    
    vector<int> result;
    for (auto& p : pipes) {
        if (p.match_name(filter)) {
            result.push_back(p.id);
        }
    }
    log("Поиск труб по названию \"" + filter + "\" найдено: " + to_string(result.size()));
    return result;
}

vector<int> find_pipes_by_repair() {
    cout << "Искать в ремонте? (1-да, 0-нет): ";
    int choice;
    cin >> choice;
    bool status = (choice == 1);
    
    vector<int> result;
    for (auto& p : pipes) {
        if (p.match_repair(status)) {
            result.push_back(p.id);
        }
    }
    log("Поиск труб по ремонту найдено: " + to_string(result.size()));
    return result;
}

void batch_edit_pipes(const vector<int>& ids) {
    if (ids.empty()) {
        cout << "Не найдено труб для редактирования" << endl;
        return;
    }
    
    cout << "Найдено " << ids.size() << " труб" << endl;
    cout << "1. Редактировать все\n2. Выбрать конкретные\nВыберите: ";
    int choice = get_int();
    
    vector<int> to_edit;
    
    if (choice == 1) {
        to_edit = ids;
    } else {
        cout << "Введите ID через пробел (0 для завершения): ";
        int id;
        while (cin >> id && id != 0) {
            if (find(ids.begin(), ids.end(), id) != ids.end()) {
                to_edit.push_back(id);
            } else {
                cout << "ID " << id << " не входит в найденные!" << endl;
            }
        }
        cin.clear();
        cin.ignore(1000, '\n');
    }
    
    cout << "1. Изменить статус ремонта\n2. Удалить\nВыберите: ";
    choice = get_int();
    
    if (choice == 1) {
        for (int id : to_edit) {
            for (auto& p : pipes) {
                if (p.id == id) {
                    p.edit();
                    log("Изменен статус трубы ID:" + to_string(id));
                }
            }
        }
        cout << "Готово!" << endl;
    } else {
        // Удаление
        auto it = remove_if(pipes.begin(), pipes.end(), 
            [&](const Pipe& p) {
                return find(to_edit.begin(), to_edit.end(), p.id) != to_edit.end();
            });
        pipes.erase(it, pipes.end());
        log("Удалено труб: " + to_string(to_edit.size()));
        cout << "Удалено " << to_edit.size() << " труб" << endl;
    }
}

// ==================== Функции работы с КС ====================
void add_ks() {
    KS k;
    k.input();
    ks_list.push_back(k);
    log("Добавлена КС ID:" + to_string(k.id));
    cout << "КС добавлена (ID:" << k.id << ")" << endl;
}

void show_ks() {
    if (ks_list.empty()) {
        cout << "Нет КС" << endl;
        return;
    }
    for (auto& k : ks_list) k.show();
}

vector<int> find_ks_by_name() {
    string filter;
    cout << "Поиск по названию: ";
    cin >> ws;
    getline(cin, filter);
    
    vector<int> result;
    for (auto& k : ks_list) {
        if (k.match_name(filter)) {
            result.push_back(k.id);
        }
    }
    log("Поиск КС по названию \"" + filter + "\" найдено: " + to_string(result.size()));
    return result;
}

vector<int> find_ks_by_unused() {
    cout << "Минимальный процент простоя: ";
    double percent = get_double();
    
    vector<int> result;
    for (auto& k : ks_list) {
        if (k.match_unused(percent)) {
            result.push_back(k.id);
        }
    }
    log("Поиск КС по простою найдено: " + to_string(result.size()));
    return result;
}

// ==================== Функции сохранения/загрузки ====================
void save_all() {
    string filename;
    cout << "Имя файла для сохранения: ";
    cin >> ws;
    getline(cin, filename);
    
    ofstream f(filename);
    f << pipes.size() << " " << ks_list.size() << endl;
    
    for (auto& p : pipes) {
        f << p.id << " " << p.name << " " << p.length << " " 
          << p.diameter << " " << p.in_repair << endl;
    }
    
    for (auto& k : ks_list) {
        f << k.id << " " << k.name << " " << k.total_shops << " " 
          << k.working_shops << endl;
    }
    
    f.close();
    log("Сохранено в файл: " + filename);
    cout << "Сохранено!" << endl;
}

void load_all() {
    string filename;
    cout << "Имя файла для загрузки: ";
    cin >> ws;
    getline(cin, filename);
    
    ifstream f(filename);
    if (!f.is_open()) {
        cout << "Файл не найден!" << endl;
        return;
    }
    
    int pipe_count, ks_count;
    f >> pipe_count >> ks_count;
    
    pipes.clear();
    for (int i = 0; i < pipe_count; i++) {
        Pipe p;
        f >> p.id >> p.name >> p.length >> p.diameter >> p.in_repair;
        pipes.push_back(p);
        if (p.id >= Pipe::next_id) Pipe::next_id = p.id + 1;
    }
    
    ks_list.clear();
    for (int i = 0; i < ks_count; i++) {
        KS k;
        f >> k.id >> k.name >> k.total_shops >> k.working_shops;
        ks_list.push_back(k);
        if (k.id >= KS::next_id) KS::next_id = k.id + 1;
    }
    
    f.close();
    log("Загружено из файла: " + filename);
    cout << "Загружено!" << endl;
}

// ==================== Главное меню ====================
int main() {
    log("=== Программа запущена ===");
    
    while (true) {
        cout << "\n=== МЕНЮ ===" << endl;
        cout << "1. Добавить трубу" << endl;
        cout << "2. Добавить КС" << endl;
        cout << "3. Показать все" << endl;
        cout << "4. Поиск и пакетное редактирование труб" << endl;
        cout << "5. Поиск КС" << endl;
        cout << "6. Управление цехами КС" << endl;
        cout << "7. Сохранить" << endl;
        cout << "8. Загрузить" << endl;
        cout << "0. Выход" << endl;
        cout << "Выбор: ";
        
        int choice = get_int();
        
        switch (choice) {
            case 1: add_pipe(); break;
            case 2: add_ks(); break;
            case 3: 
                show_pipes(); 
                show_ks(); 
                break;
                
            case 4: {
                cout << "1. Поиск по названию\n2. Поиск по ремонту\nВыберите: ";
                int search_type = get_int();
                vector<int> found_ids;
                
                if (search_type == 1) {
                    found_ids = find_pipes_by_name();
                } else {
                    found_ids = find_pipes_by_repair();
                }
                
                if (!found_ids.empty()) {
                    cout << "Выполнить пакетное редактирование? (1-да, 0-нет): ";
                    if (get_int() == 1) {
                        batch_edit_pipes(found_ids);
                    }
                } else {
                    cout << "Трубы не найдены" << endl;
                }
                break;
            }
                
            case 5: {
                cout << "1. Поиск по названию\n2. Поиск по проценту простоя\nВыберите: ";
                int search_type = get_int();
                vector<int> found_ids;
                
                if (search_type == 1) {
                    found_ids = find_ks_by_name();
                } else {
                    found_ids = find_ks_by_unused();
                }
                
                if (!found_ids.empty()) {
                    cout << "Найдено КС:" << endl;
                    for (int id : found_ids) {
                        for (auto& k : ks_list) {
                            if (k.id == id) {
                                k.show();
                                break;
                            }
                        }
                    }
                } else {
                    cout << "КС не найдены" << endl;
                }
                break;
            }
                
            case 6: {
                cout << "ID КС: ";
                int id = get_int();
                bool found = false;
                
                for (auto& k : ks_list) {
                    if (k.id == id) {
                        found = true;
                        cout << "1. Запустить цех\n2. Остановить цех\nВыберите: ";
                        int action = get_int();
                        if (action == 1) {
                            k.start_shop();
                            log("Запущен цех КС ID:" + to_string(id));
                        } else {
                            k.stop_shop();
                            log("Остановлен цех КС ID:" + to_string(id));
                        }
                        break;
                    }
                }
                if (!found) cout << "КС не найдена" << endl;
                break;
            }
                
            case 7: save_all(); break;
            case 8: load_all(); break;
            case 0: 
                log("=== Программа завершена ===");
                log_file.close();
                return 0;
                
            default: 
                cout << "Неверный выбор!" << endl;
        }
    }
}