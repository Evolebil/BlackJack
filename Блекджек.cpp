#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

// ============================================================
// АБСТРАКЦИЯ: базовый интерфейс для любого участника игры
// ============================================================
class IGameParticipant {
public:
    virtual void addCard(const std::string& card, int value) = 0;
    virtual int  getScore() const = 0;
    virtual void showHand() const = 0;
    virtual void reset() = 0;
    virtual ~IGameParticipant() = default;
};

// ============================================================
// БАЗОВЫЙ КЛАСС: общая логика для Игрока и Дилера
// ============================================================
class Participant : public IGameParticipant {
protected:
    std::vector<std::string> hand;   // карты на руке (видимые строки)
    std::vector<int>         values; // числовые значения карт
    int score;

    // Пересчёт очков с учётом туза
    void recalcScore() {
        score = 0;
        int aces = 0;
        for (int v : values) {
            if (v == 11) { ++aces; score += 11; }
            else          { score += v; }
        }
        while (score > 21 && aces > 0) {
            score -= 10;
            --aces;
        }
    }

public:
    Participant() : score(0) {}

    void addCard(const std::string& card, int value) override {
        hand.push_back(card);
        values.push_back(value);
        recalcScore();
    }

    int getScore() const override { return score; }

    void reset() override {
        hand.clear();
        values.clear();
        score = 0;
    }

    // Геттер: список карт в виде строки
    std::string getHandStr() const {
        std::string result;
        for (size_t i = 0; i < hand.size(); ++i) {
            if (i) result += " ";
            result += hand[i];
        }
        return result;
    }
};

// ============================================================
// ИГРОК
// ============================================================
class Player : public Participant {
public:
    void showHand() const override {
        std::cout << "Вы: " << getHandStr()
                  << " (" << score << ")\n";
    }
};

// ============================================================
// ДИЛЕР (наследник Participant с особым показом руки)
// Полиморфизм: showHand() переопределён — скрывает вторую карту
// ============================================================
class Dealer : public Participant {
public:
    // Показать руку с закрытой второй картой
    void showHand() const override {
        if (hand.size() >= 2) {
            std::cout << "Дилер: " << hand[0] << " [?]\n";
        } else {
            std::cout << "Дилер: " << getHandStr() << "\n";
        }
    }

    // ПОЛИМОРФИЗМ: другой метод для показа полной руки
    void showFullHand() const {
        std::cout << "Дилер: " << getHandStr()
                  << " (" << score << ")\n";
    }

    bool shouldHit() const { return score < 17; }
};

// ============================================================
// КОЛОДА КАРТ
// ============================================================
class Deck {
private:
    struct Card {
        std::string face;
        int value;
    };

    std::vector<Card> cards;
    std::mt19937 rng;
    size_t pos; // текущая позиция раздачи

    void build() {
        cards.clear();
        pos = 0;
        const std::string suits[]  = { "♠", "♥", "♦", "♣" };
        const std::string ranks[]  = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
        const int         vals[]   = {  2,  3,  4,  5,  6,  7,  8,  9, 10,  10, 10, 10, 11 };

        for (const auto& suit : suits) {
            for (int r = 0; r < 13; ++r) {
                cards.push_back({ ranks[r] + suit, vals[r] });
            }
        }
    }

public:
    Deck() : rng(static_cast<unsigned>(std::time(nullptr))), pos(0) {
        build();
        shuffle();
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
        pos = 0;
    }

    // Вернуть следующую карту; пересдать если кончились
    std::pair<std::string, int> deal() {
        if (pos >= cards.size()) {
            build();
            shuffle();
        }
        const Card& c = cards[pos++];
        return { c.face, c.value };
    }
};

// ============================================================
// ИГРОВОЙ ДВИЖОК
// ============================================================
class BlackjackGame {
private:
    Deck   deck;
    Player player;
    Dealer dealer;

    // Безопасный ввод 1 или 2
    int readChoice(const std::string& prompt) const {
        while (true) {
            std::cout << prompt;
            int n;
            if (std::cin >> n && (n == 1 || n == 2)) {
                return n - 1; // 0 = первый вариант, 1 = второй
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Неверный ввод! Введите 1 или 2.\n";
        }
    }

    void dealInitialCards() {
        player.reset();
        dealer.reset();

        for (int i = 0; i < 2; ++i) {
            auto [pf, pv] = deck.deal();
            player.addCard(pf, pv);

            auto [df, dv] = deck.deal();
            dealer.addCard(df, dv);
        }
    }

    void showTable() const {
        dealer.showHand();  // полиморфный вызов — скрывает вторую карту
        player.showHand();
    }

    // Проверка на блэкджек сразу после раздачи
    bool checkBlackjack() {
        bool pBJ = (player.getScore() == 21);
        bool dBJ = (dealer.getScore() == 21);
        if (pBJ || dBJ) {
            dealer.showFullHand();
            player.showHand();
            std::cout << "=== ИТОГ ===\n";
            if (pBJ && dBJ) std::cout << "Оба блэкджек! Ничья!\n";
            else if (pBJ)   std::cout << "БЛЭКДЖЕК! Вы победили!\n";
            else             std::cout << "Блэкджек у дилера. Вы проиграли!\n";
            return true;
        }
        return false;
    }

    void playerTurn() {
        while (true) {
            showTable();
            if (player.getScore() >= 21) break;

            std::cout << "[1] Взять карту  [2] Стоп: ";
            int choice = readChoice("");
            if (choice == 1) break; // Стоп

            auto [f, v] = deck.deal();
            player.addCard(f, v);

            if (player.getScore() > 21) {
                showTable();
                break;
            }
        }
    }

    void dealerTurn() {
        std::cout << "--- Ход дилера ---\n";
        while (dealer.shouldHit()) {
            auto [f, v] = deck.deal();
            dealer.addCard(f, v);
        }
        dealer.showFullHand();
    }

    void showResult() const {
        int ps = player.getScore();
        int ds = dealer.getScore();

        std::cout << "=== ИТОГ ===\n";
        std::cout << "Вы: " << ps << " | Дилер: " << ds << "\n";

        bool pBust = ps > 21;
        bool dBust = ds > 21;

        if (pBust && dBust) {
            // Оба перебрали — ближе к 21 побеждает
            int pDiff = ps - 21;
            int dDiff = ds - 21;
            if (pDiff < dDiff)       std::cout << "Оба перебрали, но вы ближе к 21. Вы победили!\n";
            else if (dDiff < pDiff)  std::cout << "Оба перебрали, но дилер ближе к 21. Вы проиграли!\n";
            else                     std::cout << "Оба перебрали одинаково. Ничья!\n";
        } else if (pBust) {
            std::cout << "Перебор! Вы проиграли!\n";
        } else if (dBust) {
            std::cout << "Перебор у дилера! Вы победили!\n";
        } else if (ps > ds) {
            std::cout << "Вы победили!\n";
        } else if (ps < ds) {
            std::cout << "Вы проиграли!\n";
        } else {
            std::cout << "Ничья!\n";
        }
    }

public:
    void run() {
        std::cout << "=== БЛЭКДЖЕК ===\n\n";

        while (true) {
            dealInitialCards();

            if (!checkBlackjack()) {
                playerTurn();

                if (player.getScore() <= 21) {
                    dealerTurn();
                }

                showResult();
            }

            std::cout << "\nСыграть ещё? (1/2): ";
            int again = readChoice("");
            if (again == 1) break; // "2" = нет

            std::cout << "\n=== БЛЭКДЖЕК ===\n\n";
        }

        std::cout << "Спасибо за игру!\n";
    }
};

// ============================================================
// ТОЧКА ВХОДА
// ============================================================
int main() {
    BlackjackGame game;
    game.run();
    return 0;
}