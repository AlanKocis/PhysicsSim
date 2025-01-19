#ifndef INPUT_H
#define INPUT_H

#define HOBBES_KEY_SPACE              32
#define HOBBES_KEY_APOSTROPHE         39  /* ' */
#define HOBBES_KEY_COMMA              44  /* , */
#define HOBBES_KEY_MINUS              45  /* - */
#define HOBBES_KEY_PERIOD             46  /* . */
#define HOBBES_KEY_SLASH              47  /* / */
#define HOBBES_KEY_0                  48
#define HOBBES_KEY_1                  49
#define HOBBES_KEY_2                  50
#define HOBBES_KEY_3                  51
#define HOBBES_KEY_4                  52
#define HOBBES_KEY_5                  53
#define HOBBES_KEY_6                  54
#define HOBBES_KEY_7                  55
#define HOBBES_KEY_8                  56
#define HOBBES_KEY_9                  57
#define HOBBES_KEY_SEMICOLON          59  /* ; */
#define HOBBES_KEY_EQUAL              61  /* = */
#define HOBBES_KEY_A                  65
#define HOBBES_KEY_B                  66
#define HOBBES_KEY_C                  67
#define HOBBES_KEY_D                  68
#define HOBBES_KEY_E                  69
#define HOBBES_KEY_F                  70
#define HOBBES_KEY_G                  71
#define HOBBES_KEY_H                  72
#define HOBBES_KEY_I                  73
#define HOBBES_KEY_J                  74
#define HOBBES_KEY_K                  75
#define HOBBES_KEY_L                  76
#define HOBBES_KEY_M                  77
#define HOBBES_KEY_N                  78
#define HOBBES_KEY_O                  79
#define HOBBES_KEY_P                  80
#define HOBBES_KEY_Q                  81
#define HOBBES_KEY_R                  82
#define HOBBES_KEY_S                  83
#define HOBBES_KEY_T                  84
#define HOBBES_KEY_U                  85
#define HOBBES_KEY_V                  86
#define HOBBES_KEY_W                  87
#define HOBBES_KEY_X                  88
#define HOBBES_KEY_Y                  89
#define HOBBES_KEY_Z                  90
#define HOBBES_KEY_LEFT_BRACKET       91  /* [ */
#define HOBBES_KEY_BACKSLASH          92  /* \ */
#define HOBBES_KEY_RIGHT_BRACKET      93  /* ] */
#define HOBBES_KEY_GRAVE_ACCENT       96  /* ` */
#define HOBBES_KEY_WORLD_1            161 /* non-US #1 */
#define HOBBES_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define HOBBES_KEY_ESCAPE             256
#define HOBBES_KEY_ENTER              257
#define HOBBES_KEY_TAB                258
#define HOBBES_KEY_BACKSPACE          259
#define HOBBES_KEY_INSERT             260
#define HOBBES_KEY_DELETE             261
#define HOBBES_KEY_RIGHT              262
#define HOBBES_KEY_LEFT               263
#define HOBBES_KEY_DOWN               264
#define HOBBES_KEY_UP                 265
#define HOBBES_KEY_PAGE_UP            266
#define HOBBES_KEY_PAGE_DOWN          267
#define HOBBES_KEY_HOME               268
#define HOBBES_KEY_END                269
#define HOBBES_KEY_CAPS_LOCK          280
#define HOBBES_KEY_SCROLL_LOCK        281
#define HOBBES_KEY_NUM_LOCK           282
#define HOBBES_KEY_PRINT_SCREEN       283
#define HOBBES_KEY_PAUSE              284
#define HOBBES_KEY_F1                 290
#define HOBBES_KEY_F2                 291
#define HOBBES_KEY_F3                 292
#define HOBBES_KEY_F4                 293
#define HOBBES_KEY_F5                 294
#define HOBBES_KEY_F6                 295
#define HOBBES_KEY_F7                 296
#define HOBBES_KEY_F8                 297
#define HOBBES_KEY_F9                 298
#define HOBBES_KEY_F10                299
#define HOBBES_KEY_F11                300
#define HOBBES_KEY_F12                301
#define HOBBES_KEY_F13                302
#define HOBBES_KEY_F14                303
#define HOBBES_KEY_F15                304
#define HOBBES_KEY_F16                305
#define HOBBES_KEY_F17                306
#define HOBBES_KEY_F18                307
#define HOBBES_KEY_F19                308
#define HOBBES_KEY_F20                309
#define HOBBES_KEY_F21                310
#define HOBBES_KEY_F22                311
#define HOBBES_KEY_F23                312
#define HOBBES_KEY_F24                313
#define HOBBES_KEY_F25                314
#define HOBBES_KEY_KP_0               320
#define HOBBES_KEY_KP_1               321
#define HOBBES_KEY_KP_2               322
#define HOBBES_KEY_KP_3               323
#define HOBBES_KEY_KP_4               324
#define HOBBES_KEY_KP_5               325
#define HOBBES_KEY_KP_6               326
#define HOBBES_KEY_KP_7               327
#define HOBBES_KEY_KP_8               328
#define HOBBES_KEY_KP_9               329
#define HOBBES_KEY_KP_DECIMAL         330
#define HOBBES_KEY_KP_DIVIDE          331
#define HOBBES_KEY_KP_MULTIPLY        332
#define HOBBES_KEY_KP_SUBTRACT        333
#define HOBBES_KEY_KP_ADD             334
#define HOBBES_KEY_KP_ENTER           335
#define HOBBES_KEY_KP_EQUAL           336
#define HOBBES_KEY_LEFT_SHIFT         340
#define HOBBES_KEY_LEFT_CONTROL       341
#define HOBBES_KEY_LEFT_ALT           342
#define HOBBES_KEY_LEFT_SUPER         343
#define HOBBES_KEY_RIGHT_SHIFT        344
#define HOBBES_KEY_RIGHT_CONTROL      345
#define HOBBES_KEY_RIGHT_ALT          346
#define HOBBES_KEY_RIGHT_SUPER        347
#define HOBBES_KEY_MENU               348




struct Mouse
{
	double x;
	double y;
	double last_x;
	double last_y;
};



#endif