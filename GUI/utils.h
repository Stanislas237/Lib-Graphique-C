#pragma once

/* Types de tri */
typedef enum {
    SORT_SELECTION,
    SORT_MERGE
} SortType;

static int INPUT_ACTIVE_COUNT = 0;
static SortType CURRENT_SORT_TYPE = SORT_SELECTION;
