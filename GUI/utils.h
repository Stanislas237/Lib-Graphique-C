#pragma once

/* Types de tri */
typedef enum {
    SORT_SELECTION,
    SORT_MERGE
} SortType;

#define MIN_SCREEN_WIDTH 300
#define MIN_SCREEN_HEIGHT 300
#define MAX_SCREEN_WIDTH 1500
#define MAX_SCREEN_HEIGHT 1200
static int SCREEN_WIDTH = 800;
static int SCREEN_HEIGHT = 600;
static int INPUT_ACTIVE_COUNT = 0;
static SortType CURRENT_SORT_TYPE = SORT_SELECTION;
