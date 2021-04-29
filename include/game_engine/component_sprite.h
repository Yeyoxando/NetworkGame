/*
*  Author: Diego Ochando Torres
*  Date: 06/11/2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#ifndef __SPRITE_H__
#define __SPRITE_H__ 

#include <SDL.h>

#include "component.h"

struct Transform;
class GameObject;

//------------------------------ Enumerators --------------------------------//

/// @brief indicates the origin of a loaded Sprite
enum SpriteOrigin{
  kSpriteOrigin_Null, //Default
  kSpriteOrigin_Image, //Image load from file
  kSpriteOrigin_Buffer, //SDL_CreateRGBSurfaceFrom (Buffered texture)
};

/// @brief indicates with which Pivot should the Sprite be drawn
enum PivotPoint{
  kPivotPoint_TopLeft,
  kPivotPoint_Left,
  kPivotPoint_BottomLeft,
  kPivotPoint_Bottom,
  kPivotPoint_BottomRight,
  kPivotPoint_Right,
  kPivotPoint_TopRight,
  kPivotPoint_Top,
  kPivotPoint_Center
};

/// @brief default route source to load images
const char default_sprite_folder[] = "../data/images/";

class Sprite : public Component{
  public:
//----------------------------- Constructors --------------------------------//
    /**
    * @brief Sprite default constructor. Return error if its called.
             The component needs a reference to the GameObject.
    */
    Sprite();

    /**
      * @brief Sprite copy constructor
      * @param other Sprite to copy
    */
    Sprite(GameObject& gameobject, const Sprite& other);
    
    /**
      * @brief Parameterized Sprite load image constructor
      * @param gameobject GameObject that the component will belongs
      * @param file_name name of the image to load (Only name, the folder 
      *        will be loaded with the default_sprite_folder const)
    */
    Sprite(GameObject& gameobject, const char* file_name);
    
    /**
      * @brief Parameterized Sprite load buffer constructor 
      * @param gameobject GameObject that the component will belongs
      * @param width texture width
      * @param height texture height
      * @param buffer buffered texture to load
    */
    Sprite(GameObject& gameobject, int width, int height, const unsigned char* buffer);
    
    /**
      * @brief Sprite destructor
    */
    ~Sprite();
    
//----------------------------- Initializers --------------------------------//
    /**
      * @brief Parameterized Sprite load image initializer 
      * @param file_name name of the image to load (Only name, the folder 
      *        will be loaded with the deault_folder const)
    */
    void initFromFile(const char* file_name);
    
    /**
      * @brief Parameterized Sprite load buffer constructor 
      * @param width texture width
      * @param height texture height
      * @param buffer buffered texture to load
    */
    void initFromBuffer(int width, int height, const unsigned char* buffer);
    
//------------------------------ Setters ------------------------------------//
    /**
      * @brief Sprite pivot point setter
      * @param pivot_point new Sprite PivotPoint
    */
    void set_pivotPoint(PivotPoint pivot_point);
    
//------------------------------ Getters ------------------------------------//
    /**
      * @brief Sprite width getter
    */
    const int width();
    
    /**
      * @brief Sprite height getter
    */
    const int height();
    
    /**
      * @brief Sprite PivotPoint getter
    */
    const PivotPoint pivotPoint();
      
//-------------------------------- Methods ----------------------------------//
    virtual void update() override;

    /**
      * @brief Draw the sprite with GameObject transform (Default component call)
    */
    void draw() override;

	  /**
	    * @brief ..
	  */
	  //void inspector();

    /**
      * @brief Draw the sprite with a custom transform
      * @param transform transform to modify the Sprite before drawing
    */
    void draw(Transform transform);
    
//------------------------------ Atributtes ---------------------------------//

    static unsigned char default_texture[16];
    char img_name_[128];
    PivotPoint pivot_point_;
	  int pivot_selection_;
    
  protected:
//------------------------------ Atributtes ---------------------------------//
    SDL_Surface* img_;
    SDL_Texture* texture_;
    SDL_Rect render_rect_;
    SpriteOrigin origin_;
};

#endif