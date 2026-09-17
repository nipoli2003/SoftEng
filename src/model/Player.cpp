#include "model/Player.hpp"
#include <utility>

Player::Player(int id, std::string name, bool isAI, int teamId)
    : m_id(id),
      m_teamId(teamId == -1 ? id : teamId), // Defaults to individual team if unset
      m_name(std::move(name)),
      m_isAI(isAI) {}

void Player::captureCard(const Card& card) {
    m_capturedCards.push_back(card);
}

void Player::captureCards(const std::vector<Card>& cards) {
    m_capturedCards.insert(m_capturedCards.end(), cards.begin(), cards.end());
}

void Player::resetForNewRound() noexcept {
    m_hand.clear();
    m_capturedCards.clear();
}