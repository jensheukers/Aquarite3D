/**
*	Filename: input.h
*
*	Description: Header file for Input class.
*
*	Version: 23/1/2019
*
*	� 2019, Jens Heukers
*/

//Define Button macros
#include <GLFW/glfw3.h>
#include "math/mousepicker.h"

#define		BUTTONCODE_LEFT		0
#define		BUTTONCODE_RIGHT	1
#define		BUTTONCODE_MIDDLE	2

//Define Key macros
#define		KEYCODE_EMPTY_KEY 0
#define 	KEYCODE_UNKNOWN   -1
#define 	KEYCODE_SPACE   32
#define 	KEYCODE_APOSTROPHE   39 /* ' */
#define 	KEYCODE_COMMA   44 /* , */
#define 	KEYCODE_MINUS   45 /* - */
#define 	KEYCODE_PERIOD   46 /* . */
#define 	KEYCODE_SLASH   47 /* / */
#define 	KEYCODE_0   48
#define 	KEYCODE_1   49
#define 	KEYCODE_2   50
#define 	KEYCODE_3   51
#define 	KEYCODE_4   52
#define 	KEYCODE_5   53
#define 	KEYCODE_6   54
#define 	KEYCODE_7   55
#define 	KEYCODE_8   56
#define 	KEYCODE_9   57
#define 	KEYCODE_SEMICOLON   59 /* ; */
#define 	KEYCODE_EQUAL   61 /* = */
#define 	KEYCODE_A   65
#define 	KEYCODE_B   66
#define 	KEYCODE_C   67
#define 	KEYCODE_D   68
#define 	KEYCODE_E   69
#define 	KEYCODE_F   70
#define 	KEYCODE_G   71
#define 	KEYCODE_H   72
#define 	KEYCODE_I   73
#define 	KEYCODE_J   74
#define 	KEYCODE_K   75
#define 	KEYCODE_L   76
#define 	KEYCODE_M   77
#define 	KEYCODE_N   78
#define 	KEYCODE_O   79
#define 	KEYCODE_P   80
#define 	KEYCODE_Q   81
#define 	KEYCODE_R   82
#define 	KEYCODE_S   83
#define 	KEYCODE_T   84
#define 	KEYCODE_U   85
#define 	KEYCODE_V   86
#define 	KEYCODE_W   87
#define 	KEYCODE_X   88
#define 	KEYCODE_Y   89
#define 	KEYCODE_Z   90
#define 	KEYCODE_LEFT_BRACKET   91 /* [ */
#define 	KEYCODE_BACKSLASH   92 /* \ */
#define 	KEYCODE_RIGHT_BRACKET   93 /* ] */
#define 	KEYCODE_GRAVE_ACCENT   96 /* ` */
#define 	KEYCODE_WORLD_1   161 /* non-US #1 */
#define 	KEYCODE_WORLD_2   162 /* non-US #2 */
#define 	KEYCODE_ESCAPE   256
#define 	KEYCODE_ENTER   257
#define 	KEYCODE_TAB   258
#define 	KEYCODE_BACKSPACE   259
#define 	KEYCODE_INSERT   260
#define 	KEYCODE_DELETE   261
#define 	KEYCODE_RIGHT   262
#define 	KEYCODE_LEFT   263
#define 	KEYCODE_DOWN   264
#define 	KEYCODE_UP   265
#define 	KEYCODE_PAGE_UP   266
#define 	KEYCODE_PAGE_DOWN   267
#define 	KEYCODE_HOME   268
#define 	KEYCODE_END   269
#define 	KEYCODE_CAPS_LOCK   280
#define 	KEYCODE_SCROLL_LOCK   281
#define 	KEYCODE_NUM_LOCK   282
#define 	KEYCODE_PRINT_SCREEN   283
#define 	KEYCODE_PAUSE   284
#define 	KEYCODE_F1   290
#define 	KEYCODE_F2   291
#define 	KEYCODE_F3   292
#define 	KEYCODE_F4   293
#define 	KEYCODE_F5   294
#define 	KEYCODE_F6   295
#define 	KEYCODE_F7   296
#define 	KEYCODE_F8   297
#define 	KEYCODE_F9   298
#define 	KEYCODE_F10   299
#define 	KEYCODE_F11   300
#define 	KEYCODE_F12   301
#define 	KEYCODE_F13   302
#define 	KEYCODE_F14   303
#define 	KEYCODE_F15   304
#define 	KEYCODE_F16   305
#define 	KEYCODE_F17   306
#define 	KEYCODE_F18   307
#define 	KEYCODE_F19   308
#define 	KEYCODE_F20   309
#define 	KEYCODE_F21   310
#define 	KEYCODE_F22   311
#define 	KEYCODE_F23   312
#define 	KEYCODE_F24   313
#define 	KEYCODE_F25   314
#define 	KEYCODE_KP_0   320
#define 	KEYCODE_KP_1   321
#define 	KEYCODE_KP_2   322
#define 	KEYCODE_KP_3   323
#define 	KEYCODE_KP_4   324
#define 	KEYCODE_KP_5   325
#define 	KEYCODE_KP_6   326
#define 	KEYCODE_KP_7   327
#define 	KEYCODE_KP_8   328
#define 	KEYCODE_KP_9   329
#define 	KEYCODE_KP_DECIMAL   330
#define 	KEYCODE_KP_DIVIDE   331
#define 	KEYCODE_KP_MULTIPLY   332
#define 	KEYCODE_KP_SUBTRACT   333
#define 	KEYCODE_KP_ADD   334
#define 	KEYCODE_KP_ENTER   335
#define 	KEYCODE_KP_EQUAL   336
#define 	KEYCODE_LEFT_SHIFT   340
#define 	KEYCODE_LEFT_CONTROL   341
#define 	KEYCODE_LEFT_ALT   342
#define 	KEYCODE_LEFT_SUPER   343
#define 	KEYCODE_RIGHT_SHIFT   344
#define 	KEYCODE_RIGHT_CONTROL   345
#define 	KEYCODE_RIGHT_ALT   346
#define 	KEYCODE_RIGHT_SUPER   347
#define 	KEYCODE_MENU   348
#define 	KEYCODE_LAST   KEYCODE_MENU

#ifndef INPUT_H
#define INPUT_H
#include <map>
#include "math/vec3.h"
#include "math/pointx.h"

class Input {
private:
	//global members
	static Input* _instance; /// @brief The Input singleton Instance
	
	//local members
	std::map<int, bool> _keys; /// @brief Map containing  all keys pressed this frame
	std::map<int, bool> _keysLast; /// @brief Map containing all keys pressed last frame

	std::map<int, bool> _buttons; /// @brief Map containing all buttons pressed this frame
	std::map<int, bool> _buttonsLast; /// @brief Map containing all buttons pressed last frame

	int lastKey; /// @brief The last key pressed

	Point2f _mousePos; /// @brief Point2 of floats containing Mouse Position

	MousePicker* mousePicker; /// @brief Mousepicker handles screen to world raycasting

	/**
	* Returns the instance if found, else creates a new instance and returns.
	*/
	static Input* GetInstance();
public:

	/**
	* Initialize the Input class
	*/
	static void Init(GLFWwindow* window);

	/**
	* Gets called every frame by Core to handle keys and lastkey
	*/
	static void HandleUpdates();

	/**
	* Returns true if Key is down this frame, but not down last frame.
	*/
	static bool GetKeyDown(int keyCode);

	/**
	* Returns true if Key is down this frame and last frame.
	*/
	static bool GetKey(int keyCode);

	/**
	* Returns true if Key is down last frame, but not down this frame
	*/
	static bool GetKeyUp(int keyCode);

	/**
	* Sets a key
	*/
	static void SetKey(int keyCode, bool state);

	/**
	* Returns true if Key is down this frame, but not down last frame.
	*/
	static bool GetButtonDown(int buttonCode);

	/**
	* Returns true if Key is down this frame and last frame.
	*/
	static bool GetButton(int buttonCode);

	/**
	* Returns true if Key is down last frame, but not down this frame
	*/
	static bool GetButtonUp(int buttonCode);

	/**
	* Sets a key
	*/
	static void SetButton(int buttonCode, bool state);

	/**
	* Sets the mouse position
	*/
	static void SetMousePos(Point2f point);

	/**
	* Returns the mouse position as a Point2f
	*/
	static Point2f GetMousePosition();

	/**
	* Returns the last key pressed this frame, if there is no key pressed this frame it returns KEYCODE_EMPTY_KEY
	*/
	static int GetLastKey();

	/**
	* Returns the mouse position in world space
	*/
	static Vec3 GetMousePositionWorldSpace();

	/**
	* Returns the coordinated of the mouse position + distance, requires camera as second parameter
	*/
	static Vec3 GetMouseRayPositionWorldSpace(Camera* camera, float distance);
};

#endif // !INPUT_H
