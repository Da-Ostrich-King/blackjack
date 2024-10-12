#include <algorithm>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <random>
#include <ctime>
#include <raylib.h>
#include <cctype>


// initialize random number generator

std::random_device os_seed;
const uint_least32_t seed = os_seed();
std::mt19937 generator(seed);



enum Suit {
  HEARTS = 0,
  DIAMONDS,
  CLUBS,
  SPADES
};

const char *suitValues[] = {"Hearts",
                            "Diamonds",
                            "Clubs",
                            "Spades"};

enum Rank {
  ACE = 0,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
  NINE,
  TEN,
  JACK,
  QUEEN,
  KING
};

const char *rankValues[] = {"Ace",
                            "Two",
                            "Three",
                            "Four",
                            "Five",
                            "Six",
                            "Seven",
                            "Eight",
                            "Nine",
                            "Ten",
                            "Jack",
                            "Queen",
                            "King"};

struct Card {
  Suit suit;
  Rank rank;
  std::string getFileName() {
    std::string filename = std::string(rankValues[rank]) +
                           std::string(rankValues[suit]) + ".png";

  std::transform(filename.begin(), filename.end(), filename.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return filename;
    }
};


void drawCard(std::vector<Card> &hand, std::vector<Card> &deck) {
  std::uniform_int_distribution<int> distrib(0, deck.size() - 1);
  int deckCard = distrib(generator);
  hand.push_back(deck[deckCard]);
  deck.erase(deck.begin() + deckCard);
}

void printHand(std::vector<Card> hand) {
  std::cout <<  rankValues[hand[0].rank] << " of " << suitValues[hand[0].suit] << ", ";
  for (int i = 1; i < hand.size() - 1; i++) {
    std::cout << "a " << rankValues[hand[i].rank] << " of " << suitValues[hand[i].suit] << ", ";
  }
  std::cout << "and a " << rankValues[hand[hand.size() - 1].rank] << " of " << suitValues[hand[hand.size() - 1].suit];
  std::cout << ".\n";
}

int getTotal(const std::vector<Card> &hand) {
  int total = 0;
  int aces = 0;
  for (const auto &card : hand) {
    if (card.rank == 0) {
      aces += 1;
      continue;
    } else {
      total += (card.rank + 1 <= 10) ? card.rank + 1 : 10;
    }
  }
  if (aces > 0) {
    for (int i = 0; true; i++) {
      if (total + 11 * (aces - i) < 21) {
        total += 11 * (aces - 1);
        total += i;
        break;
      }
    }


    if (total + (aces * 11) < 21) {
      total += aces * 11;
      aces = 0;
    }
    else if (total + ((aces - 1) * 11) < 21 && aces >= 2) {
      total += ((aces - 1) * 11) + 1;
    }
  }

  return total;
}

void doBlackJackRound(std::vector<Card> yourHand, std::vector<Card> dealerHand, std::vector<Card> deck) {
  drawCard(yourHand, deck);
  drawCard(yourHand, deck);
  drawCard(dealerHand, deck);
  drawCard(dealerHand, deck);

  std::cout << "Your hand: ";
  printHand(yourHand);

  std::cout << "Dealer's hand: ";
  std::cout << rankValues[dealerHand[0].rank] << " of " << suitValues[dealerHand[0].suit] << ", hidden\n";

  int yourTotal = 0;
  int dealerTotal = 0;
  yourTotal = getTotal(yourHand);
  dealerTotal = getTotal(dealerHand);
  std::cout << "You current total is: " << yourTotal << "\n";
  std::cout << "Hit or Stand?: ";
  std::string input;
  std::cin >> input;

  while (yourTotal <= 21 && (input == "hit" || input == "Hit")) {
    drawCard(yourHand, deck);
    yourTotal = getTotal(yourHand);
    std::cout << "You drew a " << rankValues[yourHand[yourHand.size() - 1].rank] << " of " << suitValues[yourHand[yourHand.size() - 1].suit] << ".\n";
    if (yourTotal <= 21) {
      std::cout << "Your current total is: " << yourTotal << "\n";
      std::cout << "Hit or Stand?: ";
      std::cin >> input;
    }
  }
  if (yourTotal > 21) {
    std::cout << "You bust, you have a " << getTotal(yourHand) << "\nYou had: "; printHand(yourHand);
  }
  else {
    std::cout << "Dealer has: "; printHand(dealerHand);
    while (dealerTotal <= 17) {
      drawCard(dealerHand, deck);
      dealerTotal = getTotal(dealerHand);
      std::cout << "Dealer drew a " << rankValues[dealerHand[dealerHand.size() - 1].rank] << " of " << suitValues[dealerHand[dealerHand.size() - 1].suit] << ".\n";
    }
    if (getTotal(dealerHand) > 21) {
      std::cout << "Dealer bust" << std::endl;
    }
  }

}

std::vector<Card> genDeck() {
  std::vector<Card> deck;
  for (int suit = HEARTS; suit <= SPADES; suit++) {
    for (int rank = ACE; rank <= KING; rank++) {
      deck.push_back(Card{static_cast<Suit>(suit), static_cast<Rank>(rank)});
    }
  }
  return deck;
}


int main(int argc, char *argv[]) {
  std::vector<Card> deck = genDeck();
  std::vector<Card> usedCards;

  std::cout << "\n\n\nWelcome to Casino Blackjack\n";

  std::vector<Card> yourHand;
  std::vector<Card> dealerHand;

  InitWindow(800, 600, "Blackjack");

  while (!WindowShouldClose()) {

    BeginDrawing();

    EndDrawing();

  }

  return 0;
}
