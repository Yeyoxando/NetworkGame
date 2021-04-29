/*
*  Author: Diego Ochando Torres
*  Date: 06/11/2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#include <SDL_ttf.h>
#include <glm.hpp>

#include "component.h"

class GameObject;

#ifndef __LABEL_H__
#define __LABEL_H__ 

const int kLabelMaxLength = 256;

/// @brief default route source to load images
const char kDefault_font_folder[] = "../data/fonts/";
const char kDefault_font[] = "../data/fonts/impact.ttf";

/**
  * @brief Indicates the alignment of a text
*/
enum TextAlign{
  kTextAlign_Left,
  kTextAlign_Right,
  kTextAlign_Center
};

/**
  * @brief Indicates the style of a text
*/
enum TextStyle{
  kTextStyle_Normal,
  kTextStyle_Bold,
  kTextStyle_Italic
};

/**
  * @brief Class to use and manage ttf fonts as GameObjects
*/
class Label : public Component{
  //----------------------------- Constructors --------------------------------//
  public:
    /**
      * @brief Label constructor
      * @param game object GameObject that the component will belongs
    */
    Label(GameObject& game_object);
    
    /**
      * @brief Label copy constructor
      * @param other Label reference to copy
    */
    Label(GameObject& game_object, const Label& other);
    
    /**
      * @brief Parameterized Label constructor
      * @param game object GameObject that the component will belongs
      * @param text text that will contain the Label
      * @param font_src relative source to the font file (ttf)
      * @param size size that will use the Label
      * @param color color that the Label will use
    */
    Label(GameObject& game_object, const char* text, const char* font_src,
          int size, const glm::vec4 color);
          
    /**
      * @brief Label destructor
    */
    ~Label();
    
//----------------------------- Initializers --------------------------------//
    /**
      * @brief Label initializer
    */
    void init();

    /**
      * @brief parameterized Label initializer
      * @param text text that will contain the Label
      * @param font_src relative source to the font file (ttf)
      * @param size size that will use the Label
      * @param color color that the Label will use
    */
    void init(const char* text, const char* font_src, 
              int size, const glm::vec4 color);
    
//------------------------------ Setters ------------------------------------//
    /**
      * @brief text setter
      * @param text text that will contain the Label
    */
    void set_text(const char* text);
    
    /**
      * @brief font setter
      * @param font_src relative source to the font file (ttf)
    */
    void set_font(const char* font_src);
    
    /**
      * @brief size setter
      * @param size size that will use the Label
    */
    void set_size(int size);
    
    /**
      * @brief color setter
      * @param color color that the Label will use
    */
    void set_color(const glm::vec4 color);
    
    /**
      * @brief color setter overload
      * @param r red component
      * @param g green component
      * @param b blue component
      * @param a alpha component
    */
    void set_color(const char r, const char g, const char b, const char a);
    
    /**
      * @brief alignment setter, use it with TextAlign enumerator
      * @param alignment alignment that the Label will use
    */
    void set_alignment(const TextAlign alignment);
    
    /**
      * @brief style setter, use it with TextStyle enumerator
      * @param style style that the Label will use
    */
    void set_style(const TextStyle style);
    
//------------------------------ Getters ------------------------------------//
    /**
      * @brief text getter
      * @result current const char text
    */
    const char* text();
    
    /**
      * @brief font getter
      * @result current SDL_TTF font
    */
    const TTF_Font* font();
    
    /**
      * @brief size getter
      * @result current font size (int)
    */
    const int size();
    
    /**
      * @brief color getter
      * @result current label color
    */
    const glm::vec4 color();
    
    /**
      * @brief alignment getter
      * @result current text alignment
    */
    const TextAlign alignment();
    
    /**
      * @brief style getter
      * @result current text style
    */
    const TextStyle style();
    
//---------------------------------- Methods --------------------------------//
    /**
      * @brief draw the Label using its current attributes (Default component call)
    */
    void draw() override;

		/**
			* @brief inspector for label
	  */
    //void inspector();
    
//-------------------------------- Attributes -------------------------------//
    static int label_counter_;
    char font_name_[128];
    
  protected:
//-------------------------------- Attributes -------------------------------//
    char text_[kLabelMaxLength];
    TTF_Font* font_;
    int size_;
    glm::vec4 color_;
    TextAlign alignment_;
    TextStyle style_;

  private:
//----------------------------- Constructors --------------------------------//
    /**
    * @brief Label default constructor. Return error if its called.
    *        The component needs a reference to the GameObject.
    */
    Label();

 };

#endif