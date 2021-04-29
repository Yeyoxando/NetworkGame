/*
*  Author: Diego Ochando Torres
*  Date: 02-11-2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#include "component_boxcollider.h"
#include "game_object.h"

//------------------------------ Constructors -------------------------------//

BoxCollider::BoxCollider() {
#ifdef DEBUG_
  printf("\n***ERROR*** Do not call BoxCollider default constructor." \
    "(Needs a GameObject reference). Use instead another constructor");
#endif // DEBUG_
}

BoxCollider::BoxCollider(GameObject& game_object) {
  id_ = total_components_;
  component_tag_ = kComponentKind_BoxCollider;
  gameobject_ = &game_object;
  glm::vec3 empty = { 0.0f, 0.0f, 0.0f };
  origin_ = empty;
  end_ = empty;
  rect_ = { (int)origin_.x, (int)origin_.y, (int)gameobject_->transform_.size_.x, (int)gameobject_->transform_.size_.y };
  color_ = glm::vec4(255, 255, 255, 255);
}

BoxCollider::BoxCollider(GameObject& game_object, const BoxCollider& other) {
  id_ = total_components_;
  component_tag_ = kComponentKind_BoxCollider;
  gameobject_ = &game_object;
  origin_ = other.origin_;
  end_ = other.end_;
  rect_ = other.rect_;
  color_ = other.color_;
}

BoxCollider::BoxCollider(GameObject& game_object, const glm::vec3 origin) {
  id_ = total_components_;
  component_tag_ = kComponentKind_BoxCollider;
  gameobject_ = &game_object;
  origin_ = origin;
  end_ = glm::vec3(origin.x + gameobject_->transform_.size_.x, origin.y + gameobject_->transform_.size_.y, 0);
  rect_ = { (int)origin_.x, (int)origin_.y, (int)gameobject_->transform_.size_.x, (int)gameobject_->transform_.size_.y };
  color_ = glm::vec4(255, 255, 255, 255);
}


BoxCollider::~BoxCollider() {

}

//---------------------------------------------------------------------------//

//----------------------------- Initializers --------------------------------//

void BoxCollider::init() {
  component_tag_ = kComponentKind_BoxCollider;
  glm::vec3 empty = { 0.0f, 0.0f, 0.0f };
  origin_ = empty;
  end_ = empty;
  rect_ = { (int)origin_.x, (int)origin_.y, (int)gameobject_->transform_.size_.x, (int)gameobject_->transform_.size_.y };
  color_ = glm::vec4(255, 255, 255, 255);
}

void BoxCollider::init(const glm::vec3 origin) {
  component_tag_ = kComponentKind_BoxCollider;
  origin_ = origin;
  end_ = glm::vec3(origin.x + gameobject_->transform_.size_.x, origin.y + gameobject_->transform_.size_.y, 0);
  rect_ = { (int)origin_.x, (int)origin_.y, (int)gameobject_->transform_.size_.x, (int)gameobject_->transform_.size_.y };
  color_ = glm::vec4(255, 255, 255, 255);
}

//---------------------------------------------------------------------------//

//------------------------------- Setters -----------------------------------//

void BoxCollider::set_color(const glm::vec4 color) {
  color_ = color;
}

void BoxCollider::set_color(const unsigned char r, const unsigned char g,
  const unsigned char b) {
  color_.r = r;
  color_.g = g;
  color_.b = b;
  color_.a = 255;
}

//---------------------------------------------------------------------------//

//------------------------------- Getters -----------------------------------//

const glm::vec3 BoxCollider::origin() {
  return origin_;
}

const glm::vec4 BoxCollider::color() {
  return color_;
}

//---------------------------------------------------------------------------//

//------------------------------- Methods -----------------------------------//
bool BoxCollider::checkCollision(const BoxCollider& other_collider) {
	bool first_check = false;
	bool second_check = false;
	int collision_points = 0;

	if ((other_collider.origin_.x <= end_.x &&
		other_collider.origin_.x >= origin_.x)
		||
		(other_collider.end_.x <= end_.x &&
			other_collider.end_.x >= origin_.x)) {
		collision_points++;
	}
	if ((other_collider.origin_.y <= end_.y &&
		other_collider.origin_.y >= origin_.y)
		||
		(other_collider.end_.y <= end_.y &&
			other_collider.end_.y >= origin_.y)) {
		collision_points++;
	}

	first_check = collision_points == 2;
	collision_points = 0;

	if ((origin_.x <= other_collider.end_.x &&
		origin_.x >= other_collider.origin_.x)
		||
		(end_.x <= other_collider.end_.x &&
			end_.x >= other_collider.origin_.x)) {
		collision_points++;
	}
	if ((origin_.y <= other_collider.end_.y &&
		origin_.y >= other_collider.origin_.y)
		||
		(end_.y <= other_collider.end_.y &&
			end_.y >= other_collider.origin_.y)) {
		collision_points++;
	}

	second_check = collision_points == 2;

	return first_check || second_check;
}

bool BoxCollider::checkCollision(const BoxCollider& collider1, 
                                 const BoxCollider& collider2) {
  bool first_check = false;
  bool second_check = false;
  int collision_points = 0;

  if ((collider2.origin_.x <= collider1.end_.x &&
    collider2.origin_.x >= collider1.origin_.x)
    ||
    (collider2.end_.x <= collider1.end_.x &&
      collider2.end_.x >= collider1.origin_.x)) {
    collision_points++;
  }
  if ((collider2.origin_.y <= collider1.end_.y &&
    collider2.origin_.y >= collider1.origin_.y)
    ||
    (collider2.end_.y <= collider1.end_.y &&
      collider2.end_.y >= collider1.origin_.y)) {
    collision_points++;
  }

  first_check = collision_points == 2;
  collision_points = 0;

  if ((collider1.origin_.x <= collider2.end_.x &&
    collider1.origin_.x >= collider2.origin_.x)
    ||
    (collider1.end_.x <= collider2.end_.x &&
      collider1.end_.x >= collider2.origin_.x)) {
    collision_points++;
  }
  if ((collider1.origin_.y <= collider2.end_.y &&
    collider1.origin_.y >= collider2.origin_.y)
    ||
    (collider1.end_.y <= collider2.end_.y &&
      collider1.end_.y >= collider2.origin_.y)) {
    collision_points++;
  }

  second_check = collision_points == 2;

  return first_check || second_check;
}

bool BoxCollider::checkCollision(const BoxCollider& collider, 
                                 const glm::vec2 point) {
  bool check = false;
  int collision_points = 0;

  if (collider.origin_.x <= point.x && collider.end_.x >= point.x) {
    collision_points++;
  }
  if (collider.origin_.y <= point.y && collider.end_.y >= point.y) {
    collision_points++;
  }

  check = collision_points == 2;
  return check;
}

/*void BoxCollider::inspector() {
  if (ImGui::TreeNode("BoxCollider")) {

    ImGui::TreePop();
  }
}*/

void BoxCollider::update() {
  origin_ = gameobject_->transform_.position_;
  end_.x = origin_.x + gameobject_->transform_.size_.x * gameobject_->transform_.scale_.x;
  end_.y = origin_.y + gameobject_->transform_.size_.y * gameobject_->transform_.scale_.y;

  origin_.x -= ((gameobject_->transform_.size_.x * gameobject_->transform_.scale_.x) * 0.1f);
  origin_.y -= ((gameobject_->transform_.size_.y * gameobject_->transform_.scale_.y) * 0.1f);
  end_.x += ((gameobject_->transform_.size_.x * gameobject_->transform_.scale_.x) * 0.1f);
  end_.y += ((gameobject_->transform_.size_.y * gameobject_->transform_.scale_.y) * 0.1f);

  origin_.x -= gameobject_->transform_.size_.x / 2;
  origin_.y -= gameobject_->transform_.size_.y / 2;
  end_.x -= gameobject_->transform_.size_.x / 2;
  end_.y -= gameobject_->transform_.size_.y / 2;


  rect_ = { (int)origin_.x, (int)origin_.y,
           (int)(end_.x - origin_.x), (int)(end_.y - origin_.y) };
}

void BoxCollider::draw() {
  /*if (Editor::draw_colliders_) {
    SDL_SetRenderDrawColor(Game::window_.renderer_, color_.r, color_.g, color_.b, color_.a);

    SDL_RenderDrawRect(Game::window_.renderer_, &rect_);
  }*/
}

void BoxCollider::drawWithDiagonal() {
  /*if (Editor::draw_colliders_) {
    SDL_SetRenderDrawColor(Game::window_.renderer_, color_.r, color_.g, color_.b, color_.a);

    SDL_RenderDrawRect(Game::window_.renderer_, &rect_);
    SDL_RenderDrawLine(Game::window_.renderer_, (int)origin_.x, (int)origin_.y,
      (int)end_.x, (int)end_.y);
  }*/
}

//---------------------------------------------------------------------------//
