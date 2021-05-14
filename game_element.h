#include "cpputils/graphics/image.h"

#ifndef GAME_ELEMENT_H
#define GAME_ELEMENT_H

class GameElement {
 public:
  GameElement() : GameElement(0, 0, 50, 50) {}
  GameElement(int x, int y, int width, int height)
      : x_(x), y_(y), width_(width), height_(height) {}

  bool GetIsActive() { return is_active_; }
  void SetIsActive(bool active) { is_active_ = active; }
  int GetX() { return x_; }
  int GetY() { return y_; }
  int GetWidth() { return width_; }
  int GetHeight() { return height_; }
  void SetX(int x) { x_ = x; }
  void SetY(int y) { y_ = y; }
  virtual void Draw(graphics::Image& image) = 0;
  virtual void Move(const graphics::Image& image) = 0;
  bool IntersectsWith(GameElement* opp) {
    int oppWidth = opp->GetWidth();
    int oppHeight = opp->GetHeight();
    int oppX = opp->GetX();
    int oppY = opp->GetY();
    if (oppX < x_ + width_ && oppX + oppWidth > x_ && oppY < y_ + height_ &&
        oppY + oppHeight > y_) {
      return true;
    }
    return false;
  }
  bool IsOutOfBounds(const graphics::Image& image) {
    if (x_ < 0 || y_ < 0 || x_ + width_ > image.GetWidth() ||
        y_ + height_ > image.GetHeight()) {
      return true;
    } else {
      return false;
    }
  }

 protected:
  bool is_active_ = true;
  int x_;
  int y_;
  int width_;
  int height_;
};

#endif
