#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include "assets_icons.h"

extern const uint8_t u8g2_font_helvB18_tr[];

typedef struct {
    const char* word;
    const Icon* icon;
} CatacracWord;

static const CatacracWord catacrac_words[] = {
    {"GAT", &A_cat},
    {"GOS", &A_dog},
    {"OCELL", &A_bird},
    {"PEIX", &A_fish},
    {"CAVALL", &A_horse},
    {"VACA", &A_cow},
    {"PORC", &A_pig},
    {"OVELLA", &A_sheep},
    {"CONILL", &A_rabbit},
    {"GRANOTA", &A_frog},
    {"GIRAFA", &A_giraffe},
    {"GALLINA", &A_hen},
};

#define CATACRAC_WORD_COUNT (sizeof(catacrac_words) / sizeof(catacrac_words[0]))
#define CATACRAC_ANIM_PERIOD_MS 500

typedef struct {
    size_t word_index;
    uint32_t anim_frame;
    bool word_revealed;
    ViewPort* view_port;
} CatacracState;

static void catacrac_draw_callback(Canvas* canvas, void* ctx) {
    CatacracState* state = ctx;
    const CatacracWord* word = &catacrac_words[state->word_index];

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    uint8_t word_y = 30;
    bool show_word = state->word_revealed || !word->icon;
    if(word->icon) {
        uint32_t frame = state->anim_frame % icon_get_frame_count(word->icon);
        canvas_draw_bitmap(
            canvas,
            (128 - icon_get_width(word->icon)) / 2,
            2,
            icon_get_width(word->icon),
            icon_get_height(word->icon),
            icon_get_frame_data(word->icon, frame));
        word_y = 54;
    }

    if(show_word) {
        canvas_set_custom_u8g2_font(canvas, u8g2_font_helvB18_tr);
        canvas_draw_str_aligned(canvas, 64, word_y, AlignCenter, AlignCenter, word->word);
    } else {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, word_y, AlignCenter, AlignCenter, "OK?");
    }

    char counter[16];
    snprintf(
        counter,
        sizeof(counter),
        "%u/%u",
        (unsigned)(state->word_index + 1),
        (unsigned)CATACRAC_WORD_COUNT);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 126, 2, AlignRight, AlignTop, counter);
}

static void catacrac_input_callback(InputEvent* input_event, void* ctx) {
    FuriMessageQueue* queue = ctx;
    furi_message_queue_put(queue, input_event, FuriWaitForever);
}

static void catacrac_anim_timer_callback(void* ctx) {
    CatacracState* state = ctx;
    state->anim_frame++;
    view_port_update(state->view_port);
}

int32_t catacrac_app(void* p) {
    UNUSED(p);

    CatacracState state = {
        .word_index = 0, .anim_frame = 0, .word_revealed = false, .view_port = NULL};

    FuriMessageQueue* input_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* view_port = view_port_alloc();
    state.view_port = view_port;
    view_port_draw_callback_set(view_port, catacrac_draw_callback, &state);
    view_port_input_callback_set(view_port, catacrac_input_callback, input_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);

    FuriTimer* anim_timer = furi_timer_alloc(
        catacrac_anim_timer_callback, FuriTimerTypePeriodic, &state);
    furi_timer_start(anim_timer, furi_ms_to_ticks(CATACRAC_ANIM_PERIOD_MS));

    InputEvent event;
    bool running = true;
    while(running) {
        if(furi_message_queue_get(input_queue, &event, FuriWaitForever) == FuriStatusOk) {
            if(event.type == InputTypeShort || event.type == InputTypeRepeat) {
                switch(event.key) {
                case InputKeyRight:
                    state.word_index = (state.word_index + 1) % CATACRAC_WORD_COUNT;
                    state.anim_frame = 0;
                    state.word_revealed = false;
                    view_port_update(view_port);
                    break;
                case InputKeyLeft:
                    state.word_index =
                        (state.word_index + CATACRAC_WORD_COUNT - 1) % CATACRAC_WORD_COUNT;
                    state.anim_frame = 0;
                    state.word_revealed = false;
                    view_port_update(view_port);
                    break;
                case InputKeyOk:
                    if(catacrac_words[state.word_index].icon && !state.word_revealed) {
                        state.word_revealed = true;
                    } else {
                        state.anim_frame = 0;
                    }
                    view_port_update(view_port);
                    break;
                case InputKeyBack:
                    running = false;
                    break;
                default:
                    break;
                }
            }
        }
    }

    furi_timer_stop(anim_timer);
    furi_timer_free(anim_timer);
    gui_remove_view_port(gui, view_port);
    furi_record_close(RECORD_GUI);
    view_port_free(view_port);
    furi_message_queue_free(input_queue);

    return 0;
}
