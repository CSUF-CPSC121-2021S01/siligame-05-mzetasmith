#include <memory>

#include "game_element.h"

#ifndef OPPONENT_H
#define OPPONENT_H

class OpponentProjectile : public GameElement {
 public:
  OpponentProjectile() : OpponentProjectile(0, 0) {}
  OpponentProjectile(int x, int y) : GameElement(x, y, 10, 10) {}
  OpponentProjectile(int x, int y, int width, int height)
      : GameElement(x, y, width, height) {}
  void Draw(graphics::Image& image) override {
    graphics::Image opp;
    opp.Load("catcall.bmp");
    if (GetIsActive() == true) {
      for (int i = 0; i < opp.GetWidth(); i++) {
        for (int j = 0; j < opp.GetHeight(); j++) {
          image.SetRed(x_ + i, y_ + j, opp.GetRed(i, j));
          image.SetGreen(x_ + i, y_ + j, opp.GetGreen(i, j));
          image.SetBlue(x_ + i, y_ + j, opp.GetBlue(i, j));
        }
      }
    }
  }
  void Move(const graphics::Image& image) override {
    if (IsOutOfBounds(image) == true) {
      is_active_ = false;
    } else {
      y_ += 2;
    }
  }
};

class Opponent : public GameElement {
 public:
  Opponent() : Opponent(0, 0) {}
  Opponent(int x, int y) : GameElement(x, y, 50, 50) {}
  Opponent(int x, int y, int width, int height)
      : GameElement(x, y, width, height) {}
  void Draw(graphics::Image& image) override {
    graphics::Image opp;
    opp.Load("pirate.bmp");
    if (GetIsActive() == true) {
      for (int i = 0; i < opp.GetWidth(); i++) {
        for (int j = 0; j < opp.GetHeight(); j++) {
          image.SetRed(x_ + i, y_ + j, opp.GetRed(i, j));
          image.SetGreen(x_ + i, y_ + j, opp.GetGreen(i, j));
          image.SetBlue(x_ + i, y_ + j, opp.GetBlue(i, j));
        }
      }
    }
  }
  void Move(const graphics::Image& image) override {
    if (IsOutOfBounds(image) == true) {
      is_active_ = false;
    } else {
      x_++;
      y_++;
    }
  }
  std::unique_ptr<OpponentProjectile> Launch() {
    std::unique_ptr<OpponentProjectile> proj(
        new OpponentProjectile(x_, y_ + 60));
    return proj;
  }
  std::unique_ptr<OpponentProjectile> LaunchProjectile() {
    count_++;
    if (count_ % 20 == 0) {
      std::unique_ptr<OpponentProjectile> proj(
          new OpponentProjectile(x_, y_ + 60));
      return proj;
    } else {
      return nullptr;
    }
  }

 private:
  int count_;
};
#endif
