#include <gtest/gtest.h>
#include "model/Deck.hpp"
#include "model/Hand.hpp"
#include "model/Player.hpp"

TEST(DeckTest, InitialDeckHas40Cards) {
    Deck deck = DeckFactory::createItalian40();
    EXPECT_EQ(deck.remainingCards(), 40);
    EXPECT_FALSE(deck.empty());
}

TEST(DeckTest, DrawReducesDeckCount) {
    Deck deck = DeckFactory::createItalian40();
    auto card = deck.draw();
    EXPECT_TRUE(card.has_value());
    EXPECT_EQ(deck.remainingCards(), 39);
}

TEST(HandTest, AddAndPlayCard) {
    Hand hand;
    Deck deck = DeckFactory::createItalian40();

    auto cardOpt = deck.draw();
    ASSERT_TRUE(cardOpt.has_value());

    hand.addCard(*cardOpt);
    EXPECT_EQ(hand.size(), 1);
    EXPECT_FALSE(hand.empty());

    auto playedCard = hand.playCard(0);
    ASSERT_TRUE(playedCard.has_value());
    EXPECT_EQ(hand.size(), 0);
    EXPECT_TRUE(hand.empty());

    // Out-of-bounds play returns std::nullopt
    auto invalidPlay = hand.playCard(99);
    EXPECT_FALSE(invalidPlay.has_value());
}

TEST(PlayerTest, CaptureAndResetRound) {
    Player player(0, "Alice", false, 0);
    Deck deck = DeckFactory::createItalian40();

    auto c1 = deck.draw();
    auto c2 = deck.draw();
    ASSERT_TRUE(c1.has_value() && c2.has_value());

    player.hand().addCard(*c1);
    player.captureCard(*c2);

    EXPECT_EQ(player.hand().size(), 1);
    EXPECT_EQ(player.capturedCards().size(), 1);

    player.resetForNewRound();
    EXPECT_TRUE(player.hand().empty());
    EXPECT_TRUE(player.capturedCards().empty());
}