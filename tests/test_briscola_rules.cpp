#include <gtest/gtest.h>
#include "model/BriscolaRules.hpp"
#include "model/Player.hpp"

TEST(BriscolaRulesTest, PointsCalculation) {
    BriscolaRules rules(Suit::Bastoni_Clubs);

    EXPECT_EQ(rules.calculateCardPoints(Card(Rank::Ace, Suit::Bastoni_Clubs, 0)), 11);
    EXPECT_EQ(rules.calculateCardPoints(Card(Rank::Three, Suit::Coppe_Hearts, 0)), 10);
    EXPECT_EQ(rules.calculateCardPoints(Card(Rank::King_Re, Suit::Denari_Diamonds, 0)), 4);
    EXPECT_EQ(rules.calculateCardPoints(Card(Rank::Knight_Cavallo, Suit::Spade_Spades, 0)), 3);
    EXPECT_EQ(rules.calculateCardPoints(Card(Rank::Jack_Fante, Suit::Spade_Spades, 0)), 2);
    EXPECT_EQ(rules.calculateCardPoints(Card(Rank::Seven, Suit::Denari_Diamonds, 0)), 0);
}

TEST(BriscolaRulesTest, TrumpBeatsLeadSuit) {
    // Trump is Bastoni
    BriscolaRules rules(Suit::Bastoni_Clubs);

    // Player 0 leads Ace of Coppe (11 pts)
    // Player 1 plays Two of Bastoni (lowest trump)
    std::vector<Card> trick = {
        Card(Rank::Ace, Suit::Coppe_Hearts, 0),
        Card(Rank::Two, Suit::Bastoni_Clubs, 0)
    };

    EXPECT_EQ(rules.evaluateTrick(trick), 1); // Index 1 (Trump) wins
}

TEST(BriscolaRulesTest, FourPlayerTrick) {
    BriscolaRules rules(Suit::Spade_Spades);

    // 4 players (P0, P1, P2, P3)
    std::vector<Card> trick = {
        Card(Rank::Seven, Suit::Coppe_Hearts, 0),  // P0 leads Coppe
        Card(Rank::Three, Suit::Coppe_Hearts, 0),  // P1 plays 3 of Coppe
        Card(Rank::Two, Suit::Spade_Spades, 0),    // P2 plays low Trump
        Card(Rank::King_Re, Suit::Spade_Spades, 0) // P3 plays King of Trump
    };

    EXPECT_EQ(rules.evaluateTrick(trick), 3); // P3's Trump King wins
}

TEST(BriscolaRulesTest, TeamScoring2v2) {
    BriscolaRules rules(Suit::Denari_Diamonds);

    Player p0(0, "North", false, 0); // Team 0
    Player p1(1, "East",  false, 1); // Team 1
    Player p2(2, "South", false, 0); // Team 0
    Player p3(3, "West",  false, 1); // Team 1

    p0.captureCard(Card(Rank::Ace, Suit::Coppe_Hearts, 0));   // 11 pts
    p2.captureCard(Card(Rank::Three, Suit::Spade_Spades, 0)); // 10 pts
    p1.captureCard(Card(Rank::King_Re, Suit::Denari_Diamonds, 0)); // 4 pts

    std::vector<Player> players = { p0, p1, p2, p3 };
    auto scores = rules.calculateTeamScores(players);

    EXPECT_EQ(scores[0], 21); // 11 + 10
    EXPECT_EQ(scores[1], 4);  // 4
}