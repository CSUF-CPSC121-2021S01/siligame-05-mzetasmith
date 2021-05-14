#include "opponent.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public GameElement {
 public:
  Player() : Player(0, 0) {}
  Player(int x, int y) : GameElement(x, y, 50, 50) {}
  Player(int x, int y, int width, int height)
      : GameElement(x, y, width, height) {}
  void Draw(graphics::Image& image) override {
    graphics::Image opp;
    opp.Load("mermaid.bmp");
    for (int i = 0; i < opp.GetWidth(); i++) {
      for (int j = 0; j < opp.GetHeight(); j++) {
        image.SetRed(x_ + i, y_ + j, opp.GetRed(i, j));
        image.SetGreen(x_ + i, y_ + j, opp.GetGreen(i, j));
        image.SetBlue(x_ + i, y_ + j, opp.GetBlue(i, j));
      }
    }
  }
  void Move(const graphics::Image& image) override {}
};

class PlayerProjectile : public GameElement {
 public:
  PlayerProjectile() : PlayerProjectile(0, 0) {}
  PlayerProjectile(int x, int y) : GameElement(x, y, 10, 10) {}
  PlayerProjectile(int x, int y, int width, int height)
      : GameElement(x, y, width, height) {}
  void Draw(graphics::Image& image) override {
    graphics::Image opp;
    opp.Load("song.bmp");
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
      y_ -= 2;
    }
  }
};

#endif

/*
  bool IntersectsWithProj(OpponentProjectile opp) {
    int oppWidth = opp.GetWidth();
    int oppHeight = opp.GetHeight();
    for (int i = -0.5 * width_; i == 0.5 * width_; i++) {
      for (int j = -0.5 * height_; j == 0.5 * height_; j++) {
        if (x_ + i == oppWidth + opp.GetX() || x_ + i == opp.GetX() ||
            y_ + j == oppHeight + opp.GetY() || y_ + j == opp.GetY()) {
          return true;
        }
      }
    }
    return false;
  }*/
