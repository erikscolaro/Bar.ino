//********** Gui settings **********//

#define RECIPEBOOK_LEN 32
#define BUF_LEN 50

//  Touchscreen settings
#define MILLIS_CHECK_TOUCH 200 //in milliseconds
#define MINPRESSURE 200
#define MAXPRESSURE 1000

//  Screen settings
#define TOUCH_ORIENTATION  0

//  Color palette
#define TRANSPARENT 0x0001 //standard
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define CCCC 0x3880 //personal 
#define CCC 0xfbc8
#define CC 0xfd0f
#define C 0xfed9
#define C_BACK 0xffbe

#define CUTOFF_COLOR 0xFFBF

//********** Gui.homepage settings **********//
#define HOMEPAGE_MAX_PAGES 9
#define TILE4ROW 3
#define TILE4COL 2
#define TILE4PAGE TILE4ROW*TILE4COL
#define IMAGES_DIR "/images/"
#define TILE_CHAR4LINE 10

//********** Gui.drinkpage settings **********//
#define SETTINGS_MAX_NUM 5
enum DrinkCapacity: short {
    SMALL = 50,
    MEDIUM = 200,
    LARGE = 400
};

//********** Gui.settings settings **********//


//********** Gui.executer settings **********//
