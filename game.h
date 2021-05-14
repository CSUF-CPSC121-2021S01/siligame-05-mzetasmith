#include <random>
#include <string>
#include <vector>

#include "cpputils/graphics/image.h"
#include "cpputils/graphics/image_event.h"
#include "player.h"

#ifndef GAME_H
#define GAME_H

class Game : public graphics::AnimationEventListener,
             graphics::MouseEventListener {
 public:
  Game() : Game(800, 600) {}
  Game(int KWidth, int KHeight) : KWidth_(KWidth), KHeight_(KHeight) {}
  graphics::Image& GetGameScreen() { return screen_; }
  int GetScore() { return score_; }
  std::vector<std::unique_ptr<Opponent> >& GetOpponents() { return opponents; }
  std::vector<std::unique_ptr<OpponentProjectile> >& GetOpponentProjectiles() {
    return oppProjVec_;
  }
  std::vector<std::unique_ptr<PlayerProjectile> >& GetPlayerProjectiles() {
    return playProjVec_;
  }
  Player& GetPlayer() { return player_; }
  void CreateOpponents() {
    opponents.push_back(
        std::unique_ptr<Opponent>(new Opponent((rand() % KWidth_), 30)));
  }
  void MoveGameElements() {
    for (int i = 0; i < opponents.size(); i++) {
      opponents[i].get()->Move(screen_);
      if (opponents[i].get()->IsOutOfBounds(screen_) == true) {
        opponents[i].get()->SetIsActive(false);
      }
    }
    for (int i = 0; i < oppProjVec_.size(); i++) {
      oppProjVec_[i].get()->Move(screen_);
      if (oppProjVec_[i].get()->IsOutOfBounds(screen_) == true) {
        oppProjVec_[i].get()->SetIsActive(false);
      }
    }
    for (int i = 0; i < playProjVec_.size(); i++) {
      playProjVec_[i].get()->Move(screen_);
      if (playProjVec_[i].get()->IsOutOfBounds(screen_) == true) {
        playProjVec_[i].get()->SetIsActive(false);
      }
    }
  }
  void FilterIntersections() {
    for (int i = 0; i < opponents.size(); i++) {
      if (player_.IntersectsWith(opponents[i].get()) == true) {
        player_.SetIsActive(false);
        opponents[i].get()->SetIsActive(false);
      }
    }
    for (int i = 0; i < oppProjVec_.size(); i++) {
      if (player_.IntersectsWith(oppProjVec_[i].get()) == true) {
        player_.SetIsActive(false);
        oppProjVec_[i].get()->SetIsActive(false);
      }
    }
    for (int i = 0; i < opponents.size(); i++) {
      for (int j = 0; j < playProjVec_.size(); j++) {
        if (playProjVec_[j].get()->IntersectsWith(opponents[i].get()) == true) {
          opponents[i].get()->SetIsActive(false);
          playProjVec_[j].get()->SetIsActive(false);
          if (player_.GetIsActive() == true) {
            score_++;
          }
        }
      }
    }
  }
  void Init() {
    screen_.AddMouseEventListener(*this);
    screen_.AddAnimationEventListener(*this);
    player_.SetX(200);
    player_.SetY(350);
  }
  void UpdateScreen() {
    screen_.DrawRectangle(0, 0, KWidth_, KHeight_,
                          graphics::Color(255, 255, 255));
    // if (HasLost() == false) {
    std::string s = std::to_string(score_);
    screen_.DrawText(20, 20, "Score: " + s, 36, graphics::Color(0, 0, 0));
    for (int i = 0; i < opponents.size(); i++) {
      if (opponents[i].get()->GetIsActive() == true) {
        opponents[i].get()->Draw(screen_);
      }
    }
    for (int i = 0; i < oppProjVec_.size(); i++) {
      if (oppProjVec_[i].get()->GetIsActive() == true) {
        oppProjVec_[i].get()->Draw(screen_);
      }
    }
    for (int i = 0; i < playProjVec_.size(); i++) {
      if (playProjVec_[i].get()->GetIsActive() == true) {
        playProjVec_[i].get()->Draw(screen_);
      }
    }
    if (player_.GetIsActive() == true) {
      player_.Draw(screen_);
    }
    // } else {
    // screen_.DrawText(300, 200, "You Lose :C", 36, graphics::Color(0, 0, 0));
    // }
  }

  void LaunchProjectiles() {
    opp_proj_count_++;
    if (opp_proj_count_ % 30 == 0) {
      for (int i = 0; i < opponents.size(); i++) {
        oppProjVec_.push_back(opponents[i].get()->Launch());
      }
    }
    if (opp_proj_count_ > 30) {
      opp_proj_count_ = 0;
    }
  }

  bool HasLost() {
    if (player_.GetIsActive() == true) {
      return false;
    } else {
      return true;
    }
  }
  void RemoveInactive() {
    for (int i = opponents.size() - 1; i > -1; i--) {
      if (opponents[i]->GetIsActive() == false) {
        opponents.erase(opponents.end() - i);
      }
    }
    for (int i = oppProjVec_.size() - 1; i > -1; i--) {
      if (oppProjVec_[i]->GetIsActive() == false) {
        oppProjVec_.erase(oppProjVec_.end() - i);
      }
    }
    for (int i = playProjVec_.size() - 1; i > -1; i--) {
      if (playProjVec_[i]->GetIsActive() == false) {
        playProjVec_.erase(playProjVec_.end() - i);
      }
    }
  }
  void Start() { screen_.ShowUntilClosed(); }

  void OnAnimationStep() override {
    /*if (opponents.size() < 5) {
      CreateOpponents();
    }*/
    MoveGameElements();
    LaunchProjectiles();
    FilterIntersections();
    RemoveInactive();
    UpdateScreen();
    screen_.Flush();
  }
  void OnMouseEvent(const graphics::MouseEvent& mouse) override {
    switch (mouse.GetMouseAction()) {
      case graphics::MouseAction::kDragged:
        if (mouse.GetX() >= 0 && mouse.GetY() >= 0 &&
            mouse.GetX() <= screen_.GetWidth() &&
            mouse.GetY() <= screen_.GetHeight()) {
          player_.SetX(mouse.GetX() - 25);
          player_.SetY(mouse.GetY() - 25);
          playProjVec_.push_back(std::unique_ptr<PlayerProjectile>(
              new PlayerProjectile(mouse.GetX() + 25, mouse.GetY() + 10)));
        }
        /*play_proj_count_ ++;
        if (play_proj_count_ % 20 == 0) {
          playProjVec_.push_back(std::unique_ptr<PlayerProjectile> (
            new PlayerProjectile(mouse.GetX() + 25, mouse.GetY() +10)));
        }
        if (play_proj_count_ > 20) {
          play_proj_count_ = 0;
        }*/
        break;
      case graphics::MouseAction::kMoved:
        if (mouse.GetX() >= 0 && mouse.GetY() >= 0 &&
            mouse.GetX() <= screen_.GetWidth() - 25 &&
            mouse.GetY() <= screen_.GetHeight() - 25) {
          player_.SetX(mouse.GetX() - 25);
          player_.SetY(mouse.GetY() - 25);
        }
        break;
      case graphics::MouseAction::kPressed:
        playProjVec_.push_back(std::unique_ptr<PlayerProjectile>(
            new PlayerProjectile(mouse.GetX() + 25, mouse.GetY() + 10)));
      default:
        break;
    }
  }

 private:
  std::vector<std::unique_ptr<Opponent> > opponents;
  std::vector<std::unique_ptr<OpponentProjectile> > oppProjVec_;
  std::vector<std::unique_ptr<PlayerProjectile> > playProjVec_;
  Player player_;
  int KWidth_;
  int KHeight_;
  graphics::Image screen_ = graphics::Image(KWidth_, KHeight_);
  int score_ = 0;
  bool is_alive_ = true;
  int opp_proj_count_ = 0;
  // int play_proj_count_ = 0;
};

#endif
