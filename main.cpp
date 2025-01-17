//-------------------------------------------------
// ZMPROG - WAT
// Data: 30.11.2024
// Autor: Damian Kaczor
// Opis:
//      Wzorce projektowe singleton, factory method tworzenie loggera.
// Wersja: 1.1
// ------------------------------------------------


// Biblioteki
#include <iostream>
#include <vector>
#include <memory>
#include <string>

//------------------------------------------------

// Bazowa klasa abstrakcyjna dla log�w
class Log {
public:
    virtual ~Log() = default; // Wirtualny destruktor dla poprawnego usuwania obiekt�w pochodnych
    virtual std::string pobierzWiadomosc() const = 0; // Czysto wirtualna metoda
};

// Klasa reprezentuj�ca log informacyjny
class LogInformacyjny : public Log {
public:
    explicit LogInformacyjny(const std::string& wiadomosc) : wiadomosc_("INFO: " + wiadomosc) {}
    std::string pobierzWiadomosc() const override { return wiadomosc_; }

private:
    std::string wiadomosc_;
};

// Klasa reprezentuj�ca log ostrzegawczy
class LogOstrzezenie : public Log {
public:
    explicit LogOstrzezenie(const std::string& wiadomosc) : wiadomosc_("WARNING: " + wiadomosc) {}
    std::string pobierzWiadomosc() const override { return wiadomosc_; }

private:
    std::string wiadomosc_;
};

// Klasa reprezentuj�ca log b��du
class LogBlad : public Log {
public:
    explicit LogBlad(const std::string& wiadomosc) : wiadomosc_("ERROR: " + wiadomosc) {}
    std::string pobierzWiadomosc() const override { return wiadomosc_; }

private:
    std::string wiadomosc_;
};

// Klasa Tw�rca Log�w (Factory Method)
class TworcaLogow {
public:
    // Tworzenie logu na podstawie typu
    static std::unique_ptr<Log> stworzLog(const std::string& typ, const std::string& wiadomosc) {
        if (typ == "info") return std::make_unique<LogInformacyjny>(wiadomosc);
        if (typ == "warn") return std::make_unique<LogOstrzezenie>(wiadomosc);
        if (typ == "err") return std::make_unique<LogBlad>(wiadomosc);
        throw std::invalid_argument("Nieznany typ logu: " + typ);
    }
};



// Klasa Logger (Singleton)
class Logger {
public:
    // Pobranie jedynej instancji loggera
    static Logger* pobierzInstancje() {
        if (!instancja_) {
            instancja_ = new Logger(); // Tworzenie loggera, je�li jeszcze nie istnieje
        }
        return instancja_;
    }

    // Dodanie logu
    void dodajLog(const std::string& typ, const std::string& wiadomosc) {
        logi_.emplace_back(TworcaLogow::stworzLog(typ, wiadomosc));
    }

    // Wy�wietlenie wszystkich log�w
    void pokazLogi() const {
        for (const auto& log : logi_) {
            std::cout << log->pobierzWiadomosc() << std::endl;
        }
    }

    // Zniszczenie loggera (opcjonalne)
    static void usunInstancje() {
        delete instancja_;
        instancja_ = nullptr;
    }

private:
    Logger() = default; // Prywatny konstruktor
    Logger(const Logger&) = delete; // Zabronione kopiowanie
    Logger& operator=(const Logger&) = delete; // Zabronione przypisanie

    static Logger* instancja_; // Wska�nik na jedyn� instancj� loggera
    std::vector<std::unique_ptr<Log>> logi_; // Przechowywane logi
};

// Inicjalizacja wska�nika statycznego
Logger* Logger::instancja_ = nullptr;

int main() {
    // Pobranie loggera
    Logger* logger1 = Logger::pobierzInstancje();
    Logger* logger2 = Logger::pobierzInstancje();

    // Sprawdzenie, czy wska�niki wskazuj� na ten sam obiekt
    if (logger1 == logger2) {
        std::cout << "Debug: logi maja ten sam adres! Ponizej sprawdzamy wskazniki:" << std::endl;
        std::cout << "Debug: logger1: " << logger1 << std::endl;
        std::cout << "Debug: logger2: " << logger2 << std::endl;
    } else {
        std::cout << "Co� jest nie tak! :(" << std::endl; // sprawdzenie ze logery maj� inne wska�niki na adres
    }

    // Dodawanie log�w
    logger1->dodajLog("info", "D: Informuje, ze dzialam.");
    logger2->dodajLog("warn", "D: ostrzezenie za brak pracy domowej.");
    logger1->dodajLog("err", "D: Nie masz RAMu na taki program.");

    // Wy�wietlenie log�w
    std::cout << "\nPokaz kotku co masz w srodku:\n" << std::endl;
    logger1->pokazLogi();
    //logger2->pokazLogi();

    // Usuni�cie loggera
    Logger::usunInstancje();

    return 0;
}
