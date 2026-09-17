#include "model/Card.hpp"
#include <string>

std::string Card::toString() const {
    if (isJoker()) return "Joker";

    std::string_view rankStr;
    switch (m_rank) {
        case Rank::Ace:            rankStr = "Asso/Ace"; break;
        case Rank::Two:            rankStr = "2"; break;
        case Rank::Three:          rankStr = "3"; break;
        case Rank::Four:           rankStr = "4"; break;
        case Rank::Five:           rankStr = "5"; break;
        case Rank::Six:            rankStr = "6"; break;
        case Rank::Seven:          rankStr = "7"; break;
        case Rank::Eight:          rankStr = "8"; break;
        case Rank::Nine:           rankStr = "9"; break;
        case Rank::Ten:            rankStr = "10"; break;
        case Rank::Jack_Fante:     rankStr = "Fante/Jack"; break;
        case Rank::Knight_Cavallo: rankStr = "Cavallo"; break;
        case Rank::Queen_Regina:   rankStr = "Regina/Queen"; break;
        case Rank::King_Re:        rankStr = "Re/King"; break;
        default:                   rankStr = "?"; break;
    }

    std::string_view suitStr;
    switch (m_suit) {
        case Suit::Bastoni_Clubs:   suitStr = "Bastoni/Clubs"; break;
        case Suit::Coppe_Hearts:    suitStr = "Coppe/Hearts"; break;
        case Suit::Denari_Diamonds: suitStr = "Denari/Diamonds"; break;
        case Suit::Spade_Spades:    suitStr = "Spade/Spades"; break;
        default:                    suitStr = "None"; break;
    }

    std::string result;
    result.reserve(rankStr.size() + 4 + suitStr.size());
    result.append(rankStr).append(" of ").append(suitStr);
    return result;
}