/*
*  Author: Diego Ochando Torres
*  Date: 27-11-2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#include "component.h"

int Component::total_components_ = 0;

Component::Component() {
  component_tag_ = kComponentKind_Default;
  ++total_components_;
}

Component::Component(const Component& other) {
  ++total_components_;
}

Component::~Component() {

}

void Component::init() {

}

void Component::update() {

}

void Component::input() {

}

void Component::draw() {

}

void Component::inspector() {

}

void Component::onCollision(int other_tag_id) {
  
}

void Component::onCollision(GameObject* other) {

}