typedef struct{
    int r;
    int g;
    int b;
} MY_RGB;

typedef struct {
    int period;
    int start;
} TimeThing;

typedef struct {
    int index;
    MY_RGB rgb;
    TimeThing timer;
} ColorKeys;

#define _NUM_APEX_COLOR_MAP_KEYS 4
struct ApexColorMap {
    int num_keys;
    ColorKeys color_keys[_NUM_APEX_COLOR_MAP_KEYS];
};

#define _NUM_DEFUALT_COLOR_MAP_KEYS 4
struct DefaultColorMap {
    int num_keys;
    ColorKeys color_keys[_NUM_DEFUALT_COLOR_MAP_KEYS];
};

struct ApexColorMap apex_color_map;
struct DefaultColorMap default_color_map;

void initialize_color_maps(void) {
    ;
    apex_color_map.num_keys = _NUM_APEX_COLOR_MAP_KEYS;
    apex_color_map.color_keys[0] = (ColorKeys){52, {0, 0, 255}, {500, 0}};// ,
    apex_color_map.color_keys[1] = (ColorKeys){53, {255, 0, 0}, {500, 0}};// .
    apex_color_map.color_keys[2] = (ColorKeys){54, {0, 0, 255}, {500, 0}};// /
    apex_color_map.color_keys[3] = (ColorKeys){41, {255, 0, 0}, {500, 0}};// '

    
    default_color_map.num_keys = _NUM_DEFUALT_COLOR_MAP_KEYS;
    default_color_map.color_keys[0] = (ColorKeys){52, {255, 0, 0}, {500, 0}};
    default_color_map.color_keys[1] = (ColorKeys){53, {255, 0, 0}, {500, 0}};
    default_color_map.color_keys[2] = (ColorKeys){54, {255, 0, 0}, {500, 0}};
    default_color_map.color_keys[3] = (ColorKeys){41, {255, 0, 0}, {500, 0}};
}
