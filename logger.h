#include <iostream>
#include <vector>
#include <memory>
#include <string>

// Bazowa klasa abstrakcyjna dla log�w
class Log {
public:
    virtual ~Log() = default;                           // Wirtualny destruktor dla poprawnego usuwania obiekt�w.
    virtual std::string pobierzWiadomosc() const = 0;   // Czysto wirtualna metoda
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
        if (typ == "ostrzezenie") return std::make_unique<LogOstrzezenie>(wiadomosc);
        if (typ == "blad") return std::make_unique<LogBlad>(wiadomosc);
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
        std::cout << "Logger dzia�a jako Singleton. Oba wska�niki wskazuj� na t� sam� instancj�." << std::endl;
    } else {
        std::cout << "B��d: Istnieje wi�cej ni� jeden logger!" << std::endl;
    }

    // Dodawanie log�w
    logger1->dodajLog("info", "Aplikacja zosta�a uruchomiona.");
    logger1->dodajLog("ostrzezenie", "Niski poziom pami�ci.");
    logger1->dodajLog("blad", "Nie znaleziono pliku.");

    // Wy�wietlenie log�w
    std::cout << "\nLogi:" << std::endl;
    logger1->pokazLogi();

    // Usuni�cie loggera
    Logger::usunInstancje();

    return 0;
}
